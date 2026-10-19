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
#include "logotype.h"

#include "misc/fonts/Montserrat-Medium.h"
#include "misc/fonts/Montserrat-Regular.h"
#include "misc/fonts/Montserrat-ExtraBold.h"
#include "misc/fonts/EvolutionIconsQunion.h"

#include <vector>
#include <imgui_internal.h>
#include <examples/example_win32_directx9/Search.hpp>
#include "options.hpp"

ImFont* weapon_icons;
ImFont* g_pTabsFont;
ImFont* g_pNameFont;
ImFont* g_pMenuFont;
ImFont* g_pLogoFont;
ImFont* g_pIconsisFont;
ImFont* g_pLoaderFont;
ImFont* g_pIconsFont;
ImFont* g_pIconsisFont2;
ImFont* g_sub_sab;
ImFont* g_pIconsWatermarkFont;
ImFont* g_pBigIconsFont;
ImFont* g_pinfoFont;
ImFont* g_pinfoFont2;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* foto = nullptr;

int tab;
int subtab ;
int weapon;
int Rage_Tab ;
int Anti_Tab;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void rage_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void legit_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void anti_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void visuals_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void player_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void misc_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void configs_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void lua_tab()
{
    ImGui::SetCursorPos(ImVec2(210, 120));
    ImGui::MenuChild("Main", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(210, 460));
    ImGui::MenuChild("Settings", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 120));
    ImGui::MenuChild("General", ImVec2(320, 300));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(550, 460));
    ImGui::MenuChild("Exploits", ImVec2(320, 150));
    {

    }
    ImGui::EndChild();
}

void tabs()
{
    ImGui::SetCursorPosX(7);
    ImGui::SetCursorPosY(260);
    ImGui::BeginGroup();
    {

        ImGui::PushFont(g_pLoaderFont);
        ImGui::SetCursorPosX(20);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        ImGui::TextColored(ImVec4(105 / 255.f, 125 / 255.f, 203 / 255.f, ImGui::GetStyle().Alpha), "AimBot");
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont);
        if (ImGui::tab("A", "RageBot", !tab)) tab = 0;
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont2);
        if (ImGui::tab("H", "LegitBot", tab == 1)) tab = 1;
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont);
        if (ImGui::tab("B", "Anti Aim", tab == 2)) tab = 2;
        ImGui::PopFont();

        ImGui::PushFont(g_pLoaderFont);
        ImGui::SetCursorPosX(20);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        ImGui::TextColored(ImVec4(105 / 255.f, 125 / 255.f, 203 / 255.f, ImGui::GetStyle().Alpha), "Visuals");
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont2);
        if (ImGui::tab("J", "Visuals", tab == 3)) tab = 3;
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont);
        if (ImGui::tab("D", "Players", tab == 4)) tab = 4;
        ImGui::PopFont();

        ImGui::PushFont(g_pLoaderFont);
        ImGui::SetCursorPosX(20);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        ImGui::TextColored(ImVec4(105 / 255.f, 125 / 255.f, 203 / 255.f, ImGui::GetStyle().Alpha), "Miscellaneous");
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont2);
        if (ImGui::tab("I", "Misc", tab == 5)) tab = 5;
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont);
        if (ImGui::tab("L", "Configs", tab == 6)) tab = 6;
        ImGui::PopFont();

        ImGui::PushFont(g_pIconsisFont2);
        if (ImGui::tab("O", "Lua's", tab == 7)) tab = 7;
        ImGui::PopFont();
    }
    ImGui::EndGroup();

    switch (tab)
    {
    case 0:     rage_tab();       break;
    case 1:     legit_tab();       break;
    case 2:     anti_tab();       break;
    case 3:     visuals_tab();       break;
    case 4:     player_tab();       break;
    case 5:     misc_tab();       break;
    case 6:     configs_tab();       break;
    case 7:    lua_tab();       break;
    }
}

void sub_tab()
{
    if (tab == 0)
    {

        ImGui::PushFont(g_pIconsisFont);
        ImGui::SetCursorPos(ImVec2(180, 16)); ImGui::BeginGroup();
        {
            if (ImGui::SubTab("A", subtab == 0, ImVec2(94, 53)))
                subtab = 0;
            ImGui::SameLine(100, 0);
            if (ImGui::SubTab("B", subtab == 1, ImVec2(94, 53)))
                subtab = 1;
            ImGui::SameLine(200, 0);
            if (ImGui::SubTab("L", subtab == 2, ImVec2(94, 53)))
                subtab = 2;
        }
        ImGui::PopFont();
        ImGui::EndGroup();
    }
};

std::string comp_name()
{

    char buff[MAX_PATH];
    GetEnvironmentVariableA("USERNAME", buff, MAX_PATH);

    return std::string(buff);
}

