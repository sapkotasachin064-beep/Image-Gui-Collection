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


static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 1020, 718 };

IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* foto = nullptr;
IDirect3DTexture9* merfthefurry = nullptr;
IDirect3DTexture9* ghoul = nullptr;
IDirect3DTexture9* plogo = nullptr;


bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

static int tab;
static int subtab;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

template<class T, class U>
static T clamp(const T& in, const U& low, const U& high)
{
    if (in <= low)
        return low;

    if (in >= high)
        return high;

    return in;
}

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

    it_hover->second = clamp(it_hover->second + (2.15f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.f)), 0.0f, 0.0f);
    it_hover->second *= ImGui::GetStyle().Alpha;

    static std::map<ImGuiID, float> filled_animation;
    auto it_filled = filled_animation.find(id);

    if (it_filled == filled_animation.end())
    {
        filled_animation.insert({ id, 0.f });
        it_filled = filled_animation.find(id);
    }

    it_filled->second = clamp(it_filled->second + (2.15f * ImGui::GetIO().DeltaTime * (selected ? 1.f : -1.5f)), it_hover->second, 1.f);
    it_filled->second *= ImGui::GetStyle().Alpha;

    ImGui::PushFont(InterMedium);
    if (selected) {
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), ImColor(60, 61, 81, int(255 * ImGui::GetStyle().Alpha)), 5);
    }
    ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 5 + 27, bb.Min.y + 7), ImColor(96, 96, 109, int(255 * ImGui::GetStyle().Alpha)), text);

    if (selected)
    {
        ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 5 + 27, bb.Min.y + 7), ImColor(178, 182, 255, int(255 * it_filled->second)), text);
    }
    ImGui::PopFont();

    ImGui::PushFont(IconFont);
    ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + 4), ImColor(96, 96, 109, int(255 * ImGui::GetStyle().Alpha)), icon);

    if (selected)
    {
        ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + 4), ImColor(178, 182, 255, int(255 * it_filled->second)), icon);
    }
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
        window->DrawList->AddRectFilled({ bb.Min.x - 12,bb.Min.y - 3 }, { bb.Min.x - 10,bb.Max.y + 4 }, ImColor(193, 154, 164));
        window->DrawList->AddRectFilledMultiColor({ bb.Min.x - 10,bb.Min.y - 3 }, { bb.Min.x + 131,bb.Max.y + 4 }, ImColor(46, 46, 46), ImColor(34, 34, 34), ImColor(34, 34, 34), ImColor(46, 46, 46));

        ImGui::PushFont(InterMedium);
        window->DrawList->AddText(ImVec2(bb.Min.x, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(230, 230, 230), label);
        ImGui::PopFont();
    }
    else {
        ImGui::PushFont(InterMedium);
        window->DrawList->AddText(ImVec2(bb.Min.x, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(140, 140, 140), label);
        ImGui::PopFont();
    }

    return pressed;
}

void rage()
{
    if (subtab == 0) {
        ImGui::PushFont(InterMedium);

        ImGui::SetCursorPos(ImVec2(273, 83));
        ImGui::MenuChild("Aimbot", ImVec2(218 + 139, 130 + 71));
        {
            auto settings = &opt.ragebot[0];

            ImGui::Checkbox("Auto Fire", &settings->enabled);
            ImGui::Checkbox("Auto Scope", &settings->silent);
            ImGui::Checkbox("Silent Aim", &settings->hpbar);
            ImGui::SliderInt("Fov", &settings->priority, 0, 180);

        }
        ImGui::EndChild();
        ImGui::PopFont();
    }
}

