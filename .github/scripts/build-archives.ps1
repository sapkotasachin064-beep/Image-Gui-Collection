# Extracts the archives of one part and builds every C++ solution it finds, in Release.
# Failures are recorded in report.json instead of failing the job, so one broken menu
# never hides the builds of the other menus.
param(
    [Parameter(Mandatory = $true)][string]$NamesJson,
    [Parameter(Mandatory = $true)][int]$Index,
    [string]$ArchiveDir = "images",
    [string]$OutDir = ""
)

$ErrorActionPreference = 'Continue'

if ($NamesJson -isnot [string]) { $NamesJson = ($NamesJson | ConvertTo-Json -Compress) }
$names = @($NamesJson | ConvertFrom-Json)
if (-not $OutDir) { $OutDir = "artifacts/menus-part-$Index" }
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$tempRoot = Join-Path $env:RUNNER_TEMP "menus-$Index"
Remove-Item $tempRoot -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $tempRoot | Out-Null

$sevenZip = (Get-Command 7z.exe -ErrorAction SilentlyContinue).Source
if (-not $sevenZip) { $sevenZip = "C:\Program Files\7-Zip\7z.exe" }

function Get-Slug([string]$name) {
    $base = [System.IO.Path]::GetFileNameWithoutExtension($name)
    ($base -replace '[^A-Za-z0-9._-]', '-').Trim('-')
}

function Get-Solutions([string]$root) {
    Get-ChildItem -LiteralPath $root -Recurse -File -Filter *.sln -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\\.vs\\' -and $_.FullName -notmatch '\\(Debug|Release|x64|x86|Build|Bin)\\[^\\]*$' } |
        Select-Object -First 3
}

function Get-ReleasePlatforms([string]$sln) {
    $found = Select-String -LiteralPath $sln -Pattern '^\s*Release\|([^=\s]+)\s*=' -AllMatches |
        ForEach-Object { $_.Matches | ForEach-Object { $_.Groups[1].Value } } |
        Select-Object -Unique
    return @($found)
}

function Get-BinaryMap([string]$root) {
    $map = @{}
    Get-ChildItem -LiteralPath $root -Recurse -File -Include *.exe,*.dll -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\obj\\' -and $_.FullName -notmatch '\\\.vs\\' } |
        ForEach-Object { $map[$_.FullName] = "$($_.Length)|$($_.LastWriteTimeUtc.Ticks)" }
    return $map
}

$rows = New-Object System.Collections.Generic.List[object]

foreach ($name in $names) {
    $slug = Get-Slug $name
    $work = Join-Path $tempRoot $slug
    Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Force -Path $work | Out-Null
    $log = Join-Path $OutDir "$slug-build.log"
    "=== $name" | Out-File -Encoding utf8 $log

    & $sevenZip x -y "-o$work" (Join-Path $ArchiveDir $name) > $null 2>&1
    if ($LASTEXITCODE -ne 0) {
        $rows.Add([pscustomobject]@{ target = $slug; archive = $name; status = 'extract-failed'; platform = ''; solution = ''; outputs = ''; error = '7z could not extract the archive' })
        continue
    }

    $solutions = @(Get-Solutions $work)
    if (-not $solutions) {
        # Some archives pack the real project inside another archive.
        Get-ChildItem -LiteralPath $work -Recurse -File -Include *.rar,*.zip -ErrorAction SilentlyContinue |
            Select-Object -First 10 |
            ForEach-Object { & $sevenZip x -y "-o$work" $_.FullName > $null 2>&1 }
        $solutions = @(Get-Solutions $work)
    }
    $solutions = @($solutions | Sort-Object { $_.FullName.Length })

    if (-not $solutions) {
        $rows.Add([pscustomobject]@{ target = $slug; archive = $name; status = 'no-solution'; platform = ''; solution = ''; outputs = ''; error = 'no .sln found' })
        Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
        continue
    }

    $built = $null
    $lastCode = $null
    foreach ($sln in $solutions) {
        $platforms = Get-ReleasePlatforms $sln.FullName
        $order = @()
        foreach ($pref in @('x64', 'x86', 'Win32')) { if ($platforms -contains $pref) { $order += $pref } }
        if (-not $order) { $order = @('x64', 'Win32', 'x86') }
        $order = @($order | Select-Object -First 3)

        foreach ($platform in $order) {
            $variants = @(
                @(),
                @('/p:PlatformToolset=v143', '/p:WindowsTargetPlatformVersion=10.0')
            )
            foreach ($variant in $variants) {
                $before = Get-BinaryMap $work
                $arguments = @($sln.FullName, '/nologo', '/m', '/v:minimal', '/p:Configuration=Release', "/p:Platform=$platform") + $variant
                $output = & msbuild @arguments 2>&1
                $lastCode = $LASTEXITCODE
                $output | Out-File -Append -Encoding utf8 $log
                if ($lastCode -ne 0) { continue }

                $after = Get-BinaryMap $work
                $fresh = @()
                foreach ($key in $after.Keys) {
                    if (-not $before.ContainsKey($key) -or $before[$key] -ne $after[$key]) { $fresh += $key }
                }
                if (-not $fresh) { continue }

                $outputs = @()
                foreach ($file in $fresh) {
                    $asset = "$slug-$platform-" + (Split-Path $file -Leaf)
                    Copy-Item -LiteralPath $file -Destination (Join-Path $OutDir $asset) -Force
                    $outputs += $asset
                }
                $built = [pscustomobject]@{
                    target   = $slug
                    archive  = $name
                    status   = 'ok'
                    platform = $platform
                    solution = (Split-Path $sln.FullName -Leaf)
                    outputs  = ($outputs -join ', ')
                    error    = ''
                }
                break
            }
            if ($built) { break }
        }
        if ($built) { break }
    }

    if (-not $built) {
        $errors = @(Select-String -LiteralPath $log -Pattern 'error [A-Z]+[0-9]+|fatal error|error MSB[0-9]+' -AllMatches |
                Select-Object -First 3 | ForEach-Object { $_.Line.Trim() })
        $rows.Add([pscustomobject]@{
                target   = $slug
                archive  = $name
                status   = 'failed'
                platform = ''
                solution = (Split-Path $solutions[0].FullName -Leaf)
                outputs  = ''
                error    = (($errors -join ' | ') -replace '\s+', ' ')
            })
    } else {
        $rows.Add($built)
    }

    Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
}

$json = $rows | ConvertTo-Json -Depth 4
$json | Set-Content -Encoding utf8 (Join-Path $OutDir "report.json")

$ok = @($rows | Where-Object status -eq 'ok').Count
$failed = @($rows | Where-Object status -eq 'failed').Count
$skipped = @($rows | Where-Object { $_.status -in @('no-solution', 'extract-failed') }).Count

$lines = @("### menus part $Index", '', "built: $ok   failed: $failed   skipped: $skipped", '', '| target | status | platform | outputs |', '| --- | --- | --- | --- |')
foreach ($row in $rows) { $lines += "| $($row.target) | $($row.status) | $($row.platform) | $($row.outputs) |" }
($lines -join "`n") | Out-File -Append -Encoding utf8 $env:GITHUB_STEP_SUMMARY

Write-Output "menus part $Index -> built=$ok failed=$failed skipped=$skipped"
foreach ($row in $rows) { Write-Output ("  {0,-42} {1,-14} {2}" -f $row.target, $row.status, $row.outputs) }
