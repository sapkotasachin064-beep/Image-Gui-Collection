#include "imgui.h"


namespace c {

	inline ImVec4 accent = ImColor(208, 118, 255);
	inline ImVec4 accent_gradient = ImColor(174, 35, 250);

	namespace bg {

		inline ImVec4 shadow = ImColor(14, 14, 16, 0);

		inline ImVec4 background = ImColor(14, 14, 16, 255);
		inline ImVec4 background_pad = ImColor(17, 17, 20, 255);

		inline ImVec4 border = ImColor(25, 26, 30, 255);

		inline ImVec2 size = ImVec2(900, 600);

		inline float rounding = 12.f;

		namespace child {
			inline ImVec4 shadow = ImColor(21, 20, 27, 0);

			inline ImVec4 background = ImColor(19, 18, 25, 255);
			inline ImVec4 background_cap = ImColor(25, 24, 31, 255);
			inline ImVec4 text_name = ImColor(200, 205, 210, 255);
			inline ImVec4 border = ImColor(35, 36, 40, 255);

			inline float rounding = 6.f;
		}

		namespace child_header {
			inline ImVec4 background = ImColor(24, 23, 30, 255);
			inline ImVec4 border = ImColor(35, 36, 40, 255);

			inline float rounding = 6.f;
		}

	}

	namespace tabs {

		inline ImVec4 background_active = ImColor(25, 24, 31, 255);
		inline ImVec4 background_hov = ImColor(21, 20, 27, 255);
		inline ImVec4 background = ImColor(21, 20, 27, 0);

		inline ImVec4 border_active = ImColor(35, 36, 40, 255);
		inline ImVec4 border_inactive = ImColor(35, 36, 40, 0);

		inline float rounding = 3.f;
	}

	namespace text {

		inline ImVec4 text_selected = ImColor(21, 20, 27, 100);
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(93, 91, 105, 255);
		inline ImVec4 text = ImColor(73, 71, 85, 255);

	}

	namespace scrollbar {
		inline ImVec4 border = ImColor(55, 60, 70, 200);

		inline float rounding = 30.f;
	}

	namespace checkbox {
		inline ImVec4 background_hov = ImColor(35, 36, 45, 255);
		inline ImVec4 background = ImColor(30, 31, 40, 255);
		inline ImVec4 border = ImColor(35, 36, 40, 255);

		inline ImVec4 checkmark_active = ImColor(0, 0, 0, 255);
		inline ImVec4 checkmark_inactive = ImColor(0, 0, 0, 0);

		inline float rounding = 3.f;
	}

	namespace slider {

		inline ImVec4 background_hov = ImColor(35, 36, 45, 255);
		inline ImVec4 background = ImColor(30, 31, 40, 255);
		inline ImVec4 border = ImColor(35, 36, 40, 255);
		inline ImVec4 circle = ImColor(45, 46, 55, 255);

	}

	namespace button {

		inline ImVec4 background_hov = ImColor(35, 36, 45, 255);
		inline ImVec4 background = ImColor(30, 31, 40, 255);
		inline ImVec4 border = ImColor(35, 36, 40, 255);

		inline float rounding = 3.f;
	}

	namespace input {

		inline ImVec4 background_hov = ImColor(35, 36, 45, 255);
		inline ImVec4 background = ImColor(30, 31, 40, 255);
		inline ImVec4 border = ImColor(35, 36, 40, 255);

		inline float rounding = 3.f;
	}

	namespace combo {

		inline ImVec4 shadow = ImColor(30, 31, 40, 0);


		inline ImVec4 background_hov = ImColor(35, 36, 45, 255);
		inline ImVec4 background = ImColor(30, 31, 40, 255);
		inline ImVec4 border = ImColor(35, 36, 40, 255);

		inline float rounding = 3.f;
	}

	namespace picker {

		inline ImVec4 background = ImColor(30, 31, 40, 255);
		inline ImVec4 border = ImColor(35, 36, 40, 255);

		inline float rounding = 3.f;
	}

	namespace keybind {

		inline ImVec4 background_hov = ImColor(35, 36, 45, 255);
		inline ImVec4 background = ImColor(30, 31, 40, 255);

		inline float rounding = 3.f;
	}

	namespace alpha_preview {
		inline ImVec4 alpha_bland_one = ImColor(30, 30, 30, 255);
		inline ImVec4 alpha_bland_two = ImColor(55, 55, 55, 255);

	}

	namespace separator {
		inline ImVec4 border = ImColor(35, 36, 40, 255);
	}



}