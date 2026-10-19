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
ImFont* user_info;
ImFont* g_pNameFont;
ImFont* g_pMenuFont;
ImFont* g_pMenuSub;
ImFont* g_pLogoFont;
ImFont* g_pIconsisFont;
ImFont* g_pLoaderFont;
ImFont* g_lodvmf_d;
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

auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 1090, 1200 };

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* foto = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

static int tab;
static int Rage_Tab;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::string comp_name()
{
    char buff[MAX_PATH];
    GetEnvironmentVariableA("USERNAME", buff, MAX_PATH);

    return std::string(buff);
}

void rage_tab()
{
    if (Rage_Tab == 0)
    {
        ImGui::SetCursorPos(ImVec2(35, 170));
        ImGui::MenuChild("General", ImVec2(230, 325));
        {
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(295, 170));
        ImGui::MenuChild("Other", ImVec2(230, 325));
        {
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(555, 170));
        ImGui::MenuChild("Weapon", ImVec2(230, 325));
        {

        }
        ImGui::EndChild();
    }

    if (Rage_Tab == 1)
    {
     
    }
}

void sub_tabs()
{
    if (tab == 0)
    {
        ImGui::SetCursorPos(ImVec2{ 65 , 90 });
        if (ImGui::tab("  General", "  ", !Rage_Tab)) Rage_Tab = 0;

        ImGui::SetCursorPos(ImVec2{ 215 , 90 });
        if (ImGui::tab("   Exploit", "   ", Rage_Tab == 1)) Rage_Tab = 1;

        ImGui::SetCursorPos(ImVec2{ 365 , 90 });
        if (ImGui::tab(" Weapon", "     ", Rage_Tab == 2)) Rage_Tab = 2;

        ImGui::SetCursorPos(ImVec2{ 505 , 90 });
        if (ImGui::tab("Anti-Aim", "       ", Rage_Tab == 3)) Rage_Tab = 3;

        ImGui::SetCursorPos(ImVec2{ 645 , 90 });
        if (ImGui::tab("Fake-Lag", "      ", Rage_Tab == 4)) Rage_Tab = 4;
    }
}


void tabs()
{
    ImGui::SetCursorPos(ImVec2{ 160 , 28 });
    if (ImGui::tab("      RAGE", "A", !tab)) tab = 0;

    ImGui::SetCursorPos(ImVec2{ 290 , 28 });
    if (ImGui::tab("    VISUAL", "C", tab == 1)) tab = 1;

    ImGui::SetCursorPos(ImVec2{ 420 , 28 });
    if (ImGui::tab("      MISC", "I", tab == 2)) tab = 2;

    ImGui::SetCursorPos(ImVec2{ 550 , 28 });
    if (ImGui::tab("      MAIN", "K", tab == 3)) tab = 3;

    switch (tab)
    {
          case 0:     rage_tab();       break;
    }
}

void derectory()
{
    s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2);
    p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);
    auto draw = ImGui::GetWindowDrawList();

    draw->AddRect(ImVec2(p.x + 819, p.y - 1), ImVec2(p.x + 1051, p.y + 351), ImColor(40, 40, 40), 6);
    draw->AddRect(ImVec2(p.x + 818, p.y - 2), ImVec2(p.x + 1052, p.y + 352), ImColor(25, 25, 25), 6);
    draw->AddRectFilled(ImVec2(p.x + 820, p.y ), ImVec2(p.x + 1050, p.y + 350), ImColor(20, 20, 20), 6);
};

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
    g_pLoaderFont = io.Fonts->AddFontFromMemoryTTF(MontserratExtraBold, sizeof(MontserratExtraBold), 26, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_lodvmf_d = io.Fonts->AddFontFromMemoryTTF(MontserratExtraBold, sizeof(MontserratExtraBold), 12, NULL, io.Fonts->GetGlyphRangesCyrillic());
    user_info = io.Fonts->AddFontFromMemoryTTF(MontserratExtraBold, sizeof(MontserratExtraBold), 12, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::GetIO().FontIcons = g_pIconsFont;
    ImGui::GetIO().Font = g_pLoaderFont;
    ImGui::GetIO().Water = g_pIconsFont;

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
            ImGui::SetNextWindowSize(ImVec2(gs));
            if (ImGui::Begin(("###"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
            {
                s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2);
                p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);
                auto draw = ImGui::GetWindowDrawList();

                draw->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 800, p.y + 500), ImColor(20, 20, 20), 6);
                draw->AddRect(ImVec2(p.x - 1, p.y - 1), ImVec2(p.x + 801, p.y + 501), ImColor(40, 40, 40), 6);
                draw->AddRect(ImVec2(p.x - 2, p.y - 2), ImVec2(p.x + 802, p.y + 502), ImColor(25, 25, 25), 6);
                draw->AddRectFilled(ImVec2(p.x + 134, p.y), ImVec2(p.x + 136, p.y + 70), ImColor(40, 40, 40));

                {
                   tabs();
                   sub_tabs();
                }

                draw->AddRectFilled(ImVec2(p.x, p.y + 70), ImVec2(p.x + 800, p.y + 72), ImColor(40, 40, 40));
                draw->AddRectFilled(ImVec2(p.x, p.y + 120), ImVec2(p.x + 800, p.y + 122), ImColor(40, 40, 40));

                draw->AddRectFilled(ImVec2(p.x + 668, p.y), ImVec2(p.x + 670, p.y + 70), ImColor(40, 40, 40));
                draw->AddRectFilled(ImVec2(p.x + 737, p.y + 34), ImVec2(p.x + 792, p.y + 36), ImColor(40, 40, 40));

                ImGui::PushFont(user_info);
                draw->AddText(ImVec2(p.x + 745, p.y + 15), ImColor(250, 166, 24), "name");
                draw->AddText(ImVec2(p.x + 734, p.y + 42), ImColor(100, 100, 100), "12:34:23");
                ImGui::PopFont();

                ImGui::SetCursorPos(ImVec2(680, 15));
                ImGui::Image(foto, ImVec2(ImVec2(60.f, 55.f)));
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
