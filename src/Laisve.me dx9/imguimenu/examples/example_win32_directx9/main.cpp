// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "customgui.h"
#include "customfont.h"

; namespace GUIHelper
{
    void Spacing(int amount)
    {
        for (int i = 0; i < amount; i++)
        {
            ImGui::Spacing();
        }
    }

    void News(const char* label, const char* news)
    {
        Spacing(4);
        ImGui::SameLine(12);
        CustomGUI::News(label, news);
    }
};


; int main(int, char**)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

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

    CustomGUI::fonts::SFUIlight_default = io.Fonts->AddFontFromMemoryCompressedTTF(light_compressed_data, light_compressed_size, 14);
    CustomGUI::fonts::SFUIlight_big = io.Fonts->AddFontFromMemoryCompressedTTF(light_compressed_data, light_compressed_size, 18);

    CustomGUI::fonts::SFUIbold_default = io.Fonts->AddFontFromMemoryCompressedTTF(bold_compressed_data, bold_compressed_size, 12);
    CustomGUI::fonts::SFUIbold_big = io.Fonts->AddFontFromMemoryCompressedTTF(bold_compressed_data, bold_compressed_size, 18);
    CustomGUI::fonts::SFUIbold_bigger = io.Fonts->AddFontFromMemoryCompressedTTF(bold_compressed_data, bold_compressed_size, 20);

    CustomGUI::fonts::ICONS_default = io.Fonts->AddFontFromMemoryCompressedTTF(icons_compressed_data,icons_compressed_size,14);
    CustomGUI::fonts::ICONS_big = io.Fonts->AddFontFromMemoryCompressedTTF(icons_compressed_data, icons_compressed_size, 32);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    bool show_demo_window = true;
    bool show_another_window = false;
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
        ImVec2 position;
        ImDrawList* draw;

        static int tab = 0;
        static int subtab = 0;

        // FLAGS
        auto window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        // ATRIBUTES
        ImGui::SetNextWindowSize({ 800,550 });

        // STYLES
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 1);

        // COLORS
        ImGui::PushStyleColor(ImGuiCol_WindowBg, { 235 / 255.f,235 / 255.f,235 / 255.f,255  / 255.f });
        ImGui::PushStyleColor(ImGuiCol_Text, { 70 / 255.f,70 / 255.f,70 / 255.f,255.f / 255.f });
        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, { 35.f / 255.f,35.f / 255.f,35.f / 255.f,0 / 255.f });

        ImGui::Begin("Another Window", &show_another_window, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        {
            position = ImGui::GetWindowPos();
            draw = ImGui::GetWindowDrawList();

            draw->AddRectFilled(ImVec2{ position.x + 180,position.y + 50 }, ImVec2{ position.x + 800,position.y + 550 }, ImColor(229 / 255.f, 229 / 255.f, 231 / 255.f, 255 / 255.f),4,ImDrawCornerFlags_BotRight);
            draw->AddLine(ImVec2{ position.x + 180,position.y }, ImVec2{ position.x + 180,position.y + 550 }, ImColor(120 / 255.f, 120 / 255.f, 120 / 255.f, 70.f / 255.f));
            draw->AddLine(ImVec2{ position.x,position.y + 50 }, ImVec2{ position.x + 800,position.y + 50 }, ImColor(120 / 255.f, 120 / 255.f, 120 / 255.f, 70.f / 255.f));


            draw->AddLine(ImVec2{ position.x + 200,position.y + 90 }, ImVec2{ position.x + 780,position.y + 90 }, ImColor(120 / 255.f, 120 / 255.f, 120 / 255.f, 100.f / 255.f));

            draw->AddText({ position.x + 20,position.y + 70 }, ImColor(70 / 255.f, 70 / 255.f, 70 / 255.f, 255.f / 255.f), "FEATURES");
            draw->AddText({ position.x + 20,position.y + 210 }, ImColor(70 / 255.f, 70 / 255.f, 70 / 255.f, 255.f / 255.f), "INFORMATION");

            ImGui::PushFont(CustomGUI::fonts::SFUIbold_bigger);
            draw->AddText({ position.x + 20,position.y + 50 / 2 - ImGui::CalcTextSize("Laisve.me").y/2 }, ImColor(70 / 255.f, 70 / 255.f, 70 / 255.f, 255.f / 255.f), "Laisve.me");
            ImGui::PopFont();

            {
                ImGui::SetCursorPos({ 20, 90 });
                if (CustomGUI::Tab("Ragebot", "1", tab == 0, { 140,25 }))
                    tab = 0;

                ImGui::SetCursorPos({ 20, 120 });
                if (CustomGUI::Tab("Visuals", "2", tab == 1, { 140,25 }))
                    tab = 1;

                ImGui::SetCursorPos({ 20, 150 });
                if (CustomGUI::Tab("Misc", "3", tab == 2, { 140,25 }))
                    tab = 2;

                ImGui::SetCursorPos({ 20, 180 });
                if (CustomGUI::Tab("Settings", "4", tab == 3, { 140,25 }))
                    tab = 3;

                ImGui::SetCursorPos({ 20, 230 });
                if (CustomGUI::Tab("Dashboard", "a", tab == 4, { 140,25 }))
                    tab = 4;
            }
            {
                if (tab == 0)
                {
                    ImGui::SetCursorPos({ 200, 66 });
                    if (CustomGUI::SubTab("AIMBOT", subtab == 0, { 63,25 }))
                        subtab = 0;

                    ImGui::SetCursorPos({ 270, 66 });
                    if (CustomGUI::SubTab("ANTI-AIMS", subtab == 1, { 77,25 }))
                        subtab = 1;
                }
                if (tab == 1)
                {
                    ImGui::SetCursorPos({ 200, 66 });
                    if (CustomGUI::SubTab("ESP", subtab == 0, { 41,25 }))
                        subtab = 0;

                    ImGui::SetCursorPos({ 246, 66 });
                    if (CustomGUI::SubTab("CHAMS", subtab == 1, { 62,25 }))
                        subtab = 1;


                    ImGui::SetCursorPos({ 246 + 62 + 5, 66 });
                    if (CustomGUI::SubTab("GLOW", subtab == 2, { 55,25 }))
                        subtab = 2;
                }
                if (tab == 4)
                {
                    ImGui::SetCursorPos({ 200, 66 });
                    if (CustomGUI::SubTab("NEWS", subtab == 0, { 55,25 }))
                        subtab = 0;
                }
            }
            {
                ImGui::SetCursorPos({ 200, 100 });
                ImGui::BeginChild("",ImVec2(580,440));
                {
                    if (tab == 0)
                    {
                        if (subtab == 0)
                        {
                            static bool checkbox = false;
                            static int combo = 0;
                            static int slider = 0;
                            static float slider2 = 0;
                            CustomGUI::Checkbox("Enable Ragebot", &checkbox);
                            ImGui::SameLine();
                            CustomGUI::Checkbox("Auto-Shot", &checkbox);
                   
                            CustomGUI::Combo("Combo", &combo, ("DEFAULT\0\rSOMETHING\0\0"));
                            ImGui::SameLine();
                            CustomGUI::Combo("Combo 2", &combo, ("DEFAULT\0\rSOMETHING\0\0"));

                            CustomGUI::SliderInt("Slider Int", &slider, 0, 1000, "%.3f");
                            ImGui::SameLine(294);
                            CustomGUI::SliderInt("Slider In 2t", &slider, 0, 1000, "%.3f");
                        }

                    }

                    if (tab == 4)
                    {
                        if (subtab == 0)
                        {
                            CustomGUI::News("Release", "Yeah");
                        }
                    }
                }
                ImGui::EndChild();
            }
        }
        ImGui::End();

        // STYLES
        ImGui::PopStyleVar(4);

        // COLORS
        ImGui::PopStyleColor(3);

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
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
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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
