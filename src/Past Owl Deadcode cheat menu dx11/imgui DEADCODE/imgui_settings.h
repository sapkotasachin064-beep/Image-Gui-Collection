#include "imgui.h"

namespace c
{
	inline ImVec4 accent = ImColor(0, 255, 209);
	inline ImVec4 background_widgets = ImColor(3, 3, 3, 70);

	inline float columns = 3; // 3 = 3 columns of elements

	namespace background
	{
		inline ImVec4 background = ImColor(0, 0, 0, 120);
		inline ImVec2 size = ImVec2(1050, 650);
		inline float rounding = 4;
	}

	namespace child
	{
		inline ImVec4 background = ImColor(0, 0, 0, 60);
		inline ImVec4 background_top = ImColor(0, 0, 0, 120);
		inline float rounding = 4;
	}

	namespace checkbox
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline ImVec4 mark = ImColor(0, 0, 0, 255);
		inline float rounding = 2.f;
	}

	namespace keybind
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline float rounding = 2.f;
	}

	namespace input
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline ImVec4 mark = ImColor(0, 0, 0, 255);
		inline float rounding = 2.f;
	}

	namespace slider
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline float rounding = 2.f;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline float rounding = 2.f;
	}

	namespace page
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline float rounding = 4.f;
	}

	namespace combo
	{
		inline ImVec4 background = ImColor(0, 3, 6, 120);
		inline ImVec4 list = ImColor(0, 3, 6, 220);
		inline float rounding = 2.f;
	}

	namespace position_bar
	{
		inline ImVec4 background = ImColor(0, 0, 0, 60);
		inline float rounding = 2.f;
	}

	namespace esp_preview
	{
		inline ImVec4 box_color = ImColor(255, 255, 255, 255);
		inline ImVec4 line_green = ImColor(0, 255, 0, 255);
		inline ImVec4 line_red = ImColor(255, 0, 0, 255);

		inline float rounding = 2.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text = ImColor(109, 118, 136, 255);
	}

}