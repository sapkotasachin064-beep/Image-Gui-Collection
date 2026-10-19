//
// Created by n08i40k on 08.04.2024.
//

#ifndef IMGUI_DYNAMIC_SIZE_H
#define IMGUI_DYNAMIC_SIZE_H
#include <imgui.h>
#include <utility>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

enum DynamicFont {
	DynamicFont_Main = 0,
	DynamicFont_Description,
	DynamicFont_Title
};

class imgui_dynamic_size {
	static float current_scale_factor;

	static float
	calculate_scale_factor();
	static void
	scale_style();
	static void
	scale_font();

public:

	inline static ImFont* bold_fonts[10];
	inline static ImFont* semibold_fonts[10];
	inline static ImFont* regular_fonts[10];
	inline static ImFont* medium_fonts[10];

	inline static ImFont* game_logo_font;

	static std::vector<float> font_sizes;
	static std::vector<std::string> font_types;

	static void
	scale_imgui();

	static float
	get_current_scale_factor() { return current_scale_factor; }
};

#endif //IMGUI_DYNAMIC_SIZE_H