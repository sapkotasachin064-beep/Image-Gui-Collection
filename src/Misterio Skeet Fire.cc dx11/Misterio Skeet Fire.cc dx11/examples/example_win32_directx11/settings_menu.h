#pragma once
#pragma once

#include "widgets_colors.h"

namespace settings {

    inline ImVec2 size_menu = ImVec2(700, 520);
    inline ImVec2 size_watermark = ImVec2(400, 35);
    inline ImVec2 size_preview = ImVec2(300, 400);
    inline ImVec2 item_spacing = ImVec2(0, 0);
    inline ImVec2 item_padding = ImVec2(0, 0);

    const char* cheat_name = "FIRE";
    const char* developer = "Misterio";
    const char* ping = "68ms";
    const char* fps = "190fps";
    const char* time = "10:10am";

}

namespace misc {

    int tab_count, active_tab_count = 0;

    float anim_tab = 0;

    int tab_width = 85;

    float child_add, alpha_child = 0;

}

namespace menu {

    inline ImVec4 window_bg = ImColor(18, 19, 21);
    inline ImVec4 border = ImColor(0, 0, 0, 0);
    inline ImVec4 border_shadow = ImColor(0, 0, 0, 0);
    inline ImVec4 general_child = ImColor(23, 23, 25);
    inline ImVec4 scrollbar = ImColor(30, 31, 33);
    inline ImVec4 scrollbar_hovered = ImColor(30, 31, 33);
    inline ImVec4 scrollbar_bg = ImColor(18, 19, 21);
    inline ImVec4 watermark_bg = ImColor(17, 18, 20);
    inline ImVec4 watermark_border = ImColor(17, 18, 20, 0);
    inline ImVec4 watermark_filled = ImColor(27, 25, 28);
    inline ImVec4 watermark_name = ImColor(255, 255, 255);
    inline ImVec4 line = ImColor(23, 24, 26);
    inline ImVec4 text = ImColor(140, 141, 143);
    inline ImVec4 line2 = ImColor(12, 13, 15);
    inline float window_rounding = 3.f;
    inline float scroll_size = 10.f;

}

namespace pictures {

    inline ID3D11ShaderResourceView* bg_img = nullptr;
    inline ID3D11ShaderResourceView* aim_img = nullptr;
    inline ID3D11ShaderResourceView* aim_small_img = nullptr;
    inline ID3D11ShaderResourceView* misc_img = nullptr;
    inline ID3D11ShaderResourceView* visual_img = nullptr;
    inline ID3D11ShaderResourceView* world_img = nullptr;
    inline ID3D11ShaderResourceView* settings_img = nullptr;
    inline ID3D11ShaderResourceView* pen_img = nullptr;
    inline ID3D11ShaderResourceView* logo_img = nullptr;
    inline ID3D11ShaderResourceView* keyboard_img = nullptr;
    inline ID3D11ShaderResourceView* input_img = nullptr;
    inline ID3D11ShaderResourceView* wat_logo_img = nullptr;
    inline ID3D11ShaderResourceView* fps_img = nullptr;
    inline ID3D11ShaderResourceView* player_img = nullptr;
    inline ID3D11ShaderResourceView* time_img = nullptr;

}

namespace fonts {

    ImFont* sans_font;

    ImFont* sans_font_small;

    ImFont* weapon_font;
}

namespace features {

    bool check1, check2, check3, check4, check5, check6 = true, check7 = true;

    bool check8, check9, check10, check11, check12, check13;

    int sliderint, sliderint2, sliderint3, sliderint4;

    int selectedItem, selected = 0;

    const char* items[]{ "Value", "Random" };

    const char* items_count[]{ "Combo1", "Combo2", "Combo3", "Combo4" };

    inline ImVec4 fov_color = ImColor(60, 157, 173);

    int key, var = 0;

    int key2, var2 = 0;

    int key3, var3 = 0;

    char input[64] = { "" };

    static bool multi[5] = { false, true, true, false, true };

    const char* multi_items[5] = { "Head", "Chest", "Stromatch", "Body", "Legs" };

    static bool multi_esp[7] = { false, false, false, false, false, false, false };

    const char* multi_preview[7] = { "Box", "Health", "Armor", "Nickname", "Distance", "Weapon", "Skeleton" };

    bool esp_perview;

    float preview_alpha;
}

void ShadowText(ImFont* font, const char* text, ImVec2 pos, ImVec2 rel, ImColor color, bool outline, float size) {
    float dpiscale = 1.50f;
    float relx;
    float rely;
    switch ((int)rel.x) {
    case 0:
        relx = (ImGui::CalcTextSize(text).x / (19 * dpiscale) * (size * dpiscale)) / 2;
        break;
    case 1:
        relx = 0;
        break;
    case -1:
        relx = (ImGui::CalcTextSize(text).x / (19 * dpiscale) * (size * dpiscale));
        break;
    }
    switch ((int)rel.y) {
    case 0:
        rely = (ImGui::CalcTextSize(text).y / (22 * dpiscale) * (size * dpiscale)) / 2;
        break;
    case 1:
        rely = 0;
        break;
    case -1:
        rely = (ImGui::CalcTextSize(text).y / (22 * dpiscale) * (size * dpiscale));
        break;
    }
    if (outline) {
        ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx - 1, pos.y - rely - 1), ImColor(0, 0, 0), text);
        ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx + 1, pos.y - rely - 1), ImColor(0, 0, 0), text);
        ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx - 1, pos.y - rely + 1), ImColor(0, 0, 0), text);
        ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx + 1, pos.y - rely + 1), ImColor(0, 0, 0), text);
        ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx, pos.y - rely), color, text);
    }
}
