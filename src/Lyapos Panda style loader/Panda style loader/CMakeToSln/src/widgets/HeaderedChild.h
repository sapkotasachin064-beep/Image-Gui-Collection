//
// Created by n08i40k on 07.05.2024.
//

#ifndef HEADEREDCHILD_H
#define HEADEREDCHILD_H
#include "imgui.h"

enum ImGuiHeaderedGroupState : int {
	ImGuiHeaderedGroupState_None,
	ImGuiHeaderedGroupState_Header,
	ImGuiHeaderedGroupState_Data
};

namespace ImGui {
bool
BeginHeaderedChild(const char* label,
                   const ImVec2& size = {0.F, 0.F},
                   ImGuiChildFlags child_flags = 0,
                   ImGuiWindowFlags window_flags = 0);

void
BeginHeaderedChildData();

void
EndHeaderedChild();
}

#endif //HEADEREDCHILD_H
