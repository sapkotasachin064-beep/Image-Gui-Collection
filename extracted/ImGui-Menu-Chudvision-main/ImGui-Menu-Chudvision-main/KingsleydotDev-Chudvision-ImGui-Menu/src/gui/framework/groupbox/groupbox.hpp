#pragma once

#include "imgui.h"

namespace ksd
{
    bool BeginGroupBox(const char* label, const ImVec2& size = ImVec2(0.f, 0.f), bool isCaptionCentered = false);
    void EndGroupBox();

    bool BeginColumnBox(const char* label, float heightRatio = 0.f);
}
