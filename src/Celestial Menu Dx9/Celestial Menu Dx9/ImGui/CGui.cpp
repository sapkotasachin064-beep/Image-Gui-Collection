#include "CGui.h"
#include <vector>
#include <algorithm>
#include <chrono>

#include "Resources/Fonts/IconsFontAwesome6.h"
#include "Resources/Fonts/IconsFontAwesome6Brands.h"
#include "Resources/Fonts/Poppins_Medium.h"


#include "esp_preview.h"

static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;


const char* cheat_name =  "celestial";
bool watermark = true;

static int notify_select = 0;
const char* notify_items[2]{ "Circle", "Line" };


CGui::CGui(Textures* pTextures)
{
    m_pTextures = pTextures;
}

CGui::CGui(const CGui& other)
{
}

CGui::~CGui()
{
}
void CricleProgress(const char* name, float progress, float max, float radius, const ImVec2& size)
{
    const float tickness = 3.f;
    static float position = 0.f;

    position = progress / max * 6.28f;

    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcTo(ImGui::GetCursorScreenPos() + size, radius, 0.f, 2.f * IM_PI, 120.f);
    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::elements::background_widget), 0, tickness);

    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcTo(ImGui::GetCursorScreenPos() + size, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 120.f);
    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::accent), 0, tickness);

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

        ImGui::SetNextWindowPos(ImVec2(duePercentage < 15.f ? duePercentage : 15.f, 15 + position * 120));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 150);
        ImGui::Begin(("##NOTIFY" + std::to_string(position)).c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImVec2 pos = ImGui::GetWindowPos(), spacing = ImGui::GetStyle().ItemSpacing, region = ImGui::GetContentRegionMax();



        if (notify_select == 0)
            CricleProgress("##NOTIFY", percentage, 100, 7.f, ImVec2(ImGui::GetContentRegionMax().x - 40, 11));
        if (notify_select == 1)
            ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 3), pos + ImVec2(region.x * (duePercentage / 100.0f), region.y), ImGui::GetColorU32(c::accent, alpha), c::elements::rounding);


        ImGui::TextColored(ImColor(ImGui::GetColorU32(c::accent, alpha)), "%s", "[Notification]");
        ImGui::TextColored(ImColor(ImGui::GetColorU32(c::elements::text_active, alpha)), "%s", message.c_str());
        ImGui::Dummy(ImVec2(ImGui::CalcTextSize(message.c_str()).x + 15, 5));

        ImGui::End();
        ImGui::PopStyleVar();
    }
};

NotificationSystem notificationSystem;


