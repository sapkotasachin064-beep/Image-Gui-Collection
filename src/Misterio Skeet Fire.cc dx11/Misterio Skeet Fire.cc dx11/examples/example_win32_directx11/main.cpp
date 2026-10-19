// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <chrono>
#include <algorithm>

#include "widgets_colors.h"
#include "imgui_internal.h"
#include "settings_menu.h"
#include "images.h"
#include "fonts.h"
#include "custom_widgets.hpp"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")
// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;


static int notify_select = 0;

void GradientRect(const ImVec2& min, ImVec2& max, ImColor col_top_left, ImColor col_top_right, ImColor col_bot_right, ImColor col_bot_left, float thickness, float rounding) {
    auto draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilledMultiColor({ min.x + rounding, min.y }, { max.x - rounding, min.y + thickness }, col_top_left, col_top_right, col_top_right, col_top_left);
    draw_list->AddRectFilledMultiColor({ max.x - thickness, min.y + rounding }, { max.x, max.y - rounding }, col_top_right, col_top_right, col_bot_right, col_bot_right);
    draw_list->AddRectFilledMultiColor({ min.x + rounding, max.y - thickness }, { max.x - rounding, max.y }, col_bot_left, col_bot_right, col_bot_right, col_bot_left);
    draw_list->AddRectFilledMultiColor({ min.x, min.y + rounding }, { min.x + thickness, max.y - rounding }, col_top_left, col_top_left, col_bot_left, col_bot_left);

    draw_list->PathArcToFast(min + ImVec2(rounding, rounding) + ImVec2(thickness, thickness) / 2, rounding, 6, 9);
    draw_list->PathStroke(col_top_right, 0, thickness);
    draw_list->PathArcToFast({ max.x - rounding - thickness / 2, min.y + rounding + thickness / 2 }, rounding, 9, 12);
    draw_list->PathStroke(col_top_right, 0, thickness);
    draw_list->PathArcToFast(max - ImVec2(rounding, rounding) - ImVec2(thickness, thickness) / 2, rounding, 0, 3);
    draw_list->PathStroke(col_bot_right, 0, thickness);
    draw_list->PathArcToFast({ min.x + rounding + thickness / 2, max.y - rounding }, rounding, 3, 6);
    draw_list->PathStroke(col_bot_left, 0, thickness);
}


void CricleProgress(const char* name, float progress, float max, float radius, const ImVec2& size)
{
    const float tickness = 3.f;
    static float position = 0.f;

    position = progress / max * 6.28f;

    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcTo(ImGui::GetCursorScreenPos() + size, radius, 0.f, 2.f * IM_PI, 120.f);
    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(colors::notification::notifi_bg), 0, tickness);

    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcTo(ImGui::GetCursorScreenPos() + size, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 120.f);
    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(colors::accent_color), 0, tickness);

    int procent = progress / (int)max * 100;

    std::string procent_str = std::to_string(procent) + "%";

}

struct Notification {
    int id;
    std::string message;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
};

class NotificationSystem {
public:
    NotificationSystem() : notificationIdCounter(0) {}

    void AddNotification(const std::string& message, int durationMs) {
        auto now = std::chrono::steady_clock::now();
        auto endTime = now + std::chrono::milliseconds(durationMs);
        notifications.push_back({ notificationIdCounter++, message, now, endTime });
    }

    void DrawNotifications() {
        auto now = std::chrono::steady_clock::now();

        std::sort(notifications.begin(), notifications.end(),
            [now](const Notification& a, const Notification& b) -> bool {
                float durationA = std::chrono::duration_cast<std::chrono::milliseconds>(a.endTime - a.startTime).count();
                float elapsedA = std::chrono::duration_cast<std::chrono::milliseconds>(now - a.startTime).count();
                float percentageA = (durationA - elapsedA) / durationA;

                float durationB = std::chrono::duration_cast<std::chrono::milliseconds>(b.endTime - b.startTime).count();
                float elapsedB = std::chrono::duration_cast<std::chrono::milliseconds>(now - b.startTime).count();
                float percentageB = (durationB - elapsedB) / durationB;

                return percentageA < percentageB;
            }
        );

        int currentNotificationPosition = 0;

        for (auto& notification : notifications) {
            if (now < notification.endTime) {
                float duration = std::chrono::duration_cast<std::chrono::milliseconds>(notification.endTime - notification.startTime).count();
                float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - notification.startTime).count();
                float percentage = (duration - elapsed) / duration * 100.0f;

                ShowNotification(currentNotificationPosition, notification.message, percentage);
                currentNotificationPosition++;
            }
        }

