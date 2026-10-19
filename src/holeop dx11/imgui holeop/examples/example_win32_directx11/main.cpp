#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_freetype.h"
#include <d3d11.h>
#include <tchar.h>

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "image.h"
#include "font.h"

#include "notification.h"

namespace pic
{
    ID3D11ShaderResourceView* bg = nullptr;
    ID3D11ShaderResourceView* widget = nullptr;

    ID3D11ShaderResourceView* aimbot = nullptr;
    ID3D11ShaderResourceView* visuals = nullptr;
    ID3D11ShaderResourceView* skins = nullptr;
    ID3D11ShaderResourceView* settings = nullptr;

    ID3D11ShaderResourceView* weapon_ak47 = nullptr;
    ID3D11ShaderResourceView* weapon_m4a1 = nullptr;
    ID3D11ShaderResourceView* weapon_galil = nullptr;

    ID3D11ShaderResourceView* user_circle = nullptr;
}

namespace font
{
    ImFont* inter_bold = nullptr;
    ImFont* inter_default = nullptr;
    ImFont* icon = nullptr;
}

bool info_bar = true;

const char* cheat_name = "Cheat Name";
const char* game_status = "Counter-Strike Global Offensive";
const char* developer = "Developer";

const char* ping = "7ms";
const char* world_time = "12:20am";

