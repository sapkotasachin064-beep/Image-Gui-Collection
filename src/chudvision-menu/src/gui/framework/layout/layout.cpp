#include "gui/framework/layout/layout.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const float kSlotGap = 3.f;

    static float packedRowY = -1.f;
    static float packedLeftX = 0.f;
    static int packedFrame = -1;
    static ImGuiID packedWindowId = 0;

    void PlaceRight(const ImVec2& size)
    {
        SameLine();

        ImGuiWindow* window = GetCurrentWindow();
        const float rowTop = window->DC.CursorPos.y;
        const float rowHeight = window->DC.CurrLineSize.y;

        const bool isSameRow = packedFrame == GetFrameCount()
            && packedWindowId == window->ID
            && packedRowY >= 0.f
            && ImFabs(rowTop - packedRowY) < 1.f;
        const float rightEdge = isSameRow ? packedLeftX - style::Scaled(kSlotGap) : window->WorkRect.Max.x;

        const ImVec2 pos(rightEdge - size.x, rowTop + (rowHeight - size.y) * 0.5f);
        SetCursorScreenPos(pos);

        packedRowY = rowTop;
        packedLeftX = pos.x;
        packedFrame = GetFrameCount();
        packedWindowId = window->ID;
    }

    static const float kIndentStep = 8.f;

    static ImVec2 pageAvail;
    static float currentColumnWidth = 0.f;
    static int pageFrame = -1;
    static ImGuiID pageWindowId = 0;

    void BeginColumn(float widthRatio)
    {
        ImGuiWindow* window = GetCurrentWindow();
        const bool isFirstColumn = pageFrame != GetFrameCount() || pageWindowId != window->ID;

        if (isFirstColumn)
        {
            pageFrame = GetFrameCount();
            pageWindowId = window->ID;
            pageAvail = GetContentRegionAvail();
        }
        else
        {
            SameLine();
        }

        if (widthRatio > 0.f)
        {
            currentColumnWidth = (pageAvail.x - GetStyle().ItemSpacing.x) * widthRatio;
        }
        else
        {
            currentColumnWidth = window->WorkRect.Max.x - window->DC.CursorPos.x;
        }

        BeginGroup();
    }

    void EndColumn()
    {
        EndGroup();
    }

    float ColumnWidth()
    {
        return currentColumnWidth;
    }

    float ColumnBoxHeight(float heightRatio)
    {
        if (heightRatio <= 0.f)
        {
            return GetContentRegionAvail().y;
        }
        return (pageAvail.y - GetStyle().ItemSpacing.y) * heightRatio;
    }

    void Indent()
    {
        ImGui::Indent(style::Scaled(kIndentStep));
    }

    void Unindent()
    {
        ImGui::Unindent(style::Scaled(kIndentStep));
    }
}
