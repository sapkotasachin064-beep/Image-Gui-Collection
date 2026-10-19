#pragma once

#include <Windows.h>

inline constexpr const char* glslVersion = "#version 130";

inline bool isSetup = false;
inline bool shuttingDown = false;

inline HWND window = nullptr;
inline WNDPROC originalWindowProcess = nullptr;

void Setup();
void Destroy() noexcept;

using SwapBuffersFn = BOOL(__stdcall*)(HDC);
inline SwapBuffersFn SwapBuffersOriginal = nullptr;
BOOL __stdcall SwapBuffersHook(HDC deviceContext) noexcept;

LRESULT CALLBACK WindowProcess(
	HWND hwnd,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam);
