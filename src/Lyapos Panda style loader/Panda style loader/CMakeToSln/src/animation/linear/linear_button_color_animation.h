//
// Created by n08i40k on 11.04.2024.
//

#ifndef LINEAR_BUTTON_COLOR_ANIMATION_H
#define LINEAR_BUTTON_COLOR_ANIMATION_H
#include "linear_color_animation.h"

class linear_button_color_animation final : public linear_color_animation {
	const ImVec4 default_color_;
	const ImVec4 hovered_color_;
	const ImVec4 pressed_color_;

	const float default_speed_;
	const float pressed_speed_;

	bool pressed_{};

public:
	linear_button_color_animation(::animation_id anim_id,
	                              float default_speed,
	                              float pressed_speed,
	                              const ImVec4& default_color,
	                              const ImVec4& hovered_color,
	                              const ImVec4& pressed_color);

	explicit
	linear_button_color_animation(float default_speed = 3.F,
	                              float pressed_speed = 6.F,
	                              const ImVec4& default_color = ImGui::GetStyleColorVec4(ImGuiCol_Button),
	                              const ImVec4& hovered_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered),
	                              const ImVec4& pressed_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	void
	button_update(bool pressed,
	              bool hovered);

	[[nodiscard]] bool
	get_pressed() const { return pressed_; }
};

#endif //LINEAR_BUTTON_COLOR_ANIMATION_H
