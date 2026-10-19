//
// Created by n08i40k on 12.04.2024.
//

#ifndef LINEAR_WITH_TARGET_ANIMATION_H
#define LINEAR_WITH_TARGET_ANIMATION_H

#include "linear_animation.h"

class linear_with_target_animation : public imgui_animation_base {
public:
	constexpr static animation_id animation_id = "linear_with_target_animation"_sh;

private:
	float min{};
	float max{};

	float target_{};
	float current_{};
	float speed_{};
	bool inverted{};

public:
	linear_with_target_animation(::animation_id anim_id,
	                             float speed,
	                             const vector2<float>& limits,
	                             float target,
	                             bool is_source_target);

	explicit
	linear_with_target_animation(float speed = 3.F,
	                             const vector2<float>& limits = {0.F, 1.F},
	                             float target = 0.F,
	                             bool is_source_target = false);

	void
	update(float delta) override;

	void
	set_speed(float speed);

	void
	set_target(float target,
	           bool is_source_target = false);

	void
	set_limits(vector2f limits,
	           bool recalculate_target = false);

	void
	reset_current();

	[[nodiscard]] float
	get_current(bool source = false) const;

	[[nodiscard]] float
	get_target(bool source = false) const;

	[[nodiscard]] vector2f
	get_limits() const;
};

#endif //LINEAR_WITH_TARGET_ANIMATION_H
