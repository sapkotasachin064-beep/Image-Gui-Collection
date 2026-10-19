//
// Created by n08i40k on 12.04.2024.
//

#include "linear_with_target_animation.h"

linear_with_target_animation::linear_with_target_animation(
	const ::animation_id anim_id,
	const float speed,
	const vector2<float>& limits,
	const float target,
	const bool is_source_target) : imgui_animation_base(anim_id)
	                             , speed_(speed) {
	set_limits(limits);
	set_target(target, is_source_target);
	reset_current();
}

linear_with_target_animation::linear_with_target_animation(
	const float speed,
	const vector2<float>& limits,
	const float target,
	const bool is_source_target) : linear_with_target_animation(
		                             animation_id,
		                             speed,
		                             limits,
		                             target,
		                             is_source_target) {}

void
linear_with_target_animation::update(const float delta) {
	if (current_ == target_) {
		finished = true;
		return;
	}

	const float increment = delta * speed_;

	current_ = current_ < target_
		           ? std::min(current_ + increment, target_)
		           : std::max(target_, current_ - increment);

	finished = current_ == target_;
}

void
linear_with_target_animation::set_speed(const float speed) { speed_ = speed; }

void
linear_with_target_animation::set_target(const float target,
                                         const bool is_source_target) {
	const float norm = is_source_target
		                   ? target
		                   : (target - min) / max;
	if (current_ == norm)
		return;

	target_ = norm;
	finished = current_ == target_;
}

void
linear_with_target_animation::set_limits(vector2f limits,
                                         const bool recalculate_target) {
	if (inverted = limits.x > limits.y;
		inverted)
		limits.swap();

	if (recalculate_target) {
		const float old_target = get_target();

		min = limits.x;
		max = limits.y - limits.x;

		set_target(old_target);
	}
	else {
		min = limits.x;
		max = limits.y - limits.x;
	}
}

void
linear_with_target_animation::reset_current() {
	current_ = target_;
	finished = true;
}

float
linear_with_target_animation::get_current(const bool source) const {
	if (source)
		return current_;
	return min + current_ * max;
}

float
linear_with_target_animation::get_target(const bool source) const {
	if (source)
		return target_;
	return min + target_ * max;
}

vector2f
linear_with_target_animation::get_limits() const { return vector2f{min, max}; }
