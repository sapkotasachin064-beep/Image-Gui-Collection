#pragma once
#include "imgui.h"

namespace ksd
{
	bool ColorEdit4(const char* label, float color[4], ImGuiColorEditFlags flags = 0, bool* rainbow = nullptr, float* speed = nullptr);
	bool ColorEdit4x2(const char* label, float color1[4], float color2[4], ImGuiColorEditFlags flags = 0);
	bool ColorEdit4x3(const char* label, float color1[4], float color2[4], float color3[4], ImGuiColorEditFlags flags = 0);

	void UpdateRainbows();
}
