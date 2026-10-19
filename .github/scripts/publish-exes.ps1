# Replaces the assets of the rolling release with the binaries of this run.
# Only the built binaries (exe/dll) are published; nothing else is uploaded.
$ErrorActionPreference = 'Stop'

$repo = $env:GITHUB_REPOSITORY
$tag = 'latest-build'
$dist = 'dist'

if (-not (Test-Path $dist)) { throw "the artifact directory '$dist' is missing" }

$binaries = Get-ChildItem -Path $dist -Recurse -File |
    Where-Object { $_.Extension -in '.exe', '.dll' }

if (-not $binaries) { throw "no binaries found under $dist" }

$stage = Join-Path $env:RUNNER_TEMP 'release'
New-Item -ItemType Directory -Force -Path $stage | Out-Null

$uploads = New-Object System.Collections.Generic.List[string]
foreach ($binary in $binaries) {
    $project = Split-Path -Leaf (Split-Path -Parent $binary.FullName)
    $name = ($project -replace '[\\/:*?"<>|]', '_') + $binary.Extension
    $target = Join-Path $stage $name
    Copy-Item -LiteralPath $binary.FullName -Destination $target -Force
    $uploads.Add($target)
}

gh release view $tag --repo $repo --json name 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    gh release create $tag --repo $repo --title 'Latest builds' --notes 'Binaries of the latest build of src/' --latest
}

$stale = gh release view $tag --repo $repo --json assets --jq '.assets[].name' 2>$null
foreach ($asset in $stale) {
    gh release delete-asset $tag $asset --repo $repo --yes
}

Write-Output "uploading $($uploads.Count) binaries to $tag"
gh release upload $tag $uploads --repo $repo --clobber
if ($LASTEXITCODE -ne 0) { throw "gh release upload failed with exit code $LASTEXITCODE" }

$notes = @(
    'Automated Release build of every project under `src/` (x64).',
    '',
    "commit: $env:GITHUB_SHA",
    "run: $env:GITHUB_SERVER_URL/$env:GITHUB_REPOSITORY/actions/runs/$env:GITHUB_RUN_ID",
    "assets: $($uploads.Count) binaries",
    '',
    'Only the built binaries are attached; sources stay in the repository.'
) -join "`n"
$notesFile = Join-Path $env:RUNNER_TEMP 'release-notes.md'
Set-Content -LiteralPath $notesFile -Value $notes -Encoding utf8
gh release edit $tag --repo $repo --title 'Latest builds' --notes-file $notesFile --latest

$uploads | ForEach-Object { Write-Output "  $(Split-Path -Leaf $_)" }
Write-Output "release $tag now holds $((gh release view $tag --repo $repo --json assets --jq '.assets | length')) assets"
