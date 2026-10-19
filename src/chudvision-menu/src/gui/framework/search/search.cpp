#include "gui/framework/search/search.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static char searchQuery[64];
    static bool isLastRowVisible = true;

    void SearchInput(float width)
    {
        PushStyleColor(ImGuiCol_FrameBg, style::checkboxBg);
        PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style::Scaled(4.f), style::Scaled(2.f)));
        SetNextItemWidth(width);
        InputTextWithHint("##menuSearch", "search", searchQuery, sizeof(searchQuery));
        PopStyleVar();
        PopStyleColor();

        const ImVec2 fieldMin = GetItemRectMin();
        const ImVec2 fieldMax = GetItemRectMax();
        ImDrawList* drawList = GetWindowDrawList();
        drawList->AddRectFilled(fieldMin, ImVec2(fieldMax.x, fieldMin.y + 1.f), ImColor(style::bevelDark));
        drawList->AddRectFilled(fieldMin, ImVec2(fieldMin.x + 1.f, fieldMax.y), ImColor(style::bevelDark));
        drawList->AddRectFilled(ImVec2(fieldMin.x, fieldMax.y - 1.f), fieldMax, ImColor(style::bevelLight));
        drawList->AddRectFilled(ImVec2(fieldMax.x - 1.f, fieldMin.y), fieldMax, ImColor(style::bevelLight));
    }

    void ClearSearch()
    {
        searchQuery[0] = '\0';
        isLastRowVisible = true;
    }

    bool MatchesSearch(const char* label)
    {
        if (!searchEnabled || searchQuery[0] == '\0')
        {
            isLastRowVisible = true;
            return true;
        }

        const char* labelEnd = FindRenderedTextEnd(label);
        isLastRowVisible = ImStristr(label, labelEnd, searchQuery, nullptr) != nullptr;
        return isLastRowVisible;
    }

    bool IsRowFiltered()
    {
        return !isLastRowVisible;
    }
}
