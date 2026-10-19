//
// Created by n08i40k on 11.04.2024.
//

#include "SmoothCombo.h"

#include <array>
#include <imgui_internal.h>

#include "animation/animation_manager.h"
#include "animation/linear/linear_button_color_animation.h"

constexpr float popup_close_limit = 0.09F;
constexpr float popup_start_limit = 0.1F;

bool
ImGui::BeginSmoothCombo(const char* label,
                        const char* preview_value,
                        const ImGuiComboFlags flags) {
	auto& g = *GImGui;
	auto* window = GetCurrentWindow();

	ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
	g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
	if (window->SkipItems)
		return false;

	const auto& style = g.Style;
	const auto id = window->GetID(label);
	IM_ASSERT(
		(flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton |
			ImGuiComboFlags_NoPreview)); // Can't use both flags together

	const float arrow_size = flags & ImGuiComboFlags_NoArrowButton
		                         ? 0.0f
		                         : GetFrameHeight();
	const auto label_size = CalcTextSize(label, nullptr, true);
	const float w = flags & ImGuiComboFlags_NoPreview
		                ? arrow_size
		                : CalcItemWidth();
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
	const ImRect total_bb(bb.Min,
	                      bb.Max + ImVec2(label_size.x > 0.0f
		                                      ? style.ItemInnerSpacing.x + label_size.x
		                                      : 0.0f,
	                                      0.0f));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id, &bb))
		return false;

	// Open on click
	bool hovered;
	bool held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);
	const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
	bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
	// bool popup_open = true;
	if (pressed && !popup_open) {
		OpenPopupEx(popup_id, ImGuiPopupFlags_None);
		popup_open = true;
	}

	// Render shape
	auto& frame_anim = ngui::animation_manager::get<linear_button_color_animation>(
		window,
		id,
		"smooth_combo_frame"_sh,
		1.F,
		2.F,
		GetStyleColorVec4(ImGuiCol_FrameBg),
		GetStyleColorVec4(ImGuiCol_FrameBgHovered),
		GetStyleColorVec4(ImGuiCol_FrameBgActive));

	frame_anim.button_update(popup_open, hovered);

	const ImU32 frame_col = frame_anim.get_current_u32_color();
	const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
	RenderNavHighlight(bb, id);
	if (!(flags & ImGuiComboFlags_NoPreview)) {
		window->DrawList->AddRectFilled(bb.Min,
		                                ImVec2(value_x2, bb.Max.y),
		                                frame_col,
		                                style.FrameRounding,
		                                flags & ImGuiComboFlags_NoArrowButton
			                                ? ImDrawFlags_RoundCornersAll
			                                : ImDrawFlags_RoundCornersLeft);
	}
	if (!(flags & ImGuiComboFlags_NoArrowButton)) {
		ImU32 bg_col = GetColorU32(popup_open || hovered
			                           ? ImGuiCol_ButtonHovered
			                           : ImGuiCol_Button);
		ImU32 text_col = GetColorU32(ImGuiCol_Text);
		window->DrawList->AddRectFilled(ImVec2(value_x2, bb.Min.y),
		                                bb.Max,
		                                bg_col,
		                                style.FrameRounding,
		                                w <= arrow_size
			                                ? ImDrawFlags_RoundCornersAll
			                                : ImDrawFlags_RoundCornersRight);
		if (value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x)
			RenderArrow(window->DrawList,
			            ImVec2(value_x2 + style.FramePadding.y, bb.Min.y + style.FramePadding.y),
			            text_col,
			            ImGuiDir_Down,
			            1.0f);
	}
	RenderFrameBorder(bb.Min, bb.Max, style.FrameRounding);

	// Custom preview
	if (flags & ImGuiComboFlags_CustomPreview) {
		g.ComboPreviewData.PreviewRect = ImRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
		IM_ASSERT(preview_value == nullptr || preview_value[0] == 0);
		preview_value = nullptr;
	}

	// Render preview and label
	if (preview_value != nullptr && !(flags & ImGuiComboFlags_NoPreview))
		RenderTextClipped(bb.Min + style.FramePadding, ImVec2(value_x2, bb.Max.y), preview_value, nullptr, nullptr);
	if (label_size.x > 0)
		RenderText(ImVec2(bb.Max.x + style.ItemInnerSpacing.x, bb.Min.y + style.FramePadding.y), label);

	// if (!popup_open)
	// 	return false;

	g.NextWindowData.Flags = backup_next_window_data_flags;

	std::array<char, 16> next_popup_name{};
	ImFormatString(next_popup_name.data(), next_popup_name.size(), "##Combo_%02d", g.BeginPopupStack.Size);
	// Recycle windows based on depth

	auto& popup_anim = ngui::animation_manager::get<linear_animation>(
		window,
		"smooth_combo_popup"_sh,
		"size"_sh,
		3.5F,
		vector2f{popup_start_limit, 1.F});
	if (popup_anim.first_frame) {
		popup_anim.reset_current();
		popup_anim.should_update = false;
	}

	if (pressed) {
		popup_anim.set_limits(vector2f{popup_start_limit, 1.F});
		popup_anim.reset_current();

		popup_anim.should_update = true;
	}

	if (auto* popup_window = FindWindowByName(next_popup_name.data());
		popup_window != nullptr) {
		if (popup_window->WasActive) {
			if (popup_anim.first_frame)
				popup_anim.reset_current();
			ImVec2 size_expected = {
				(bb.Max.x - bb.Min.x),
				CalcWindowNextAutoFitSize(popup_window).y * popup_anim.get_current()
			};
			SetNextWindowSize(size_expected);

			return BeginComboPopup(popup_id, bb, flags);
		}

		if (pressed) {
			popup_anim.set_limits(vector2f{popup_start_limit, 1.F});
			popup_anim.reset_current();
			return BeginComboPopup(popup_id, bb, flags);
		}

		if (popup_anim.update_frames > 5 && IsWindowFocused()) {
			OpenPopupEx(popup_id, ImGuiPopupFlags_None);

			popup_anim.set_limits({1.F, 0.F});
		}
	}

	return BeginComboPopup(popup_id, bb, flags);
}

