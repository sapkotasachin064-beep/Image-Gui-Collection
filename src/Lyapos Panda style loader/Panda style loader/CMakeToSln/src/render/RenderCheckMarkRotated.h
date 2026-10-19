//
// Created by n08i40k on 11.04.2024.
//

#ifndef RENDERCHECKMARKROTATED_H
#define RENDERCHECKMARKROTATED_H
#include <imgui.h>

namespace ImGui {
void
RenderCheckMarkRotated(ImDrawList* draw_list,
                       ImVec2 pos,
                       ImU32 col,
                       float sz,
                       float degrees);
}

#endif //RENDERCHECKMARKROTATED_H