int main(int, char**)
{

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_POPUPWINDOW, 0, 0, 1940, 1000, NULL, NULL, wc.hInstance, NULL);

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
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bg_two, sizeof(bg_two), 1920, 1080, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bgs);
    if (logo == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logotype, sizeof(logotype), 1920, 1080, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &logo);
    if (foto == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, fototype, sizeof(fototype), 1920, 1080, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &foto);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImFontConfig icons_config;
    g_pIconsisFont2 = io.Fonts->AddFontFromMemoryTTF(Icons, sizeof(Icons), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pIconsisFont = io.Fonts->AddFontFromMemoryTTF(AbsendIco, sizeof(AbsendIco), 15, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pMenuFont = io.Fonts->AddFontFromMemoryTTF(MontserratExtraBold, sizeof(MontserratExtraBold), 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pLoaderFont = io.Fonts->AddFontFromMemoryTTF(MontserratRegular, sizeof(MontserratRegular), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());

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

        auto& ss = ImGui::GetStyle();
        ss.FrameRounding = 6;
        ss.ChildRounding = 10;
        ss.PopupRounding = 5;
        ss.ScrollbarRounding = 0;
        ss.ScrollbarSize = 5;
        ss.FramePadding = ImVec2(2, 1);

        auto c = ss.Colors;
        c[ImGuiCol_FrameBg] = ImColor(25, 25, 25, 225);
        c[ImGuiCol_FrameBgHovered] = ImColor(25, 25, 25, 225);
        c[ImGuiCol_FrameBgActive] = ImColor(77, 125, 253);
        c[ImGuiCol_Separator] = ImColor(77, 125, 253);
        c[ImGuiCol_SliderGrab] = ImColor(77, 125, 253, 200);
        c[ImGuiCol_SliderGrabActive] = ImColor(77, 125, 253);
        c[ImGuiCol_PopupBg] = ImColor(25, 25, 25, 225);
        c[ImGuiCol_ScrollbarBg] = ImColor(4, 21, 39);
        c[ImGuiCol_ScrollbarGrab] = ImColor(77, 125, 253);
        c[ImGuiCol_ScrollbarGrabActive] = ImColor(77, 125, 253);
        c[ImGuiCol_ScrollbarGrabHovered] = ImColor(77, 125, 253, 200);
        c[ImGuiCol_Header] = ImColor(25, 25, 25, 225);
        c[ImGuiCol_HeaderHovered] = ImColor(77, 125, 253, 200);
        c[ImGuiCol_HeaderActive] = ImColor(77, 125, 253);
        c[ImGuiCol_Button] = ImColor(25, 25, 25, 225);
        c[ImGuiCol_ButtonHovered] = ImColor(77, 125, 253, 200);
        c[ImGuiCol_ButtonActive] = ImColor(77, 125, 253);
        c[ImGuiCol_Text] = ImColor(255, 255, 255);
        c[ImGuiCol_TextDisabled] = ImColor(100, 118, 140);

        {
            auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 916, 646 };
            ImGui::SetNextWindowSize(ImVec2(gs));
            if (ImGui::Begin(("Absend"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
            {
                s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2);
                p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);
                auto draw = ImGui::GetWindowDrawList();

                float dpi_scale = 1.f;

                //Задний фон
                draw->AddRectFilled(ImVec2(p.x + 174, p.y), ImVec2(p.x + 900, p.y + 630), ImColor(18, 18, 18), 5);

                //Верхний суб таб
                draw->AddRectFilled(ImVec2(p.x + 100, p.y), ImVec2(p.x + 900, p.y + 60), ImColor(12, 12, 12), 4);
                //линии за верхний суб таб
                draw->AddRectFilledMultiColor(ImVec2(p.x + 181, p.y + 59), ImVec2(p.x + 900, p.y + 61), ImColor(12, 12, 12), ImColor(12, 12, 12), ImColor(12, 12, 12), ImColor(12, 12, 12));
                draw->AddRectFilledMultiColor(ImVec2(p.x + 181, p.y + 61), ImVec2(p.x + 900, p.y + 64), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(15, 15, 15));

                //левый таб
                draw->AddRectFilled(p, ImVec2(p.x + 180, p.y + 630), ImColor(12, 12, 12), 5);
                //линии за левый таб
                draw->AddRectFilledMultiColor(ImVec2(p.x + 178, p.y + 63), ImVec2(p.x + 182, p.y + 630), ImColor(12, 12, 12), ImColor(12, 12, 12), ImColor(12, 12, 12), ImColor(12, 12, 12));
                draw->AddRectFilledMultiColor(ImVec2(p.x + 181, p.y + 62), ImVec2(p.x + 183, p.y + 630), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(15, 15, 15));

                draw->AddCircleFilled(ImVec2(p.x + 860, p.y + s.y - 600), 20, ImColor(105, 125, 203));

                {
                    tabs();
                    sub_tab();
                }

                {
                    ImGui::SetCursorPos(ImVec2(4, 5));
                    ImGui::Image(logo, ImVec2(ImVec2(110.f * dpi_scale * 1.7f, 35.f * dpi_scale * 6.5f)));
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
