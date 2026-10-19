//
// Created by n08i40k on 15.04.2024.
//

#include "imgui_notification.h"

#include "animation/animation_manager.h"
#include "animation/linear/linear_with_target_animation.h"
#include "dynamic_size/imgui_dynamic_size.h"

imgui_notification::imgui_notification(
	imgui_notification_options&& options): millis_left(static_cast<float>(options.alive_millis))
	                                     , options(std::move(options)) {
	assert(!this->options.title_text.empty());
	assert(!this->options.description_text.empty());
	assert(this->options.alive_millis > 0);
}

void
imgui_notification::render(const imgui_notification_manager_options& runtime_options,
                           const float delta,
                           const size_t index) {
	millis_left = std::max(0.F, millis_left - delta * 1000);
	dead_ = millis_left == 0;

	/* Тута алигн */
	if (saved_window_data_.p_window != nullptr) {
		if (p_pos_y_anim == nullptr) {
			p_pos_y_anim = ngui::animation_manager::get_ptr<linear_with_target_animation>(
				saved_window_data_.p_window,
				saved_window_data_.p_window->ID,
				"pos_y"_sh,
				0.35F);
		}
		p_pos_y_anim->queried = true;

		const bool top = (runtime_options.align & imgui_notif_align_top) != 0;
		const bool left = (runtime_options.align & imgui_notif_align_left) != 0;

		const auto viewport_size = ImGui::GetMainViewport()->Size;
		p_pos_y_anim->set_limits(vector2f{0.F, viewport_size.y}, true);

		// Свободное место между уведомлениями.
		const float y_notification_spacing = ImGui::GetStyle().ItemSpacing.y * 2 * static_cast<float>(index);

		// Отступ от уже отрендереных окон, от наслоения уведомлений друг на друга.
		const float y_window_padding = saved_window_data_.size.y * static_cast<float>(index);

		p_pos_y_anim->set_target(y_notification_spacing + y_window_padding);

		const auto viewport_padding = runtime_options.padding * imgui_dynamic_size::get_current_scale_factor();

		ImGui::SetNextWindowPos({
			left
				? viewport_padding.x
				: viewport_size.x - saved_window_data_.size.x - viewport_padding.x,
			top
				? p_pos_y_anim->get_current() + viewport_padding.y
				: viewport_size.y - p_pos_y_anim->get_current() - saved_window_data_.size.y - viewport_padding.y,
		});
	}
}

bool
imgui_notification::is_dead() const { return dead_; }

void
imgui_notification::respawn() { millis_left = static_cast<float>(options.alive_millis); }
