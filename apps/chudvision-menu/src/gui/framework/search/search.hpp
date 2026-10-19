#pragma once

#include "imgui.h"

namespace ksd
{
    inline bool searchEnabled = false;

    void SearchInput(float width);
    void ClearSearch();

    bool MatchesSearch(const char* label);
    bool IsRowFiltered();
}
