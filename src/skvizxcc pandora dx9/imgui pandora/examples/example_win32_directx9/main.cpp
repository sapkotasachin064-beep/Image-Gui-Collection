// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "bgs.h"

#include "misc/fonts/Montserrat-Regular.h"

#include <vector>
#include <imgui_internal.h>
#include "options.hpp"

ImFont* LexendDecaFont;
ImFont* IconFont;
ImFont* InterRegular;
ImFont* InterRegularTab;
ImFont* InterRegularSubTab;



static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

auto p = ImVec2{};

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* avatar = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

static int tab;
static int subtab;

//	«Ќј„≈Ќ»я	√–”ѕѕ	//

static bool test1 = true;
static bool test2 = false;


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//“абы и суб табы нужно рендерить в imgui_widgers.cpp, но € ленива€ чурка и мне лень передавать шрифты в другой файл

bool renderer_tab(const char* icon, const char* text, bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(text);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = { 214, 30 };

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	ImGui::ItemSize(size, 0);

	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

	int current_tab;

	if (hovered || held)
		ImGui::SetMouseCursor(9);

	float deltatime = 1.5f * ImGui::GetIO().DeltaTime;
	static std::map<ImGuiID, float> hover_animation;
	auto it_hover = hover_animation.find(id)
		;
	if (it_hover == hover_animation.end())
	{
		hover_animation.insert({ id, 0.f });
		it_hover = hover_animation.find(id);
	}

	it_hover->second = ImClamp(it_hover->second + (2.15f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.f)), 0.0f, 0.0f);
	it_hover->second *= ImGui::GetStyle().Alpha;

	static std::map<ImGuiID, float> filled_animation;
	auto it_filled = filled_animation.find(id);

	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id, 0.f });
		it_filled = filled_animation.find(id);
	}

	it_filled->second = ImClamp(it_filled->second + (2.15f * ImGui::GetIO().DeltaTime * (selected ? 1.f : -1.5f)), it_hover->second, 1.f);
	it_filled->second *= ImGui::GetStyle().Alpha;

	if (selected) {
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bb.Min.x - 3, bb.Min.y + 2), ImVec2(bb.Max.x - 100 + 35, bb.Max.y + 5), ImColor(26, 25, 27, int(255 * ImGui::GetStyle().Alpha)), 8);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bb.Min.x - 3, bb.Min.y + 16 - 4 + 1), ImVec2(bb.Min.x, bb.Min.y + 12 + 16 - 4 + 1), ImColor(164, 157, 198, int(255 * ImGui::GetStyle().Alpha)), 8, ImDrawCornerFlags_Right);
	}

	ImGui::PushFont(InterRegularTab);
	ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 5 + 27, bb.Min.y + 7 + 1), selected ? ImColor(160, 160, 160, int(255 * ImGui::GetStyle().Alpha)) : ImColor(77, 77, 77, int(255 * ImGui::GetStyle().Alpha)), text);
	ImGui::PopFont();

	ImGui::PushFont(IconFont);
	ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + 7 + 1), selected ? ImColor(160, 160, 160, int(255 * ImGui::GetStyle().Alpha)) : ImColor(77, 77, 77, int(255 * ImGui::GetStyle().Alpha)), icon);
	ImGui::PopFont();

	return pressed;
}

bool renderer_subtab(const char* label, const ImVec2& size_arg, const bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	static float sizeplus = 0.f;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (hovered || held)
		ImGui::SetMouseCursor(7);

	if (selected) {

		ImGui::PushFont(InterRegularSubTab);
		window->DrawList->AddText(ImVec2(bb.Min.x, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(160, 160, 160), label);
		ImGui::PopFont();
	}
	else {
		ImGui::PushFont(InterRegularSubTab);
		window->DrawList->AddText(ImVec2(bb.Min.x, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(77, 77, 77), label);
		ImGui::PopFont();
	}

	return pressed;
}

void rage()
{
    if (subtab == 0) {
        ImGui::PushFont(InterRegularSubTab);

        ImGui::SetCursorPos(ImVec2(188, 82));
        ImGui::MenuChild("General", ImVec2(260, 366 + 16));
        {
            auto settings = &opt.ragebot[0];
			ImGui::SetCursorPosY(35 - 13);
			ImGui::ChildCheckbox("Test1", &test1);
			ImGui::ChildCheckbox("Test2", &test2);

        }
        ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(188 + 285 - 4, 82));
		ImGui::MenuChild("Accuracy", ImVec2(260, 366 + 16));
		{
			auto settings = &opt.ragebot[0];
		}
		ImGui::EndChild();

        ImGui::PopFont();
    }
}

void render_tab()
{
    {
        ImGui::SetCursorPos(ImVec2{ 15, 90 });
        if (renderer_tab("Q", "RageBot", !tab)) tab = 0;

        ImGui::SetCursorPos(ImVec2{ 15, 90 + 40 });
        if (renderer_tab("W", "AntiAim", tab == 1)) tab = 1;

        ImGui::SetCursorPos(ImVec2{ 15, 90 + 40 + 40 });
        if (renderer_tab("E", "LegitBot", tab == 2)) tab = 2;

        ImGui::SetCursorPos(ImVec2{ 15, 90 + 40 + 40 + 40 });
        if (renderer_tab("R", "Visuals", tab == 3)) tab = 3;

        ImGui::SetCursorPos(ImVec2{ 15, 90 + 40 + 40 + 40 + 40 });
        if (renderer_tab("T", "Misc", tab == 4)) tab = 4;

        ImGui::SetCursorPos(ImVec2{ 15, 90 + 40 + 40 + 40 + 40 + 40 });
        if (renderer_tab("U", "Configs", tab == 5)) tab = 5;

		switch (tab)
		{
			case 0: rage(); break;

		}
    }
}

void render_subtab()
{
	if (tab == 0)
	{
		ImGui::PushFont(InterRegularSubTab);
		ImGui::SetCursorPos(ImVec2(197, 30));
		if (ImGui::SubTab("Scout", subtab == 0)) subtab = 0; ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(197 + 62, 30));
		if (ImGui::SubTab("Scar", subtab == 1)) subtab = 1; ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(197 + 63 + 37 + 17, 30));
		if (ImGui::SubTab("Awp", subtab == 2)) subtab = 2; ImGui::SameLine();

		ImGui::PopFont();
	}
}

