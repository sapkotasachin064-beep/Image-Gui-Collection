#include "bootstrapper.hpp"
#include "gui.hpp"
#include "pages/misc/misc.hpp"

#include <stdexcept>

#include "../../ext/minhook/minhook.h"
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_opengl3.h"

#include <gl/GL.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hwnd,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam);

static bool WaitForOpenGL(const int maxAttempts, const DWORD sleepMs) noexcept
{
	for (int attempt = 0; attempt < maxAttempts; ++attempt)
	{
		const HMODULE openglModule = GetModuleHandleA("opengl32.dll");
		if (openglModule != nullptr && GetProcAddress(openglModule, "wglSwapBuffers") != nullptr)
		{
			return true;
		}

		Sleep(sleepMs);
	}

	return false;
}

static void TeardownOverlay() noexcept
{
	if (!isSetup && ImGui::GetCurrentContext() == nullptr)
	{
		return;
	}

	if (ImGui::GetCurrentContext() != nullptr)
	{
		const ImGuiIO& io = ImGui::GetIO();
		if (io.BackendRendererUserData != nullptr)
		{
			ImGui_ImplOpenGL3_Shutdown();
		}
		if (io.BackendPlatformUserData != nullptr)
		{
			ImGui_ImplWin32_Shutdown();
		}
		ImGui::DestroyContext();
	}

	if (window != nullptr && originalWindowProcess != nullptr)
	{
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWindowProcess));
	}

	window = nullptr;
	originalWindowProcess = nullptr;
	isSetup = false;
}

static bool TryReattachWindow(const HWND newWindow) noexcept
{
	if (newWindow == nullptr)
	{
		return false;
	}

	if (newWindow == window)
	{
		return true;
	}

	if (window != nullptr && originalWindowProcess != nullptr)
	{
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWindowProcess));
	}

	ImGui_ImplWin32_Shutdown();

	window = newWindow;
	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
	);

	return ImGui_ImplWin32_Init(window) != 0;
}

static void SetupOverlay(const HDC deviceContext) noexcept
{
	const HWND targetWindow = WindowFromDC(deviceContext);
	if (targetWindow == nullptr)
	{
		return;
	}

	window = targetWindow;
	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
	);

	ImGui::CreateContext();
	gui::pages::ApplyTheme(vars::iTheme);

	if (!ImGui_ImplWin32_Init(window))
	{
		TeardownOverlay();
		return;
	}

	if (!ImGui_ImplOpenGL3_Init(glslVersion))
	{
		TeardownOverlay();
		return;
	}

	isSetup = true;
}

static void RenderOverlay(const HDC deviceContext) noexcept
{
	if (!isSetup)
	{
		SetupOverlay(deviceContext);
	}

	if (!isSetup)
	{
		return;
	}

	if (!TryReattachWindow(WindowFromDC(deviceContext)))
	{
		TeardownOverlay();
		return;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureKeyboard = gui::open;
	io.WantCaptureMouse = gui::open;
	io.MouseDrawCursor = gui::open;

	ImGui::NewFrame();

	if (gui::open)
	{
		gui::Render();
	}

	ImGui::EndFrame();
	ImGui::Render();

	const GLboolean wasAlphaTestEnabled = glIsEnabled(GL_ALPHA_TEST);
	if (wasAlphaTestEnabled)
	{
		glDisable(GL_ALPHA_TEST);
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (wasAlphaTestEnabled)
	{
		glEnable(GL_ALPHA_TEST);
	}
}

void Setup()
{
	if (MH_Initialize() != MH_OK)
	{
		throw std::runtime_error("Unable to initialize minhook");
	}

	constexpr int maxAttempts = 50;
	constexpr DWORD waitSleepMs = 100;

	if (!WaitForOpenGL(maxAttempts, waitSleepMs))
	{
		throw std::runtime_error("Timed out waiting for opengl32.dll");
	}

	if (MH_CreateHookApi(
		L"gdi32.dll",
		"SwapBuffers",
		&SwapBuffersHook,
		reinterpret_cast<void**>(&SwapBuffersOriginal)) != MH_OK)
	{
		throw std::runtime_error("Unable to hook SwapBuffers()");
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		throw std::runtime_error("Unable to enable hooks");
	}
}

void Destroy() noexcept
{
	if (shuttingDown)
	{
		return;
	}

	shuttingDown = true;

	constexpr int maxAttempts = 100;
	for (int attempt = 0; attempt < maxAttempts && isSetup; ++attempt)
	{
		Sleep(10);
	}

	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(150);

	TeardownOverlay();

	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

BOOL __stdcall SwapBuffersHook(const HDC deviceContext) noexcept
{
	if (shuttingDown)
	{
		TeardownOverlay();
		return SwapBuffersOriginal(deviceContext);
	}

	RenderOverlay(deviceContext);

	return SwapBuffersOriginal(deviceContext);
}

LRESULT CALLBACK WindowProcess(
	const HWND hwnd,
	const UINT message,
	const WPARAM wideParam,
	const LPARAM longParam)
{
	if (shuttingDown)
	{
		if (originalWindowProcess != nullptr)
		{
			return CallWindowProc(
				originalWindowProcess,
				hwnd,
				message,
				wideParam,
				longParam);
		}

		return DefWindowProc(hwnd, message, wideParam, longParam);
	}

	if (message == WM_KEYDOWN && wideParam == VK_INSERT)
	{
		gui::open = !gui::open;
	}

	if (gui::open && ImGui::GetCurrentContext() != nullptr)
	{
		const LRESULT imguiResult = ImGui_ImplWin32_WndProcHandler(
			hwnd,
			message,
			wideParam,
			longParam);

		if (imguiResult != 0)
		{
			return imguiResult;
		}
	}

	return CallWindowProc(
		originalWindowProcess,
		hwnd,
		message,
		wideParam,
		longParam);
}
