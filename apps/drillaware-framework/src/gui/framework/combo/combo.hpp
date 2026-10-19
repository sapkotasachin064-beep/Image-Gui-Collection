#pragma once
#include "imgui.h"

namespace ksd
{
	bool combo(const char* label, int* currentItem, const char* const items[], int itemCount, int heightInItems);
	bool Combo(const char* label, int* currentItem, bool (*itemsGetter)(void*, int, const char**), void* itemsSource, int itemCount, int popupMaxHeightInItems);
	bool MultiCombo(const char* label, bool selection[], const char* const items[], int itemCount);
	bool BeginCombo(const char* label, const char* previewValue);
	void EndCombo();
}
