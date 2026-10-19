# Collects every artifact, publishes the binaries and a build report to the rolling release.
param(
    [string]$DistDir = "dist",
    [string]$ReleaseTag = "latest-build",
    [string]$Repo = $env:GITHUB_REPOSITORY
)

$ErrorActionPreference = 'Stop'

$runUrl = "$env:GITHUB_SERVER_URL/$env:GITHUB_REPOSITORY/actions/runs/$env:GITHUB_RUN_ID"
$stamp = (Get-Date).ToUniversalTime().ToString('yyyy-MM-dd HH:mm:ss') + ' UTC'
$stage = Join-Path $DistDir "_release"
New-Item -ItemType Directory -Force -Path $stage | Out-Null

$binaries = @()
$menuRows = @()
$projectRows = @()
$zips = @()

foreach ($dir in (Get-ChildItem $DistDir -Directory | Where-Object { $_.Name -ne '_release' })) {
    foreach ($file in (Get-ChildItem $dir.FullName -File)) {
        switch ($file.Name) {
            'report.json' { $menuRows += @(Get-Content $file.FullName -Raw | ConvertFrom-Json) }
            'build-info.txt' {
                $info = @{}
                foreach ($line in (Get-Content $file.FullName)) {
                    if ($line -match '^\s*([^:]+):\s*(.*)$') { $info[$Matches[1].Trim()] = $Matches[2].Trim() }
                }
                $projectRows += [pscustomobject]@{ artifact = $dir.Name; info = $info }
            }
            default {
                if ($file.Extension -in @('.exe', '.dll')) {
                    $binaries += [pscustomobject]@{ artifact = $dir.Name; path = $file.FullName; name = $file.Name }
                }
            }
        }
    }
    if ($dir.Name -notlike 'menus-part-*') {
        $zip = Join-Path $DistDir "$($dir.Name).zip"
        if (Test-Path $zip) { Remove-Item $zip -Force }
        Compress-Archive -Path (Join-Path $dir.FullName '*') -DestinationPath $zip -Force
        $zips += $zip
    }
}

# Binaries keep their own name when it is unique, otherwise they get the artifact name in front.
$taken = @{}
foreach ($binary in $binaries) {
    $asset = $binary.name
    if ($taken.ContainsKey($asset.ToLower())) { $asset = "$($binary.artifact)-$($binary.name)" }
    $suffix = 1
    while ($taken.ContainsKey($asset.ToLower())) {
        $suffix++
        $asset = "$($binary.artifact)-$suffix-$($binary.name)"
    }
    $taken[$asset.ToLower()] = $true
    $binary | Add-Member -NotePropertyName asset -NotePropertyValue $asset
    Copy-Item -LiteralPath $binary.path -Destination (Join-Path $stage $asset) -Force
}

foreach ($zip in $zips) { Copy-Item -LiteralPath $zip -Destination $stage -Force }

$reportLines = @(
    '# Build report',
    '',
    "- commit: ``$env:GITHUB_SHA``",
    "- run: $runUrl",
    "- built: $stamp",
    "- binaries: $(@($binaries).Count)",
    '',
    '## Tracked projects (src/)',
    '',
    '| artifact | app | platform | configuration | commit |',
    '| --- | --- | --- | --- | --- |'
)
foreach ($row in ($projectRows | Sort-Object artifact)) {
    $reportLines += "| $($row.artifact) | $($row.info['app']) | $($row.info['platform']) | $($row.info['configuration']) | $($row.info['commit']) |"
}

$ok = @($menuRows | Where-Object status -eq 'ok')
$failed = @($menuRows | Where-Object status -eq 'failed')
$skipped = @($menuRows | Where-Object { $_.status -in @('no-solution', 'extract-failed') })

$reportLines += @(
    '',
    '## Menus (images/ archives)',
    '',
    "built: $($ok.Count)   failed: $($failed.Count)   skipped: $($skipped.Count)   total: $($menuRows.Count)",
    '',
    '### Built',
    '',
    '| target | platform | outputs |',
    '| --- | --- | --- |'
)
foreach ($row in ($ok | Sort-Object target)) { $reportLines += "| $($row.target) | $($row.platform) | $($row.outputs) |" }

if ($failed.Count -gt 0) {
    $reportLines += @('', '### Failed', '', '| target | first error |', '| --- | --- |')
    foreach ($row in ($failed | Sort-Object target)) {
        $error = ($row.error -replace '\|', '/')
        if ($error.Length -gt 220) { $error = $error.Substring(0, 220) + '...' }
        $reportLines += "| $($row.target) | ``$error`` |"
    }
}

if ($skipped.Count -gt 0) {
    $reportLines += @('', '### Skipped (no .sln in the archive)', '', ($skipped | Sort-Object target | ForEach-Object { "- $($_.target) ($($_.error))" }) -join "`n")
}

$reportLines += @('', '## Binaries in this release', '')
foreach ($binary in ($binaries | Sort-Object asset)) { $reportLines += "- ``$($binary.asset)``" }

$reportMd = Join-Path $stage 'BUILD-REPORT.md'
($reportLines -join "`n") | Set-Content -Encoding utf8 $reportMd
$reportJson = Join-Path $stage 'BUILD-REPORT.json'
[pscustomobject]@{
    commit  = $env:GITHUB_SHA
    run     = $runUrl
    built   = $stamp
    projects = $projectRows
    menus    = $menuRows
} | ConvertTo-Json -Depth 6 | Set-Content -Encoding utf8 $reportJson

gh release view $ReleaseTag --repo $Repo > $null 2>&1
if ($LASTEXITCODE -ne 0) {
    gh release create $ReleaseTag --repo $Repo --title "Latest builds" --notes "Rolling release of every Release build."
    if ($LASTEXITCODE -ne 0) { throw "could not create release $ReleaseTag" }
}

$assets = @(Get-ChildItem $stage -File | ForEach-Object { $_.FullName })
Write-Output "uploading $($assets.Count) assets to $ReleaseTag"
gh release upload $ReleaseTag --repo $Repo --clobber @assets
if ($LASTEXITCODE -ne 0) { throw "release upload failed" }

$notes = "Rolling release with every Release build of ``main``.`n`n- commit: ``$env:GITHUB_SHA```n- run: $runUrl`n- binaries: $(@($binaries).Count) ($($ok.Count) menus, $($projectRows.Count) projects)`n- built: $stamp`n`nSee BUILD-REPORT.md for the per-target status."
gh release edit $ReleaseTag --repo $Repo --notes $notes
gh release view $ReleaseTag --repo $Repo --json assets --jq '.assets | length, (.[].name)' > $null

$summaryLines = @("## latest-build", '', "- binaries: $($binaries.Count)", "- menu builds ok: $($ok.Count), failed: $($failed.Count), skipped: $($skipped.Count)", "- projects: $($projectRows.Count)", "- report: BUILD-REPORT.md", '')
($summaryLines -join "`n") | Out-File -Append -Encoding utf8 $env:GITHUB_STEP_SUMMARY
Write-Output "published $($binaries.Count) binaries and $($zips.Count) zips"
