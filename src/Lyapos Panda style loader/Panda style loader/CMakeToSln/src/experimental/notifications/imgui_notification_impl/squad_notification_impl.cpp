//
// Created by n08i40k on 15.04.2024.
//

#include "squad_notification_impl.h"

#include <format>

#include "imgui_internal.h"
#include "animation/animation_manager.h"
#include "animation/linear/linear_animation.h"
#include "math/vector2.h"
#include "dynamic_size/imgui_dynamic_size.h"

void
squad_notification_impl::render(const imgui_notification_manager_options& runtime_options,
                                const float delta,
                                const size_t index) {
	imgui_notification::render(runtime_options, delta, index);

	if (p_alpha_anim_ != nullptr) {
		if (dead_)
			p_alpha_anim_->set_inverted(true);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, p_alpha_anim_->get_current());
		p_alpha_anim_->queried = true;
	}

	/* Тута рендер */
	if (ImGui::Begin(std::format("##notification{}", options.uuid).c_str(),
	                 nullptr,
	                 ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
	                 ImGuiWindowFlags_AlwaysAutoResize)) {
		auto* p_window = ImGui::GetCurrentWindow();

		const auto window_rect = p_window->Rect();

		if (first_frame_ || saved_window_data_.p_window == nullptr)
			p_window->Hidden = true;

		if (first_frame_) {
			p_alpha_anim_ = ngui::animation_manager::get_ptr<linear_animation>(
				p_window,
				p_window->ID,
				"alpha"_sh,
				3.F,
				vector2f{0.1F, 1.F});
			p_alpha_anim_->reset_current();

			first_frame_ = false;
		}
		else {
			saved_window_data_.p_window = p_window;
			saved_window_data_.size = window_rect.Max - window_rect.Min;
		}

		auto* const p_drawlist = ImGui::GetWindowDrawList();

		const float ttd_mult = millis_left / static_cast<float>(options.alive_millis);
		const auto window_rounding = ImGui::GetStyle().WindowRounding;

		const auto pb_pos = window_rect.Min;
		const auto pb_size = window_rect.Max - window_rect.Min;

		ImGui::PushClipRect(pb_pos,
							pb_pos + ImVec2(pb_size.x * ttd_mult, pb_size.y), false);

		p_drawlist->AddRectFilled(window_rect.Min, window_rect.Max,
		                          ImGui::GetColorU32(options.progressbar_color),
		                          window_rounding);

		ImGui::PopClipRect();

		ImGui::PushFont(imgui_dynamic_size::medium_fonts[3]);
		ImGui::TextColored(options.title_color, "%s", options.title_text.c_str());
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::Text("%s", options.description_text.c_str());

		ImGui::End();
	}

	if (p_alpha_anim_ != nullptr && !p_alpha_anim_->first_frame)
		ImGui::PopStyleVar();
}

bool
squad_notification_impl::is_dead() const {
	if (p_alpha_anim_ == nullptr)
		return false;

	return dead_ && p_alpha_anim_->is_finished();
}

void
squad_notification_impl::respawn() {
	imgui_notification::respawn();

	p_alpha_anim_->set_inverted(false);
}
