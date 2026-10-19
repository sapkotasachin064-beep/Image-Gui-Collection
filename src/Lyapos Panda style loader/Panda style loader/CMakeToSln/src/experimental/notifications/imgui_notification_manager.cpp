//
// Created by n08i40k on 15.04.2024.
//

#include "imgui_notification_manager.h"

#include <algorithm>
#include <ranges>

imgui_notification_manager::imgui_notification_manager(const imgui_notification_manager_options& options) : options(options) {
#ifdef _DEBUG
	const auto align = options.align;
#endif
	assert((align & imgui_notif_align_top) != 0 || (align & imgui_notif_align_bottom) != 0);
	assert((align & imgui_notif_align_top) == 0 || (align & imgui_notif_align_bottom) == 0);

	assert((align & imgui_notif_align_left) != 0 || (align & imgui_notif_align_right) != 0);
	assert((align & imgui_notif_align_left) == 0 || (align & imgui_notif_align_right) == 0);
}

void
imgui_notification_manager::render() {
	size_t index{};
	const float delta = ImGui::GetIO().DeltaTime;

	for (auto& notification : *this) {
		notification->render(options, delta, index);
		++index;
	}

	std::erase_if(*this, [](const value_type& notification) { return notification->is_dead(); });
}

std::weak_ptr<notification_impl_t>
imgui_notification_manager::add(imgui_notification_options&& options) {
	for (auto& notification : *this) {
		if (notification->options.uuid == options.uuid) {
			notification->respawn();
			return std::dynamic_pointer_cast<notification_impl_t, imgui_notification>(notification);
		}
	}

	return std::dynamic_pointer_cast<notification_impl_t, imgui_notification>(
		emplace_back(
			std::shared_ptr<imgui_notification>(
				reinterpret_cast<imgui_notification*>(
					new notification_impl_t(std::move(options))
				)
			)
		)
	);
}

std::weak_ptr<imgui_notification_manager>
imgui_notification_manager::create_instance(const imgui_notification_manager_options& options) {
	assert(p_instance_ == nullptr);

	return p_instance_ = std::make_shared<imgui_notification_manager>(options);
}

void
imgui_notification_manager::delete_instance() {
	assert(p_instance_ != nullptr);

	p_instance_.reset();
}

std::shared_ptr<imgui_notification_manager> imgui_notification_manager::p_instance_ = nullptr;
