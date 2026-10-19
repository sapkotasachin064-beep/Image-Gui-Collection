#pragma once

#include "imgui.h"

namespace colors {

    inline ImVec4 accent_color = ImColor(154, 192, 108);
    inline ImVec4 outline_color = ImColor(0, 0, 0);

    namespace tabs {

        inline ImVec4 icon_hovered = ImColor(150, 150, 150);
        inline ImVec4 icon_inactive = ImColor(76, 76, 77);
        inline ImVec4 fill = ImColor(24, 25, 27);
        inline ImVec4 fill_outline = ImColor(5, 5, 5);
        inline ImVec4 shadow_inactive = ImColor(0, 0, 0, 0);

    }

    namespace child {

        inline ImVec4 child_background = ImColor(24, 25, 25);
        inline ImVec4 child_top = ImColor(25, 25, 27);
        inline ImVec4 child_text = ImColor(255, 255, 255);
        inline ImVec4 col_bot_right = ImColor(24, 25, 25);
        inline ImVec4 col_bot_left = ImColor(24, 25, 25);
        inline ImVec4 col_top_right = ImColor(5, 5, 5);
        inline ImVec4 col_top_left = ImColor(5, 5, 5);

    }

    namespace checkbox {

        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
        inline ImVec4 shadow_inactive = ImColor(0, 0, 0, 0);
        inline ImVec4 checkbox_bg_inactive = ImColor(36, 37, 39);
        inline ImVec4 rect = ImColor(5, 5, 5);

    }

    namespace slider {

        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
        inline ImVec4 slider_inactive = ImColor(35, 36, 38);
        inline ImVec4 rect = ImColor(5, 5, 5);

    }

    namespace combo {


        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
        inline ImVec4 combo_bg = ImColor(37, 38, 40);
        inline ImVec4 rect = ImColor(5, 5, 5);
        inline ImVec4 preview_bg = ImColor(19, 20, 22);
    }

    namespace color_picker {

        inline ImVec4 picker_bg = ImColor(21, 22, 24);
        inline ImVec4 picker_rect = ImColor(15, 16, 19);
        inline ImVec4 picker_circle_shadow = ImColor(15, 5, 5);
        inline ImVec4 picker_circle = ImColor(255, 255, 255);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);

    }

    namespace binder {

        inline ImVec4 bind_settings = ImColor(15, 16, 18);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
    }

    namespace button {

        inline ImVec4 button_bg = ImColor(36, 37, 39);
        inline ImVec4 button_gradient = ImColor(22, 23, 25);
        inline ImVec4 rect = ImColor(5, 5, 5);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
    }

    namespace input {

        inline ImVec4 input_bg = ImColor(36, 37, 39);
        inline ImVec4 input_rect = ImColor(5,5,5);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
    }

    namespace preview {

        inline ImVec4 rect = ImColor(40, 40, 42);
        inline ImVec4 name = ImColor(255, 255, 255);
        inline ImVec4 distance = ImColor(255, 255, 255);
        inline ImVec4 head = ImColor(255, 255, 255);
        inline ImVec4 dice = ImColor(255, 255, 255);

    }

    namespace notification {

        inline ImVec4 notifi_bg = ImColor(16, 17, 19);
        inline ImVec4 notify_rect = ImColor(5, 5, 5);
        inline ImVec4 notify_text = ImColor(255, 255, 255);
    }

}

namespace settings {
    inline float checkbox_rounding = 100.f;
}

