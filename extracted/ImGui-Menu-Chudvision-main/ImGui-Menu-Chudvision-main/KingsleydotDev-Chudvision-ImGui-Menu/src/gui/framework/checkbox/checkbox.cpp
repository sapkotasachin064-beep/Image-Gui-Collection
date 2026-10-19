#include "gui/framework/checkbox/checkbox.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const float kBoxSize = 10.f;
    static const float kLabelGap = 3.f;

    bool Checkbox(const char* label, bool* isEnabled)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }
        if (!MatchesSearch(label))
        {
            return false;
        }

        const ImGuiID id = window->GetID(label);
        const char* labelEnd = FindRenderedTextEnd(label);
        const ImVec2 labelSize = CalcTextSize(label, labelEnd);

        const float boxSize = style::Scaled(kBoxSize);
        const float labelGap = style::Scaled(kLabelGap);
        const float rowHeight = ImMax(boxSize, labelSize.y);

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + ImVec2(boxSize + labelGap + labelSize.x, rowHeight));

        ItemSize(bb);
        if (!ItemAdd(bb, id))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        const bool isPressed = ButtonBehavior(bb, id, &isHovered, &isHeld);
        if (isPressed)
        {
            *isEnabled = !*isEnabled;
            MarkItemEdited(id);
        }

        const ImVec2 boxMin(pos.x, pos.y + (rowHeight - boxSize) * 0.5f);
        const ImVec2 boxMax(boxMin.x + boxSize, boxMin.y + boxSize);

        ImDrawList* drawList = window->DrawList;
        drawList->AddRectFilled(boxMin, boxMax, ImColor(style::checkboxBg));

        drawList->AddRectFilled(boxMin, ImVec2(boxMax.x, boxMin.y + 1.f), ImColor(style::bevelDark));
        drawList->AddRectFilled(boxMin, ImVec2(boxMin.x + 1.f, boxMax.y), ImColor(style::bevelDark));
        drawList->AddRectFilled(ImVec2(boxMin.x, boxMax.y - 1.f), boxMax, ImColor(style::bevelLight));
        drawList->AddRectFilled(ImVec2(boxMax.x - 1.f, boxMin.y), boxMax, ImColor(style::bevelLight));

        if (*isEnabled)
        {
            drawList->PathLineTo(ImVec2(boxMin.x + boxSize * 0.22f, boxMin.y + boxSize * 0.50f));
            drawList->PathLineTo(ImVec2(boxMin.x + boxSize * 0.44f, boxMin.y + boxSize * 0.72f));
            drawList->PathLineTo(ImVec2(boxMin.x + boxSize * 0.80f, boxMin.y + boxSize * 0.24f));
            drawList->PathStroke(ImColor(style::accent), 0, style::Scaled(2.f));
        }

        const ImVec2 labelPos(boxMax.x + labelGap, pos.y + (rowHeight - labelSize.y) * 0.5f);
        const ImU32 labelColor = ImColor(*isEnabled ? style::textStrong : style::text);
        drawList->AddText(labelPos, labelColor, label, labelEnd);

        return isPressed;
    }
}
