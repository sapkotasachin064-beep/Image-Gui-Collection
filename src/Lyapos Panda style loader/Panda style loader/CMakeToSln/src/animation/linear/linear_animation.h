//
// Created by n08i40k on 09.04.2024.
//

#ifndef LINEAR_ANIMATION_H
#define LINEAR_ANIMATION_H
#include "animation/imgui_animation_base.h"
#include "math/vector2.h"

class linear_animation : public imgui_animation_base {
public:
	constexpr static animation_id animation_id = "linear_animation"_sh;

protected:
	vector2<float> limits_;
	float current_;
	float speed_;

	bool inverted_;

public:
	linear_animation(::animation_id anim_id,
	                 float speed,
	                 const vector2<float>& limits,
	                 bool inverted);

	explicit
	linear_animation(float speed = 3.F,
	                 const vector2<float>& limits = {0.F, 1.F},
	                 bool inverted = false);

	void
	update(float delta) override;

	[[nodiscard]] float
	get_current() const;

	void
	reset_current(bool inverted = false);

	void
	set_speed(float speed);

	const vector2f&
	get_limits() { return limits_; }

	void
	set_limits(const vector2f& limits);

	[[nodiscard]] bool
	is_inverted() const;

	void
	set_inverted(bool inverted);
};

#endif //LINEAR_ANIMATION_H
