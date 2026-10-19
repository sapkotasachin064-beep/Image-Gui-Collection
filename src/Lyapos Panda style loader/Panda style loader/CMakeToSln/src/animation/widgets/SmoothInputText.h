//
// Created by Lyapos on 22.04.2024.
//

#ifndef IMGUI_TEMPLATE_SRC_ANIMATION_WIDGETS_SMOOTHINPUTTEXT_H
#define IMGUI_TEMPLATE_SRC_ANIMATION_WIDGETS_SMOOTHINPUTTEXT_H


#include <imgui.h>
namespace ImGui {
bool SmoothInputText(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);


}

#endif //IMGUI_TEMPLATE_SRC_ANIMATION_WIDGETS_SMOOTHINPUTTEXT_H
