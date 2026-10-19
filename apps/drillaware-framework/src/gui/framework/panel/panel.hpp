#pragma once
#include "imgui.h"

namespace ksd
{
	inline ImVec2 panelOffset = ImVec2(0.f, 0.f);

	bool BeginChild(const char* title, const ImVec2& size);
	void EndChild();
}
