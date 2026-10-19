#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "background.h"
#include "imgui_internal.h"
#include "imgui_freetype.h"

#include "imgui_settings.h"

#include "config.h"

#include "poppins_font.h"
#include "icon_font.h"
#include <string>

#include <iostream>
#include <dwmapi.h>

namespace pic {
    ID3D11ShaderResourceView* bg = nullptr;
    ID3D11ShaderResourceView* bg_menu = nullptr;
    ID3D11ShaderResourceView* theme = nullptr;


}

bool theme = true;


namespace font {
    ImFont* default_poppins = nullptr;
    ImFont* default_icon = nullptr;
}

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwnd;
RECT rc;

void move_window() {

    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::InvisibleButton("Move_detector", ImVec2(c::bg::size)));
    if (ImGui::IsItemActive()) {

        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, c::bg::size.x, c::bg::size.y, TRUE);
    }

}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"ImGui";
    wc.lpszClassName = L"Example";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Example", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (c::bg::size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (c::bg::size.y / 2), c::bg::size.x, c::bg::size.y, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    font::default_poppins = io.Fonts->AddFontFromMemoryTTF(poppins, sizeof(poppins), 22.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::default_icon = io.Fonts->AddFontFromMemoryTTF(icon, sizeof(icon), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    if (pic::bg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, bg, sizeof(bg), &info, pump, &pic::bg, 0);
    if (pic::bg_menu == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background_menu, sizeof(background_menu), &info, pump, &pic::bg_menu, 0);
    if (pic::theme == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, theme_icon, sizeof(theme_icon), &info, pump, &pic::theme, 0);

    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // !!! Менюшка из за этого всегда поверх всех окон. !!!

        static float accent0[4] = { 174 / 255.f, 35 / 255.f, 250 / 255.f, 255 / 255.f };
        static float accent1[4] = { 200 / 255.f, 40 / 255.f, 240 / 255.f, 255 / 255.f };
        const float theme_speed = 25.f;
        
        c::accent = ImColor(accent0[0], accent0[1], accent0[2]);
        c::accent_gradient = ImColor(accent1[0], accent1[1], accent1[2]);

        c::bg::background = ImLerp(c::bg::background, theme ? ImColor(14, 14, 16, 255) : ImColor(245, 247, 250, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::shadow = ImLerp(c::bg::shadow, theme ? ImColor(14, 14, 16, 255) : ImColor(245, 247, 250, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::background_pad = ImLerp(c::bg::background_pad, theme ? ImColor(17, 17, 20, 255) : ImColor(248, 251, 254, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::border = ImLerp(c::bg::border, theme ? ImColor(35, 36, 40, 255) : ImColor(229, 229, 229, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::bg::child::shadow = ImLerp(c::bg::child::shadow, theme ? ImColor(19, 18, 25, 0) : ImColor(240, 240, 245, 0), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::child::background = ImLerp(c::bg::child::background, theme ? ImColor(19, 18, 25, 255) : ImColor(240, 240, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::child::background_cap = ImLerp(c::bg::child::background_cap, theme ? ImColor(25, 24, 31, 255) : ImColor(236, 236, 241, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::child::text_name = ImLerp(c::bg::child::text_name, theme ? ImColor(200, 205, 210, 255) : ImColor(20, 25, 30, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::child::border = ImLerp(c::bg::child::border, theme ? ImColor(35, 36, 40, 255) : ImColor(235, 236, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::bg::child_header::background = ImLerp(c::bg::child_header::background, theme ? ImColor(24, 23, 30, 255) : ImColor(224, 223, 230, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::bg::child_header::border = ImLerp(c::bg::child_header::border, theme ? ImColor(35, 36, 40, 255) : ImColor(235, 236, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::tabs::background_active = ImLerp(c::tabs::background_active, theme ? ImColor(25, 24, 31, 255) : ImColor(225, 224, 231, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::tabs::background_hov = ImLerp(c::tabs::background_hov, theme ? ImColor(21, 20, 27, 255) : ImColor(221, 220, 227, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::tabs::background = ImLerp(c::tabs::background, theme ? ImColor(21, 20, 27, 0) : ImColor(221, 220, 227, 0), ImGui::GetIO().DeltaTime * theme_speed);
        c::tabs::border_active = ImLerp(c::tabs::border_active, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::tabs::border_inactive = ImLerp(c::tabs::border_inactive, theme ? ImColor(35, 36, 40, 0) : ImColor(215, 216, 220, 0), ImGui::GetIO().DeltaTime * theme_speed);

        c::checkbox::background_hov = ImLerp(c::checkbox::background_hov, theme ? ImColor(35, 36, 45, 255) : ImColor(235, 236, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::checkbox::background = ImLerp(c::checkbox::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::checkbox::border = ImLerp(c::checkbox::border, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::checkbox::checkmark_active = ImLerp(c::checkbox::checkmark_active, theme ? ImColor(0, 0, 0, 255) : ImColor(255, 255, 255, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::checkbox::checkmark_inactive = ImLerp(c::checkbox::checkmark_inactive, theme ? ImColor(0, 0, 0, 0) : ImColor(255, 255, 255, 0), ImGui::GetIO().DeltaTime * theme_speed);

        c::slider::background_hov = ImLerp(c::slider::background_hov, theme ? ImColor(35, 36, 45, 255) : ImColor(235, 236, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::slider::background = ImLerp(c::slider::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::slider::border = ImLerp(c::slider::border, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::slider::circle = ImLerp(c::slider::circle, theme ? ImColor(45, 46, 50, 255) : ImColor(245, 246, 250, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::button::background_hov = ImLerp(c::button::background_hov, theme ? ImColor(35, 36, 45, 255) : ImColor(235, 236, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::button::background = ImLerp(c::button::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::button::border = ImLerp(c::button::border, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::input::background_hov = ImLerp(c::input::background_hov, theme ? ImColor(35, 36, 45, 255) : ImColor(235, 236, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::input::background = ImLerp(c::input::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::input::border = ImLerp(c::input::border, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::combo::background_hov = ImLerp(c::combo::background_hov, theme ? ImColor(35, 36, 45, 255) : ImColor(235, 236, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::combo::background = ImLerp(c::combo::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::combo::border = ImLerp(c::combo::border, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::combo::shadow = ImLerp(c::combo::shadow, theme ? ImColor(30, 31, 40, 0) : ImColor(230, 231, 240, 0), ImGui::GetIO().DeltaTime * theme_speed);

        c::keybind::background_hov = ImLerp(c::keybind::background_hov, theme ? ImColor(35, 36, 45, 255) : ImColor(235, 236, 245, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::keybind::background = ImLerp(c::keybind::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::separator::border = ImLerp(c::separator::border, theme ? ImColor(35, 36, 40, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::scrollbar::border = ImLerp(c::scrollbar::border, theme ? ImColor(55, 60, 70, 255) : ImColor(215, 216, 220, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::picker::background = ImLerp(c::picker::background, theme ? ImColor(30, 31, 40, 255) : ImColor(230, 231, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::picker::border = ImLerp(c::picker::border, theme ? ImColor(30, 31, 40, 255) : ImColor(235, 236, 240, 255), ImGui::GetIO().DeltaTime * theme_speed);

        c::text::text_selected = ImLerp(c::text::text_selected, theme ? ImColor(21, 20, 27, 100) : ImColor(221, 220, 227, 100), ImGui::GetIO().DeltaTime * theme_speed);
        c::text::text_active = ImLerp(c::text::text_active, theme ? ImColor(255, 255, 255, 255) : ImColor(15, 15, 15, 255), ImGui::GetIO().DeltaTime * theme_speed);
        c::text::text_hov = ImLerp(c::text::text_hov, theme ? ImColor(93, 91, 105, 255) : ImColor(33, 31, 45, 255) , ImGui::GetIO().DeltaTime * theme_speed);
        c::text::text = ImLerp(c::text::text, theme ? ImColor(73, 71, 85, 255) : ImColor(93, 91, 105, 255), ImGui::GetIO().DeltaTime * theme_speed);

        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding = ImVec2(0, 0);
        style->ItemSpacing = ImVec2(25, 25);
        style->ScrollbarSize = 13.f;

        ImGui::SetNextWindowSize(ImVec2(c::bg::size));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("IMGUI", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);      
        {
            const ImVec2& s = ImGui::GetContentRegionMax();
            const ImVec2& p = ImGui::GetWindowPos();

            ImGui::GetBackgroundDrawList()->AddRectFilled(p, p + ImVec2(s), ImGui::GetColorU32(c::bg::background), c::bg::rounding);

            ImGui::GetBackgroundDrawList()->AddImage(pic::bg_menu, ImGui::GetCursorScreenPos() + ImVec2(200, 0), ImGui::GetCursorScreenPos() + ImVec2(900, 596), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::bg::background_pad));

            //// NAME
            ImGui::GetBackgroundDrawList()->AddRectFilled(p + ImVec2(style->ItemSpacing.x, (s.y - style->ItemSpacing.y) - 50), p + ImVec2((220 - style->ItemSpacing.x), (s.y - style->ItemSpacing.y)), ImGui::GetColorU32(c::bg::background_pad), c::bg::child::rounding);
            ImGui::GetBackgroundDrawList()->AddRect(p + ImVec2(style->ItemSpacing.x, (s.y - style->ItemSpacing.y) - 50), p + ImVec2((220 - style->ItemSpacing.x), (s.y - style->ItemSpacing.y)), ImGui::GetColorU32(c::bg::border), c::bg::child::rounding);
            const char* cheat_name = "CHEAT NAME";
            ImGui::GetBackgroundDrawList()->AddText(p + ImVec2((220 / 2) - (ImGui::CalcTextSize(cheat_name).x / 2), (s.y - style->ItemSpacing.y) - (50 / 2) - (ImGui::CalcTextSize(cheat_name).y / 2)), ImGui::GetColorU32(c::accent), cheat_name);
            //// NAME

            ImGui::GetBackgroundDrawList()->AddLine(p + ImVec2(220, style->ItemSpacing.y), p + ImVec2(220.f, s.y - style->ItemSpacing.y), ImGui::GetColorU32(c::bg::border), 1.f);

            const char* tab_array[] = { "Aimbot", "Visuals", "Misc", "Skins", "Settings" };
            const char* ico_array[] = { "A", "B", "C", "D", "E" };

            static int tabs = 0;

            ImGui::SetCursorPos(ImVec2(style->ItemSpacing));

            ImGui::BeginGroup();
            {
                for (int i = 0; i < sizeof(tab_array) / sizeof(tab_array[0]); i++) if (ImGui::tabs(i == tabs, ico_array[i], tab_array[i], ImVec2(220 - (style->ItemSpacing.x * 2), 45))) tabs = i;
            }
            ImGui::EndGroup();

      //      ImGui::GetBackgroundDrawList()->AddLine(p + ImVec2(style->ItemSpacing.x, (45 * 4) + (style->ItemSpacing.y * 5)), p + ImVec2((220 - style->ItemSpacing.x), (45 * 4) + (style->ItemSpacing.y * 5)), ImGui::GetColorU32(c::bg::border), 1.f);

            ImGui::SetCursorPos(ImVec2(220, 0) + style->ItemSpacing - ImVec2(c::bg::child::rounding, c::bg::child::rounding));

            ImGui::BeginChild("GENERAL", ImVec2(s.x - (220 + style->ItemSpacing.x), s.y - 40), false);
            {

                static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;
                tab_alpha = ImClamp(tab_alpha + (6.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

                static float anim_slow;
                anim_slow = ImLerp(anim_slow, (tab_alpha < 1.f) ? 150.f : 0.f, ImGui::GetIO().DeltaTime * 10.f);

                ImGui::SetCursorPos(ImVec2(c::bg::child::rounding, (c::bg::child::rounding + anim_slow)));

                if (active_tab == 0) {

                    ImGui::BeginGroup();
                    {

                        ImGui::DefaultBeginChild("Child One", ImVec2((s.x / 2) - (220 / 2) - (style->ItemSpacing.x) - (style->ItemSpacing.x / 2), s.y - (style->ItemSpacing.y * 2)));
                        {
                            static bool checkbox0 = true; /* */ static int key, m;
                            ImGui::CheckboxKeybind("Checkbox active", &checkbox0, &key, &m);

                            static bool checkbox1 = false;
                            ImGui::Checkbox("Checkbox inactive", &checkbox1);

                            static int slider_int = 1;
                            ImGui::SliderInt("Slider Integer", &slider_int, 1, 100, "%d%%");

                            static float slider_float = 1.f;
                            ImGui::SliderFloat("Slider Float", &slider_float, 1.f, 100.f, "%.1ff");

                            static int select = 0;
                            const char* items[3]{ "One", "Two", "Three" };
                            ImGui::Combo("Combo", &select, items, IM_ARRAYSIZE(items), 3);

                            static bool multi_num[8] = { false, true, true, true, false, true, true, true };
                            const char* multi_items[8] = { "One", "Two", "Three", "Four", "Five", "six", "saven", "Eight"};
                            ImGui::MultiCombo("Multi combo", multi_num, multi_items, 8);

                        }
                        ImGui::DefaultEndChild();

                        ImGui::DefaultBeginChild("Child Two", ImVec2((s.x / 2) - (220 / 2) - (ImGui::GetStyle().ItemSpacing.x) - (style->ItemSpacing.x / 2), (s.y / 2) - (style->ItemSpacing.y / 2) - style->ItemSpacing.y));
                        {
                            static bool items1[20];
                            for (int i = 0; i != 20; i++) ImGui::Checkbox(std::to_string(i).c_str(), &items1[i]);
                        }
                        ImGui::DefaultEndChild();

                        ImGui::DefaultBeginChild("Child Three", ImVec2((s.x / 2) - (220 / 2) - (style->ItemSpacing.x) - (style->ItemSpacing.x / 2), s.y - (style->ItemSpacing.y * 2)));
                        {
                            if (ImGui::Button("Dark", ImVec2(((ImGui::GetContentRegionMax().x - style->WindowPadding.x) / 2) - (style->ItemSpacing.x / 2), 35))) theme = true;

                            ImGui::SameLine();

                            if (ImGui::Button("Light", ImVec2(((ImGui::GetContentRegionMax().x - style->WindowPadding.x) / 2) - (style->ItemSpacing.x / 2), 35))) theme = false;

                        }
                        ImGui::DefaultEndChild();

                    }
                    ImGui::EndGroup();

                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    {
                        ImGui::DefaultBeginChild("Child Four", ImVec2((s.x / 2) - (220 / 2) - (ImGui::GetStyle().ItemSpacing.x) - (ImGui::GetStyle().ItemSpacing.x / 2), (s.y / 2) - (style->ItemSpacing.y / 2) - ImGui::GetStyle().ItemSpacing.y));
                        {

                            if (ImGui::Button("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 35)));

                            static char buf[16] = { "" };
                            ImGui::InputTextEx("##0", "Your text...", buf, 16, ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 35), NULL);

                        }
                        ImGui::DefaultEndChild();

                        ImGui::DefaultBeginChild("Child Five", ImVec2((s.x / 2) - (220 / 2) - (ImGui::GetStyle().ItemSpacing.x) - (ImGui::GetStyle().ItemSpacing.x / 2), (s.y / 2) - (style->ItemSpacing.y / 2) - ImGui::GetStyle().ItemSpacing.y));
                        {

                            ImGui::ColorEdit4("Color picker", accent0, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

                            ImGui::ColorEdit4("Color picker ", accent1, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

                            static int key0 = 0, m0 = 0;
                            ImGui::Keybind("Keybind", &key0, &m0, true);
                        }
                        ImGui::DefaultEndChild();


                        ImGui::DefaultBeginChild("Child Saven", ImVec2((s.x / 2) - (220 / 2) - (ImGui::GetStyle().ItemSpacing.x) - (ImGui::GetStyle().ItemSpacing.x / 2), (s.y / 2) - (style->ItemSpacing.y / 2) - ImGui::GetStyle().ItemSpacing.y));
                        {
                            static bool items[20];
                            if (ImGui::BeginListBox("List Box", ImGui::GetContentRegionMax() - ImVec2(style->WindowPadding)))
                            {

                                for (int i = 0; i != 20; i++)
                                    ImGui::Checkbox(std::to_string(i).c_str(), &items[i]);

                                ImGui::EndListBox();
                            }
                        }
                        ImGui::DefaultEndChild();
                    }
                    ImGui::EndGroup();

                }
                else if (active_tab == 1) {

                    ImGui::BeginGroup();
                    {
                    ImGui::DefaultBeginChild("Settings", ImVec2((s.x / 2) - (220 / 2) - (style->ItemSpacing.x) - (style->ItemSpacing.x / 2), s.y - (style->ItemSpacing.y * 2)));
                    {
                        config_add();
                    }
                    ImGui::DefaultEndChild();
                    }
                    ImGui::EndGroup();

                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    {
                        ImGui::DefaultBeginChild("Config List", ImVec2((s.x / 2) - (220 / 2) - (style->ItemSpacing.x) - (style->ItemSpacing.x / 2), s.y - (style->ItemSpacing.y * 2)));
                        {
                            config_list();
                        }
                        ImGui::DefaultEndChild();
                    }
                    ImGui::EndGroup();

                }

                ImGui::PopStyleVar();

            }
            ImGui::EndChild();

            move_window();
        }
        ImGui::End();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);

    }

     ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) 
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
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
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
