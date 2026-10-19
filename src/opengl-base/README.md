# ImGui OpenGL Base

C++ DLL template for OpenGL overlays using **ImGui**, **MinHook**, and **mINI**.

Hooks `gdi32!SwapBuffers`, waits for `opengl32.dll` before installing the hook, resolves the target window from the device context, and rebinds when the host recreates its window or context.

Hook exactly one swap entry point. `gdi32!SwapBuffers` and `opengl32!wglSwapBuffers` dispatch into each other, so patching both makes them recurse until the stack is exhausted — the host freezes, then dies. SDL-based hosts reach the driver through gdi32; if you meet one that bypasses gdi32 and calls `wglSwapBuffers` directly, move the hook in `Setup()` rather than adding a second one.

## Controls


| Key      | Action      |
| -------- | ----------- |
| `INSERT` | Toggle menu |
| `END`    | Unload DLL  |


Menu tabs: **Player**, **Visuals**, **Misc** (theme, accent color, save/load config).

## Build

**Visual Studio**

1. Open `ImGui OpenGL Base.sln`.
2. Select **Release  x64**.
3. Build — output is `Build/Corrupted.dll`.

**VS Code / Cursor**

- Default build task: **Release  x64** (`.vscode/tasks.json`).

**Requirements**

- Configuration type: Dynamic Library (`.dll`)
- Windows SDK 10
- Platform toolset: v143
- C++20, Multi-Byte character set
- Link: `opengl32.lib`

## Layout

```
ImGui OpenGL Base/
  src/
    dllmain.cpp
    gui/
      bootstrapper.cpp   — OpenGL hooks, overlay lifetime
      gui.cpp            — menu shell
      pages/             — player, visuals, misc
    game/                — game hooks, offsets, structs (stubs)
    project/             — config (INI), helpers, your features
  ext/                   — imgui, minhook, mINI (read-only)
  Build/                 — Corrupted.dll
```

Put your logic in `src/project/` and game-facing code in `src/game/`. Config saves to `Corrupted.ini` next to the host executable.

The ImGui renderer backend is `imgui_impl_opengl3`, which needs no external loader. The GLSL version it compiles against is pinned by `glslVersion` in `src/gui/bootstrapper.hpp`; drop it to `#version 110` if you hit a host running a strict legacy context.

## Usage

Inject `Build/Corrupted.dll` into an **OpenGL** process.

Screenshot
