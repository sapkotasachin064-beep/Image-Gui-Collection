#include "gui/framework/list/list.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

#include <cstring>

namespace ksd
{
    using namespace ImGui;

    static const float kRowHeight = 13.f;
    static const float kBoxSize = 10.f;
    static const float kBoxInset = 4.f;
    static const float kLabelGap = 5.f;
    static const float kPadPull = 6.f;

    static ImVector<int> listRowStack;

    static ImVec4 RowFill(int rowIndex)
    {
        if (rowIndex % 2 == 0)
        {
            return style::listRowEven;
        }
        return style::listRowOdd;
    }

    bool BeginList(const char* id, const ImVec2& size)
    {
        const float pull = style::Scaled(kPadPull);
        SetCursorPosX(GetCursorPosX() - pull);

        ImVec2 listSize = size;
        if (listSize.x == 0.f)
        {
            listSize.x = GetContentRegionAvail().x + pull;
        }
        if (listSize.y == 0.f)
        {
            listSize.y = GetContentRegionAvail().y + style::Scaled(3.f);
        }

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

        const bool isOpen = BeginChild(id, listSize, ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar);
        listRowStack.push_back(0);
        return isOpen;
    }

    static bool ListRowBase(const char* label, bool* isChecked, bool isSelected)
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

        int& rowIndex = listRowStack.back();
        const float rowHeight = style::Scaled(kRowHeight);
        const float rowWidth = window->WorkRect.Max.x - window->WorkRect.Min.x;

        const ImGuiID id = window->GetID(label);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + ImVec2(rowWidth, rowHeight));

        ItemSize(bb);
        if (!ItemAdd(bb, id))
        {
            rowIndex++;
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        const bool isPressed = ButtonBehavior(bb, id, &isHovered, &isHeld);

        ImDrawList* drawList = window->DrawList;
        drawList->AddRectFilled(bb.Min, bb.Max, ImColor(RowFill(rowIndex)));
        rowIndex++;

        float labelX = bb.Min.x + style::Scaled(kBoxInset);
        bool wasSelected = false;

        if (isChecked != nullptr)
        {
            const float boxSize = style::Scaled(kBoxSize);
            const ImVec2 boxMin(bb.Min.x + style::Scaled(kBoxInset), bb.Min.y + (rowHeight - boxSize) * 0.5f);
            const ImVec2 boxMax(boxMin.x + boxSize, boxMin.y + boxSize);

            if (isPressed)
            {
                if (GetIO().MousePos.x <= boxMax.x + style::Scaled(3.f))
                {
                    *isChecked = !*isChecked;
                    MarkItemEdited(id);
                }
                else
                {
                    wasSelected = true;
                }
            }

            drawList->AddRectFilled(boxMin, boxMax, ImColor(style::checkboxBg));

            drawList->AddRectFilled(boxMin, ImVec2(boxMax.x, boxMin.y + 1.f), ImColor(style::bevelDark));
            drawList->AddRectFilled(boxMin, ImVec2(boxMin.x + 1.f, boxMax.y), ImColor(style::bevelDark));
            drawList->AddRectFilled(ImVec2(boxMin.x, boxMax.y - 1.f), boxMax, ImColor(style::bevelLight));
            drawList->AddRectFilled(ImVec2(boxMax.x - 1.f, boxMin.y), boxMax, ImColor(style::bevelLight));

            if (*isChecked)
            {
                drawList->PathLineTo(ImVec2(boxMin.x + boxSize * 0.22f, boxMin.y + boxSize * 0.50f));
                drawList->PathLineTo(ImVec2(boxMin.x + boxSize * 0.44f, boxMin.y + boxSize * 0.72f));
                drawList->PathLineTo(ImVec2(boxMin.x + boxSize * 0.80f, boxMin.y + boxSize * 0.24f));
                drawList->PathStroke(ImColor(style::accent), 0, style::Scaled(2.f));
            }

            labelX = boxMax.x + style::Scaled(kLabelGap);
        }
        else if (isPressed)
        {
            wasSelected = true;
        }

        ImVec4 labelColor = style::text;
        if (isSelected)
        {
            labelColor = style::accent;
        }

        const char* labelEnd = FindRenderedTextEnd(label);
        const ImVec2 labelSize = CalcTextSize(label, labelEnd);
        drawList->AddText(ImVec2(labelX, bb.Min.y + (rowHeight - labelSize.y) * 0.5f), ImColor(labelColor), label, labelEnd);

        return wasSelected;
    }

    bool Selection::Is(const char* label) const
    {
        return strcmp(current, label) == 0;
    }

    void Selection::Set(const char* label)
    {
        strncpy_s(current, label, _TRUNCATE);
    }

    bool ListRow(const char* label, bool* isChecked, bool isSelected)
    {
        return ListRowBase(label, isChecked, isSelected);
    }

    bool ListRow(const char* label, bool* isChecked, Selection& selection)
    {
        if (ListRowBase(label, isChecked, selection.Is(label)))
        {
            selection.Set(label);
            return true;
        }
        return false;
    }

    bool ListRowText(const char* label, bool isSelected)
    {
        return ListRowBase(label, nullptr, isSelected);
    }

    bool ListRowText(const char* label, Selection& selection)
    {
        if (ListRowBase(label, nullptr, selection.Is(label)))
        {
            selection.Set(label);
            return true;
        }
        return false;
    }

    void EndList()
    {
        int rowIndex = listRowStack.back();
        listRowStack.pop_back();

        ImGuiWindow* window = GetCurrentWindow();
        if (!window->SkipItems)
        {
            const float rowHeight = style::Scaled(kRowHeight);
            const float left = window->WorkRect.Min.x;
            const float right = window->WorkRect.Max.x;
            const float bottom = window->InnerRect.Max.y;

            float rowTop = window->DC.CursorPos.y;
            while (rowTop < bottom)
            {
                window->DrawList->AddRectFilled(
                    ImVec2(left, rowTop),
                    ImVec2(right, ImMin(rowTop + rowHeight, bottom)),
                    ImColor(RowFill(rowIndex)));
                rowTop += rowHeight;
                rowIndex++;
            }
        }

        EndChild();
        PopStyleVar(2);

        GetWindowDrawList()->AddRect(GetItemRectMin(), GetItemRectMax(), ImColor(style::stroke));
    }
}
