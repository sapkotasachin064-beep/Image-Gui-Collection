//
// Created by n08i40k on 09.04.2024.
//

#ifndef LINEAR_WITH_SPACE_ANIMATION_H
#define LINEAR_WITH_SPACE_ANIMATION_H
#include "animation/imgui_animation_base.h"
#include "math/vector2.h"

class linear_with_space_animation final : public imgui_animation_base {
public:
	constexpr static animation_id animation_id = "linear_with_space_animation"_sh;

private:
	vector2f limits_;
	vector2f space_;

	float speed_;
	float current_;

public:
	void
	update(float delta) override;

	linear_with_space_animation(::animation_id anim_id,
	                            float speed,
	                            const vector2f& limits,
	                            const vector2f& space);

	explicit
	linear_with_space_animation(float speed = 3.F,
	                            const vector2f& limits = {0.F, 0.F},
	                            const vector2f& space = {0.F, 0.F});

	[[nodiscard]] float
	get_current() const;
	void
	reset_current();
};

#endif //LINEAR_WITH_SPACE_ANIMATION_H
