#pragma once

#include "imgui.h"
#include "gui/pages/pages.hpp"

namespace gui
{
    enum Tab
    {
        Tab_Aimbot,
        Tab_Esp,
        Tab_Players,
        Tab_Visuals,
        Tab_Misc,
        Tab_Config,
        Tab_Count
    };

    inline bool open = true;
    inline int currentTab = Tab_Aimbot;

    inline const ImVec2 menuSize(458.f, 373.f);
    inline const ImVec2 menuMinSize(400.f, 320.f);

    void Render() noexcept;
}
