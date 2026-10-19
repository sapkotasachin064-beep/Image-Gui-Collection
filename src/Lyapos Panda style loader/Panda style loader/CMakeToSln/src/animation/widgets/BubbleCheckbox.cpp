//
// Created by n08i40k on 10.04.2024.
//

#include "BubbleCheckbox.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "animation/animation_manager.h"
#include "animation/linear/linear_button_color_animation.h"
#include "animation/linear/linear_color_animation.h"
#include "render/RenderCheckMarkRotated.h"
#include "widgets/TextWithDescription.h"

float
bubbleFunction(const float x) {
	if (x >= 0.F && x <= 0.5F)
		return 2.F * x; // от 0 до 1
	if (x > 0.5F && x < 0.7F)
		return 1.F - 5.F * (x - 0.5F) / 2.F; // от 1 до 0.8
	if (x >= 0.7F && x < 0.9F)
		return 0.8F + 5.F * (x - 0.7F) / 2.F; // от 0.8 до 1

	return 1.F - 5.F * (x - 0.9F) / 2.F; // от 1 до 0.8
}

bool
ImGui::BubbleCheckbox(const char* label,
                      bool* v) {
	auto* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	auto& g = *GImGui;
	const auto& style = g.Style;
	const auto id = window->GetID(label);
	const auto label_size = CalcTextSize(label, nullptr, true);

	const float square_sz = GetFrameHeight();
	const auto pos = window->DC.CursorPos;
	const ImRect total_bb(pos,
	                      pos + ImVec2(square_sz + (label_size.x > 0.0F
		                                                ? style.ItemInnerSpacing.x + label_size.x
		                                                : 0.0F),
	                                   label_size.y + style.FramePadding.y * 2.0F));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id))
		return false;

	bool hovered;
	bool held;
	bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed) {
		*v = !*v;
		MarkItemEdited(id);
	}

	auto& linear_anim = ngui::animation_manager::get<linear_animation>(
		window,
		id,
		"checkbox_linear"_sh,
		3.F,
		vector2f{0.F, 1.F});
	auto& color_anim = ngui::animation_manager::get<linear_button_color_animation>(
		window,
		id,
		"checkbox_color"_sh,
		6.F,
		12.F,
		GetStyleColorVec4(ImGuiCol_FrameBg),
		GetStyleColorVec4(ImGuiCol_FrameBgHovered),
		GetStyleColorVec4(ImGuiCol_FrameBgActive));

	color_anim.button_update(*v, hovered);

	const float linear_current = linear_anim.get_current();
	if (*v)
		linear_anim.set_limits(vector2f{0.F, 1.F});
	else if (hovered) {
		if (linear_current > 0.5F)
			linear_anim.set_limits(vector2f{1.F, 0.5F});
		else
			linear_anim.set_limits(vector2f{0.F, 0.5F});
	}
	else
		linear_anim.set_limits(vector2f{1.F, 0.F});

	if (linear_anim.first_frame)
		linear_anim.reset_current(true);

	const ImRect check_bb(pos + ImVec2{0.F, label_size.y / 4}, pos + ImVec2(square_sz, square_sz) + ImVec2{0.F, label_size.y / 4});

	RenderNavHighlight(total_bb, id);
	RenderFrame(check_bb.Min,
	            check_bb.Max,
	            color_anim.get_current_u32_color(),
	            true,
	            style.FrameRounding);

	if (const float mark_size_mult = linear_current * 0.8F;
		mark_size_mult > 0) {
		const float new_sq_size = mark_size_mult * square_sz;
		const float pad = (square_sz - new_sq_size) / 2;
		const float degree = linear_current * (360.F * 2);

		RenderCheckMarkRotated(window->DrawList,
		                       check_bb.Min + ImVec2(pad, pad),
		                       GetColorU32(ImGuiCol_CheckMark),
		                       new_sq_size,
		                       degree);
	}

	auto label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, total_bb.Min.y + style.FramePadding.y);
	if (label_size.x > 0.0F) {
		const auto cursor_pos = GetCursorScreenPos();
		SetCursorScreenPos(label_pos);
		TextWithDescription(label);
		SetCursorScreenPos(cursor_pos);
	}

	return pressed;
}
