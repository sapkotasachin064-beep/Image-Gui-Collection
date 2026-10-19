# Image GUI Collection

Monorepo of ImGui-based GUI apps plus one TypeScript/pnpm project. Every app builds in GitHub Actions on every push to `main`.

## Apps

| App | Path | Tech | Build output |
| --- | --- | --- | --- |
| Drillaware framework | `apps/drillaware-framework` | C++ / ImGui / DX11 (v143) | `KingsleydotDev-Drillaware-ImGui-Menu.exe` |
| Chudvision framework | `apps/chudvision-framework` | C++ / ImGui / DX11 (v143) | `ImGui-Chudvision-Framework-KingsleydotDev.exe` |
| Chudvision menu | `apps/chudvision-menu` | C++ / ImGui / DX11 (v143) | `ImGui-Chudvision-Framework-KingsleydotDev.exe` |
| OpenGL base | `apps/opengl-base` | C++ / ImGui / OpenGL / MinHook / mINI | `Corrupted.dll` |
| MemeSense DX11 | `apps/memesense-dx11` | C++ / ImGui / DX11 (v143) | `MemeSenseFramework.exe` |
| Static DX11 | `apps/static-dx11` | C++ / ImGui / DX11 (v143) | `StaticFramework.exe` |
| Kdenlive MCP | `apps/kdenlive-mcp` | TypeScript / pnpm workspace / Node 22 | `packages/*/dist` |

## CI workflow

`.github/workflows/build.yml` runs two jobs:

- `native` — matrix over the 6 C++ apps, each built with MSBuild in `Release` for `x64` and `x86` on `windows-latest`. One artifact per app and platform, named `<app>-<platform>-Release`.
- `kdenlive-mcp` — `pnpm install --frozen-lockfile`, `pnpm build`, `pnpm typecheck`, `pnpm test` on `ubuntu-latest`, with a `dist` artifact.

Run a manual build with **Actions → Build all apps → Run workflow**.

## Local builds

C++ apps need Visual Studio 2022 with the `v143` toolset and the Windows 10/11 SDK:

```
msbuild "apps/drillaware-framework/KingsleydotDev-Drillaware-ImGui-Menu.sln" /p:Configuration=Release /p:Platform=x64
```

Kdenlive MCP needs Node 22 and pnpm 9.5:

```
cd apps/kdenlive-mcp
pnpm install --frozen-lockfile
pnpm build
```

## Notes

- `apps/drillaware-framework/ext/freetype` vendors an x64 FreeType static lib (`freetype.lib`). Only the `x64` configurations link it, so `x64` builds follow the project's original setup. `Debug|x64` is not built in CI because the vendored lib is a Release build.
- Solution platforms are named `x86`/`x64`; `x86` maps to the project's `Win32` platform.
- The six C++ app folders carry no license file from their original authors, so this repository stays private. `apps/kdenlive-mcp` is Apache-2.0 (upstream: AMMIROSOH/Kdenlive-mcp) and keeps its license, docs, and its own nested workflow files, which GitHub ignores at this path.
