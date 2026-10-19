//
// Created by n08i40k on 09.04.2024.
//

#include "animation_manager.h"

#include <memory>
#include <ranges>

#include "imgui_animation_base.h"

namespace ngui {
void
animation_manager::pre_render() {
	const float delta = ImGui::GetIO().DeltaTime;

	for (auto& window_animations : windows_ | std::views::values) {
		for (auto& animation_list : window_animations | std::views::values) {
			// Удаляются анимации с предыдущего кадра, которые не были использованы.
			std::erase_if(animation_list,
			  [](const std::pair<uint32_t, animation_ptr_t>& animation) { return !animation.second->queried; });

			// Выключается флаг того, что анимация была использована и вызывается фукнция обновления.
			for (auto& animation : animation_list | std::views::values) {
				animation->queried = false;

				if (animation->should_update) {
					animation->update(delta);

					animation->first_frame = false;
					++animation->update_frames;
				}
			}
		}

		// Удаляются неиспользуемые анимации у предмета.
		std::erase_if(window_animations,
			  [](const std::pair<ImGuiID, animation_list_t>& window_animation) { return window_animation.second.empty(); });
	}

	// Удаляются неиспользуемые окна.
	std::erase_if(windows_,
				  [](const std::pair<ImGuiWindow*, window_animations_t>& window) { return window.second.empty(); });
}

animation_manager*
animation_manager::create_instance() {
	assert(p_instance_ == nullptr);

	p_instance_ = std::make_unique<animation_manager>();

	return p_instance_.get();
}

void
animation_manager::delete_instance() {
	assert(p_instance_ != nullptr);

	p_instance_.reset();
}

std::unique_ptr<animation_manager> animation_manager::p_instance_{};
}
