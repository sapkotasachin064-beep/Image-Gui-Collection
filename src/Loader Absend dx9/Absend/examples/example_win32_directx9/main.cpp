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

ImFont* g_pTabsFont;
ImFont* g_pNameFont;
ImFont* g_pMenuFont;
ImFont* g_pLogoFont;
ImFont* g_pLoaderFont;
ImFont* g_pIconsFont;
ImFont* g_pIconsWatermarkFont;
ImFont* g_pBigIconsFont;
ImFont* g_pinfoFont;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* foto = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::string comp_name()
{
    char buff[MAX_PATH];
    GetEnvironmentVariableA("USERNAME", buff, MAX_PATH);
    if (std::string(buff) == "wansis")
        std::string(buff) = "w2n515";
    return std::string(buff);
}

void userinformation()
{
    ImGui::SetCursorPos(ImVec2(250, 141));
    ImGui::MenuChild("Some function", ImVec2(310, 200));
    {
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(250, 370));
    ImGui::MenuChild("listboxe's", ImVec2(310, 280));
    {
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(570, 141));
    ImGui::MenuChild("Here combo's", ImVec2(310, 280));
    {
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(570, 450));
    ImGui::MenuChild("multi", ImVec2(310, 200));
    {
    }
    ImGui::EndChild();
}

void Lastupdate()
{
    ImGui::SetCursorPos(ImVec2(250, 141));
    ImGui::MenuChild("Some function", ImVec2(310, 200));
    {
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(250, 370));
    ImGui::MenuChild("listboxe's", ImVec2(310, 280));
    {
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(570, 141));
    ImGui::MenuChild("Here combo's", ImVec2(310, 280));
    {
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(570, 450));
    ImGui::MenuChild("multi", ImVec2(310, 200));
    {
    }
    ImGui::EndChild();
}

