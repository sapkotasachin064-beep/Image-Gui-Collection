//
// Created by n08i40k on 11.04.2024.
//

#ifndef RENDERCHECKPASKHALKOROTATED_H
#define RENDERCHECKPASKHALKOROTATED_H
#include <imgui.h>

namespace ImGui {
void RenderHalfPaskhalkoRotated(ImDrawList* draw_list,
					   ImVec2 pos,
					   ImU32 col,
					   float sz,
					   float degrees);
void
RenderPaskhalkoRotated(ImDrawList* draw_list,
                       ImVec2 pos,
                       ImU32 col,
                       float sz,
                       float degrees);
}

#endif //RENDERCHECKPASKHALKOROTATED_H
