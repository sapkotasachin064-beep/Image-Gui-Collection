# Builds the Release binary of every project of one matrix chunk.
# x64 is the architecture that is built; a solution without an x64 configuration
# falls back to the Release platform its solution file declares.
param(
    [Parameter(Mandatory = $true)]
    [string[]] $Projects,

    [string] $OutRoot = 'artifacts'
)

$ErrorActionPreference = 'Continue'
$msbuild = (Get-Command msbuild -ErrorAction Stop).Source
$marker = (Get-Date).AddSeconds(-30)
$summary = New-Object System.Collections.Generic.List[string]

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
    $files = Get-ChildItem -LiteralPath $Dir -Recurse -File -ErrorAction SilentlyContinue |
        Where-Object {
            $_.Extension -in '.exe', '.dll' -and
            $_.LastWriteTime -gt $marker -and
            $_.FullName -notmatch '\\obj\\'
        }
    $exe = $files | Where-Object Extension -eq '.exe' | Sort-Object { $_.FullName.Length } | Select-Object -First 1
    if ($exe) { return $exe }
    return ($files | Where-Object Extension -eq '.dll' | Sort-Object { $_.FullName.Length } | Select-Object -First 1)
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
                & $msbuild @msbuildArgs *>&1 | Tee-Object -FilePath $log -Append | Out-Null
                if ($LASTEXITCODE -eq 0) { $state = 'built'; break }
                Write-Host "msbuild exited $LASTEXITCODE, retrying with another toolset"
            }
        }
        elseif (Test-Path (Join-Path $dir 'package.json')) {
            Write-Host "no solution under $project, running the node build instead"
            Push-Location $dir
            try {
                & corepack enable *>&1 | Out-Null
                & pnpm install --frozen-lockfile *>&1 | Tee-Object -FilePath $log -Append | Out-Null
                $state = if ($LASTEXITCODE -eq 0) { 'built (node)' } else { 'failed (node)' }
                if ($LASTEXITCODE -eq 0) {
                    & pnpm build *>&1 | Tee-Object -FilePath $log -Append | Out-Null
                    $state = if ($LASTEXITCODE -eq 0) { 'built (node)' } else { 'failed (node build)' }
                }
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

    $seconds = [int]([datetime]::Now - $started).TotalSeconds
    $name = if ($binary) { Split-Path -Leaf $binary.FullName } else { '-' }
    $summary.Add("| $project | $state | $platform | $name | ${seconds}s |")
    Write-Host "::endgroup::"
}

if ($env:GITHUB_STEP_SUMMARY) {
    @(
        "| project | result | platform | binary | time |",
        "| --- | --- | --- | --- | --- |"
    ) + $summary | Add-Content -Path $env:GITHUB_STEP_SUMMARY
}
