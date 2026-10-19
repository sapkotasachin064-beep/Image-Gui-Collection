#pragma once
#include "imgui.h"

namespace ksd
{
	bool SliderFloat(const char* label, float* value, float min, float max, const char* format = "%.3f");
	bool SliderInt(const char* label, int* value, int min, int max, const char* format = "%d");
	bool SliderFloatRange2(const char* label, float* valueMin, float* valueMax, float min, float max, float minGap = 0.f, const char* format = "%.2f");
	bool SliderIntRange2(const char* label, int* valueMin, int* valueMax, int min, int max, int minGap = 1);
}
