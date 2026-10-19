# Builds the Release binary of every project of one matrix entry. The workflow passes
# a single project, so one job reports one project.
# x64 is the architecture that is built; a solution without an x64 configuration
# falls back to the Release platform its solution file declares.
param(
    [Parameter(Mandatory = $true)]
    [string[]] $Projects,

    [string] $OutRoot = 'artifacts'
)

$ErrorActionPreference = 'Continue'
# Keep a failing native command from becoming a terminating error, and keep
# msbuild's exit code from becoming the exit code of this script.
$PSNativeCommandUseErrorActionPreference = $false
$msbuild = (Get-Command msbuild -ErrorAction Stop).Source
$marker = (Get-Date).AddSeconds(-30)
$summary = New-Object System.Collections.Generic.List[string]
$result = $null

function Find-PrimarySolution {
    param([string] $Dir)
    $solutions = Get-ChildItem -LiteralPath $Dir -Recurse -File -Filter *.sln -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\freetype\\' -and $_.FullName -notmatch '\\thirdparty\\' }
    if (-not $solutions) { return $null }
    # The shortest path is the top-level solution of the project.
    return ($solutions | Sort-Object { $_.FullName.Length } | Select-Object -First 1)
}

function Get-SolutionPlatform {
    param([string] $Solution)
    $text = Get-Content -LiteralPath $Solution -Raw
    if ($text -match 'Release\|x64') { return 'x64' }
    if ($text -match 'Release\|Win32') { return 'Win32' }
    if ($text -match 'Release\|x86') { return 'x86' }
    return 'x64'
}

function Find-BuiltBinary {
    param([string] $Dir)
    # Only files the build wrote in this run are considered, so a prebuilt binary
    # that ships with the project is never published.
    $files = Get-ChildItem -LiteralPath $Dir -Recurse -File -ErrorAction SilentlyContinue |
        Where-Object {
            $_.Extension -in '.exe', '.dll' -and
            $_.LastWriteTime -gt $marker -and
            $_.FullName -notmatch '\\obj\\'
        }
    $scored = foreach ($file in $files) {
        $score = 0
        # A vendored library is not the program of the project.
        if ($file.FullName -match '\\freetype\\|\\thirdparty\\|\\libs\\|\\SDK\\|\\Dependencies\\') { $score += 100 }
        if ($file.Extension -eq '.dll') { $score += 10 }
        if ($file.FullName -notmatch 'release|x64|\\bin\\') { $score += 1 }
        [pscustomobject]@{
            File = $file
            Score = $score
            Depth = ($file.FullName -split '\\').Count
            Written = $file.LastWriteTime.Ticks
        }
    }
    $best = $scored | Sort-Object Score, Depth, @{ Expression = { - $_.Written } } | Select-Object -First 1
    if ($best) { return $best.File }
    return $null
}

function Get-BuildErrors {
    param([string] $Log)
    if (-not (Test-Path $Log)) { return @() }
    return @(Select-String -Path $Log -Pattern 'error [A-Z]{1,4}[0-9]{2,5}|fatal error|LNK[0-9]{4}|MSB[0-9]{4}' -ErrorAction SilentlyContinue |
        ForEach-Object { ($_.Line -replace '\s+', ' ').Trim() } |
        Select-Object -Unique -First 3)
}

foreach ($project in $Projects) {
    $dir = Join-Path 'src' $project
    $out = Join-Path $OutRoot $project
    New-Item -ItemType Directory -Force -Path $out | Out-Null
    $log = Join-Path $out 'build.log'
    $started = Get-Date
    $solution = Find-PrimarySolution -Dir $dir
    $platform = ''
    $state = 'skipped'
    $binary = $null

    Write-Host "::group::$project"
    try {
        if ($solution) {
            $platform = Get-SolutionPlatform -Solution $solution.FullName
            $common = @($solution.FullName, '/nologo', '/m', '/v:minimal', '/p:Configuration=Release', "/p:Platform=$platform")
            $attempts = @(
                @(),
                @('/p:PlatformToolset=v143', '/p:WindowsTargetPlatformVersion=10.0'),
                @('/p:PlatformToolset=v142', '/p:WindowsTargetPlatformVersion=10.0')
            )
            $state = 'failed'
            foreach ($extra in $attempts) {
                $msbuildArgs = $common + $extra
                Write-Host "msbuild $($msbuildArgs -join ' ')"
                & $msbuild @msbuildArgs 2>&1 | Tee-Object -FilePath $log -Append
                if ($LASTEXITCODE -eq 0) { $state = 'built'; break }
                Write-Host "msbuild exited $LASTEXITCODE, retrying with another toolset"
            }
        }
        elseif (Test-Path (Join-Path $dir 'package.json')) {
            Write-Host "no solution under $project, running the node build instead"
            Push-Location $dir
            try {
                & corepack enable *>&1 | Out-Null
                & pnpm install --frozen-lockfile 2>&1 | Tee-Object -FilePath $log -Append
                if ($LASTEXITCODE -eq 0) {
                    & pnpm build 2>&1 | Tee-Object -FilePath $log -Append
                    $state = if ($LASTEXITCODE -eq 0) { 'built (node)' } else { 'failed (node build)' }
                }
                else { $state = 'failed (node install)' }
            }
            finally {
                Pop-Location
            }
        }
        else {
            Write-Host "no project files under $project, nothing to build"
        }
    }
    catch {
        Write-Host "error: $_"
        $state = 'error'
    }

    $binary = Find-BuiltBinary -Dir $dir
    if ($binary) {
        $target = Join-Path $out ($project + $binary.Extension)
        Copy-Item -LiteralPath $binary.FullName -Destination $target -Force
        Write-Host "collected $(Split-Path -Leaf $binary.FullName) -> $target"
    }
    else {
        Write-Host "no fresh binary under $project"
    }

    $errors = Get-BuildErrors -Log $log
    foreach ($line in $errors) { Write-Host "  $line" }

    $seconds = [int]([datetime]::Now - $started).TotalSeconds
    $name = if ($binary) { Split-Path -Leaf $binary.FullName } else { '-' }
    $detail = if ($errors.Count) { ($errors -join ' / ') -replace '\|', '/' } else { '' }
    if ($detail.Length -gt 220) { $detail = $detail.Substring(0, 220) }
    $summary.Add("| $project | $state | $platform | $name | $detail | ${seconds}s |")
    # Only a project without a binary is a warning: the workflow releases that project
    # only when the binary exists, so a green job with a warning is the expected shape
    # of a project that does not compile.
    if (-not $binary) {
        Write-Host "##[warning] ${project}: $state ($platform) $(if ($detail) { $detail } else { 'no compiler error reported' })"
    }
    Write-Host "::endgroup::"

    $result = [pscustomobject]@{
        Project = $project
        State   = $state
        Binary  = [bool] $binary
    }
}

if ($env:GITHUB_STEP_SUMMARY) {
    @(
        "| project | result | platform | binary | first errors | time |",
        "| --- | --- | --- | --- | --- | --- |"
    ) + $summary | Add-Content -Path $env:GITHUB_STEP_SUMMARY
}

# The workflow reads these to decide whether this project has something to release.
if ($env:GITHUB_OUTPUT -and $result) {
    Add-Content -Path $env:GITHUB_OUTPUT -Value "state=$($result.State)"
    Add-Content -Path $env:GITHUB_OUTPUT -Value "binary=$($result.Binary.ToString().ToLowerInvariant())"
}

# A failed build must not fail the job: the release collects whatever was built.
exit 0
