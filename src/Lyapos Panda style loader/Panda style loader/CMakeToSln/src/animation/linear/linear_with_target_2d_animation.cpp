//
// Created by n08i40k on 13.04.2024.
//

#include "linear_with_target_2d_animation.h"

linear_with_target_2d_animation::linear_with_target_2d_animation(
	const ::animation_id anim_id,
	const float speed,
	const vector2f& min,
	const vector2f& max,
	const vector2f& target,
	const bool is_source_target) : imgui_animation_base(anim_id)
	                             , speed_(speed) {
	set_limits(min, max);
	set_target(target, is_source_target);
	reset_current();
}

linear_with_target_2d_animation::linear_with_target_2d_animation(
	const float speed,
	const vector2f& min,
	const vector2f& max,
	const vector2f& target,
	const bool is_source_target) : linear_with_target_2d_animation(
		                             animation_id,
		                             speed,
		                             min,
		                             max,
		                             target,
		                             is_source_target) {}

void
linear_with_target_2d_animation::set_target(const vector2f& target,
                                            const bool is_source_target) {
	const auto norm = is_source_target
		                  ? target
		                  : (target - min_) / max_;
	if (current_ == norm)
		return;

	target_ = norm;
	finished = false;
}

void
linear_with_target_2d_animation::set_limits(vector2f min,
                                            vector2f max,
                                            const bool recalculate_target) {
	if (inverted = min.x > max.x;
		inverted) {
		const auto saved_min = min;
		min = max;
		max = saved_min;
	}

	if (recalculate_target) {
		const auto old_target = get_target();

		min_ = min;
		max_ = max - min;

		set_target(old_target);
	}
	else {
		min_ = min;
		max_ = max - min;
	}
}

void
linear_with_target_2d_animation::update(const float delta) {
	// ReSharper disable once CppIdenticalOperandsInBinaryExpression
	if (current_.x != current_.x || current_.y != current_.y) {
		current_ = target_;
		finished = true;
		return;
	}

	if (current_ == target_) {
		finished = true;
		return;
	}

	const float current_distance = current_.distance(target_);
	const float increment = delta * speed_;

	const float new_distance = current_distance - increment;

	if (new_distance <= 0.F) {
		current_ = target_;
		finished = true;

		return;
	}

	const float dx = target_.x - current_.x;
	const float dy = target_.y - current_.y;

	const float scale = (current_distance - new_distance) / current_distance;

	current_.x = current_.x + dx * scale;
	current_.y = current_.y + dy * scale;

	finished = current_ == target_;
}

void
linear_with_target_2d_animation::set_speed(const float speed) { speed_ = speed; }

void
linear_with_target_2d_animation::reset_current() {
	current_ = target_;
	finished = true;
}

vector2f
linear_with_target_2d_animation::get_current() const {
	if (current_.x != current_.x || current_.y != current_.y)
		return min_ + max_ * target_;
	return min_ + max_ * current_;
}

vector2f
linear_with_target_2d_animation::get_target() const { return min_ + max_ * target_; }
