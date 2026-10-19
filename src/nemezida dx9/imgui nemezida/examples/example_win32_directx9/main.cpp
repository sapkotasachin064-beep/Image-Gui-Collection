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
#include "misc/fonts/Montserrat-Regular.h"
#include "bytes4.h"
#include <vector>
#include <imgui_internal.h>
#include <string>

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_POPUPWINDOW, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    auto& styles = ImGui::GetStyle();
    styles.FrameRounding = 6;
    styles.ChildRounding = 10;
    styles.PopupRounding = 5;

    ImFont* nem_main{};
    ImFont* nem_default{};
    ImFont* nem_tab_icon{};

    nem_main = io.Fonts->AddFontFromMemoryTTF(SFUIDisplay_SemiBold, sizeof(SFUIDisplay_SemiBold), 14.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    nem_tab_icon = io.Fonts->AddFontFromMemoryTTF(Nema1488_, sizeof(Nema1488_), 25.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    nem_default = io.Fonts->AddFontFromMemoryTTF(PENA88895, sizeof(PENA88895), 25.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

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
        {
            ImGui::SetNextWindowSize(ImVec2(800, 515.f), ImGuiCond_Once);
            ImGui::SetNextWindowBgAlpha(0.f);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_DisplayRGB);
            ImGui::Begin("###", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar);
            {
                static int nem_tab_sel{};
                auto draw = ImGui::GetWindowDrawList();
                ImVec2 pos = ImGui::GetWindowPos();

                // background 1

                draw->AddRectFilled(ImVec2(pos.x + 50, pos.y + 1), ImVec2(pos.x + 211, pos.y + 514), ImColor(32, 32, 32, 255));
                draw->AddRectFilled(ImVec2(pos.x + 210, pos.y + 1), ImVec2(pos.x + 795, pos.y + 514), ImColor(26, 26, 26, 255));
                draw->AddText(nem_default, 27, ImVec2(pos.x + 65, pos.y + 12),
                    ImColor(200, 200, 200),
                    "Nemezida__");

                // information

                draw->AddRectFilled(ImVec2(pos.x + 60, pos.y + 466), ImVec2(pos.x + 200, pos.y + 503), ImColor(41, 41, 41, 255), 5.f, ImDrawCornerFlags_All);
                draw->AddRect(ImVec2(pos.x + 60, pos.y + 466), ImVec2(pos.x + 200, pos.y + 503), ImColor(50, 50, 50, 255), 5.f, ImDrawCornerFlags_All);

                // information text

                draw->AddText(nem_default,
                    16,
                    ImVec2(pos.x + 105, pos.y + 468),
                    ImColor(180, 180, 180),
                    "Javaw");

                draw->AddText(nem_default,
                    16,
                    ImVec2(pos.x + 105, pos.y + 483),
                    ImColor(95, 95, 95),
                    "Till: ");

                draw->AddText(nem_default,
                    16,
                    ImVec2(pos.x + 125, pos.y + 483),
                    ImColor(180, 180, 180),
                    "Never");

                // Aimbot

                draw->AddText(nem_default, 17, ImVec2(pos.x + 65, pos.y + 59), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Aimbot");

                ImGui::SetCursorPos(ImVec2(62, 79));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "e", "Ragebot", nem_tab_sel == 0)) {
                    nem_tab_sel = 0;
                }

                ImGui::SetCursorPos(ImVec2(62, 113));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "a", "Anti-hit", nem_tab_sel == 1)) {
                    nem_tab_sel = 1;
                }

                // Visuals

                draw->AddText(nem_default, 17, ImVec2(pos.x + 65, pos.y + 153), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Visuals");

                ImGui::SetCursorPos(ImVec2(62, 173));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "x", "Players", nem_tab_sel == 2)) {
                    nem_tab_sel = 2;
                }

                ImGui::SetCursorPos(ImVec2(62, 206));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "w", "World", nem_tab_sel == 3)) {
                    nem_tab_sel = 3;
                }

                ImGui::SetCursorPos(ImVec2(62, 240));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "v", "View", nem_tab_sel == 4)) {
                    nem_tab_sel = 4;
                }

                // Miscellaneous

                draw->AddText(nem_default, 17, ImVec2(pos.x + 65, pos.y + 279), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Miscellaneous");

                ImGui::SetCursorPos(ImVec2(62, 299));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "z", "Misc", nem_tab_sel == 5)) {
                    nem_tab_sel = 5;
                }

                ImGui::SetCursorPos(ImVec2(62, 334));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "s", "Skins", nem_tab_sel == 6)) {
                    nem_tab_sel = 6;
                }

                ImGui::SetCursorPos(ImVec2(62, 367));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "c", "Configs", nem_tab_sel == 7)) {
                    nem_tab_sel = 7;
                }

                // Scripts

                draw->AddText(nem_default, 17, ImVec2(pos.x + 65, pos.y + 404), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Scripts");

                ImGui::SetCursorPos(ImVec2(62, 427));
                if (ImGui::Rendertab(nem_tab_icon, nem_default, "l", "Luas", nem_tab_sel == 8)) {
                    nem_tab_sel = 8;
                }
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }
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

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));

    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}