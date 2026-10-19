#pragma once

#include "imgui.h"

namespace ksd
{
    bool SliderInt(const char* label, int* value, int minValue, int maxValue, const char* format = "%d");
    bool SliderFloat(const char* label, float* value, float minValue, float maxValue, const char* format = "%.1f");

    bool SliderDistance(const char* label, int* value, int maxValue);
}
