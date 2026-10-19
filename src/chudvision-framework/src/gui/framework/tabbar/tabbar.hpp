#pragma once

#include "imgui.h"

namespace ksd
{
    bool BeginTabBar(const char* id);
    void EndTabBar();

    bool BeginTabItem(const char* label);
    void EndTabItem();
}