bool CGui::Initialize(void* hWnd, IDirect3DDevice9* pDevice)
{
    if (!pDevice)
        return false;
    
    bool result = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.IniFilename = nullptr;                               // Disable INI File  
    GImGui->NavDisableHighlight = true;                     // Disable Highlighting

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();


    // Custom styles
    style.WindowRounding    = 3;
    style.ChildRounding     = 2;
    style.FrameRounding     = 2;
    style.PopupRounding     = 2;
    style.GrabRounding      = 2;
    style.TabRounding       = 2;
    style.ScrollbarRounding = 1;

    style.ButtonTextAlign   = { 0.5f, 0.5f };
    style.WindowTitleAlign  = { 0.5f, 0.5f };
    style.FramePadding      = { 8.0f, 8.0f };
    style.ItemSpacing       = { 14.0f, 14.0f };
    style.WindowPadding     = { 14.0f, 14.0f };
    style.ItemInnerSpacing  = { 8.0f, 4.0f };

    style.WindowBorderSize  = 0;
    style.FrameBorderSize   = 0;

    style.ScrollbarSize     = 12.f;
    style.GrabMinSize       = 8.f;
    style.WindowShadowSize  = 2.f;
    
    //style.Colors[ImGuiCol_WindowShadow]         = ImAdd::HexToColorVec4(0x96BBFD, 0.1f);

    style.Colors[ImGuiCol_WindowBg]             = c::background::filling;
    style.Colors[ImGuiCol_PopupBg]              = c::background::filling;
    style.Colors[ImGuiCol_ChildBg]              = c::background::child;
    style.Colors[ImGuiCol_MenuBarBg]            = c::background::child;
    style.Colors[ImGuiCol_TitleBg]              = c::background::child;
    style.Colors[ImGuiCol_ScrollbarBg]          = c::background::child;

    style.Colors[ImGuiCol_Border]               = c::elements::border;
    style.Colors[ImGuiCol_Separator]            = c::elements::seperator;

    style.Colors[ImGuiCol_CheckMark]            = c::elements::mark;
    style.Colors[ImGuiCol_Text]                 = c::elements::text;
    style.Colors[ImGuiCol_TextDisabled] = c::elements::text_disabled;

    style.Colors[ImGuiCol_ScrollbarBg] = c::elements::scrollbar_background;
    style.Colors[ImGuiCol_ScrollbarGrab] = c::elements::scrollbar_grabbed;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = c::elements::scrollbar_grab_hovered;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = c::elements::scrollbar_active;

    style.Colors[ImGuiCol_SliderGrab] = c::accent;
    style.Colors[ImGuiCol_SliderGrabActive]     = c::accent;

    style.Colors[ImGuiCol_Button] = c::elements::button;
        style.Colors[ImGuiCol_ButtonHovered] = c::elements::button_hovered;
        style.Colors[ImGuiCol_ButtonActive] = c::elements::button_active;

        style.Colors[ImGuiCol_FrameBg] = c::elements::button;
    style.Colors[ImGuiCol_FrameBgHovered]       = c::elements::button_hovered;
    style.Colors[ImGuiCol_FrameBgActive]        = c::elements::button_active;

    style.Colors[ImGuiCol_Header]               = c::background::child;
    style.Colors[ImGuiCol_HeaderHovered]        = c::background::child; style.Colors[ImGuiCol_HeaderHovered].w = 0.7f;
    style.Colors[ImGuiCol_HeaderActive]         = c::background::child; style.Colors[ImGuiCol_HeaderActive].w = 0.5f;
    
    // Setup Font
    ImFontConfig cfg;
    //cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    font::mainfont = io.Fonts->AddFontFromMemoryCompressedTTF(Poppins_Medium_compressed_data, Poppins_Medium_compressed_size, 20.f, &cfg, io.Fonts->GetGlyphRangesDefault());
  

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };

    ImFontConfig fa_config; fa_config.MergeMode = true; fa_config.PixelSnapH = true;
    fa_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;

    ImFont* fontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14, &fa_config, icons_ranges);
    ImFont* fontAwesomeBrands = io.Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14, &fa_config, icons_ranges_brands);

    // Setup Platform/Renderer backends
    result &= ImGui_ImplWin32_Init(hWnd);
    result &= ImGui_ImplDX9_Init(pDevice);

	return result;
}

void CGui::Shutdown()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


