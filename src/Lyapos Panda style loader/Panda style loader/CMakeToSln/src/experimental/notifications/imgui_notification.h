//
// Created by n08i40k on 15.04.2024.
//

#ifndef IMGUI_NOTIFICATION_H
#define IMGUI_NOTIFICATION_H

#include <string>

#include "hash_string.h"
#include "imgui.h"
#include "math/vector2.h"

struct ImGuiWindow;
class linear_with_target_animation;

enum imgui_notif_align : int {
	imgui_notif_align_left = 0x02,
	imgui_notif_align_right = 0x04,
	imgui_notif_align_top = 0x08,
	imgui_notif_align_bottom = 0x10,

	imgui_notif_align_left_top = imgui_notif_align_left | imgui_notif_align_top,
	imgui_notif_align_left_bottom = imgui_notif_align_left | imgui_notif_align_bottom,

	imgui_notif_align_right_top = imgui_notif_align_right | imgui_notif_align_top,
	imgui_notif_align_right_bottom = imgui_notif_align_right | imgui_notif_align_bottom
};

struct imgui_notification_manager_options {
	imgui_notif_align align = imgui_notif_align_right_bottom;
	vector2f padding;
};

struct imgui_notification_options {
	ImVec4 title_color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
	// устанавливается автоматически, взависимости от title_color, но можно и вручную
	ImVec4 progressbar_color = ImVec4(title_color.x, title_color.y, title_color.z, title_color.w * 0.1F);

	std::string title_text;
	std::string description_text;

	const uint32_t uuid = hash_string(title_text) + hash_string(description_text);

	const size_t alive_millis = 3000;
};

class imgui_notification {
protected:
	bool dead_{};

	float millis_left;

	linear_with_target_animation* p_pos_y_anim{};

	bool first_frame_{true};

	struct saved_window_data {
		ImGuiWindow* p_window{};

		ImVec2 size;
	} saved_window_data_{};

public:
	imgui_notification_options options;

	virtual ~imgui_notification() = default;

	explicit
	imgui_notification(imgui_notification_options&& options);

	virtual void
	render(const imgui_notification_manager_options& runtime_options,
	       float delta,
	       size_t index);

	[[nodiscard]] virtual bool
	is_dead() const;

	virtual void
	respawn();
};

#endif //IMGUI_NOTIFICATION_H
