//
// Created by n08i40k on 11.04.2024.
//

#include "RenderCheckMarkRotated.h"

#include <imgui_internal.h>

#include "math/vector2.h"

void
ImGui::RenderCheckMarkRotated(ImDrawList* draw_list,
                              ImVec2 pos,
                              const ImU32 col,
                              float sz,
                              const float degrees) {
	float thickness = ImMax(sz / 5.0f, 1.0f);
	sz -= thickness * 0.5f;
	pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

	float third = sz / 3.0f;
	float bx = pos.x + third;
	float by = pos.y + sz - third * 0.5f;

	const vector2f left_point(bx - third, by - third);
	const vector2f botton_point(bx, by);
	const vector2f right_point(bx + third * 2.0f, by - third * 2.0f);

	const auto center = vector2f(pos) + vector2f(sz / 2, sz / 2);
	draw_list->PathLineTo(left_point.rotate(center, degrees).convert<ImVec2>());
	draw_list->PathLineTo(botton_point.rotate(center, degrees).convert<ImVec2>());
	draw_list->PathLineTo(right_point.rotate(center, degrees).convert<ImVec2>());
	draw_list->PathStroke(col, 0, thickness);
}
