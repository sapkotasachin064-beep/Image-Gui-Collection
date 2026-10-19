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

#include "misc/fonts/Montserrat-Regular.h"

#include <vector>
#include <imgui_internal.h>
#include <examples/example_win32_directx9/Search.hpp>
#include "options.hpp"

ImFont* LexendDecaFont;
ImFont* IconFont;
ImFont* IconFontLogs;
ImFont* InterMedium;
ImFont* InterMediumone;


static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 662, 460 };

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* circle = nullptr;
IDirect3DTexture9* foto = nullptr;
IDirect3DTexture9* merfthefurry = nullptr;
IDirect3DTexture9* ghoul = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

static int tab;
static int enemy_tab;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void legit()
{
    if (tab == 1)
    {
        ImGui::SetCursorPos(ImVec2(78, 130));
        ImGui::MenuChild("tab 1", ImVec2(280, 325));
        {
            ImGui::Spacing();

            auto settings = &opt.ragebot[0];
            static int procent = 60;
            static int procent2 = 5;
            static char input[128];

            ImGui::Checkbox("Enabled ", &settings->enabled);
            ImGui::Checkbox("Disabled ", &settings->hpbar);

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::Checkbox("Enabled ", &settings->enabled);
            ImGui::Checkbox("Disabled ", &settings->hpbar);

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(370, 130));
        ImGui::MenuChild("tab 2", ImVec2(280, 325));
        {
            ImGui::Spacing();

            auto settings = &opt.ragebot[0];
            static int procent = 0;
            static int procent2 = 100;

            ImGui::Checkbox("Disabled ", &settings->hpbar);
            ImGui::Checkbox("Enabled ", &settings->enabled);

            ImGui::SliderInt("Slider 1 ", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2", &procent2, 0, 100, "%d%%");

            ImGui::Checkbox("Enabled ", &settings->enabled);
            ImGui::Checkbox("Disabled ", &settings->hpbar);

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");

            ImGui::SliderInt("Slider 1", &procent, 0, 100, "%d%%");
            ImGui::SliderInt("Slider 2 ", &procent2, 0, 100, "%d%%");
        }
        ImGui::EndChild();
    }
}

void render_sub()
{
    ImGui::SetCursorPos(ImVec2(90, 30));
    ImGui::Image(circle, ImVec2(40, 40));//cirlce

    if (tab == 1)
    {
        ImGui::GetWindowDrawList()->AddText(InterMedium, 16, ImVec2(p.x + 135, p.y + 25), ImColor(255, 255, 255), "Visual");//name tab
        ImGui::GetWindowDrawList()->AddText(IconFont, 25, ImVec2(p.x + 90, p.y + 29), ImColor(107, 107, 107), "B");//icon

        ImGui::SetCursorPos(ImVec2{ 143, 50 });
        if (ImGui::sub("Enemy", !enemy_tab)) enemy_tab = 0;

       ImGui::SetCursorPos(ImVec2{ 190, 50 });
       if (ImGui::sub("Ally", enemy_tab == 1)) enemy_tab = 1;

       ImGui::SetCursorPos(ImVec2{ 220, 50 });
       if (ImGui::sub("World", enemy_tab == 2)) enemy_tab = 2;
    }
}

void render_tab()
{
    ImGui::PushFont(IconFont);
    {
        ImGui::SetCursorPos(ImVec2{ 26, 90 });
        if (ImGui::tab("A", !tab)) tab = 0;

        ImGui::SetCursorPos(ImVec2{ 26, 136 });
        if (ImGui::tab("B", tab == 1)) tab = 1;

        ImGui::SetCursorPos(ImVec2{ 26, 182 });
        if (ImGui::tab("C", tab == 2)) tab = 2;

        ImGui::SetCursorPos(ImVec2{ 26, 228 });
        if (ImGui::tab("D", tab == 3)) tab = 3;

        ImGui::SetCursorPos(ImVec2{ 26, 350 });
        if (ImGui::tab("F", tab == 4)) tab = 4;

    }
    ImGui::PopFont();

    switch (tab)
    {
          case 1:     legit();       break;
    }
}

static char search[64] = "";
void Search() {
    auto p = ImGui::GetWindowPos();

    ImGui::SetCursorPos(ImVec2(95, 85));
    ImGui::PushFont(InterMedium);
    {
        ImGui::PushItemWidth(160);
        if (ImGui::InputText("", search, 13))
            g_Search.find = true;
        ImGui::PopItemWidth();
    }
    ImGui::PopFont();

    if (!ImGui::IsItemActive() && strlen(search) == 0)
    {
        ImGui::GetWindowDrawList()->AddText(InterMedium, 17, ImVec2(p.x + 100, p.y + 87), ImColor(83, 86, 93), "search");//search
        ImGui::GetWindowDrawList()->AddText(IconFont, 17, ImVec2(p.x + 233, p.y + 88), ImColor(83, 86, 93), "V");//search
    }
}

void other()
{
    //ImGui::Begin("##SPEC", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_UnsavedDocument);
    //{
    //    ImGui::SetCursorPos(ImVec2(5, -10));
    //    ImGui::Image_getforeg(merfthefurry, ImVec2(18, 18));//merfthefurry

    //    ImGui::SetCursorPos(ImVec2(5, 15));
    //    ImGui::Image_getforeg(ghoul, ImVec2(18, 18));//ghoul
    //}
    //ImGui::End();

    //ImGui::SetNextWindowPos({ 1500, 60 });
    //ImGui::Begin("##WATER", nullptr, ImGuiWindowFlags_NoDecoration  | ImGuiWindowFlags_KeybindWindow);
    //{
    //   
    //}
    //ImGui::End();

    //ImGui::SetNextWindowPos({ 100, 60 });
    //ImGui::Begin("##LOGS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_LogsdWindow);
    //{

    //}
    //ImGui::End();
}

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bg_two, sizeof(bg_two), 1920, 1080, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bgs);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logotype, sizeof(logotype), 100, 100, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &logo);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, fototype, sizeof(fototype), 35, 35, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &foto);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, circletype, sizeof(circletype), 40, 40, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &circle);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, merfthefurry_type, sizeof(merfthefurry_type), 18, 18, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &merfthefurry);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, ghoul_type, sizeof(ghoul_type), 18, 18, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &ghoul);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImFontConfig icons_config;
    IconFont = io.Fonts->AddFontFromMemoryTTF(Icons, sizeof(Icons), 23, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IconFontLogs = io.Fonts->AddFontFromMemoryTTF(IconFontLog, sizeof(IconFontLog), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    LexendDecaFont = io.Fonts->AddFontFromMemoryTTF(LexendDeca, sizeof(LexendDeca), 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
    InterMedium = io.Fonts->AddFontFromMemoryTTF(Intermedium, sizeof(Intermedium), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

    InterMediumone = io.Fonts->AddFontFromMemoryTTF(Intermedium, sizeof(Intermedium), 14, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (!g_Search.find)
        g_Search.Init();

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
            auto& styles = ImGui::GetStyle();
            styles.FrameRounding = 6;
            styles.ChildRounding = 10;
            styles.PopupRounding = 5;

            ImGui::SetNextWindowSize(ImVec2(gs));
            if (ImGui::Begin(("###"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
            {
                s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2);
                p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 650, p.y + 450), ImColor(15, 17, 16), 10); //bg
            
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 10, p.y + 10), ImVec2(p.x + 60, p.y + 440), ImColor(23, 25, 24), 5);//bg 1
                ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 10, p.y + 10), ImVec2(p.x + 60, p.y + 440), ImColor(32, 35, 34), 5, 15, 1.5);//rect

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 71, p.y + 10), ImVec2(p.x + 641, p.y + 115), ImColor(23, 25, 24), 5);//bg 2
                ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x +71, p.y +10), ImVec2(p.x + 641, p.y + 115), ImColor(32, 35, 34), 5, 15, 1.5);//rect
             
                ImGui::SetCursorPos(ImVec2(10.1, 22));
                ImGui::Image(logo, ImVec2(65, 67));//logo

                ImGui::SetCursorPos(ImVec2(25, 400));
                ImGui::Image(foto, ImVec2(35, 35));//foto  site
                //hooks
                {
                    Search();
                    render_sub();
                    render_tab();
                    other();
                }

                if (g_Search.find && search[0] != '\0' && strlen(search) >= 2) // strlen(search) - количество символов при котором начинеться поиск
                {
                    ImGui::SetCursorPos(ImVec2(78, 130));
                    ImGui::MenuChild("Search", ImVec2(575, 320));
                    {
                        ImGui::Spacing();
                        g_Search.drawqueryresults(search);
                    }
                    ImGui::EndChild();
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
