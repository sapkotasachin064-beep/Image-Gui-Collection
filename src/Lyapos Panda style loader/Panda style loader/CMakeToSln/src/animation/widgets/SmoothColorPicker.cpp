//
// Created by n08i40k on 12.04.2024.
//

#include "SmoothColorPicker.h"

#include <array>
#include <imgui_internal.h>

#include "animation/animation_manager.h"
#include "animation/linear/linear_with_target_2d_animation.h"
#include "animation/linear/linear_with_target_animation.h"

bool
ImGui::SmoothColorPicker3(const char* label,
                          float col[3],
                          const ImGuiColorEditFlags flags) {
	float col4[4] = {col[0], col[1], col[2], 1.0f};
	if (!SmoothColorPicker4(label, col4, flags | ImGuiColorEditFlags_NoAlpha))
		return false;
	col[0] = col4[0];
	col[1] = col4[1];
	col[2] = col4[2];
	return true;
}

static void
ColorEditRestoreHS(const float* col,
                   float* H,
                   float* S,
                   float* V) {
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.ColorEditCurrentID != 0);
	if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(
		ImVec4(col[0], col[1], col[2], 0)))
		return;

	// When S == 0, H is undefined.
	// When H == 1 it wraps around to 0.
	if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
		*H = g.ColorEditSavedHue;

	// When V == 0, S is undefined.
	if (*V == 0.0f)
		*S = g.ColorEditSavedSat;
}

static void
ColorEditRestoreH(const float* col,
                  float* H) {
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.ColorEditCurrentID != 0);
	if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(
		ImVec4(col[0], col[1], col[2], 0)))
		return;
	*H = g.ColorEditSavedHue;
}

static void
RenderArrowsForVerticalBar(ImDrawList* draw_list,
                           ImVec2 pos,
                           ImVec2 half_sz,
                           float bar_w,
                           float alpha) {
	ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
	ImGui::RenderArrowPointingAt(draw_list,
	                             ImVec2(pos.x + half_sz.x + 1, pos.y),
	                             ImVec2(half_sz.x + 2, half_sz.y + 1),
	                             ImGuiDir_Right,
	                             IM_COL32(0, 0, 0, alpha8));
	ImGui::RenderArrowPointingAt(draw_list,
	                             ImVec2(pos.x + half_sz.x, pos.y),
	                             half_sz,
	                             ImGuiDir_Right,
	                             IM_COL32(255, 255, 255, alpha8));
	ImGui::RenderArrowPointingAt(draw_list,
	                             ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y),
	                             ImVec2(half_sz.x + 2, half_sz.y + 1),
	                             ImGuiDir_Left,
	                             IM_COL32(0, 0, 0, alpha8));
	ImGui::RenderArrowPointingAt(draw_list,
	                             ImVec2(pos.x + bar_w - half_sz.x, pos.y),
	                             half_sz,
	                             ImGuiDir_Left,
	                             IM_COL32(255, 255, 255, alpha8));
}

