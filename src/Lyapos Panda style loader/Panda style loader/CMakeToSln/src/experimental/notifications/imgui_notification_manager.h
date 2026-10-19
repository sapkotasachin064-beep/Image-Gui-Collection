//
// Created by n08i40k on 15.04.2024.
//

#ifndef IMGUI_NOTIFICATION_MANAGER_H
#define IMGUI_NOTIFICATION_MANAGER_H

#include <memory>
#include <vector>

#include "imgui_notification.h"
#include "imgui_notification_impl/squad_notification_impl.h"

using notification_impl_t = squad_notification_impl;

class imgui_notification_manager : public std::vector<std::shared_ptr<imgui_notification> > {
	static std::shared_ptr<imgui_notification_manager> p_instance_;

public:
	imgui_notification_manager_options options;

	explicit
	imgui_notification_manager(const imgui_notification_manager_options& options);

	void
	render();

	std::weak_ptr<notification_impl_t>
	add(imgui_notification_options&& options);

	[[nodiscard]] static std::weak_ptr<imgui_notification_manager>
	get_instance() {
		assert(p_instance_ != nullptr);

		return p_instance_;
	}

	static std::weak_ptr<imgui_notification_manager>
	create_instance(const imgui_notification_manager_options& options = {});
	static void
	delete_instance();
};

#endif //IMGUI_NOTIFICATION_MANAGER_H