void render_tab()
{
    {
        ImGui::SetCursorPos(ImVec2{ 26, 116 });
        if (renderer_tab("A", "Rage Bot", !tab)) tab = 0;

        ImGui::SetCursorPos(ImVec2{ 26, 151 });
        if (renderer_tab("B", "Legit Bot", tab == 1)) tab = 1;

        ImGui::SetCursorPos(ImVec2{ 26, 186 });
        if (renderer_tab("C", "Anti Aim", tab == 2)) tab = 2;

        ImGui::SetCursorPos(ImVec2{ 26, 262 });
        if (renderer_tab("D", "Player", tab == 3)) tab = 3;

        ImGui::SetCursorPos(ImVec2{ 26, 297 });
        if (renderer_tab("E", "World", tab == 4)) tab = 4;

        ImGui::SetCursorPos(ImVec2{ 26, 332 });
        if (renderer_tab("F", "Other", tab == 5)) tab = 5;

        ImGui::SetCursorPos(ImVec2{ 26, 408 });
        if (renderer_tab("G", "Main", tab == 6)) tab = 6;

        ImGui::SetCursorPos(ImVec2{ 26, 443 });
        if (renderer_tab("I", "Skins", tab == 7)) tab = 7;

        ImGui::SetCursorPos(ImVec2{ 26, 478 });
        if (renderer_tab("J", "Configs", tab == 8)) tab = 8;

    }

    switch (tab) {
        case 0: rage();
    }
}

void render_subtab()
{
    if (tab == 0)
    {
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 266, p.y + 16), ImVec2(p.x + 266 + 726, p.y + 16 + 48), ImColor(26, 26, 30), 8);
        ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 266, p.y + 16), ImVec2(p.x + 266 + 726, p.y + 16 + 48), ImColor(43, 43, 49), 8); 

        ImGui::GetWindowDrawList()->AddText(InterMedium, 17, ImVec2(p.x + 32 + 250, p.y + 31.5f), ImColor(255, 255, 255), "Weapon");

        ImGui::PushFont(InterMedium);
        ImGui::SetCursorPos(ImVec2(75 + 280 + 6, 26 + 14));
        if (ImGui::SubTab("General", subtab == 0, ImGui::CalcTextSize("General"))) subtab = 0; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6, 26 + 14));
        if (ImGui::SubTab("Auto", subtab == 1, ImGui::CalcTextSize("Auto"))) subtab = 1; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 - 20, 26 + 14));
        if (ImGui::SubTab("Scout", subtab == 2, ImGui::CalcTextSize("Scout"))) subtab = 2; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 - 20 - 15, 26 + 14));
        if (ImGui::SubTab("AWP", subtab == 3, ImGui::CalcTextSize("AWP"))) subtab = 3; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 + 67 - 20 - 15 - 20, 26 + 14));
        if (ImGui::SubTab("Rifles", subtab == 4, ImGui::CalcTextSize("Rifles"))) subtab = 4; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 + 67 + 67 - 20 - 15 - 35, 26 + 14));
        if (ImGui::SubTab("SMG", subtab == 5, ImGui::CalcTextSize("SMG"))) subtab = 5; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 + 67 + 67 + 67 - 20 - 15 - 35 - 20, 26 + 14));
        if (ImGui::SubTab("Heavy", subtab == 6, ImGui::CalcTextSize("Heavy"))) subtab = 6; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 + 67 + 67 + 67 + 67 - 20 - 15 - 35 - 20 - 10, 26 + 14));
        if (ImGui::SubTab("Pistols", subtab == 7, ImGui::CalcTextSize("Pistols"))) subtab = 7; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 + 67 + 67 + 67 + 67 + 67 - 20 - 15 - 35 - 20 - 10 - 5, 26 + 14));
        if (ImGui::SubTab("Deagle", subtab == 8, ImGui::CalcTextSize("Deagle"))) subtab = 8; ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(142 + 280 + 6 + 67 + 67 + 67 + 67 + 67 + 67 + 67 + 67 - 20 - 15 - 35 - 20 - 10 - 5 - 5, 26 + 14));
        if (ImGui::SubTab("Revolver", subtab == 9, ImGui::CalcTextSize("Revolver"))) subtab = 9; ImGui::SameLine();

        ImGui::PopFont();
    }
}

