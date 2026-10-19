//
// Created by n08i40k on 11.04.2024.
//

#ifndef SMOOTHCOMBO_H
#define SMOOTHCOMBO_H
#include <imgui.h>

namespace ImGui {

bool BeginSmoothCombo(const char* label, const char* preview_value, ImGuiComboFlags flags);
bool SmoothSelectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = ImGuiSelectableFlags_None, const ImVec2& size_arg = {0, 0});
bool SmoothSelectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
}



#endif //SMOOTHCOMBO_H
