#include "gui/gui.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/bind/bind.hpp"
#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/tab/tab.hpp"

static const char* const kTabNames[gui::Tab_Count] = { "Aimbot", "ESP", "Players", "Visuals", "Misc", "Config" };

static const float kTabRowInset = 6.f;
static const float kContentGap = 0.f;
static const float kPagePadding = 4.f;
static const ImVec2 kWindowPadding(3.f, 3.f);
static const ImVec2 kItemSpacing(8.f, 5.f);

static void ApplyBaseStyle() noexcept
{
    ImGuiStyle& imStyle = ImGui::GetStyle();

    imStyle.WindowRounding = 0.f;
    imStyle.ChildRounding = 0.f;
    imStyle.FrameRounding = 0.f;
    imStyle.PopupRounding = 0.f;
    imStyle.GrabRounding = 0.f;
    imStyle.ScrollbarRounding = 0.f;

    imStyle.WindowBorderSize = 1.f;
    imStyle.ChildBorderSize = 0.f;
    imStyle.PopupBorderSize = 1.f;
    imStyle.FrameBorderSize = 0.f;

    imStyle.WindowPadding = ImVec2(style::Scaled(kWindowPadding.x), style::Scaled(kWindowPadding.y));
    imStyle.ItemSpacing = ImVec2(style::Scaled(kItemSpacing.x), style::Scaled(kItemSpacing.y));

    imStyle.Colors[ImGuiCol_WindowBg] = style::headerBg;
    imStyle.Colors[ImGuiCol_ChildBg] = ImVec4(0.f, 0.f, 0.f, 0.f);
    imStyle.Colors[ImGuiCol_PopupBg] = style::listRowEven;
    imStyle.Colors[ImGuiCol_Border] = style::windowBorder;
    imStyle.Colors[ImGuiCol_Text] = style::text;
    imStyle.Colors[ImGuiCol_TextDisabled] = style::textDisabled;
    imStyle.Colors[ImGuiCol_TextSelectedBg] = ImVec4(style::accent.x, style::accent.y, style::accent.z, 0.3f);

    imStyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.f, 0.f, 0.f, 0.f);
    imStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
    imStyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.f, 0.f, 0.f, 0.f);

    imStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
    imStyle.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.f, 0.f, 0.f, 0.f);

    imStyle.Colors[ImGuiCol_NavCursor] = ImVec4(0.f, 0.f, 0.f, 0.f);
    imStyle.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.f, 0.f, 0.f, 0.f);
    imStyle.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.f, 0.f, 0.f, 0.f);
}

static void DrawTitle(ImDrawList* drawList, const ImVec2& pos) noexcept
{
    ImGui::PushFont(fonts::bold, style::Scaled(style::titleSize));

    const ImVec2 sizeChud = ImGui::CalcTextSize("chud");
    const ImVec2 sizeVision = ImGui::CalcTextSize("vision");

    drawList->AddText(pos, ImColor(style::titleText), "chud");
    drawList->AddText(ImVec2(pos.x + sizeChud.x, pos.y), ImColor(style::accent), "vision");
    drawList->AddText(ImVec2(pos.x + sizeChud.x + sizeVision.x, pos.y), ImColor(style::titleText), ".net");

    ImGui::PopFont();
}