int main(int, char**)
{

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_POPUPWINDOW, 0, 0, 1440, 900, NULL, NULL, wc.hInstance, NULL);

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
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logotype, sizeof(logotype), 200, 200, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &logo);
    if (foto == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, fototype, sizeof(fototype), 200, 200, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &foto);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    if (!g_Search.find)
        g_Search.Init();

    ImFontConfig icons_config;
    g_pNameFont = io.Fonts->AddFontFromMemoryTTF(MontserratExtraBold, sizeof(MontserratExtraBold), 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pBigIconsFont = io.Fonts->AddFontFromMemoryTTF(EvolutionIconsQunion, sizeof(EvolutionIconsQunion), 51, &icons_config, io.Fonts->GetGlyphRangesDefault());
    g_pIconsWatermarkFont = io.Fonts->AddFontFromMemoryTTF(EvolutionIconsQunion, sizeof(EvolutionIconsQunion), 28, &icons_config, io.Fonts->GetGlyphRangesDefault());
    g_pIconsFont = io.Fonts->AddFontFromMemoryTTF(EvolutionIconsQunion, sizeof(EvolutionIconsQunion), 28, &icons_config, io.Fonts->GetGlyphRangesDefault());
    g_pTabsFont = io.Fonts->AddFontFromMemoryTTF(MontserratRegular, sizeof(MontserratRegular), 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pLogoFont = io.Fonts->AddFontFromMemoryTTF(MontserratRegular, sizeof(MontserratRegular), 26, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pMenuFont = io.Fonts->AddFontFromMemoryTTF(MontserratRegular, sizeof(MontserratRegular), 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pLoaderFont = io.Fonts->AddFontFromMemoryTTF(MontserratRegular, sizeof(MontserratRegular), 12, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_pinfoFont = io.Fonts->AddFontFromMemoryTTF(MontserratRegular, sizeof(MontserratRegular), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());

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

        {
            auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 920, 690 };
            ImGui::SetNextWindowSize(ImVec2(gs));
            if (ImGui::Begin(("Absend"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
            {
                s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2);
                p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);

                auto draw = ImGui::GetWindowDrawList();

                float dpi_scale = 1.f;

                //Задний фон
                draw->AddRectFilled(ImVec2(p.x + 174, p.y), ImVec2(p.x + 600, p.y + 400), ImColor(19, 19, 19), 8);
                {

                    //левый таб
                    draw->AddRectFilled(p, ImVec2(p.x + 190, p.y + 400), ImColor(19, 19, 19), 8);

                    //верхний таб
                    draw->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 600, p.y + 70), ImColor(15, 15, 15), 8, ImDrawCornerFlags_Top);
                    {
                        draw->AddRectFilledMultiColor(ImVec2(p.x, p.y + 70), ImVec2(p.x + 300, p.y + 71), ImColor(15, 15, 15), ImColor(100, 125, 208), ImColor(100, 125, 208), ImColor(15, 15, 15));
                        draw->AddRectFilledMultiColor(ImVec2(p.x + 300, p.y + 70), ImVec2(p.x + 600, p.y + 71), ImColor(100, 125, 208), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(100, 125, 208));

                        //Лого
                        draw->AddRectFilled(ImVec2(p.x + 10, p.y + 10), ImVec2(p.x + 60, p.y + 60), ImColor(80, 99, 162), 8);

                        ImGui::PushFont(g_pLogoFont);
                        draw->AddText(ImVec2(p.x + 70, p.y + 20), ImColor(200, 200, 200), "ABS");
                        draw->AddText(ImVec2(p.x + 115, p.y + 20), ImColor(100, 125, 208), "END");
                        ImGui::PopFont();

                        ImGui::PushFont(g_pLoaderFont);
                        draw->AddText(ImVec2(p.x + 75, p.y + 45), ImColor(200, 200, 200), "feel the");
                        draw->AddText(ImVec2(p.x + 115, p.y + 45), ImColor(100, 125, 208), "dark side");
                        draw->AddText(ImVec2(p.x + 75, p.y + 10), ImColor(200, 200, 200), "fear and");
                        draw->AddText(ImVec2(p.x + 118, p.y + 10), ImColor(100, 125, 208), "no more");
                        ImGui::PopFont();

                        ImGui::SetCursorPos(ImVec2(10, 10));
                        ImGui::Image(logo, ImVec2(ImVec2(110.f * dpi_scale * 0.6f, 35.f * dpi_scale * 2.0f)));
                    }

                    //user information
                    draw->AddRectFilled(ImVec2(p.x + 10, p.y + 80), ImVec2(p.x + 270, p.y + 230), ImColor(15, 15, 15), 8);
                    {
                        ImGui::PushFont(g_pLoaderFont);
                        draw->AddText(ImVec2(p.x + 84, p.y + 84), ImColor(200, 200, 200), "USER INFORMATION");
                        ImGui::PopFont();

                        draw->AddRectFilledMultiColor(ImVec2(p.x + 10, p.y + 100), ImVec2(p.x + 135, p.y + 101), ImColor(15, 15, 15), ImColor(100, 125, 208), ImColor(100, 125, 208), ImColor(15, 15, 15));
                        draw->AddRectFilledMultiColor(ImVec2(p.x + 135, p.y + 100), ImVec2(p.x + 270, p.y + 101), ImColor(100, 125, 208), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(100, 125, 208));
                    }

                    //loader information
                    draw->AddRectFilled(ImVec2(p.x + 10, p.y + 240), ImVec2(p.x + 270, p.y + 390), ImColor(15, 15, 15), 8);
                    {
                        ImGui::PushFont(g_pLoaderFont);
                        draw->AddText(ImVec2(p.x + 80, p.y + 244), ImColor(200, 200, 200), "LOADER INFORMATION");
                        ImGui::PopFont();

                        draw->AddRectFilledMultiColor(ImVec2(p.x + 10, p.y + 260), ImVec2(p.x + 135, p.y + 261), ImColor(15, 15, 15), ImColor(100, 125, 208), ImColor(100, 125, 208), ImColor(15, 15, 15));
                        draw->AddRectFilledMultiColor(ImVec2(p.x + 135, p.y + 260), ImVec2(p.x + 270, p.y + 261), ImColor(100, 125, 208), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(100, 125, 208));
                    }

                    //last update
                    draw->AddRectFilled(ImVec2(p.x + 280, p.y + 80), ImVec2(p.x + 590, p.y + 390), ImColor(15, 15, 15), 8);
                    {
                        ImGui::PushFont(g_pLoaderFont);
                        draw->AddText(ImVec2(p.x + 397, p.y + 84), ImColor(200, 200, 200), "LAST UPDATE");
                        ImGui::PopFont();

                        draw->AddRectFilledMultiColor(ImVec2(p.x + 280, p.y + 100), ImVec2(p.x + 435, p.y + 101), ImColor(15, 15, 15), ImColor(100, 125, 208), ImColor(100, 125, 208), ImColor(15, 15, 15));
                        draw->AddRectFilledMultiColor(ImVec2(p.x + 435, p.y + 100), ImVec2(p.x + 600, p.y + 101), ImColor(100, 125, 208), ImColor(15, 15, 15), ImColor(15, 15, 15), ImColor(100, 125, 208));
                    }
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
