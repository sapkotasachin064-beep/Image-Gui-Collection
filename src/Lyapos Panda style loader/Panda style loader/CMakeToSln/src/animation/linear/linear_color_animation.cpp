//
// Created by n08i40k on 09.04.2024.
//

#include "linear_color_animation.h"

#include <imgui_internal.h>

linear_color_animation::linear_color_animation(
	const ::animation_id anim_id,
	const float speed,
	const ImVec4& source_color,
	const ImVec4& target_color) : linear_animation(anim_id,
	                                               speed,
	                                               {0.F, 1.F},
	                                               false)
	                            , source_color_(source_color)
	                            , target_color_(target_color) {}

linear_color_animation::linear_color_animation(
	const float speed,
	const ImVec4& source_color,
	const ImVec4& target_color) : linear_color_animation(
		                            animation_id,
		                            speed,
		                            source_color,
		                            target_color) {}

ImVec4
linear_color_animation::get_current_color() const { return ImLerp(source_color_, target_color_, get_current()); }

ImU32
linear_color_animation::get_current_u32_color() const { return ImGui::ColorConvertFloat4ToU32(get_current_color()); }

void
linear_color_animation::reset_current_color() { reset_current(); }

void
linear_color_animation::set_target_color(const ImVec4& target_color) {
	if (memcmp(&target_color, &target_color_, sizeof(ImVec4)) == 0)
		return;

	source_color_ = get_current_color();
	target_color_ = target_color;

	reset_current();
}

const ImVec4&
linear_color_animation::get_source_color() const { return source_color_; }

const ImVec4&
linear_color_animation::get_target_color() const { return target_color_; }
