//
// Created by n08i40k on 19.04.2024.
//

#ifndef SMOOTHCOLOREDBUTTON_H
#define SMOOTHCOLOREDBUTTON_H
#include "imgui.h"

namespace ImGui {
bool
SmoothColoredButton(const char* label,
                    const ImVec4& active_color = GetStyleColorVec4(ImGuiCol_ButtonActive),
                    const ImVec4& default_color = GetStyleColorVec4(ImGuiCol_Button),
                    const ImVec4& hovered_color = GetStyleColorVec4(ImGuiCol_ButtonHovered),
                    const ImVec2& size = ImVec2(0, 0));
}

#endif //SMOOTHCOLOREDBUTTON_H
