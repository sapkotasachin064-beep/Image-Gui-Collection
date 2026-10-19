#include "gui/framework/combo/combo.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const float kBoxHeight = 15.f;
    static const float kTextInset = 5.f;
    static const float kArrowSize = 3.f;
    static const float kCaptionGap = 2.f;
    static const float kItemHeight = 13.f;

    bool Combo(const char* label, int* current, const char* const items[], int itemCount)
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
        const char* captionEnd = FindRenderedTextEnd(label);
        const float textInset = style::Scaled(kTextInset);

        if (captionEnd != label)
        {
            const ImVec2 captionSize = CalcTextSize(label, captionEnd);
            window->DrawList->AddText(window->DC.CursorPos, ImColor(style::text), label, captionEnd);
            window->DC.CursorPos.y += captionSize.y + style::Scaled(kCaptionGap);
        }

        const ImVec2 pos = window->DC.CursorPos;
        const float boxWidth = ImMax(window->WorkRect.Max.x - pos.x, 16.f);
        const ImRect bb(pos, pos + ImVec2(boxWidth, style::Scaled(kBoxHeight)));

        ItemSize(bb);
        if (!ItemAdd(bb, id))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        if (ButtonBehavior(bb, id, &isHovered, &isHeld))
        {
            OpenPopup(label);
        }

        ImDrawList* drawList = window->DrawList;
        drawList->AddRectFilled(bb.Min, bb.Max, ImColor(style::controlBg));
        drawList->AddRect(bb.Min, bb.Max, ImColor(isHovered ? style::strokeHover : style::stroke));

        const char* currentText = (*current >= 0 && *current < itemCount) ? items[*current] : "";
        const ImVec2 currentSize = CalcTextSize(currentText);
        drawList->AddText(
            ImVec2(bb.Min.x + textInset, bb.Min.y + (bb.GetHeight() - currentSize.y) * 0.5f),
            ImColor(style::textStrong), currentText);

        const float arrowSize = style::Scaled(kArrowSize);
        const ImVec2 arrowCenter(bb.Max.x - textInset - arrowSize, (bb.Min.y + bb.Max.y) * 0.5f);
        drawList->AddTriangleFilled(
            ImVec2(arrowCenter.x - arrowSize, arrowCenter.y - arrowSize * 0.5f),
            ImVec2(arrowCenter.x + arrowSize, arrowCenter.y - arrowSize * 0.5f),
            ImVec2(arrowCenter.x, arrowCenter.y + arrowSize * 0.7f),
            ImColor(style::textDisabled));

        bool hasChanged = false;
        const float itemHeight = style::Scaled(kItemHeight);

        SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y));
        SetNextWindowSize(ImVec2(boxWidth, itemHeight * itemCount + style::Scaled(4.f)));
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, style::Scaled(2.f)));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

        if (BeginPopup(label, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImGuiWindow* popup = GetCurrentWindow();
            for (int index = 0; index < itemCount; index++)
            {
                const ImVec2 itemPos = popup->DC.CursorPos;
                const ImRect itemBb(itemPos, itemPos + ImVec2(boxWidth, itemHeight));
                const ImGuiID itemId = popup->GetID(index);

                ItemSize(itemBb);
                if (!ItemAdd(itemBb, itemId))
                {
                    continue;
                }

                bool isItemHovered = false;
                bool isItemHeld = false;
                if (ButtonBehavior(itemBb, itemId, &isItemHovered, &isItemHeld))
                {
                    *current = index;
                    hasChanged = true;
                    CloseCurrentPopup();
                }

                const ImVec4 itemColor = (index == *current || isItemHovered) ? style::accent : style::text;
                const ImVec2 itemTextSize = CalcTextSize(items[index]);
                popup->DrawList->AddText(
                    ImVec2(itemBb.Min.x + textInset, itemBb.Min.y + (itemHeight - itemTextSize.y) * 0.5f),
                    ImColor(itemColor), items[index]);
            }
            EndPopup();
        }

        PopStyleVar(2);
        return hasChanged;
    }
}
