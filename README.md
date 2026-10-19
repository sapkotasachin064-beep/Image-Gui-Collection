# Image GUI Collection

ImGui-based GUI projects plus one TypeScript/pnpm workspace. Every app builds in GitHub Actions on each push to `main`.

## Layout

```
.github/workflows/build.yml   CI: builds every app in src/
src/                          all ImageGUI projects
images/                       ImGui-Menus reference collection (101 archives)
README.md
.gitignore
.gitattributes
```

## Apps in `src/`

| App | Tech | CI targets | Build output |
| --- | --- | --- | --- |
| `drillaware-framework` | C++ / ImGui / DX11 | `x64`, `x86` | `KingsleydotDev-Drillaware-ImGui-Menu.exe` |
| `chudvision-framework` | C++ / ImGui / DX11 | `x64`, `x86` | `ImGui-Chudvision-Framework-KingsleydotDev.exe` |
| `chudvision-menu` | C++ / ImGui / DX11 | `x64`, `x86` | `ImGui-Chudvision-Framework-KingsleydotDev.exe` |
| `opengl-base` | C++ / ImGui / OpenGL / MinHook / mINI | `x64`, `x86` | `Corrupted.dll` |
| `memesense-dx11` | C++ / ImGui / DX11 | `x64`, `x86` | `MemeSenseFramework.exe` |
| `static-dx11` | C++ / ImGui / DX11 | `x64`, `x86` | `StaticFramework.exe` |
| `animated-imgui-loader` | C++ / ImGui / Direct3D 9 | `x64`, `x86` | `Control-UsetInterface.exe` |
| `directx9-cheat-base` | C++ / ImGui / Direct3D 9 / MinHook | `x86` | `DirectX9 Cheat Base With Menu.dll` |
| `kdenlive-mcp` | TypeScript / pnpm workspace / Node 22 | Linux | `packages/*/dist` |

## CI workflow

`.github/workflows/build.yml` runs two jobs:

- `native` — one job per app and platform (16 in total). MSBuild builds `Release` on `windows-latest`. Each job uploads an artifact named `<app>-<platform>-Release`.
- `kdenlive-mcp` — `pnpm install --frozen-lockfile`, `pnpm build`, `pnpm typecheck`, `pnpm test` on `ubuntu-latest`.

Manual runs: **Actions → Build all apps → Run workflow**.

## Local builds

C++ apps need Visual Studio 2022 with the `v143` toolset and the Windows 10/11 SDK:

```
msbuild "src/drillaware-framework/KingsleydotDev-Drillaware-ImGui-Menu.sln" /p:Configuration=Release /p:Platform=x64
```

The two Direct3D 9 apps also need the D3DX9 headers and libraries for `d3dx9.h` / `d3dx9tex.h`:

- Option A: install the legacy DirectX SDK (June 2010). It sets `DXSDK_DIR`, which both projects read.
- Option B: restore `Microsoft.DXSDK.D3DX` from NuGet, then add its `build/native/include` to the include path and `build/native/release/lib/<arch>` to the library path.

CI takes option B: the `Install D3DX9 SDK` step downloads the NuGet package, stages `Include`, `Lib/x86` and `Lib/x64`, and exports `DXSDK_DIR`, `INCLUDE` and `LIB`.

Kdenlive MCP needs Node 22 and pnpm 9.5:

```
cd src/kdenlive-mcp
pnpm install --frozen-lockfile
pnpm build
```

## Notes

- `src/drillaware-framework/ext/freetype` vendors an x64 FreeType static lib (`freetype.lib`). Only the `x64` configurations link it. `Debug|x64` is not built in CI because the vendored lib is a Release build.
- Solution platforms are named `x86`/`x64`; `x86` maps to the project's `Win32` platform.
- `src/directx9-cheat-base` is built for `x86` only. Its `Release|x64` configuration has no D3DX9 include or library paths, so it does not compile as shipped.
- `src/animated-imgui-loader` prefers `$(DXSDK_DIR)`; its original include path pointed at a local DirectX SDK install, which CI cannot use.
- `images/` holds the ImGui-Menus archive collection: 101 packed menu projects, about 380 MB. It is tracked, so a fresh clone downloads all of it. Commits that touch only `images/` or Markdown files skip CI.
- The six original C++ app folders carry no license file from their authors, and the archives in `images/` come from third parties without a license. This repository is public, so review the rights before redistributing anything from it. `src/kdenlive-mcp` is Apache-2.0 (upstream: AMMIROSOH/Kdenlive-mcp) and keeps its license and docs.
