# Image GUI Collection

Collection of ImGui-based GUI projects plus one TypeScript/pnpm workspace. GitHub Actions builds
every project in `Release` on each push to `main`, uploads one artifact per app and platform, and
pushes those builds to the rolling `latest-build` release.

[![Build all (Release)](https://github.com/sapkotasachin064-beep/Image-Gui-Collection/actions/workflows/build.yml/badge.svg)](https://github.com/sapkotasachin064-beep/Image-Gui-Collection/actions/workflows/build.yml)

## Repository layout

```
.github/workflows/build.yml   CI: builds every app in Release and publishes the results
.github/scripts/              helper scripts the workflow calls on the runner
src/                          every project (8 apps + kdenlive-mcp)
images/                       ImGui-Menus reference collection: 99 archives (76 rar, 23 zip)
README.md
.gitignore
.gitattributes
```

## Projects in `src/`

| Project | Stack | CI platforms | Output binary |
| --- | --- | --- | --- |
| `drillaware-framework` | C++ / ImGui / DirectX 11 | `x64`, `x86` | `KingsleydotDev-Drillaware-ImGui-Menu.exe` |
| `chudvision-framework` | C++ / ImGui / DirectX 11 | `x64`, `x86` | `ImGui-Chudvision-Framework-KingsleydotDev.exe` |
| `chudvision-menu` | C++ / ImGui / DirectX 11 | `x64`, `x86` | `ImGui-Chudvision-Framework-KingsleydotDev.exe` |
| `opengl-base` | C++ / ImGui / OpenGL / MinHook / mINI | `x64`, `x86` | `Corrupted.dll` |
| `memesense-dx11` | C++ / ImGui / DirectX 11 | `x64`, `x86` | `MemeSenseFramework.exe` |
| `static-dx11` | C++ / ImGui / DirectX 11 | `x64`, `x86` | `StaticFramework.exe` |
| `animated-imgui-loader` | C++ / ImGui / Direct3D 9 | `x64`, `x86` | `Control-UsetInterface.exe` |
| `directx9-cheat-base` | C++ / ImGui / Direct3D 9 / MinHook | `x86` | `DirectX9 Cheat Base With Menu.dll` |
| `kdenlive-mcp` | TypeScript / pnpm workspace / Node 22 | Linux | `apps/*/dist`, `packages/*/dist` |

Fifteen native jobs build the Windows projects, one job builds the TypeScript workspace.

## Continuous integration

Workflow: `.github/workflows/build.yml` (`Build all (Release)`). Everything runs on GitHub
runners; the `.github/scripts` helpers only run there too.

Triggers:

- push to `main`, except commits that touch only `**.md`
- pull requests
- manual run: **Actions → Build all (Release) → Run workflow**

Jobs:

1. `discover` — lists the archives in `images/` and splits them into parts of 10.
2. `projects` — one job per app and platform for the eight projects in `src/`. Steps: checkout,
   `microsoft/setup-msbuild`, D3DX SDK step where needed, `msbuild <solution> /p:Configuration=Release
   /p:Platform=<platform>`, locate the binary, stage it with `build-info.txt`, upload artifact
   `<app>-<platform>-Release`.
3. `menus` — one job per part. Each job stages the D3DX SDK, extracts its archives with 7-Zip,
   picks the solution out of every archive, builds `Release` (trying `x64`, then `x86`, then `Win32`,
   and retrying with `PlatformToolset=v143` and `WindowsTargetPlatformVersion=10.0` when a project
   wants an older toolset), detects the freshly produced `.exe`/`.dll` files, and records everything
   in `report.json`. A broken menu never stops the other menus: its error lands in the report.
4. `kdenlive-mcp` — `pnpm install --frozen-lockfile`, `pnpm build`, `pnpm typecheck`, `pnpm test` on
   `ubuntu-latest`. Artifact: `kdenlive-mcp-dist`.
5. `publish` — runs after the build jobs, only on a push to `main` and also when some builds failed.
   Uploads every produced binary to the rolling release `latest-build` with `--clobber`, plus one zip
   per project artifact and the `BUILD-REPORT.md` summary.

## Download the builds

Release page: <https://github.com/sapkotasachin064-beep/Image-Gui-Collection/releases/tag/latest-build>

- Every built binary is a release asset: `<app>-<platform>-<name>.exe` or `.dll`. Menus appear as
  `<menu-name>-<platform>-<binary>`, for example `Loader-Saturn-dx9-x64-Loader Saturn.exe`.
- `BUILD-REPORT.md` lists which targets built, which failed with their first compiler error, and
  which archives hold no solution.
- Zip assets (`<app>-<platform>-Release.zip`, `kdenlive-mcp-dist.zip`) still exist for the eight
  projects in `src/` and the TypeScript workspace. Per-run artifacts stay under the **Actions** tab.

## Runtime requirements

- Windows 10 or 11. Direct3D 11 and DXGI are part of the operating system, so no DirectX install is
  needed for the DirectX 11 projects.
- The Direct3D 9 projects, and any project that uses D3DX helpers, need the D3DX runtime:
  `d3dx9_43.dll`, `d3dx11_43.dll` and `d3dcompiler_43.dll`. Install the
  [DirectX End-User Runtime](https://www.microsoft.com/download/details.aspx?id=35), or the offline
  `directx_Jun2010_redist.exe` package, and run `DXSETUP.exe /silent`.
- The Visual C++ 2015-2022 Redistributable (`vcruntime140.dll`, `msvcp140.dll`), x64 and x86,
  because the projects link the toolset `v143` runtime dynamically.
- `directx9-cheat-base` links `d3dx9d.lib` in its `Release|Win32` configuration. The linker resolves
  the used symbols from `d3dx9.lib` first, so the release runtime is enough. If a symbol ever
  resolves from the debug import library, the app needs `D3DX9d_43.dll`, which ships only with a full
  DirectX SDK installation and is not redistributable.

## Local builds

Requirements: Visual Studio 2022 with the `v143` toolset and the Windows 10/11 SDK.

```
msbuild "src/drillaware-framework/KingsleydotDev-Drillaware-ImGui-Menu.sln" /p:Configuration=Release /p:Platform=x64
msbuild "src/static-dx11/StaticFramework.sln" /p:Configuration=Release /p:Platform=x86
```

Solution platforms are named `x86` and `x64`; `x86` maps to the project platform `Win32`.

TypeScript workspace:

```
cd src/kdenlive-mcp
pnpm install --frozen-lockfile
pnpm build
```

## Direct3D 9 projects and the D3DX9 SDK

`animated-imgui-loader` and `directx9-cheat-base` include `d3dx9.h` and `d3dx9tex.h` and link
`d3dx9.lib`. Both read the `DXSDK_DIR` environment variable.

- Local option A: install the legacy DirectX SDK (June 2010). The installer sets `DXSDK_DIR`.
- Local option B and the CI path: restore the NuGet package `Microsoft.DXSDK.D3DX` (version
  `9.29.952.8`). Headers live in `build/native/include`, libraries in `build/native/release/lib/<arch>`
  and `build/native/debug/lib/<arch>`.

The CI step `Install D3DX9 SDK (Microsoft.DXSDK.D3DX)` downloads the package, stages it as
`<temp>/sdk/Include`, `<temp>/sdk/Lib/x86` and `<temp>/sdk/Lib/x64`, exports `DXSDK_DIR`, `INCLUDE`
and `LIB` for the matching architecture, and copies the matching-architecture `d3dx9.lib` over the
vendored `src/animated-imgui-loader/d3dx9.lib`. That vendored file is an x64 import library, and the
project folder wins over the SDK search path, which broke the `x86` link before.

`src/animated-imgui-loader/Control-UsetInterface.vcxproj` originally pointed its include path at
`C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include`. It now uses `$(DXSDK_DIR)Include`,
so the project works on any machine that sets `DXSDK_DIR`.

## Known limits

- `directx9-cheat-base` builds for `x86` only. Its solution has no `x64` configurations, and its
  `Release|x64` project configuration carries no D3DX9 include or library paths.
- `src/drillaware-framework/ext/freetype` vendors an x64 FreeType static library
  (`freetype.lib`). Only the `x64` configurations link it, and CI builds `Release` only, because the
  vendored library is a Release build.
- `opengl-base` builds as a dynamic library named `Corrupted.dll` and writes to `Build/` inside the
  project folder.

## `images/`

The ImGui-Menus reference collection: 99 packed menu projects (76 `.rar`, 23 `.zip`), about 380 MB.
The files are tracked, so a fresh clone downloads all of them.

CI builds straight from these archives, so the extracted copies never enter git. If you extract an
archive into `src/` for local work, `.gitignore` keeps the common folders out of commits
(`src/* dx9/`, `src/* dx11/`, `src/* opengl3/`).

## Licensing

- `src/kdenlive-mcp` is Apache-2.0 (upstream: <https://github.com/AMMIROSOH/Kdenlive-mcp>) and keeps
  its license and documentation.
- The other project folders carry no license file from their authors, and the archives in `images/`
  come from third parties without a license. This repository is public, so check the rights before
  redistributing anything from it.
