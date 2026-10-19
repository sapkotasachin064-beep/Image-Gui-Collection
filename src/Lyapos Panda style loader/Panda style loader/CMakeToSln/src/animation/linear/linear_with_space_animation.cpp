//
// Created by n08i40k on 09.04.2024.
//

#include "linear_with_space_animation.h"

linear_with_space_animation::linear_with_space_animation(
	const ::animation_id anim_id,
	const float speed,
	const vector2f& limits,
	const vector2f& space) : imgui_animation_base(anim_id)
	                       , limits_(limits)
	                       , space_(space)
	                       , speed_(speed)
	                       , current_(limits_.y) {}

linear_with_space_animation::linear_with_space_animation(
	const float speed,
	const vector2f& limits,
	const vector2f& space) : linear_with_space_animation(
		                       animation_id,
		                       speed,
		                       limits,
		                       space) {}

void
linear_with_space_animation::update(const float delta) { current_ = std::min(current_ + delta * speed_, limits_.y); }

float
linear_with_space_animation::get_current() const {
	if (current_ > space_.x && current_ < space_.y)
		return space_.x;

	return current_;
}

void
linear_with_space_animation::reset_current() { current_ = limits_.x; }
