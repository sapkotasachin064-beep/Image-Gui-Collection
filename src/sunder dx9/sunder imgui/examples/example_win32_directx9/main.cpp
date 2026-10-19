// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <dinput.h>
#include <tchar.h>
#include "bgs.h"

#include "misc/fonts/fonts.h"

#include <vector>
#include "options.hpp"

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

auto p = ImVec2{};

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* avatars = nullptr;
IDirect3DTexture9* skins = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

static int tab;
static int subtab;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void weapon()
{
    if (subtab == 0) {
        ImGui::PushFont(opt.InterRegular);

        ImGui::SetCursorPos(ImVec2(168 + 30, 110));
        ImGui::MenuChild("1", ImVec2(284, 135)); {
            auto settings = &opt.ragebot[0];
            ImGui::SliderInt("FOV", &settings->fov1, 0, 180);
            ImGui::Checkbox("Aimbot", &settings->Aimbot);
            ImGui::Button("Button", ImVec2(242, 23));
        }ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(168 + 30, 285));
        ImGui::MenuChild("2", ImVec2(284, 135)); {
            auto settings = &opt.ragebot[0];
            ImGui::SliderInt("FOV", &settings->fov2, 0, 180);
            ImGui::Checkbox("Silent aim", &settings->silent);
            ImGui::Button("Button", ImVec2(242, 23));
            
        }ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(519 + 14 - 7, 110));
        ImGui::MenuChild("3", ImVec2(286, 400)); {
            ImGui::GetWindowDrawList()->AddImage(skins, ImVec2(p.x + 550 + 34, p.y + 124), ImVec2(p.x + 550 + 195, p.y + 81 + 400));
        }ImGui::EndChild();


        ImGui::PopFont();
    }
}


void render_tab()
{
    ImGui::SetCursorPos(ImVec2(30, 106));
    ImGui::BeginGroup();

    ImGui::render_tab("Q", "Visuals", 0, tab);
    ImGui::render_tab("W", "Legit", 1, tab);
    ImGui::render_tab("E", "Rage", 2, tab);
    ImGui::render_tab("R", "Changers", 3, tab);
    ImGui::render_tab("T", "Misc", 4, tab);
    ImGui::render_tab("Y", "Config", 5, tab);

    ImGui::EndGroup();

    switch (tab)
    {
    case 0: weapon();
    }
};

void render_subtab()
{
    if (tab == 0)
    {
        ImGui::PushFont(opt.InterRegular);
        ImGui::SetCursorPos(ImVec2(183, 20));
        ImGui::BeginGroup();

        if (ImGui::SubTab("Weapon", subtab == 0, ImVec2(115, 45))) subtab = 0; ImGui::SameLine();

        if (ImGui::SubTab("General", subtab == 1, ImVec2(115, 45))) subtab = 1; ImGui::SameLine();

        ImGui::EndGroup();
        ImGui::PopFont();
    };
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
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bg, sizeof(bg), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bgs);

    if (avatars == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, avatar, sizeof(avatar), 32, 32, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &avatars);

    if (skins == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, skin, sizeof(skin), 226, 400, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &skins);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImFontConfig icons_config;
    opt.IconFont = io.Fonts->AddFontFromMemoryTTF(Icons, sizeof(Icons), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
    opt.InterSemiBold = io.Fonts->AddFontFromMemoryTTF(Intersemibold, sizeof(Intersemibold), 29, NULL, io.Fonts->GetGlyphRangesCyrillic());
    opt.InterRegular = io.Fonts->AddFontFromMemoryTTF(Interregular, sizeof(Interregular), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());

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
            ImGui::SetNextWindowSize(ImVec2(ImVec2{860, 610}));
            if (ImGui::Begin(("###"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
            {
                p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y);

                // это лучше перенести в decorations, а не рендерить в бегине. Именно из за этого у тебя весь рендер отстаёт от границы

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 850, p.y + 600), ImColor(33, 33, 33), 20); //фон

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 160, p.y + 600), ImColor(18, 18, 18), 20, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft); //фон табов

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 850, p.y + 77), ImColor(18, 18, 18), 20, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight); //фон суб табов

                ImGui::GetWindowDrawList()->AddText(opt.InterSemiBold, 29, ImVec2(p.x + 30, p.y + 24), ImColor(245, 245, 245), "SUNDER"); //logo

                ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 30, p.y + 75), ImVec2(p.x + 30 + 100, p.y + 75), ImColor(58, 58, 58)); //сепаратор под лого

                ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 30, p.y + 506), ImVec2(p.x + 30 + 100, p.y + 506), ImColor(58, 58, 58)); //сепаратор над профилем

                ImGui::GetWindowDrawList()->ImDrawList::AddImageRounded(avatars, ImVec2(p.x + 30, p.y + 538), ImVec2(p.x + 30 + 32, p.y + 538 + 32), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255), 16); //аватарка

                ImGui::GetWindowDrawList()->AddText(opt.InterRegular, 13, ImVec2(p.x + 77, p.y + 547), ImColor(189, 189, 189), "Skvizxc"); //ник

                render_tab();
                render_subtab();
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