int main(int, char**)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_POPUPWINDOW, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, wc.hInstance, NULL);

	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (bgs == nullptr)
		D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bg_type, sizeof(bg_type), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bgs);

	if (avatar == nullptr)
		D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, avatar_byte, sizeof(avatar_byte), 30, 30, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &avatar);


	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	ImFontConfig icons_config;
	IconFont = io.Fonts->AddFontFromMemoryTTF(Icons, sizeof(Icons), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//LexendDecaFont = io.Fonts->AddFontFromMemoryTTF(LexendDeca, sizeof(LexendDeca), 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
	InterRegular = io.Fonts->AddFontFromMemoryTTF(InterRegularByte, sizeof(InterRegularByte), 23, NULL, io.Fonts->GetGlyphRangesCyrillic());
	InterRegularTab = io.Fonts->AddFontFromMemoryTTF(InterRegularByte, sizeof(InterRegularByte), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
	InterRegularSubTab = io.Fonts->AddFontFromMemoryTTF(InterRegularByte, sizeof(InterRegularByte), 15, NULL, io.Fonts->GetGlyphRangesCyrillic());

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(-1, -1));
		ImGui::SetNextWindowSizeConstraints(ImVec2(w, h), ImVec2(w, h));
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::Begin("bg", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
		{
			auto Render = ImGui::GetWindowDrawList();
			ImVec2 p = ImGui::GetWindowPos();
			Render->AddImage(bgs, p, ImVec2(p.x + w, p.y + h));
		}
		ImGui::End();
		ImGui::PopStyleVar();

		{
			ImGui::SetNextWindowSize(ImVec2(ImVec2{ 765, 490 })); //размер виртуальных рамок менюшки (обычно + 10 пискелей к размерам окна)
			if (ImGui::Begin(("###"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
			{
				p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);

				// это лучше перенести в decorations, а не рендерить в бегине. »менно из за этого у теб€ весь рендер отстаЄт от границы

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 750, p.y + 475), ImColor(26, 25, 27), 10); //фон

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 750, p.y + 60), ImColor(19, 19, 19), 10, ImDrawCornerFlags_Top); //верхн€€ полоска

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 60), ImVec2(p.x + 160, p.y + 415 + 60), ImColor(19, 19, 19), 10, ImDrawCornerFlags_BotLeft); //лева€ полоска

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 60), ImVec2(p.x + 750, p.y + 61), ImColor(34, 32, 34)); //полоска между верхней и левой

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 475 - 45), ImVec2(p.x + 160, p.y + 475 - 45 + 1), ImColor(34, 32, 34)); //полоска между левой и аватаркой

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 160, p.y + 60), ImVec2(p.x + 161, p.y + 415 + 60), ImColor(34, 32, 34)); //полоска справа от левой

				ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x, p.y), ImVec2(p.x + 750, p.y + 475), ImColor(34, 32, 34), 10); //обводка

				ImGui::GetWindowDrawList()->AddText(InterRegular, 23, ImVec2(p.x + 42 + 2, p.y + 20 - 4), ImColor(164, 157, 198), "pandora");//Tab name #1

				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + 22, p.y + 475 - 22), 15, ImColor(164, 157, 198), 20); //круг под аватаркой

				ImGui::GetWindowDrawList()->AddImage(avatar, ImVec2(p.x + 22 + 2 - 19 + 1 + 3, p.y + 475 - 30 + 2 - 1 - 6), ImVec2(p.x + 22 + 2 + 30 - 19 - 3 + 3, p.y + 475 - 30 + 2 + 30 - 4 - 7)); //аватарка

				ImGui::GetWindowDrawList()->AddText(InterRegular, 15, ImVec2(p.x + 43, p.y + 475 - 40 + 4), ImColor(160, 160, 160), "Skvizxc");//ник

				ImGui::GetWindowDrawList()->AddText(InterRegular, 15, ImVec2(p.x + 43 + 0.5f, p.y + 475 - 40 + 18), ImColor(77, 77, 77), "Till: 20.04.2023");//верси€

				{
					render_subtab();
					render_tab();
				}

			}
			ImGui::End();
		}

		// Rendering
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	return true;
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
