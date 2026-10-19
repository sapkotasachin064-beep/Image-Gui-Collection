//
// Created by n08i40k on 09.04.2024.
//

#include "linear_animation.h"

#include <algorithm>

void
linear_animation::update(const float delta) {
	const auto min = inverted_
		                 ? limits_.y
		                 : limits_.x;
	const auto max = inverted_
		                 ? limits_.x
		                 : limits_.y;

	if (min == max) {
		current_ = min;
		finished = true;
		return;
	}

	const float increment = delta * speed_;

	if (min > max)
		current_ = std::max(current_ - increment, max);
	else
		current_ = std::min(current_ + increment, max);

	finished = current_ == max;
}

linear_animation::linear_animation(
	const ::animation_id anim_id,
	const float speed,
	const vector2<float>& limits,
	const bool inverted) : imgui_animation_base(anim_id)
	                     , limits_(limits)
	                     , current_(limits_.y > limits_.x
		                                ? limits_.y
		                                : limits_.x)
	                     , speed_(speed)
	                     , inverted_(inverted) {}

linear_animation::linear_animation(
	const float speed,
	const vector2<float>& limits,
	const bool inverted) : linear_animation(animation_id, speed, limits, inverted) {}

float
linear_animation::get_current() const { return current_; }

void
linear_animation::reset_current(const bool inverted) {
	current_ = inverted
		           ? limits_.y
		           : limits_.x;
	finished = current_ == limits_.y;
}

void
linear_animation::set_speed(const float speed) { speed_ = speed; }

void
linear_animation::set_limits(const vector2f& limits) { limits_ = limits; }

bool
linear_animation::is_inverted() const { return inverted_; }

void
linear_animation::set_inverted(const bool inverted) {
	if (inverted_ == inverted)
		return;

	inverted_ = inverted;

	finished = current_ == (inverted
		                        ? limits_.x
		                        : limits_.y);
}
