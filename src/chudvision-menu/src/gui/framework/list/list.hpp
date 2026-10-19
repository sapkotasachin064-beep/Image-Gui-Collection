#pragma once

#include "imgui.h"

namespace ksd
{
    struct Selection
    {
        char current[64] = "";

        bool Is(const char* label) const;
        void Set(const char* label);
    };

    bool BeginList(const char* id, const ImVec2& size = ImVec2(0.f, 0.f));

    bool ListRow(const char* label, bool* isChecked, bool isSelected);
    bool ListRow(const char* label, bool* isChecked, Selection& selection);

    bool ListRowText(const char* label, bool isSelected);
    bool ListRowText(const char* label, Selection& selection);

    void EndList();
}
