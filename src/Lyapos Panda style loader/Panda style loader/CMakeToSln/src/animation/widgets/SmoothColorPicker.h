//
// Created by n08i40k on 12.04.2024.
//

#ifndef SMOOTHCOLORPICKER_H
#define SMOOTHCOLORPICKER_H
#include <imgui.h>

namespace ImGui {
bool SmoothColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);

bool SmoothColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = nullptr);
}


#endif //SMOOTHCOLORPICKER_H