bool
ImGui::SmoothSelectable(const char* label,
                        bool selected,
                        const ImGuiSelectableFlags flags,
                        const ImVec2& size_arg) {
	auto* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	auto& g = *GImGui;
	const auto& style = g.Style;

	// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
	ImGuiID id = window->GetID(label);
	auto label_size = CalcTextSize(label, nullptr, true);
	ImVec2 size(size_arg.x != 0.0f
		            ? size_arg.x
		            : label_size.x,
	            size_arg.y != 0.0f
		            ? size_arg.y
		            : label_size.y);
	auto pos = window->DC.CursorPos;
	pos.y += window->DC.CurrLineTextBaseOffset;
	ItemSize(size, 0.0f);

	// Fill horizontal space
	// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
	const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
	const float min_x = span_all_columns
		                    ? window->ParentWorkRect.Min.x
		                    : pos.x;
	const float max_x = span_all_columns
		                    ? window->ParentWorkRect.Max.x
		                    : window->WorkRect.Max.x;
	if (size_arg.x == 0.0f || flags & ImGuiSelectableFlags_SpanAvailWidth)
		size.x = ImMax(label_size.x, max_x - min_x);

	// Text stays at the submission position, but bounding box may be extended on both sides
	const auto text_min = pos;
	const ImVec2 text_max(min_x + size.x, pos.y + size.y);

	// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
	ImRect bb(min_x, pos.y, text_max.x, text_max.y);
	if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0) {
		const float spacing_x = span_all_columns
			                        ? 0.0f
			                        : style.ItemSpacing.x;
		const float spacing_y = style.ItemSpacing.y;
		const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
		const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
		bb.Min.x -= spacing_L;
		bb.Min.y -= spacing_U;
		bb.Max.x += spacing_x - spacing_L;
		bb.Max.y += spacing_y - spacing_U;
	}
	//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

	// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
	const float backup_clip_rect_min_x = window->ClipRect.Min.x;
	const float backup_clip_rect_max_x = window->ClipRect.Max.x;
	if (span_all_columns) {
		window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
		window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
	}

	const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
	const bool item_add = ItemAdd(bb,
	                              id,
	                              nullptr,
	                              disabled_item
		                              ? ImGuiItemFlags_Disabled
		                              : ImGuiItemFlags_None);
	if (span_all_columns) {
		window->ClipRect.Min.x = backup_clip_rect_min_x;
		window->ClipRect.Max.x = backup_clip_rect_max_x;
	}

	if (!item_add)
		return false;

	const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
	if (disabled_item && !disabled_global) // Only testing this as an optimization
		BeginDisabled();

	// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
	// which would be advantageous since most selectable are not selected.
	if (span_all_columns && window->DC.CurrentColumns)
		PushColumnsBackground();
	else if (span_all_columns && g.CurrentTable)
		TablePushBackgroundChannel();

	// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
	ImGuiButtonFlags button_flags = 0;
	if (flags & ImGuiSelectableFlags_NoHoldingActiveID)
		button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
	if (flags & ImGuiSelectableFlags_NoSetKeyOwner)
		button_flags |= ImGuiButtonFlags_NoSetKeyOwner;
	if (flags & ImGuiSelectableFlags_SelectOnClick)
		button_flags |= ImGuiButtonFlags_PressedOnClick;
	if (flags & ImGuiSelectableFlags_SelectOnRelease)
		button_flags |= ImGuiButtonFlags_PressedOnRelease;
	if (flags & ImGuiSelectableFlags_AllowDoubleClick)
		button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
	if (flags & ImGuiSelectableFlags_AllowOverlap || g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)
		button_flags |= ImGuiButtonFlags_AllowOverlap;

	const bool was_selected = selected;
	bool hovered;
	bool held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

	// Auto-select when moved into
	// - This will be more fully fleshed in the range-select branch
	// - This is not exposed as it won't nicely work with some user side handling of shift/control
	// - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
	//   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
	//   - (2) usage will fail with clipped items
	//   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
	if (flags & ImGuiSelectableFlags_SelectOnNav && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.
		CurrentFocusScopeId) {
		if (g.NavJustMovedToId == id)
			selected = pressed = true;
	}

	// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
	if (pressed || (hovered && flags & ImGuiSelectableFlags_SetNavIdOnHover)) {
		if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent) {
			SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb));
			// (bb == NavRect)
			g.NavDisableHighlight = true;
		}
	}
	if (pressed)
		MarkItemEdited(id);

	// In this branch, Selectable() cannot toggle the selection so this will never trigger.
	if (selected != was_selected) //-V547
		g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

	// Render
	auto& frame_anim = ngui::animation_manager::get<linear_button_color_animation>(
		window,
		id,
		"smooth_selectable_frame"_sh,
		3.F,
		6.F,
		ImVec4{0, 0, 0, 0},
		GetStyleColorVec4(ImGuiCol_HeaderHovered),
		GetStyleColorVec4(ImGuiCol_HeaderActive));

	frame_anim.button_update(held && hovered, hovered);

	const ImU32 col = frame_anim.get_current_u32_color();
	RenderFrame(bb.Min, bb.Max, col, false, 0.0f);

	RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_NoRounding);

	if (span_all_columns && window->DC.CurrentColumns)
		PopColumnsBackground();
	else if (span_all_columns && g.CurrentTable)
		TablePopBackgroundChannel();

	RenderTextClipped(text_min, text_max, label, nullptr, &label_size, style.SelectableTextAlign, &bb);

	if (auto* p_popup_anim = ngui::animation_manager::get_ptr_or_nullptr<linear_animation>(
			window->ParentWindow,
			"smooth_combo_popup"_sh,
			"size"_sh,
			false);
		p_popup_anim != nullptr) {
		if (p_popup_anim->get_limits().y != 1.F
			&& p_popup_anim->get_current() < popup_start_limit) {
			CloseCurrentPopup();

			p_popup_anim->should_update = false;
			p_popup_anim->update_frames = 0;
		}

		// Automatically close popups
		if (pressed
			&& window->Flags & ImGuiWindowFlags_Popup
			&& !(flags & ImGuiSelectableFlags_DontClosePopups)
			&& !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
			p_popup_anim->set_limits({1.F, 0.F});
	}

	if (disabled_item && !disabled_global)
		EndDisabled();

	return pressed;
}

bool
ImGui::SmoothSelectable(const char* label,
	bool* p_selected,
	const ImGuiSelectableFlags flags,
	const ImVec2& size_arg) {
	if (SmoothSelectable(label, *p_selected, flags, size_arg))
	{
		*p_selected = !*p_selected;
		return true;
	}
	return false;
}