void CGui::PreRender()
{
    ImGuiStyle& style = ImGui::GetStyle();


    static int   CurrentPage  = 0;
    static int   SubPage  = 0;
    static float SideBarWidth = 150;

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        notificationSystem.DrawNotifications();

        if (watermark)
        {
            const float cheat_name_size = ImGui::CalcTextSize(cheat_name).x;

            const float ibar_size = cheat_name_size * 2;

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - (ibar_size + 15), 15));
            ImGui::SetNextWindowSize(ImVec2(ibar_size, 45));

            ImGui::Begin("info-bar", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
            {
                const ImVec2& pos = ImGui::GetWindowPos(), spacing = style.ItemSpacing, region = ImGui::GetContentRegionMax();

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(ImGuiCol_WindowBg), 5.f);
                ImGui::GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32(ImGuiCol_WindowBg), 5.f);

                const char* info_set[1] = { cheat_name };

                ImGui::SetCursorPos(ImVec2(ImGui::CalcTextSize(cheat_name).x / 2 - 5, (45 - ImGui::CalcTextSize(cheat_name).y) / 2 - 6));
                ImGui::BeginGroup();
                {
                    for (int i = 0; i < ARRAYSIZE(info_set); i++) {
                        ImGui::TextColored(i < 1 ? ImColor(ImGui::GetColorU32(ImGuiCol_SliderGrab)) : ImColor(ImGui::GetColorU32(ImGuiCol_Text)), info_set[i]);
                        ImGui::SameLine();
                        ImGui::SameLine();
                    }
                }
                ImGui::EndGroup();
            }
            ImGui::End();
        }



        static float ui_dpi_scale = 0.f;
        static bool show_menu = false;


        if (GetAsyncKeyState(VK_INSERT) & 1) { show_menu = !show_menu; }

        if (!show_menu && ui_dpi_scale <= 0.05f)
            return;


        ui_dpi_scale = ImLerp(ui_dpi_scale, show_menu ? 1.0f : -0.5f, 0.5f * (12.0f * ImGui::GetIO().DeltaTime));


        ImGui::SetNextWindowSize(ImVec2({ c::size.x, c::size.y * ui_dpi_scale }));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
        ImGui::PopStyleVar(2);
        // Renders
        {
            ImVec2 pos = ImGui::GetWindowPos();
            ImVec2 size = ImGui::GetWindowSize();
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            float new_x = ImLerp(c::size.x, (active_tab == 1) ? 1050.0f : 650.0f, ImGui::GetIO().DeltaTime * 15.0f);
            c::size = ImVec2(new_x, c::size.y);

            drawList->AddRectFilled(pos, pos + ImVec2(size.x, ImGui::GetFrameHeight()), ImGui::GetColorU32(ImGuiCol_ChildBg), style.WindowRounding, ImDrawFlags_RoundCornersTop);
            drawList->AddRectFilled(pos + ImVec2(0, ImGui::GetFrameHeight()), pos + ImVec2(size.x, size.y - ImGui::GetFrameHeight()), ImGui::GetColorU32(ImGuiCol_WindowBg), style.WindowRounding, ImDrawFlags_RoundCornersNone);
            drawList->AddRectFilled(pos + ImVec2(0, size.y - ImGui::GetFrameHeight()), pos + size, ImGui::GetColorU32(ImGuiCol_ChildBg), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
            drawList->AddText(pos + ImVec2(style.FramePadding.x, style.FramePadding.y), ImGui::GetColorU32(ImGuiCol_SliderGrab), cheat_name);
            drawList->AddText(pos + ImVec2(style.FramePadding.x, size.y - ImGui::GetFontSize() - style.FramePadding.y), ImGui::GetColorU32(ImGuiCol_TextDisabled), "30 day left");
            /*ImVec2 center = { ImGui::GetCurrentWindow()->Rect().GetCenter().x, ImGui::GetCurrentWindow()->Rect().Max.y + 20 };
            drawList->AddCircle(center, 50, ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.3f), 0, 2);
            drawList->AddCircle(center, 100, ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.2f), 0, 2);
            drawList->AddCircle(center, 180, ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.1f), 0, 2);
            drawList->AddCircle(center, 260, ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.075f), 0, 2);*/
        }
        // Content
        {

            static std::vector<const char*> subList = { "pistol", "test", "rifles", "heavy", "melees" };
            float subWidth = 0;

            for (size_t i = 0; i < subList.size(); i++)
            {
                subWidth += ImGui::CalcTextSize(subList[i]).x + style.ItemSpacing.x;
            }

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - subWidth);
            ImGui::BeginChild("SubTabs", ImVec2(0, ImGui::GetFrameHeight()), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                {
                    for (size_t i = 0; i < subList.size(); i++)
                    {
                        ImAdd::RadioButton(subList[i], &SubPage, i, ImVec2(0, -0.1f));
                        ImGui::SameLine();
                    }
                    ImGui::NewLine(); // cancel last new line
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndChild();

            ImGui::SetCursorPosY(ImGui::GetFrameHeight() + 10);
            ImGui::BeginGroup();
            {
                ImGui::BeginChild("Sidebar", ImVec2(SideBarWidth, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_ChildBg]);
                    {
                        ImAdd::RadioButtonIcon("aimbot", ICON_FA_CROSSHAIRS, "Aimbot", &CurrentPage, 0, ImVec2(-0.1f, 0));
                        ImAdd::RadioButtonIcon("weapons", ICON_FA_GUN, "Weapons", &CurrentPage, 1, ImVec2(-0.1f, 0));
                        ImAdd::RadioButtonIcon("vehicles", ICON_FA_CAR, "Vehicles", &CurrentPage, 2, ImVec2(-0.1f, 0));
                        ImAdd::RadioButtonIcon("players", ICON_FA_USER, "Players", &CurrentPage, 3, ImVec2(-0.1f, 0));
                        ImAdd::RadioButtonIcon("objects", ICON_FA_CUBE, "Objects", &CurrentPage, 4, ImVec2(-0.1f, 0));
                        ImAdd::RadioButtonIcon("executor", ICON_FA_CODE, "Executor", &CurrentPage, 5, ImVec2(-0.1f, 0));
                        ImGui::Separator();
                        ImAdd::RadioButtonIcon("configs", ICON_FA_LIST, "Configs", &CurrentPage, 6, ImVec2(-0.1f, 0));
                        ImAdd::RadioButtonIcon("settings", ICON_FA_GEAR, "Settings", &CurrentPage, 7, ImVec2(-0.1f, 0));
                    }
                    ImGui::PopStyleColor();
                }
                ImGui::EndChild();

                tab_alpha = ImLerp(tab_alpha, (CurrentPage == active_tab) ? 1.f : 0.f, 12.f * ImGui::GetIO().DeltaTime);
                if (tab_alpha < 0.01f && tab_add < 0.01f) active_tab = CurrentPage;

                ImGui::SetCursorPos(ImVec2(SideBarWidth - style.WindowPadding.x, style.WindowPadding.y + 30));
                ImGui::SetCursorPos(ImVec2(SideBarWidth - style.WindowPadding.x, 140 - (tab_alpha * 100)));

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style.Alpha);
                {
                    if (active_tab == 0)
                    {
                        ImGui::BeginChild("Content", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                        {
                            static int key1 = 0;
                            if (ImAdd::Keybind("Keybind", &key1))
                            {
                                notificationSystem.AddNotification("You have successfully bound a key!", 2500);
                            }


                            static int iSlider = 7;
                            static float fSlider = 3;
                            ImAdd::SliderInt("Int Slider", &iSlider, 0, 100);
                            ImAdd::SliderFloat("Float Slider", &fSlider, 0, 180);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                            {
                                ImGui::SetTooltip("Select at least one difficulty");
                            }

                            static bool bCheckbox = true;
                            ImGrp::Checkbox("AimAssist", &bCheckbox);
                            if (ImGui::IsItemClicked() && ImGui::IsItemHovered()) notificationSystem.AddNotification("You clicked on the check box, the notification is here!", 4000);


                            static int select = 0;
                            const char* items[2]{ "Enemy", "yourdaddy"};
                            ImAdd::Combo("Combo", "Select type", &select, items, IM_ARRAYSIZE(items), 2);


                            static bool multi_num[5] = { false, true, true, false, true };
                            const char* multi_items[5] = { "Head", "Chest", "Stromatch", "Body", "Legs" };
                            ImAdd::MultiCombo("MultiCombo", "What body parts will the ragebot shoot at?", multi_num, multi_items, 5);


                            static ImVec4 colpick = ImVec4(1, 1, 0, 0.5f);
                            ImGrp::ColorEdit4("FOV Color", (float*)&colpick);

                            static char input[255] = { "" };
                            ImGui::InputTextEx(ICON_FA_HOT_TUB_PERSON, "Enter text", input, 255, ImVec2(ImGui::GetContentRegionMax().x - 15, 50), NULL);
                            ImAdd::Button("Hit me", ImVec2(ImGui::GetContentRegionMax().x - style.WindowPadding.x, 50));
                        }
                        ImGui::EndChild();

                    }


                    if (active_tab == 1)
                    {
                        ImGui::BeginChild("Content", ImVec2(c::size.x / 2, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                        {

                            ImGrp::Checkbox("Box", &esp_ff[0]);
                            ImGrp::Checkbox("Health", &esp_ff[1]);
                            ImGrp::Checkbox("Armor", &esp_ff[2]);
                            ImGrp::Checkbox("Reload", &esp_ff[3]);
                            ImGrp::Checkbox("Nickname", &esp_ff[4]);
                            ImGrp::Checkbox("Distance", &esp_ff[5]);
                            ImGrp::Checkbox("Flash", &esp_ff[6]);
                            ImGrp::Checkbox("Money", &esp_ff[7]);
                            ImGrp::Checkbox("Blind", &esp_ff[8]);
                            ImGrp::Checkbox("Lc", &esp_ff[9]);
                            ImGrp::Checkbox("Scope", &esp_ff[10]);
                            ImGrp::Checkbox("Ping", &esp_ff[11]);
                            ImGrp::Checkbox("KD", &esp_ff[12]);
                            ImGrp::Checkbox("Weapon Icon", &esp_ff[13]);

                        }
                        ImGui::EndChild();
                        ImGui::SameLine();
                        ImGui::BeginChild("Content22", ImVec2(-1, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                        {

                            m_esp_preview.set_positions(esp_ff[1], esp_ff[2], esp_ff[3], esp_ff[4], esp_ff[5], esp_ff[6], esp_ff[7], esp_ff[8], esp_ff[9], esp_ff[10], esp_ff[11], esp_ff[12], esp_ff[13]);
                            m_esp_preview.on_draw();
                        }
                        ImGui::EndChild();
                    }
                    else
                    if (active_tab == 6)
                    {
                        ImGui::BeginChild("Content", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                        {
                            ImGrp::Checkbox("1231231", &watermark);



                        }
                        ImGui::EndChild();
                    }
                    else
                    if (active_tab == 7)
                    {
                        ImGui::BeginChild("Content", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);
                        {
                            ImGrp::Checkbox("Watermark", &watermark);
                            ImAdd::SliderFloat("Glow thickness", &c::ShadowThickness, 0, 30);


                        }
                        ImGui::EndChild();
                    }

                }
                ImGui::PopStyleVar();

            }
            ImGui::EndGroup();
        }
        ImGui::End();
    }
    // Rendering
    ImGui::EndFrame();
}


void CGui::Render()
{
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CGui::MsgProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    return ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam);
}