static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    font::inter_default = io.Fonts->AddFontFromMemoryTTF(inter_medium, sizeof(inter_medium), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icon = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    if (pic::bg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &pic::bg, 0);
    if (pic::widget == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, widget_icon, sizeof(widget_icon), &info, pump, &pic::widget, 0);

    if (pic::aimbot == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, aimbot, sizeof(aimbot), &info, pump, &pic::aimbot, 0);
    if (pic::visuals == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, visuals, sizeof(visuals), &info, pump, &pic::visuals, 0);
    if (pic::skins == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, skins, sizeof(skins), &info, pump, &pic::skins, 0);
    if (pic::settings == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, settings, sizeof(settings), &info, pump, &pic::settings, 0);

    if (pic::weapon_ak47 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, weapon_ak47, sizeof(weapon_ak47), &info, pump, &pic::weapon_ak47, 0);
    if (pic::weapon_m4a1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, weapon_m4a1, sizeof(weapon_m4a1), &info, pump, &pic::weapon_m4a1, 0);
    if (pic::weapon_galil == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, weapon_galil, sizeof(weapon_galil), &info, pump, &pic::weapon_galil, 0);

    if (pic::user_circle == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, user_circle, sizeof(user_circle), &info, pump, &pic::user_circle, 0);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {

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

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiStyle* style = &ImGui::GetStyle();

            style->WindowPadding = ImVec2(0, 0), style->WindowBorderSize = 0;
            style->ItemSpacing = ImVec2(12, 12);
            style->ScrollbarSize = 7.f;

            static float color[4] = { 0.f, 1.f, 1.f, 1.f };
            c::accent = {color[0], color[1], color[2], 1.f};

            ImGui::GetBackgroundDrawList()->AddImage(pic::bg, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255));

            ImGui::SetNextWindowSize({ c::bg::size });

            ImGui::Begin("IMGUI", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                const ImVec2& pos = ImGui::GetWindowPos();
                const ImVec2& spacing = style->ItemSpacing;
                const ImVec2& region = ImGui::GetContentRegionMax();

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, { pos + region }, ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + spacing, pos + ImVec2(155, 90), ImGui::GetColorU32(c::child::background), c::child::rounding);
                ImGui::GetBackgroundDrawList()->AddRect(pos + spacing, pos + ImVec2(155, 90), ImGui::GetColorU32(c::child::outline), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, 90) + spacing, pos + ImVec2(155, region.y - spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);
                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(0, 90) + spacing, pos + ImVec2(155, region.y - spacing.y), ImGui::GetColorU32(c::child::outline), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(155, 0) + spacing, pos + ImVec2(region.x - 90 - spacing.x, 90), ImGui::GetColorU32(c::child::background), c::child::rounding);
                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(155, 0) + spacing, pos + ImVec2(region.x - 90 - spacing.x, 90), ImGui::GetColorU32(c::child::outline), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(region.x - 90 - spacing.x, 0) + spacing, pos + ImVec2(region.x - spacing.x, 90), ImGui::GetColorU32(c::child::background), c::child::rounding);
                ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(region.x - 90 - spacing.x, 0) + spacing, pos + ImVec2(region.x - spacing.x, 90), ImGui::GetColorU32(c::child::outline), c::child::rounding);

                ImGui::GetBackgroundDrawList()->AddImageRounded(pic::user_circle, pos + ImVec2(region.x - 90 - spacing.x, 0) + spacing + ImVec2(13, 13), pos + ImVec2(region.x - spacing.x, 90) - ImVec2(13, 13), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::text::text_active), 30.f);

                ImGui::GetBackgroundDrawList()->AddCircle(pos + ImVec2(region.x - (90 + spacing.x) / 2, (90 + spacing.x) / 2), 28.f, ImGui::GetColorU32(c::accent), 100.f, 2.f);

                ImGui::GetBackgroundDrawList()->AddText(font::inter_bold, 17, pos + (ImVec2(155, 90) + spacing - ImGui::CalcTextSize("CHEAT NAME")) / 2, ImGui::GetColorU32(c::text::text_active), "CHEAT NAME");

                static int tabs = 0, sub_tabs = 0;

                ImGui::SetCursorPos(ImVec2(spacing.x * 2, 90 + spacing.y * 2));
                ImGui::BeginGroup();
                {
                    if (ImGui::Tabs(0 == tabs, pic::aimbot, "Aimbot", ImVec2(155 - spacing.x * 3 ,35), NULL)) tabs = 0;

                    if (ImGui::Tabs(1 == tabs, pic::visuals, "Visuals", ImVec2(155 - spacing.x * 3, 35), NULL)) tabs = 1;

                    if (ImGui::Tabs(2 == tabs, pic::skins, "Skins", ImVec2(155 - spacing.x * 3, 35), NULL)) tabs = 2;

                    if (ImGui::Tabs(3 == tabs, pic::settings, "Settings", ImVec2(155 - spacing.x * 3, 35), NULL)) tabs = 3;
                }
                ImGui::EndGroup();

                ImGui::SetCursorPos(ImVec2(155 + (spacing.x * 2) + (48 / 2), ((48 + spacing.y) / 2) - 1));
                ImGui::BeginGroup();
                {
                    if (ImGui::SubTab(0 == sub_tabs, pic::weapon_ak47, ImVec2(48, 48))) sub_tabs = 0;
                    ImGui::SameLine(0, 24);
                    if (ImGui::SubTab(1 == sub_tabs, pic::weapon_m4a1, ImVec2(48, 48))) sub_tabs = 1;
                    ImGui::SameLine(0, 24);
                    if (ImGui::SubTab(2 == sub_tabs, pic::weapon_galil, ImVec2(48, 48))) sub_tabs = 2;
                }
                ImGui::EndGroup();

                static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

                ImGui::SetCursorPos(ImVec2(155, 90) + spacing);

                if (active_tab == 0) {

                    ImGui::BeginGroup();
                    {
                        ImGui::BeginChild("Visuals", ImVec2((region.x - (spacing.x * 3 + 155)) / 2, (region.y - (spacing.y * 3 + 90)) / 2.0f));
                        {

                            static bool check_0 = true;
                            ImGui::Checkbox("Checkbox Active", &check_0);

                            static bool check_1 = false;
                            ImGui::Checkbox("Checkbox Inactive", &check_1);

                            static float slider_0 = 0.5f;
                            ImGui::SliderFloat("Slider Float", &slider_0, 0.f, 1.f, "%.1fF");

                            static int slider_1 = 50;
                            ImGui::SliderInt("Slider Integer", &slider_1, 0, 100, "%d%%");

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 200);

                        }
                        ImGui::EndChild();

                        ImGui::BeginChild("Glow", ImVec2((region.x - (spacing.x * 3 + 155)) / 2, (region.y - (spacing.y * 3 + 90)) / 2.0f));
                        {

                            static int select = 0;
                            const char* items[10]{ "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" };
                            ImGui::Combo("Combo", &select, items, IM_ARRAYSIZE(items), 10);

                            static bool multi_num[5] = { false, true, true, true, false };
                            const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };
                            ImGui::MultiCombo("MultiCombo", multi_num, multi_items, 5);

                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndGroup();

                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    {
                        ImGui::BeginChild("World", ImVec2((region.x - (spacing.x * 3 + 155)) / 2, (region.y - (spacing.y * 3 + 90)) / 2.0f));
                        {

                            static char input[64] = { "" };
                            if (ImGui::InputTextEx("TextField", "Enter your text here", input, 64, ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40), NULL));

                            static int number = 0;
                            if (ImGui::Button("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                                
                                if (number >= 4) number = 1; else number++;

                                if (number == 1) ImGui::Notification({ ImGuiToastType_Success, 4000, "The design of the notification was made by PAST OWL\nDesiqn.For sale from 27.07.2023" });
                                if (number == 2) ImGui::Notification({ ImGuiToastType_Info, 4000, "The design of the notification was made by PAST OWL\nDesiqn.For sale from 27.07.2023" });
                                if (number == 3) ImGui::Notification({ ImGuiToastType_Warning, 4000, "The design of the notification was made by PAST OWL\nDesiqn.For sale from 27.07.2023" });
                                if (number == 4) ImGui::Notification({ ImGuiToastType_Error, 4000, "The design of the notification was made by PAST OWL\nDesiqn.For sale from 27.07.2023" });
                            }
                        }
                        ImGui::EndChild();

                        ImGui::BeginChild("Wallhack", ImVec2((region.x - (spacing.x * 3 + 155)) / 2, (region.y - (spacing.y * 3 + 90)) / 2.0f));
                        {

                            ImGui::ColorEdit4("Color Picker", color, picker_flags);

                            static int key, m;
                            ImGui::Keybind("Keybind", &key, &m);

                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndGroup();
                }
                ImGui::PopStyleVar();
            }
            ImGui::End();

            static float ibar_size = ImGui::CalcTextSize(cheat_name).x + ImGui::CalcTextSize("|").x + ImGui::CalcTextSize(developer).x + ImGui::CalcTextSize("|").x + ImGui::CalcTextSize(ping).x + ImGui::CalcTextSize("|").x + ImGui::CalcTextSize(world_time).x + (style->ItemSpacing.x * 8);
            static float position = GetSystemMetrics(SM_CXSCREEN);
            position = ImLerp(position, info_bar ? GetSystemMetrics(SM_CXSCREEN) - (ibar_size + 7) : GetSystemMetrics(SM_CXSCREEN), ImGui::GetIO().DeltaTime * 8.f);

            if (position <= (GetSystemMetrics(SM_CXSCREEN) - 2)) {

                ImGui::SetNextWindowPos(ImVec2(position, 5));
                ImGui::SetNextWindowSize(ImVec2(ibar_size, 45));

                ImGui::Begin("info-bar", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
                {
                    const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(ibar_size, 45), ImGui::GetColorU32(c::bg::background), c::child::rounding);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, 10), pos + ImVec2(4, 35), ImGui::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_Right);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(region.x - 4, 10), pos + ImVec2(region.x, 35), ImGui::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_Left);
                    ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(ibar_size, 45), ImGui::GetColorU32(c::child::outline), c::child::rounding);

                    const char* info_set[4] = { cheat_name, developer, ping, world_time };
                    static int info_bar_size = 0;

                    ImGui::SetCursorPos(ImVec2(spacing.x, (45 - ImGui::CalcTextSize(developer).y) / 2));
                    ImGui::BeginGroup();
                    {

                        for (int i = 0; i < sizeof(info_set) / sizeof(info_set[0]); i++) {
                            ImGui::TextColored(i < 1 ? ImColor(ImGui::GetColorU32(c::accent)) : ImColor(ImGui::GetColorU32(c::text::text)), info_set[i]);
                            ImGui::SameLine();

                            if (i < 3) {
                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::text::text)), "|");
                                ImGui::SameLine();
                            }
                        }
                    }
                    ImGui::EndGroup();
                }
                ImGui::End();
            }

            ImGui::RenderNotifications();

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    // Cleanup
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
    // Setup swap chain
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
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
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
