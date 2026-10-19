//
// Created by n08i40k on 18.01.2024.
//

#ifndef IMGUIMENU_H
#define IMGUIMENU_H

#include <cassert>
#include <functional>
#include <imgui.h>
#include <map>
#include <memory>
#include <string>

enum imgui_menu_flags : uint8_t {
	imgui_menu_flags_none = 0,
	imgui_menu_flags_no_list = 0x02
};

inline bool bTheme;

namespace plugin_systems {
class imgui_menu {
public:
	struct menu_tab {
		std::string title;
		std::string description;
		std::function<void()> render_callback;
	};

private:
	static std::shared_ptr<imgui_menu> p_instance_;

	bool show_{true};
	imgui_menu_flags flags_;

	size_t selected_tab_id_{};

	bool view_tab_id_changed_{};
	size_t view_tab_id_{};
	size_t previous_view_tab_id_{};
	size_t hovered_tab_id_{};

	size_t next_map_id_{};

	std::map<size_t, menu_tab> tabs_;

public:
	explicit
	imgui_menu(imgui_menu_flags flags);

	/* Render */

	void
	render_tab_list_button(size_t tab_index,
	                       const ImVec2& tab_size,
	                       float invisible_border_size);
	void
	render_tab_list();
	void
	render(ImTextureID logotype = nullptr, ImTextureID bg = nullptr);

	/* Tabs manipulate */

	size_t
	emplace(std::string&& title,
	        std::string&& description,
	        const std::function<void()>& render_callback);

	void
	set_current_tab(size_t tab_id);

	/* View manipulate */

	bool
	toggle();

	[[nodiscard]] bool
	is_show() const;

	/* Instance */

	static std::weak_ptr<imgui_menu>
	get_instance() {
		assert(p_instance_ != nullptr);

		return p_instance_;
	}

	static std::weak_ptr<imgui_menu>
	create_instance(imgui_menu_flags flags = imgui_menu_flags_none);
	static void
	delete_instance();
};
}

#endif //IMGUIMENU_H
