//
// Created by n08i40k on 11.04.2024.
//

#include "RenderPaskhalkoRotated.h"

#include <imgui_internal.h>

#include "math/vector2.h"

void
ImGui::RenderHalfPaskhalkoRotated(ImDrawList* draw_list,
                                  ImVec2 pos,
                                  const ImU32 col,
                                  float sz,
                                  const float degrees) {
	float thickness = ImMax(sz / 5.0f, 1.0f) * 0.75F;
	sz -= thickness * 0.5f;
	pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

	const vector2f center_point(sz / 2, sz / 2);
	const vector2f pos_(pos);

	const float half_thickness = thickness / 2;

	const auto right_top_point = vector2f(sz, half_thickness ) + pos_;
	const auto top_point = vector2f(center_point.x, half_thickness) + pos_;
	const auto bottom_point = vector2f(center_point.x, sz - half_thickness) + pos_;
	const auto left_bottom_point = vector2f(0.F, sz - half_thickness) + pos_;

	const auto center_abs = pos_ + center_point;
	draw_list->PathLineTo(right_top_point.rotate(center_abs, degrees).convert<ImVec2>());
	draw_list->PathLineTo(top_point.rotate(center_abs, degrees).convert<ImVec2>());
	draw_list->PathLineTo(bottom_point.rotate(center_abs, degrees).convert<ImVec2>());
	draw_list->PathLineTo(left_bottom_point.rotate(center_abs, degrees).convert<ImVec2>());
	draw_list->PathStroke(col, 0, thickness);
}

void
ImGui::RenderPaskhalkoRotated(ImDrawList* draw_list,
                              const ImVec2 pos,
                              const ImU32 col,
                              const float sz,
                              const float degrees) {
	RenderHalfPaskhalkoRotated(draw_list, pos, col, sz, degrees);
	RenderHalfPaskhalkoRotated(draw_list, pos, col, sz, degrees + 90.F);
}