bool
ImGui::SmoothColorPicker4(const char* label,
                          float col[4],
                          ImGuiColorEditFlags flags,
                          const float* ref_col) {
	auto& g = *GImGui;
	auto* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	auto* draw_list = window->DrawList;
	auto& style = g.Style;
	auto& io = g.IO;

	const float width = CalcItemWidth();
	g.NextItemData.ClearFlags();

	PushID(label);
	const bool set_current_color_edit_id = g.ColorEditCurrentID == 0;
	if (set_current_color_edit_id)
		g.ColorEditCurrentID = window->IDStack.back();
	BeginGroup();

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
		flags |= ImGuiColorEditFlags_NoSmallPreview;

	// Context menu: display and store options.
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ColorPickerOptionsPopup(col, flags);

	// Read stored options
	if (!(flags & ImGuiColorEditFlags_PickerMask_)) {
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_
			          ? g.ColorEditOptions
			          : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
	}
	if (!(flags & ImGuiColorEditFlags_InputMask_)) {
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_
			          ? g.ColorEditOptions
			          : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
	}
	IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_)); // Check that only 1 is selected
	IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));  // Check that only 1 is selected
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags |= g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar;

	// install
	int components = flags & ImGuiColorEditFlags_NoAlpha
		                 ? 3
		                 : 4;
	bool alpha_bar = flags & ImGuiColorEditFlags_AlphaBar && !(flags & ImGuiColorEditFlags_NoAlpha);
	auto picker_pos = window->DC.CursorPos;

	float square_sz = GetFrameHeight();
	float bars_width = square_sz; // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 1,
	                             width - (alpha_bar
		                                      ? 2
		                                      : 1) * (bars_width + style.ItemInnerSpacing.x));

	// Saturation/Value picking box
	float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
	float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

	std::array<float, 4> backup_initial_col{};
	memcpy(backup_initial_col.data(), col, components * sizeof(float));

	float wheel_thickness = sv_picker_size * 0.08f;
	float wheel_r_outer = sv_picker_size * 0.50f;
	float wheel_r_inner = wheel_r_outer - wheel_thickness;
	ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width) * 0.5f, picker_pos.y + sv_picker_size * 0.5f);

	// Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
	float triangle_r = wheel_r_inner - static_cast<int>(sv_picker_size * 0.027f);
	auto triangle_pa = ImVec2(triangle_r, 0.0f);                            // Hue point.
	auto triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
	auto triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

	float H = col[0];
	float S = col[1];
	float V = col[2];

	float R = col[0];
	float G = col[1];
	float B = col[2];

	if (flags & ImGuiColorEditFlags_InputRGB) {
		// Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
		ColorConvertRGBtoHSV(R, G, B, H, S, V);
		ColorEditRestoreHS(col, &H, &S, &V);
	}
	else if (flags & ImGuiColorEditFlags_InputHSV)
		ColorConvertHSVtoRGB(H, S, V, R, G, B);

	bool value_changed = false;
	bool value_changed_h = false;
	bool value_changed_sv = false;

	PushItemFlag(ImGuiItemFlags_NoNav, true);
	if (flags & ImGuiColorEditFlags_PickerHueWheel) {
		// Hue wheel + SV triangle logic
		InvisibleButton("hsv", ImVec2(sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size));
		if (IsItemActive()) {
			auto initial_off = g.IO.MouseClickedPos[0] - wheel_center;
			auto current_off = g.IO.MousePos - wheel_center;
			float initial_dist2 = ImLengthSqr(initial_off);
			if (initial_dist2 >= (wheel_r_inner - 1) * (wheel_r_inner - 1) && initial_dist2 <= (wheel_r_outer + 1) * (
				wheel_r_outer + 1)) {
				// Interactive with Hue wheel
				H = ImAtan2(current_off.y, current_off.x) / IM_PI * 0.5f;
				if (H < 0.0f)
					H += 1.0f;
				value_changed = value_changed_h = true;
			}
			float cos_hue_angle = ImCos(-H * 2.0f * IM_PI);
			float sin_hue_angle = ImSin(-H * 2.0f * IM_PI);
			if (ImTriangleContainsPoint(triangle_pa,
			                            triangle_pb,
			                            triangle_pc,
			                            ImRotate(initial_off, cos_hue_angle, sin_hue_angle))) {
				// Interacting with SV triangle
				auto current_off_unrotated = ImRotate(current_off, cos_hue_angle, sin_hue_angle);
				if (!ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated)) {
					current_off_unrotated = ImTriangleClosestPoint(triangle_pa,
					                                               triangle_pb,
					                                               triangle_pc,
					                                               current_off_unrotated);
				}
				float uu;
				float vv;
				float ww;

				ImTriangleBarycentricCoords(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww);
				V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
				S = ImClamp(uu / V, 0.0001f, 1.0f);
				value_changed = value_changed_sv = true;
			}
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
	}
	else if (flags & ImGuiColorEditFlags_PickerHueBar) {
		// SV rectangle logic
		InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
		if (IsItemActive()) {
			S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
			V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			ColorEditRestoreH(col, &H);
			// Greatly reduces hue jitter and reset to 0 when hue == 255 and color is rapidly modified using SV square.
			value_changed = value_changed_sv = true;
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

		// Hue bar logic
		SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
		InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
		if (IsItemActive()) {
			H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = value_changed_h = true;
		}
	}

	// Alpha bar logic
	if (alpha_bar) {
		SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (IsItemActive()) {
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}
	PopItemFlag(); // ImGuiItemFlags_NoNav

	if (!(flags & ImGuiColorEditFlags_NoSidePreview)) {
		SameLine(0, style.ItemInnerSpacing.x);
		BeginGroup();
	}

	if (!(flags & ImGuiColorEditFlags_NoLabel)) {
		if (const auto* label_display_end = FindRenderedTextEnd(label);
			label != label_display_end) {
			if (flags & ImGuiColorEditFlags_NoSidePreview)
				SameLine(0, style.ItemInnerSpacing.x);
			TextEx(label, label_display_end);
		}
	}

	if (!(flags & ImGuiColorEditFlags_NoSidePreview)) {
		PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
		ImVec4 col_v4(col[0],
		              col[1],
		              col[2],
		              flags & ImGuiColorEditFlags_NoAlpha
			              ? 1.0f
			              : col[3]);
		if (flags & ImGuiColorEditFlags_NoLabel)
			Text("Current");

		ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR |
			ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip;
		ColorButton("##current", col_v4, flags & sub_flags_to_forward, ImVec2(square_sz * 3, square_sz * 2));
		if (ref_col != nullptr) {
			Text("Original");
			ImVec4 ref_col_v4(ref_col[0],
			                  ref_col[1],
			                  ref_col[2],
			                  flags & ImGuiColorEditFlags_NoAlpha
				                  ? 1.0f
				                  : ref_col[3]);
			if (ColorButton("##original",
			                ref_col_v4,
			                flags & sub_flags_to_forward,
			                ImVec2(square_sz * 3, square_sz * 2))) {
				memcpy(col, ref_col, components * sizeof(float));
				value_changed = true;
			}
		}
		PopItemFlag();
		EndGroup();
	}

	// Convert back color to RGB
	if (value_changed_h || value_changed_sv) {
		if (flags & ImGuiColorEditFlags_InputRGB) {
			ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
			g.ColorEditSavedHue = H;
			g.ColorEditSavedSat = S;
			g.ColorEditSavedID = g.ColorEditCurrentID;
			g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
		}
		else if (flags & ImGuiColorEditFlags_InputHSV) {
			col[0] = H;
			col[1] = S;
			col[2] = V;
		}
	}

	// R,G,B and H,S,V slider color editor
	bool value_changed_fix_hue_wrap = false;
	if ((flags & ImGuiColorEditFlags_NoInputs) == 0) {
		PushItemWidth((alpha_bar
			               ? bar1_pos_x
			               : bar0_pos_x) + bars_width - picker_pos.x);
		ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_InputMask_ |
			ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions |
			ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview |
			ImGuiColorEditFlags_AlphaPreviewHalf;
		ImGuiColorEditFlags sub_flags = flags & sub_flags_to_forward | ImGuiColorEditFlags_NoPicker;
		if (flags & ImGuiColorEditFlags_DisplayRGB || (flags & ImGuiColorEditFlags_DisplayMask_) == 0) {
			if (ColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_DisplayRGB)) {
				// FIXME: Hackily differentiating using the DragInt (ActiveId != 0 && !ActiveIdAllowOverlap) vs. using the InputText or DropTarget.
				// For the later we don't want to run the hue-wrap canceling code. If you are well versed in HSV picker please provide your input! (See #2050)
				value_changed_fix_hue_wrap = g.ActiveId != 0 && !g.ActiveIdAllowOverlap;
				value_changed = true;
			}
		}
		if (flags & ImGuiColorEditFlags_DisplayHSV || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
			value_changed |= ColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_DisplayHSV);
		if (flags & ImGuiColorEditFlags_DisplayHex || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
			value_changed |= ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_DisplayHex);
		PopItemWidth();
	}

	// Try to cancel hue wrap (after ColorEdit4 call), if any
	if (value_changed_fix_hue_wrap && flags & ImGuiColorEditFlags_InputRGB) {
		float new_H;
		float new_S;
		float new_V;

		ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0) {
			if (new_V <= 0 && V != new_V) {
				ColorConvertHSVtoRGB(H,
				                     S,
				                     new_V <= 0
					                     ? V * 0.5f
					                     : new_V,
				                     col[0],
				                     col[1],
				                     col[2]);
			}
			else if (new_S <= 0) {
				ColorConvertHSVtoRGB(H,
				                     new_S <= 0
					                     ? S * 0.5f
					                     : new_S,
				                     new_V,
				                     col[0],
				                     col[1],
				                     col[2]);
			}
		}
	}

	if (value_changed) {
		if (flags & ImGuiColorEditFlags_InputRGB) {
			R = col[0];
			G = col[1];
			B = col[2];
			ColorConvertRGBtoHSV(R, G, B, H, S, V);
			ColorEditRestoreHS(col, &H, &S, &V); // Fix local Hue as display below will use it immediately.
		}
		else if (flags & ImGuiColorEditFlags_InputHSV) {
			H = col[0];
			S = col[1];
			V = col[2];
			ColorConvertHSVtoRGB(H, S, V, R, G, B);
		}
	}

	const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
	const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
	const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
	const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
	const ImU32 col_hues[6 + 1] = {
		IM_COL32(255, 0, 0, style_alpha8),
		IM_COL32(255, 255, 0, style_alpha8),
		IM_COL32(0, 255, 0, style_alpha8),
		IM_COL32(0, 255, 255, style_alpha8),
		IM_COL32(0, 0, 255, style_alpha8),
		IM_COL32(255, 0, 255, style_alpha8),
		IM_COL32(255, 0, 0, style_alpha8)
	};

	ImVec4 hue_color_f(1, 1, 1, style.Alpha);
	ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
	ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha));
	// Important: this is still including the main rendering/style alpha!!

	ImVec2 sv_cursor_pos;

	const auto id = ImHashStr(label);

	if (flags & ImGuiColorEditFlags_PickerHueWheel) {
		// Render Hue Wheel
		const float aeps = 0.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
		const int segment_per_arc = ImMax(4, static_cast<int>(wheel_r_outer) / 12);
		for (int n = 0; n < 6; n++) {
			const float a0 = static_cast<float>(n) / 6.0f * 2.0f * IM_PI - aeps;
			const float a1 = (static_cast<float>(n) + 1.0f) / 6.0f * 2.0f * IM_PI + aeps;
			const int vert_start_idx = draw_list->VtxBuffer.Size;
			draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer) * 0.5f, a0, a1, segment_per_arc);
			draw_list->PathStroke(col_white, 0, wheel_thickness);
			const int vert_end_idx = draw_list->VtxBuffer.Size;

			// Paint colors over existing vertices
			ImVec2 gradient_p0(wheel_center.x + ImCos(a0) * wheel_r_inner, wheel_center.y + ImSin(a0) * wheel_r_inner);
			ImVec2 gradient_p1(wheel_center.x + ImCos(a1) * wheel_r_inner, wheel_center.y + ImSin(a1) * wheel_r_inner);
			ShadeVertsLinearColorGradientKeepAlpha(draw_list,
			                                       vert_start_idx,
			                                       vert_end_idx,
			                                       gradient_p0,
			                                       gradient_p1,
			                                       col_hues[n],
			                                       col_hues[n + 1]);
		}

		// Render Cursor + preview on Hue Wheel
		float cos_hue_angle = ImCos(H * 2.0f * IM_PI);
		float sin_hue_angle = ImSin(H * 2.0f * IM_PI);
		ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f,
		                      wheel_center.y + sin_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f);
		float hue_cursor_rad = value_changed_h
			                       ? wheel_thickness * 0.65f
			                       : wheel_thickness * 0.55f;
		int hue_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(hue_cursor_rad);
		// Lock segment count so the +1 one matches others.
		draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
		draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad + 1, col_midgrey, hue_cursor_segments);
		draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, col_white, hue_cursor_segments);

		// Render SV triangle (rotated according to hue)
		auto tra = wheel_center + ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
		auto trb = wheel_center + ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
		auto trc = wheel_center + ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
		auto uv_white = GetFontTexUvWhitePixel();
		draw_list->PrimReserve(3, 3);
		draw_list->PrimVtx(tra, uv_white, hue_color32);
		draw_list->PrimVtx(trb, uv_white, col_black);
		draw_list->PrimVtx(trc, uv_white, col_white);
		draw_list->AddTriangle(tra, trb, trc, col_midgrey, 1.5f);
		sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
	}
	else if (flags & ImGuiColorEditFlags_PickerHueBar) {
		// Render SV Square
		draw_list->AddRectFilledMultiColor(picker_pos,
		                                   picker_pos + ImVec2(sv_picker_size, sv_picker_size),
		                                   col_white,
		                                   hue_color32,
		                                   hue_color32,
		                                   col_white);
		draw_list->AddRectFilledMultiColor(picker_pos,
		                                   picker_pos + ImVec2(sv_picker_size, sv_picker_size),
		                                   0,
		                                   0,
		                                   col_black,
		                                   col_black);
		RenderFrameBorder(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), 0.0f);
		sv_cursor_pos.x = ImClamp(
			IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size),
			picker_pos.x + 2,
			picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
		sv_cursor_pos.y = ImClamp(
			IM_ROUND(picker_pos.y + ImSaturate(1 - V) * sv_picker_size),
			picker_pos.y + 2,
			picker_pos.y + sv_picker_size - 2);

		// Render Hue Bar
		for (int i = 0; i < 6; ++i) {
			draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)),
			                                   ImVec2(bar0_pos_x + bars_width,
			                                          picker_pos.y + (i + 1) * (sv_picker_size / 6)),
			                                   col_hues[i],
			                                   col_hues[i],
			                                   col_hues[i + 1],
			                                   col_hues[i + 1]);
		}
		float bar0_line_y = IM_ROUND(picker_pos.y + H * sv_picker_size);
		RenderFrameBorder(ImVec2(bar0_pos_x, picker_pos.y),
		                  ImVec2(bar0_pos_x + bars_width, picker_pos.y + sv_picker_size),
		                  0.0f);

		auto& vertical_bar0_y_anim = ngui::animation_manager::get<
			linear_with_target_animation>(
			window,
			id,
			"vertical_bar0_y"_sh,
			3.F);

		vertical_bar0_y_anim.set_limits({picker_pos.y, picker_pos.y + sv_picker_size});
		vertical_bar0_y_anim.set_target(bar0_line_y);

		RenderArrowsForVerticalBar(draw_list,
		                           ImVec2(bar0_pos_x - 1, vertical_bar0_y_anim.get_current()),
		                           ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz),
		                           bars_width + 2.0f,
		                           style.Alpha);
	}

	// Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be out of range)
	float sv_cursor_rad = value_changed_sv
		                      ? 10.0f
		                      : 6.0f;
	int sv_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(sv_cursor_rad);
	// Lock segment count so the +1 one matches others.

	auto& sv_cursor_pos_smooth_anim = ngui::animation_manager::get<linear_with_target_2d_animation>(
		window,
		id,
		"sv_cursor_pos_x"_sh,
		6.F);
	sv_cursor_pos_smooth_anim.set_limits({picker_pos.x, picker_pos.x + sv_picker_size},
	                                     {picker_pos.y, picker_pos.y + sv_picker_size});
	sv_cursor_pos_smooth_anim.set_target(vector2f{sv_cursor_pos});

	const auto sv_cursor_pos_smooth = sv_cursor_pos_smooth_anim.get_current().convert<ImVec2>();

	draw_list->AddCircleFilled(sv_cursor_pos_smooth, sv_cursor_rad, user_col32_striped_of_alpha, sv_cursor_segments);
	draw_list->AddCircle(sv_cursor_pos_smooth, sv_cursor_rad + 1, col_midgrey, sv_cursor_segments);
	draw_list->AddCircle(sv_cursor_pos_smooth, sv_cursor_rad, col_white, sv_cursor_segments);

	// Render alpha bar
	if (alpha_bar) {
		float alpha = ImSaturate(col[3]);
		ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);
		RenderColorRectWithAlphaCheckerboard(draw_list,
		                                     bar1_bb.Min,
		                                     bar1_bb.Max,
		                                     0,
		                                     bar1_bb.GetWidth() / 2.0f,
		                                     ImVec2(0.0f, 0.0f));
		draw_list->AddRectFilledMultiColor(bar1_bb.Min,
		                                   bar1_bb.Max,
		                                   user_col32_striped_of_alpha,
		                                   user_col32_striped_of_alpha,
		                                   user_col32_striped_of_alpha & ~IM_COL32_A_MASK,
		                                   user_col32_striped_of_alpha & ~IM_COL32_A_MASK);
		float bar1_line_y = IM_ROUND(picker_pos.y + (1.0f - alpha) * sv_picker_size);

		auto& vertical_bar1_y_anim =
			ngui::animation_manager::get<linear_with_target_animation>(
				window,
				id,
				"vertical_bar1_y"_sh,
				3.F);

		vertical_bar1_y_anim.set_limits({picker_pos.y, picker_pos.y + sv_picker_size});
		vertical_bar1_y_anim.set_target(bar1_line_y);

		RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
		RenderArrowsForVerticalBar(draw_list,
		                           ImVec2(bar1_pos_x - 1, vertical_bar1_y_anim.get_current()),
		                           ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz),
		                           bars_width + 2.0f,
		                           style.Alpha);
	}

	EndGroup();

	if (value_changed && memcmp(backup_initial_col.data(), col, components * sizeof(float)) == 0)
		value_changed = false;
	if (value_changed && g.LastItemData.ID != 0)
		// In case of ID collision, the second EndGroup() won't catch g.ActiveId
		MarkItemEdited(g.LastItemData.ID);

	if (set_current_color_edit_id)
		g.ColorEditCurrentID = 0;
	PopID();

	return value_changed;
}