        notifications.erase(std::remove_if(notifications.begin(), notifications.end(),
            [now](const Notification& notification) { return now >= notification.endTime; }),
            notifications.end());
    }

private:
    std::vector<Notification> notifications;
    int notificationIdCounter;

    void ShowNotification(int position, const std::string& message, float percentage) {

        float duePercentage = 100.0f - percentage;
        float alpha = percentage > 10.0f ? 1.0f : percentage / 10.0f;
        ImGui::GetStyle().WindowPadding = ImVec2(15, 10);

        ImGui::SetNextWindowPos(ImVec2(duePercentage < 15.f ? duePercentage : 15.f, 15 + position * 90));

        ImGui::Begin(("##NOTIFY" + std::to_string(position)).c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImVec2 pos = ImGui::GetWindowPos(), spacing = ImGui::GetStyle().ItemSpacing, region = ImGui::GetContentRegionMax();

        const auto& position_menu = ImGui::GetWindowPos();

        ImGui::GetBackgroundDrawList()->AddRectFilledMultiColorRounded(pos, pos + region, ImGui::GetColorU32(colors::notification::notifi_bg, alpha), ImGui::GetColorU32(colors::accent_color, 0.05f), ImGui::GetColorU32(colors::accent_color, 0.05f), ImGui::GetColorU32(colors::color_picker::picker_bg, alpha), 2);
        ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(colors::notification::notifi_bg, 0.4f), 2.f);
        custom::GradientRect(ImGui::GetForegroundDrawList(), pos, pos + ImVec2(region.x + 16, region.y + 10), ImGui::GetColorU32(colors::notification::notifi_bg, alpha), ImGui::GetColorU32(colors::notification::notifi_bg, alpha), ImGui::GetColorU32(colors::accent_color, alpha), ImGui::GetColorU32(colors::accent_color, alpha), 1, 2.f);
        if (notify_select == 0)
            CricleProgress("##NOTIFY", percentage, 100, 7.f, ImVec2(ImGui::GetContentRegionMax().x - 40, 9));
        if (notify_select == 1)
            ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 3), pos + ImVec2(region.x * (duePercentage / 100.0f), region.y), ImGui::GetColorU32(colors::accent_color, alpha), 2);

        ImGui::PushFont(fonts::sans_font_small);
        ImGui::TextColored(ImColor(ImGui::GetColorU32(colors::accent_color, alpha)), "%s", "[Notification]");
        ImGui::TextColored(ImColor(ImGui::GetColorU32(colors::notification::notify_text, alpha)), "%s", message.c_str());
        ImGui::Dummy(ImVec2(ImGui::CalcTextSize(message.c_str()).x + 15, 5));
        ImGui::PopFont();

        ImGui::End();
    }
};

NotificationSystem notificationSystem;


// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
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

    fonts::sans_font = io.Fonts->AddFontFromMemoryTTF(&sans, sizeof sans, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
    fonts::sans_font_small = io.Fonts->AddFontFromMemoryTTF(&sans, sizeof sans, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
    fonts::weapon_font = io.Fonts->AddFontFromMemoryTTF(&weapon, sizeof weapon, 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Our state
    bool show_demo_window = true;
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

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        RECT screen_rect;
        GetWindowRect(GetDesktopWindow(), &screen_rect);
        auto x = float(screen_rect.right - 640) / 2.f;
        auto y = float(screen_rect.bottom - 520) / 2.f;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        D3DX11_IMAGE_LOAD_INFO image; ID3DX11ThreadPump* pump{ nullptr };

        if (pictures::bg_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &bg, sizeof(bg), &image, pump, &pictures::bg_img, 0);
        if (pictures::aim_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &aim, sizeof(aim), &image, pump, &pictures::aim_img, 0);
        if (pictures::logo_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &logo, sizeof(logo), &image, pump, &pictures::logo_img, 0);
        if (pictures::misc_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &other, sizeof(other), &image, pump, &pictures::misc_img, 0);
        if (pictures::visual_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &visual, sizeof(visual), &image, pump, &pictures::visual_img, 0);
        if (pictures::pen_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &pen, sizeof(pen), &image, pump, &pictures::pen_img, 0);
        if (pictures::world_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &world, sizeof(world), &image, pump, &pictures::world_img, 0);
        if (pictures::settings_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, setting, sizeof(setting), &image, pump, &pictures::settings_img, 0);
        if (pictures::aim_small_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &aim_small, sizeof(aim_small), &image, pump, &pictures::aim_small_img, 0);

        ImGui::GetBackgroundDrawList()->AddImage(pictures::bg_img, ImVec2(0, 0), ImVec2(1920, 1080));

        ImGui::PushFont(fonts::sans_font_small);
        {

            const float cheat_name_size = ImGui::CalcTextSize(settings::cheat_name).x;
            const float developer_size = ImGui::CalcTextSize(settings::developer).x;
            const float ping_size = ImGui::CalcTextSize(settings::ping).x;
            const float fps_size = ImGui::CalcTextSize(settings::fps).x;
            const float time_size = ImGui::CalcTextSize(settings::time).x;
            const float bar_size = ImGui::CalcTextSize("|").x;

            const float size_names = cheat_name_size + bar_size + ping_size + bar_size + fps_size + bar_size + time_size * 3.9f;

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - (size_names + 40), 15));
            ImGui::SetNextWindowSize(ImVec2(size_names, 35));

            ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
            {

                ImGuiStyle* style = &ImGui::GetStyle();

                const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(menu::watermark_bg), 3);

                custom::GradientRect(ImGui::GetBackgroundDrawList(), pos, pos + region, ImGui::GetColorU32(menu::watermark_bg), ImGui::GetColorU32(menu::watermark_bg), ImGui::GetColorU32(colors::accent_color), ImGui::GetColorU32(colors::accent_color), 1.f, 2.f);

                const char* info_names[5] = { settings::cheat_name, settings::developer, settings::ping, settings::fps, settings::time };

                ImGui::SetCursorPos(ImVec2(10, (35 - ImGui::CalcTextSize(settings::developer).y) / 2));
                ImGui::BeginGroup();
                {
                    for (int i = 0; i < ARRAYSIZE(info_names); i++) {
                        ImGui::TextColored(i < 1 ? ImColor(ImGui::GetColorU32(colors::accent_color)) : ImColor(ImGui::GetColorU32(menu::text)), info_names[i]);
                        ImGui::SameLine(0, 8);
                        ImGui::TextColored(ImColor(ImGui::GetColorU32(menu::text)), "|");
                        ImGui::SameLine(0, 8);
                    }
                }
                ImGui::EndGroup();
            }
            ImGui::End();
        }
        ImGui::PopFont();

        {
            ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
            ImGui::SetNextWindowSize(settings::size_menu);
            ImGui::Begin("ImGui menu by misterio", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);

            const auto& pos = ImGui::GetWindowPos();
            const auto& draw_list = ImGui::GetWindowDrawList();
            ImGuiStyle* style = &ImGui::GetStyle();

            notificationSystem.DrawNotifications();

            {
                style->Colors[ImGuiCol_WindowBg] = menu::window_bg;
                style->Colors[ImGuiCol_Border] = menu::border;
                style->Colors[ImGuiCol_WindowShadow] = menu::border_shadow;
                style->Colors[ImGuiCol_ScrollbarGrab] = menu::scrollbar;
                style->Colors[ImGuiCol_ScrollbarGrabHovered] = menu::scrollbar_hovered;
                style->Colors[ImGuiCol_ScrollbarGrabActive] = colors::accent_color;
                style->Colors[ImGuiCol_ScrollbarBg] = menu::scrollbar_bg;
                style->ItemSpacing = settings::item_spacing;
                style->WindowPadding = settings::item_padding;
                style->WindowRounding = menu::window_rounding;
                style->ScrollbarSize = menu::scroll_size;

            }


            {
                draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 700, pos.y + 40), ImGui::GetColorU32(menu::line), menu::window_rounding, ImDrawCornerFlags_Top);
                draw_list->AddRectFilled(ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + 700, pos.y + 41), ImGui::GetColorU32(menu::line2));
                draw_list->AddImage(pictures::logo_img, ImVec2(pos.x + 13, pos.y + 7), ImVec2(pos.x + 37, pos.y + 31), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(colors::accent_color));
                draw_list->AddShadowRect(ImVec2(pos.x + 20, pos.y + 13), ImVec2(pos.x + 30, pos.y + 25), ImGui::GetColorU32(colors::accent_color), 35.f, ImVec2(0, 0), 100);
                ShadowText(fonts::sans_font, "FIRE.cc", ImVec2(pos.x + 59, pos.y + 21), ImVec2(0, -1), ImGui::GetColorU32(colors::accent_color), true, 12);
            }

            {
                ImGui::SetCursorPos(ImVec2(15, 55));
                ImGui::BeginGroup();
                {
                    if (custom::Tab("Aimbot", pictures::aim_img, 0 == misc::tab_count))
                        misc::tab_count = 0;

                    if (custom::Tab("Visuals", pictures::visual_img, 1 == misc::tab_count))
                        misc::tab_count = 1;

                    if (custom::Tab("Skins", pictures::pen_img, 2 == misc::tab_count))
                        misc::tab_count = 2;

                    if (custom::Tab("Misc", pictures::world_img, 3 == misc::tab_count))
                        misc::tab_count = 3;

                    if (custom::Tab("Settings", pictures::settings_img, 4 == misc::tab_count))
                        misc::tab_count = 4;

                    if (custom::Tab("Other", pictures::misc_img, 5 == misc::tab_count))
                        misc::tab_count = 5;


                }ImGui::EndGroup();
            }

            misc::alpha_child = ImLerp(misc::alpha_child, (misc::tab_count == misc::active_tab_count) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
            if (misc::alpha_child < 0.01f && misc::child_add < 0.01f) misc::active_tab_count = misc::tab_count;

            ImGui::SetCursorPos(ImVec2(95, 55));

            ImGui::BeginChild("General", ImVec2(597, 445));
            {
                ImGui::SetCursorPos(ImVec2(0, 100 - (misc::alpha_child * 100)));

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, misc::alpha_child * style->Alpha);

                if (misc::active_tab_count == 0) {

                    ImGui::BeginGroup();
                    {
                        custom::BeginChild(pictures::aim_small_img, "General", ImVec2(285, 260));
                        {
                            custom::Checkbox("Enable", &features::check1);

                            custom::Checkbox("Auto wall", &features::check2);

                            custom::Checkbox("Auto fire", &features::check3);

                            custom::SliderInt("FOV", &features::sliderint, 0, 255);

                            custom::Checkbox("Some other shit", &features::check11);

                            custom::KeybindBox("Prefer body aim", &features::check10, &features::key, &features::var);

                        }
                        custom::EndChild();

                        custom::Spacing(10.f);

                        custom::BeginChild(pictures::aim_small_img, "Aimbot", ImVec2(285, 175));
                        {

                            custom::KeybindBox("Quik Peek", &features::check8, &features::key2, &features::var2);

                            custom::KeybindBox("Trigger", &features::check9, &features::key3, &features::var3);

                            custom::ColorEdit4("Fov color", (float*)&colors::accent_color, picker_flags);

                        }
                        custom::EndChild();

                        custom::Spacing(10.f);

                        custom::BeginChild(pictures::aim_small_img, "Damage", ImVec2(285, 250));
                        {

                            custom::Button("Add score me", ImVec2(ImGui::GetWindowWidth() - 37, 30));

                            if (ImGui::IsItemClicked() && ImGui::IsItemHovered()) notificationSystem.AddNotification("I'm an indoor millionaire, like Past Owl", 10500);

                            custom::Button("Add score enemy", ImVec2(ImGui::GetWindowWidth() - 37, 30));

                            if (ImGui::IsItemClicked() && ImGui::IsItemHovered()) notificationSystem.AddNotification("Everyone laughs at Past Owl, he is a foolish person", 10500);

                            custom::Button("Add score team", ImVec2(ImGui::GetWindowWidth() - 37, 30));

                            if (ImGui::IsItemClicked() && ImGui::IsItemHovered()) notificationSystem.AddNotification("And yes, Past Owl deceives people for money", 10500);

                        }
                        custom::EndChild();

                    }
                    ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(295, 100 - (misc::alpha_child * 100)));

                    ImGui::BeginGroup();
                    {
                        custom::BeginChild(pictures::aim_small_img, "Accuracy", ImVec2(285, 300));
                        {
                            custom::Checkbox("Autmatic stop", &features::check12);

                            custom::MultiCombo("Multipoints", features::multi, features::multi_items, 5);

                            custom::SliderInt("Damage value", &features::sliderint2, 0, 255);

                            custom::SliderInt("Damage override value", &features::sliderint3, 0, 255);

                            custom::Checkbox("Double tap", &features::check6);

                            custom::Checkbox("Lag peek", &features::check7);


                        }
                        custom::EndChild();

                        custom::Spacing(10.f);

                        custom::BeginChild(pictures::aim_small_img, "Exploits", ImVec2(285, 135));
                        {

                            custom::Checkbox("Esp preview", &features::esp_perview);

                            custom::MultiCombo("Esp variables", features::multi_esp, features::multi_preview, 7);

                        }
                        custom::EndChild();

                        custom::Spacing(10.f);

                        custom::BeginChild(pictures::aim_small_img, "Legitbot", ImVec2(285, 250));
                        {

                            ImGui::InputTextWithHint("Text", "Config name", features::input, 64);

                            ImGui::BeginGroup(); {

                                custom::Button("Create Config", ImVec2(117, 30));

                                ImGui::SameLine(0, 10);

                                custom::Button("Delete Config", ImVec2(117, 30));

                            } ImGui::EndGroup();

                            ImGui::BeginGroup(); {

                                custom::Button("Load Config", ImVec2(117, 30));

                                ImGui::SameLine(0, 10);

                                custom::Button("Save Config", ImVec2(117, 30));

                            } ImGui::EndGroup();

                        }
                        custom::EndChild();

                    }
                    ImGui::EndGroup();

                }
                ImGui::PopStyleVar();
            } ImGui::EndChild();

            ImGui::End();

            ImGui::SameLine(0, 120);

            {
                features::preview_alpha = ImClamp(features::preview_alpha + (4.f * ImGui::GetIO().DeltaTime * (features::esp_perview ? 1.f : -1.f)), 0.f, 1.f);

                ImGuiStyle* style = &ImGui::GetStyle();

                ImGuiContext& g = *GImGui;

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, features::preview_alpha * style->Alpha);

                ImGui::SetNextWindowSize(settings::size_preview);

                if (features::esp_perview)
                {
                    ImGui::Begin("Esp Preview Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);

                    const auto& pos = ImGui::GetWindowPos();
                    const auto& draw_list = ImGui::GetWindowDrawList();

                    custom::GradientRect(ImGui::GetForegroundDrawList(), ImVec2(pos.x, pos.y), ImVec2(pos.x + 300, pos.y + 400), ImGui::GetColorU32(menu::border), ImGui::GetColorU32(menu::border), ImGui::GetColorU32(colors::accent_color), ImGui::GetColorU32(colors::accent_color), 1.f, 2.f);

                    {
                        if (features::multi_esp[0] == true) {

                            draw_list->AddRect(ImVec2(pos.x + 50, pos.y + 50), ImVec2(pos.x + 250, pos.y + 350), ImGui::GetColorU32(colors::preview::rect), 4.f);

                        }

                        if (features::multi_esp[1] == true) {

                            draw_list->AddRectFilled(ImVec2(pos.x + 43, pos.y + 50), ImVec2(pos.x + 45, pos.y + 350), ImGui::GetColorU32(colors::accent_color), 4.f);
                            draw_list->AddShadowRect(ImVec2(pos.x + 43, pos.y + 50), ImVec2(pos.x + 45, pos.y + 350), ImGui::GetColorU32(colors::accent_color), 10.f, ImVec2(0, 0), 0, 4.f);

                        }

                        if (features::multi_esp[2] == true) {

                            draw_list->AddRectFilled(ImVec2(pos.x + 50, pos.y + 355), ImVec2(pos.x + 250, pos.y + 357), ImGui::GetColorU32(colors::accent_color), 4.f);
                            draw_list->AddShadowRect(ImVec2(pos.x + 50, pos.y + 355), ImVec2(pos.x + 250, pos.y + 357), ImGui::GetColorU32(colors::accent_color), 10.f, ImVec2(0, 0), 0, 4.f);

                        }


                        if (features::multi_esp[3] == true) {

                            ShadowText(fonts::sans_font, "nickname", ImVec2(pos.x + 137, pos.y + 33), ImVec2(0, -1), ImGui::GetColorU32(colors::preview::name), true, 12);

                        }

                        if (features::multi_esp[4] == true) {

                            ShadowText(fonts::sans_font, "29m", ImVec2(pos.x + 270, pos.y + 60), ImVec2(0, -1), ImGui::GetColorU32(colors::preview::distance), true, 12);

                        }

                        if (features::multi_esp[5] == true) {

                            ShadowText(fonts::weapon_font, "W", ImVec2(pos.x + 133, pos.y + 378), ImVec2(0, -1), ImGui::GetColorU32(colors::preview::distance), true, 12);

                        }

                        if (features::multi_esp[6] == true) {

                            draw_list->AddCircleFilled(ImVec2(pos.x + 152, pos.y + 105), 11.f, ImGui::GetColorU32(colors::preview::head), 20.f);

                            draw_list->AddShadowCircle(ImVec2(pos.x + 152, pos.y + 105), 11.f, ImGui::GetColorU32(colors::preview::head), 25.f, ImVec2(0, 0));

                            draw_list->AddLine(ImVec2(pos.x + 152, pos.y + 135), ImVec2(pos.x + 152, pos.y + 240), ImGui::GetColorU32(colors::preview::dice));

                            // left hand

                            draw_list->AddLine(ImVec2(pos.x + 100, pos.y + 180), ImVec2(pos.x + 152, pos.y + 135), ImGui::GetColorU32(colors::preview::dice));

                            // right hand

                            draw_list->AddLine(ImVec2(pos.x + 152, pos.y + 135), ImVec2(pos.x + 204, pos.y + 180), ImGui::GetColorU32(colors::preview::dice));

                            // left leg

                            draw_list->AddLine(ImVec2(pos.x + 100, pos.y + 295), ImVec2(pos.x + 152, pos.y + 240), ImGui::GetColorU32(colors::preview::dice));

                            // right leg

                            draw_list->AddLine(ImVec2(pos.x + 152, pos.y + 240), ImVec2(pos.x + 204, pos.y + 295), ImGui::GetColorU32(colors::preview::dice));

                        }
                    }

                    ImGui::End();

                }
                ImGui::PopStyleVar();
            }

            if (features::check2) {

                ImGui::SetNextWindowSize(ImVec2(300, 200));

                ImGui::Begin("Target gui", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
                {

                    const auto& pos = ImGui::GetWindowPos();
                    const auto& draw_list = ImGui::GetWindowDrawList();

                    draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 300, pos.y + 200), ImGui::GetColorU32(menu::window_bg), menu::window_rounding);

                    custom::GradientRect(ImGui::GetForegroundDrawList(), ImVec2(pos.x, pos.y), ImVec2(pos.x + 300, pos.y + 200), ImGui::GetColorU32(menu::border), ImGui::GetColorU32(menu::border), ImGui::GetColorU32(colors::accent_color), ImGui::GetColorU32(colors::accent_color), 1.f, 2.f);

                    draw_list->AddText(fonts::sans_font_small, 16.f, ImVec2(pos.x + 8, pos.y + 10), ImColor(255, 255, 255), "Target GUI");

                    draw_list->AddRectFilledMultiColor(ImVec2(pos.x + 10, pos.y + 35), ImVec2(pos.x + 145, pos.y + 36), ImGui::GetColorU32(colors::accent_color, 0), ImGui::GetColorU32(colors::accent_color), ImGui::GetColorU32(colors::accent_color), ImGui::GetColorU32(colors::accent_color, 0));

                    draw_list->AddRectFilledMultiColor(ImVec2(pos.x + 145, pos.y + 35), ImVec2(pos.x + 290, pos.y + 36), ImGui::GetColorU32(colors::accent_color), ImGui::GetColorU32(colors::accent_color, 0), ImGui::GetColorU32(colors::accent_color, 0), ImGui::GetColorU32(colors::accent_color));

                    ImGui::SetCursorPos(ImVec2(20, 50));

                    ImGui::BeginGroup(); {

                        custom::SliderInt("Damage value", &features::sliderint2, 0, 255);

                        custom::Spacing(18.f);

                        custom::SliderInt("Damage override value", &features::sliderint3, 0, 255);

                    }ImGui::EndGroup();
                }
                ImGui::End();
            }
        }


        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
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

// Helper functions

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
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
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

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
