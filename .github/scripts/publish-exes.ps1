# Publishes the binaries of one project to that project's own release.
# Every project of src/ owns one tag, so its release holds its own exe/dll only.
param(
    [Parameter(Mandatory = $true)]
    [string] $Tag,

    [Parameter(Mandatory = $true)]
    [string] $Title,

    [string] $Dist = 'artifacts'
)

$ErrorActionPreference = 'Stop'
$PSNativeCommandUseErrorActionPreference = $false

$repo = $env:GITHUB_REPOSITORY

if (-not (Test-Path $Dist)) { throw "the artifact directory '$Dist' is missing" }

$binaries = Get-ChildItem -Path $Dist -Recurse -File |
    Where-Object { $_.Extension -in '.exe', '.dll' }

if (-not $binaries) { throw "no binaries found under $Dist" }

$stage = Join-Path $env:RUNNER_TEMP 'release'
New-Item -ItemType Directory -Force -Path $stage | Out-Null

$uploads = New-Object System.Collections.Generic.List[string]
foreach ($binary in $binaries) {
    # The asset carries the release tag, so the name does not depend on the layout
    # the artifact download produced.
    $name = $Tag + $binary.Extension
    if (Test-Path (Join-Path $stage $name)) {
        $name = $Tag + '_' + [System.IO.Path]::GetFileNameWithoutExtension($binary.Name) + $binary.Extension
    }
    $target = Join-Path $stage $name
    Copy-Item -LiteralPath $binary.FullName -Destination $target -Force
    if (-not $uploads.Contains($target)) { $uploads.Add($target) }
}

$notes = @(
    "Automated Release build of ``src/$Title`` (x64).",
    '',
    "commit: $env:GITHUB_SHA",
    "run: $env:GITHUB_SERVER_URL/$env:GITHUB_REPOSITORY/actions/runs/$env:GITHUB_RUN_ID"
) -join "`n"
$notesFile = Join-Path $env:RUNNER_TEMP 'release-notes.md'
Set-Content -LiteralPath $notesFile -Value $notes -Encoding utf8

gh release view $Tag --repo $repo --json name 2>$null | Out-Null
if ($LASTEXITCODE -ne 0) {
    Write-Output "creating release $Tag"
    gh release create $Tag --repo $repo --title $Title --notes-file $notesFile --latest=false
    if ($LASTEXITCODE -ne 0) { throw "gh release create failed with exit code $LASTEXITCODE" }
}

Write-Output "uploading $($uploads.Count) binaries to $Tag"
gh release upload $Tag $uploads --repo $repo --clobber
if ($LASTEXITCODE -ne 0) { throw "gh release upload failed with exit code $LASTEXITCODE" }

gh release edit $Tag --repo $repo --title $Title --notes-file $notesFile
if ($LASTEXITCODE -ne 0) { throw "gh release edit failed with exit code $LASTEXITCODE" }

# Drop assets left over from an earlier binary name of the same project.
$keep = $uploads | ForEach-Object { Split-Path -Leaf $_ }
$stale = gh release view $Tag --repo $repo --json assets --jq '.assets[].name' 2>$null
foreach ($asset in $stale) {
    if ($keep -notcontains $asset) {
        Write-Output "removing stale asset $asset"
        gh release delete-asset $Tag $asset --repo $repo --yes
    }
}

$uploads | ForEach-Object { Write-Output "  $(Split-Path -Leaf $_)" }
Write-Output "release $Tag now holds $((gh release view $Tag --repo $repo --json assets --jq '.assets | length')) assets"
