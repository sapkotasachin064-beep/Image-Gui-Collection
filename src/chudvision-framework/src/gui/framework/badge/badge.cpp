#include "gui/framework/badge/badge.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const ImVec2 kBadgeSize(17.f, 8.f);
    static const float kBadgeTextSize = 7.f;

    void StatusBadge(bool isOn)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return;
        }

        if (IsRowFiltered())
        {
            return;
        }

        const ImVec2 badgeSize(style::Scaled(kBadgeSize.x), style::Scaled(kBadgeSize.y));
        PlaceRight(badgeSize);

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + badgeSize);

        ItemSize(bb);
        if (!ItemAdd(bb, 0))
        {
            return;
        }

        const char* stateText = "OFF";
        ImVec4 stateColor = style::textDisabled;
        if (isOn)
        {
            stateText = "ON";
            stateColor = style::toggleOn;
        }

        ImDrawList* drawList = window->DrawList;
        drawList->AddRectFilled(bb.Min, bb.Max, ImColor(style::controlBg));
        drawList->AddRect(bb.Min, bb.Max, ImColor(style::stroke));

        PushFont(fonts::body, style::Scaled(kBadgeTextSize));
        const ImVec2 stateSize = CalcTextSize(stateText);
        drawList->AddText(fonts::body, style::Scaled(kBadgeTextSize),
            ImVec2(bb.Min.x + (badgeSize.x - stateSize.x) * 0.5f, bb.Min.y + (badgeSize.y - stateSize.y) * 0.5f),
            ImColor(stateColor), stateText);
        PopFont();
    }
}
