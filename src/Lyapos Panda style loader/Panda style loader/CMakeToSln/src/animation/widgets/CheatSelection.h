//
// Created by Lyapos on 30.04.2024.
//
#include <imgui.h>

#ifndef IMGUI_TEMPLATE_SRC_ANIMATION_WIDGETS_CHEATSELECTION_H
#define IMGUI_TEMPLATE_SRC_ANIMATION_WIDGETS_CHEATSELECTION_H


namespace ImGui {
bool CheatSelection(const char *label, const char *subscription, const char *update, ImTextureID logotype, bool active);
int SpoofButton(const char *label, bool succes);
}


#endif //IMGUI_TEMPLATE_SRC_ANIMATION_WIDGETS_CHEATSELECTION_H
