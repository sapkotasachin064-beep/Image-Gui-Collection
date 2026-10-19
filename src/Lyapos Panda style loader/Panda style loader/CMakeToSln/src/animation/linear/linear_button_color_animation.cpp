//
// Created by n08i40k on 11.04.2024.
//

#include "linear_button_color_animation.h"

linear_button_color_animation::linear_button_color_animation(
	const ::animation_id anim_id,
	const float default_speed,
	const float pressed_speed,
	const ImVec4& default_color,
	const ImVec4& hovered_color,
	const ImVec4& pressed_color) : linear_color_animation(
		                             anim_id,
		                             default_speed,
		                             default_color,
		                             default_color)
	                             , default_color_(default_color)
	                             , hovered_color_(hovered_color)
	                             , pressed_color_(pressed_color)
	                             , default_speed_(default_speed)
	                             , pressed_speed_(pressed_speed) {}

// ебейший конструктор, правда?
linear_button_color_animation::linear_button_color_animation(
	const float default_speed,
	const float pressed_speed,
	const ImVec4& default_color,
	const ImVec4& hovered_color,
	const ImVec4& pressed_color) : linear_button_color_animation(
		                             animation_id,
		                             default_speed,
		                             pressed_speed,
		                             default_color,
		                             hovered_color,
		                             pressed_color) {}

void
linear_button_color_animation::button_update(const bool pressed,
                                             const bool hovered) {
	if (pressed) {
		if (!pressed_) {
			set_target_color(pressed_color_);
			set_speed(pressed_speed_);
		}

		pressed_ = true;
		return;
	}

	if (pressed_ && get_current() != 1.F)
		return;
	pressed_ = false;
	set_speed(default_speed_);

	if (hovered)
		set_target_color(hovered_color_);
	else
		set_target_color(default_color_);
}
