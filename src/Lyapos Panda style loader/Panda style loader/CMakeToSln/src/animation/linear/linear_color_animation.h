//
// Created by n08i40k on 09.04.2024.
//

#ifndef LINEAR_COLOR_ANIMATION_H
#define LINEAR_COLOR_ANIMATION_H
#include "linear_animation.h"
#include "dynamic_size/DynImVec2.h"

class linear_color_animation : public linear_animation {
public:
	constexpr static ::animation_id current_animation_id = "linear_color_animation"_sh;

private:
	ImVec4 source_color_;
	ImVec4 target_color_;

public:
	linear_color_animation(::animation_id anim_id,
	                       float speed,
	                       const ImVec4& source_color,
	                       const ImVec4& target_color);

	explicit
	linear_color_animation(float speed = 3.F,
	                       const ImVec4& source_color = ImVec4(0.F, 0.F, 0.F, 1.F),
	                       const ImVec4& target_color = ImVec4(1.F, 1.F, 1.F, 1.F));

	[[nodiscard]] ImVec4
	get_current_color() const;

	[[nodiscard]] ImU32
	get_current_u32_color() const;

	void
	reset_current_color();
	void
	set_target_color(
		const ImVec4& target_color);

	[[nodiscard]] const ImVec4&
	get_source_color() const;
	[[nodiscard]] const ImVec4&
	get_target_color() const;
};

#endif //LINEAR_COLOR_ANIMATION_H