static char search[64] = "";
void Search() {
    auto p = ImGui::GetWindowPos();

    ImGui::SetCursorPos(ImVec2(681, 25));
    ImGui::PushFont(InterMedium);
    {
        ImGui::PushItemWidth(167);
        if (ImGui::InputText("", search, 13))
            g_Search.find = true;
        ImGui::PopItemWidth();
    }
    ImGui::PopFont();

    if (!ImGui::IsItemActive() && strlen(search) == 0)
    {
        ImGui::GetWindowDrawList()->AddText(IconFontLogs, 17, ImVec2(p.x + 686, p.y + 30), ImColor(127, 127, 127), "a");//search
        ImGui::GetWindowDrawList()->AddText(InterMedium, 17, ImVec2(p.x + 707, p.y + 30), ImColor(89, 89, 89), "Search...");//search
    }
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

    if (bgs == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bg_one, sizeof(bg_one), 1920, 1080, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bgs);
    if (logo == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logotype, sizeof(logotype), 179, 55, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &logo);
    if (foto == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, fototype, sizeof(fototype), 30, 30, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &foto);

    if (merfthefurry == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, merfthefurry_type, sizeof(merfthefurry_type), 18, 18, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &merfthefurry);

    if (ghoul == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, ghoul_type, sizeof(ghoul_type), 18, 18, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &ghoul);
    if (plogo == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, primo_logo, sizeof(primo_logo), 147, 38, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &plogo);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImFontConfig icons_config;
    IconFont = io.Fonts->AddFontFromMemoryTTF(Icons, sizeof(Icons), 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IconFontLogs = io.Fonts->AddFontFromMemoryTTF(IconFontLog, sizeof(IconFontLog), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    LexendDecaFont = io.Fonts->AddFontFromMemoryTTF(LexendDeca, sizeof(LexendDeca), 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
    InterMedium = io.Fonts->AddFontFromMemoryTTF(Intermedium, sizeof(Intermedium), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

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

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 1008, p.y + 706), ImColor(18, 18, 20), 10); //фон

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 250, p.y + 706), ImColor(21, 21, 24), 10, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft); //левая полоска

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 642), ImVec2(p.x + 250, p.y + 642 + 64), ImColor(26, 26, 30), 10, ImDrawCornerFlags_BotLeft); //левая нижняя полоска

                ImGui::GetWindowDrawList()->AddImage(logo, ImVec2(p.x + 9, p.y + 15), ImVec2(p.x + 9 + 179, p.y + 15 + 55)); //лого чита

                ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + 40, p.y + 642 + 32), 17, ImColor(178, 182, 255), 50); //фон аватарки

                ImGui::GetWindowDrawList()->AddImage(foto, ImVec2(p.x + 24 + 1, p.y + 642 + 32 - 16 + 1), ImVec2(p.x + 24 + 30 + 1, p.y + 642 + 32 + 30 - 16 + 1)); //аватарка

                ImGui::GetWindowDrawList()->AddText(InterMedium, 17, ImVec2(p.x + 67, p.y + 660), ImColor(255, 255, 255), "Byman#141");//Nickname
                ImGui::GetWindowDrawList()->AddText(InterMedium, 13, ImVec2(p.x + 67, p.y + 676), ImColor(127, 130, 135), "Expiry date: 21.12.2024");//date

                ImGui::GetWindowDrawList()->AddText(InterMedium, 20, ImVec2(p.x + 24, p.y + 82), ImColor(255, 255, 255), "Aimbot");//Tab name #1
                ImGui::GetWindowDrawList()->AddText(InterMedium, 20, ImVec2(p.x + 24, p.y + 82 + 146), ImColor(255, 255, 255), "Visuals");//Tab name #2
                ImGui::GetWindowDrawList()->AddText(InterMedium, 20, ImVec2(p.x + 24, p.y + 82 + 146 + 146), ImColor(255, 255, 255), "Misc");//Tab name #3

                {
                    render_tab();
                    render_subtab();
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