void gui::Render() noexcept
{
    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
    {
        open = !open;
    }

    if (!open)
    {
        return;
    }

    ksd::UpdateBinds();
    pages::ApplyMenuScale();
    ApplyBaseStyle();

    const ImVec2 scaledSize(style::Scaled(menuSize.x), style::Scaled(menuSize.y));

    static float appliedScale = 0.f;
    if (appliedScale != style::scale)
    {
        appliedScale = style::scale;
        ImGui::SetNextWindowSize(scaledSize, ImGuiCond_Always);
    }
    else
    {
        ImGui::SetNextWindowSize(scaledSize, ImGuiCond_FirstUseEver);
    }

    ImGui::SetNextWindowSizeConstraints(
        ImVec2(style::Scaled(menuMinSize.x), style::Scaled(menuMinSize.y)),
        ImVec2(FLT_MAX, FLT_MAX));

    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoScrollWithMouse;

    if (ImGui::Begin("###chudvision", &open, windowFlags))
    {
        ImGui::PushFont(fonts::body, style::Scaled(style::textSize));

        const ImVec2 rawPos = ImGui::GetWindowPos();
        const ImVec2 rawSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2(IM_TRUNC(rawPos.x), IM_TRUNC(rawPos.y)));
        ImGui::SetWindowSize(ImVec2(IM_TRUNC(rawSize.x), IM_TRUNC(rawSize.y)));

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const ImVec2 windowPos = ImGui::GetWindowPos();
        const float windowWidth = ImGui::GetWindowWidth();
        const float titleHeight = style::Scaled(style::titleBarHeight);
        const float tabHeight = style::Scaled(style::tabBarHeight);
        const float tabRowBottom = windowPos.y + titleHeight + tabHeight;

        drawList->AddRectFilled(
            windowPos,
            ImVec2(windowPos.x + windowWidth, tabRowBottom),
            ImColor(style::headerBg));

        DrawTitle(drawList, ImVec2(windowPos.x + style::Scaled(6.f), windowPos.y + style::Scaled(2.f)));

        if (ksd::searchEnabled)
        {
            const float searchWidth = style::Scaled(110.f);
            ImGui::SetCursorPos(ImVec2(
                windowWidth - style::Scaled(kTabRowInset) - searchWidth,
                style::Scaled(2.f)));
            ksd::SearchInput(searchWidth);
        }
        else
        {
            ksd::ClearSearch();
        }

        drawList->AddLine(
            ImVec2(windowPos.x + style::Scaled(kTabRowInset), tabRowBottom),
            ImVec2(windowPos.x + windowWidth - style::Scaled(kTabRowInset), tabRowBottom),
            ImColor(style::stroke));

        const ImVec2 contentMin(
            windowPos.x + style::Scaled(kTabRowInset),
            tabRowBottom + style::Scaled(kContentGap));
        const ImVec2 contentMax(
            windowPos.x + windowWidth - style::Scaled(kTabRowInset),
            windowPos.y + ImGui::GetWindowHeight() - style::Scaled(kTabRowInset));
        drawList->AddRectFilled(contentMin, contentMax, ImColor(style::panelBg));

        const float wellEdge = style::Scaled(1.f);
        drawList->AddRectFilled(
            ImVec2(contentMin.x, contentMin.y),
            ImVec2(contentMax.x, contentMin.y + wellEdge), ImColor(style::tabBorder));
        drawList->AddRectFilled(
            ImVec2(contentMin.x, contentMin.y),
            ImVec2(contentMin.x + wellEdge, contentMax.y), ImColor(style::tabBorder));

        drawList->AddRectFilled(
            ImVec2(contentMax.x - wellEdge, contentMin.y),
            ImVec2(contentMax.x, contentMax.y), ImColor(style::windowBorder));
        drawList->AddRectFilled(
            ImVec2(contentMin.x, contentMax.y - wellEdge),
            ImVec2(contentMax.x, contentMax.y), ImColor(style::windowBorder));

        const ImVec2 gripCorner(
            windowPos.x + windowWidth - style::Scaled(2.f),
            windowPos.y + ImGui::GetWindowHeight() - style::Scaled(2.f));
        const float gripSize = style::Scaled(6.f);
        drawList->AddTriangleFilled(
            gripCorner,
            ImVec2(gripCorner.x - gripSize, gripCorner.y),
            ImVec2(gripCorner.x, gripCorner.y - gripSize),
            ImColor(style::stroke));

        ImGui::SetCursorPos(ImVec2(style::Scaled(kTabRowInset), titleHeight));
        for (int index = 0; index < Tab_Count; index++)
        {
            if (index > 0)
            {
                ImGui::SameLine(0.f, 0.f);
            }

            if (ksd::TopTab(kTabNames[index], currentTab == index))
            {
                currentTab = index;
            }
        }

        ImGui::SetCursorPos(ImVec2(
            style::Scaled(kTabRowInset) + wellEdge,
            titleHeight + tabHeight + style::Scaled(kContentGap) + wellEdge));
        const ImVec2 pageSize(
            contentMax.x - contentMin.x - wellEdge * 2.f,
            contentMax.y - contentMin.y - wellEdge * 2.f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
            ImVec2(style::Scaled(kPagePadding), style::Scaled(kPagePadding)));
        const bool isPageVisible = ImGui::BeginChild(
            "##content", pageSize, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);
        ImGui::PopStyleVar();

        if (isPageVisible)
        {
            switch (currentTab)
            {
            case Tab_Aimbot:
                pages::RenderAimbot();
                break;
            case Tab_Esp:
                pages::RenderEsp();
                break;
            case Tab_Players:
                pages::RenderPlayers();
                break;
            case Tab_Visuals:
                pages::RenderVisuals();
                break;
            case Tab_Misc:
                pages::RenderMisc();
                break;
            case Tab_Config:
                pages::RenderConfig();
                break;
            }
        }
        ImGui::EndChild();

        ImGui::PopFont();
    }

    ImGui::End();
}
