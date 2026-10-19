//
// Created by n08i40k on 15.04.2024.
//

#ifndef SQUAD_NOTIFICATION_IMPL_H
#define SQUAD_NOTIFICATION_IMPL_H
#include "experimental/notifications/imgui_notification.h"

class linear_with_target_animation;
class linear_animation;

class squad_notification_impl final : public imgui_notification {
	linear_animation* p_alpha_anim_{};

public:
	~squad_notification_impl() override = default;

	explicit
	squad_notification_impl(imgui_notification_options&& options) : imgui_notification(std::move(options)) {}

	void
	render(const imgui_notification_manager_options& runtime_options,
	       float delta,
	       size_t index) override;

	[[nodiscard]] bool
	is_dead() const override;

	void
	respawn() override;
};

#endif //SQUAD_NOTIFICATION_IMPL_H
