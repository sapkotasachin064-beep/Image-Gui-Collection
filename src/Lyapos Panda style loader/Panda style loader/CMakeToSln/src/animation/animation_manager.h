//
// Created by n08i40k on 09.04.2024.
//

#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <cassert>
#include <imgui.h>
#include <imgui_internal.h>
#include <memory>
#include <unordered_map>

#include "imgui_animation_base.h"

class imgui_animation_base;

template<class T, class... CT>
concept constuctible_from = requires(CT&&... c_args) {
	{ new T(std::forward<CT>(c_args)...) } -> std::convertible_to<T*>;
};

namespace ngui {
class animation_manager {
	static std::unique_ptr<animation_manager> p_instance_;

	using animation_ptr_t = std::shared_ptr<imgui_animation_base>;
	using animation_list_t = std::unordered_map<uint32_t, animation_ptr_t>;
	using window_animations_t = std::unordered_map<ImGuiID, animation_list_t>;

	std::unordered_map<ImGuiWindow*, window_animations_t> windows_;

public:
	void
	pre_render();

	template<class T, typename... CT>
		requires std::is_base_of_v<imgui_animation_base, T> && constuctible_from<T, CT...>
	static T*
	get_ptr(ImGuiWindow* p_window,
	        const ImGuiID item_id,
	        const uint32_t sub_id = 0,
	        CT... constructor_args) {
		auto& this_ = get_instance_();

		if (!this_.windows_.contains(p_window))
			this_.windows_.emplace(p_window, window_animations_t{});

		auto& window_animations = this_.windows_.at(p_window);
		if (!window_animations.contains(item_id))
			window_animations.emplace(item_id, animation_list_t{});

		auto& animation_list = window_animations.at(item_id);
		if (!animation_list.contains(sub_id))
			animation_list.emplace(sub_id, std::make_shared<T>(std::forward<CT>(constructor_args)...));

		auto p_animation = animation_list.at(sub_id);
		p_animation->queried = true;

		assert(p_animation->current_animation_id == T::animation_id);

		return std::dynamic_pointer_cast<T>(p_animation).get();
	}

	template<class T, typename... CT>
		requires std::is_base_of_v<imgui_animation_base, T> && constuctible_from<T, CT...>
	static T&
	get(ImGuiWindow* p_window,
	    const ImGuiID item_id,
	    const uint32_t sub_id = 0,
	    CT... constructor_args) {
		return *get_ptr<T>(p_window, item_id, sub_id, std::forward<CT>(constructor_args)...);
	}

	template<class T, typename... CT>
		requires std::is_base_of_v<imgui_animation_base, T> && constuctible_from<T, CT...>
	static T&
	get_default(const uint32_t sub_id = 0,
	            CT... constructor_args) {
		const auto& context = *ImGui::GetCurrentContext();
		return get<T>(context.CurrentWindow,
		              context.LastItemData.ID,
		              sub_id,
		              std::forward<CT>(constructor_args)...);
	}

	template<class T>
		requires std::is_base_of_v<imgui_animation_base, T>
	static T*
	get_ptr_or_nullptr(ImGuiWindow* p_window,
	                   const ImGuiID item_id,
	                   const uint32_t sub_id = 0,
	                   const bool assign_queried_status = false) {
		auto& this_ = get_instance_();

		if (!this_.windows_.contains(p_window))
			return nullptr;

		auto& window_animations = this_.windows_.at(p_window);
		if (!window_animations.contains(item_id))
			return nullptr;

		auto& animation_list = window_animations.at(item_id);
		if (!animation_list.contains(sub_id))
			return nullptr;

		auto p_animation = animation_list.at(sub_id);
		if (assign_queried_status)
			p_animation->queried = true;

		assert(p_animation->current_animation_id == T::animation_id);

		return std::dynamic_pointer_cast<T>(p_animation).get();
	}

	template<class Animation, class... ConstructorArgs>
	[[deprecated]] [[nodiscard]] Animation*
	get_animation(ImGuiWindow* p_window,
	              const ImGuiID id,
	              const uint32_t sub_id,
	              ConstructorArgs&&... constructor_args) {
		return &get<Animation>(p_window, id, sub_id, std::forward<ConstructorArgs>(constructor_args)...);
	}

	template<class Animation, class... ConstructorArgs>
	[[deprecated]] [[nodiscard]] Animation*
	get_curr_animation(const uint32_t sub_id,
	                   ConstructorArgs... constructor_args) {
		auto* const context = ImGui::GetCurrentContext();
		return get_animation<Animation>(context->CurrentWindow,
		                                context->LastItemData.ID,
		                                sub_id,
		                                std::forward<ConstructorArgs>(constructor_args)...);
	}

	template<class Animation>
	[[deprecated]] [[nodiscard]] Animation*
	get_animation_or_nullptr(ImGuiWindow* p_window,
	                         const ImGuiID id,
	                         const uint32_t sub_id,
	                         const bool assing_query) {
		if (!windows_.contains(p_window))
			return nullptr;

		auto& window_animations = windows_.at(p_window);
		if (!window_animations.contains(id))
			return nullptr;

		auto& animation_list = window_animations.at(id);
		if (!animation_list.contains(sub_id))
			return nullptr;

		auto* const p_animation = animation_list.at(sub_id).get();
		if (assing_query)
			p_animation->queried = true;

		assert(p_animation->current_animation_id == Animation::animation_id);

		return reinterpret_cast<Animation*>(p_animation);
	}

	[[deprecated]] static animation_manager*
	get_instance() {
		assert(p_instance_ != nullptr);

		return p_instance_.get();
	}

	static animation_manager&
	get_instance_() {
		assert(p_instance_ != nullptr);

		return *p_instance_;
	}

	static animation_manager*
	create_instance();
	static void
	delete_instance();
};
}

#endif //ANIMATION_MANAGER_H
