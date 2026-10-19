#include "gui/framework/button/button.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const ImVec2 kAutoPad(12.f, 5.f);

    bool Button(const char* label, const ImVec2& size)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (!MatchesSearch(label))
        {
            return false;
        }
        if (window->SkipItems)
        {
            return false;
        }

        const ImGuiID id = window->GetID(label);
        const char* labelEnd = FindRenderedTextEnd(label);
        const ImVec2 labelSize = CalcTextSize(label, labelEnd);

        ImVec2 buttonSize = size;
        if (buttonSize.x == 0.f)
        {
            buttonSize.x = labelSize.x + style::Scaled(kAutoPad.x);
        }
        if (buttonSize.y == 0.f)
        {
            buttonSize.y = labelSize.y + style::Scaled(kAutoPad.y);
        }

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + buttonSize);

        ItemSize(bb);
        if (!ItemAdd(bb, id))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        const bool isPressed = ButtonBehavior(bb, id, &isHovered, &isHeld);

        ImVec4 borderColor = style::stroke;
        if (isHovered || isHeld)
        {
            borderColor = style::strokeHover;
        }

        ImVec4 labelColor = style::text;
        if (isHovered || isHeld)
        {
            labelColor = style::textStrong;
        }

        ImDrawList* drawList = window->DrawList;
        drawList->AddRectFilled(bb.Min, bb.Max, ImColor(style::controlBg));
        drawList->AddRect(bb.Min, bb.Max, ImColor(borderColor));
        drawList->AddText(
            ImVec2(bb.Min.x + (buttonSize.x - labelSize.x) * 0.5f, bb.Min.y + (buttonSize.y - labelSize.y) * 0.5f),
            ImColor(labelColor), label, labelEnd);

        return isPressed;
    }
}
