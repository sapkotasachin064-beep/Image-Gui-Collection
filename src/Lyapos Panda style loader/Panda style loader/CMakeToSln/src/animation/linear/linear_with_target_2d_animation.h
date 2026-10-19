//
// Created by n08i40k on 13.04.2024.
//

#ifndef LINEAR_WITH_TARGET_2D_ANIMATION_H
#define LINEAR_WITH_TARGET_2D_ANIMATION_H
#include "animation/imgui_animation_base.h"
#include "math/vector2.h"

class linear_with_target_2d_animation final : public imgui_animation_base {
public:
	constexpr static animation_id animation_id = "linear_with_target_2d_animation"_sh;

private:
	vector2f min_;
	vector2f max_;

	// 0 - 1
	vector2f target_;

	// 0 - 1
	vector2f current_;

	float speed_{};
	bool inverted{};

public:
	linear_with_target_2d_animation(::animation_id anim_id,
	                                float speed,
	                                const vector2f& min,
	                                const vector2f& max,
	                                const vector2f& target,
	                                bool is_source_target);

	explicit
	linear_with_target_2d_animation(float speed = 3.F,
	                                const vector2f& min = {0.F, 0.F},
	                                const vector2f& max = {0.F, 0.F},
	                                const vector2f& target = {0.F, 0.F},
	                                bool is_source_target = false);

	void
	update(float delta) override;

	void
	set_speed(float speed);

	void
	set_target(const vector2f& target,
	           bool is_source_target = false);

	void
	set_limits(vector2f min,
	           vector2f max,
	           bool recalculate_target = false);

	void
	reset_current();

	[[nodiscard]] vector2f
	get_current() const;

	[[nodiscard]] vector2f
	get_target() const;
};

#endif //LINEAR_WITH_TARGET_2D_ANIMATION_H
