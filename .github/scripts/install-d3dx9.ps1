# Stages the D3DX9/D3DX11 headers and libraries from the Microsoft.DXSDK.D3DX NuGet package.
# Used by every native job, because most projects include d3dx9.h, d3dx10.h or d3dx11.h.
param(
    [string]$Platform = ""
)

$ErrorActionPreference = 'Stop'

$tmp = Join-Path $env:RUNNER_TEMP "d3dx"
New-Item -ItemType Directory -Force -Path $tmp | Out-Null

$zip = Join-Path $tmp "d3dx.zip"
if (-not (Test-Path $zip)) {
    Invoke-WebRequest -Uri "https://www.nuget.org/api/v2/package/Microsoft.DXSDK.D3DX/9.29.952.8" -OutFile $zip -UseBasicParsing
}

$pkgRoot = Join-Path $tmp "pkg"
if (-not (Test-Path (Join-Path $pkgRoot "build/native/include/d3dx9.h"))) {
    Expand-Archive -LiteralPath $zip -DestinationPath $pkgRoot -Force
}
$pkg = Join-Path $pkgRoot "build/native"

$dx = Join-Path $tmp "sdk"
New-Item -ItemType Directory -Force -Path "$dx/Include", "$dx/Lib/x86", "$dx/Lib/x64" | Out-Null
Copy-Item "$pkg/include/*" "$dx/Include" -Recurse -Force
Copy-Item "$pkg/release/lib/x86/*", "$pkg/debug/lib/x86/*" "$dx/Lib/x86" -Recurse -Force
Copy-Item "$pkg/release/lib/x64/*", "$pkg/debug/lib/x64/*" "$dx/Lib/x64" -Recurse -Force

# Some projects hardcode the legacy install path instead of $(DXSDK_DIR), so fill it in too.
foreach ($legacy in @(
        "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)",
        "C:\Program Files\Microsoft DirectX SDK (June 2010)")) {
    New-Item -ItemType Directory -Force -Path "$legacy\Include", "$legacy\Lib\x86", "$legacy\Lib\x64" | Out-Null
    Copy-Item "$dx\Include\*" "$legacy\Include" -Force
    Copy-Item "$dx\Lib\x86\*" "$legacy\Lib\x86" -Force
    Copy-Item "$dx\Lib\x64\*" "$legacy\Lib\x64" -Force
}

$arch = if ($Platform -eq "x86") { "x86" } else { "x64" }
# src/animated-imgui-loader ships an x64 d3dx9.lib in its project folder, and the project
# folder wins over the SDK search path. Put the matching architecture library back.
$vendored = "src/animated-imgui-loader/d3dx9.lib"
if (Test-Path $vendored) { Copy-Item "$dx/Lib/$arch/d3dx9.lib" $vendored -Force }
Add-Content -Path $env:GITHUB_ENV -Value "LIB=$dx\Lib\$arch;$env:LIB"
Add-Content -Path $env:GITHUB_ENV -Value "DXSDK_DIR=$dx\"
Add-Content -Path $env:GITHUB_ENV -Value "INCLUDE=$dx\Include;$env:INCLUDE"
Write-Output "D3DX SDK staged at $dx (platform: $arch)"
