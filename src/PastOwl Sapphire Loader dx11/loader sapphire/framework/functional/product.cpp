#include "../settings/functions.h"
#include "../shader/blur.hpp"

bool c_widget::product(ImTextureID texture, std::string_view label, std::string_view update, product_status type)
{
    struct c_product
    {
        ImVec4 fade_background;
        ImVec2 button_offset;

        float offset_stauts, offset_icons;
        float alpha;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, pos + SCALE(240, 130));
    const ImRect info_bar(pos + SCALE(0, 70), pos + SCALE(240, 115));

    ImVec2 button_padding{ 15, 15 };
    ImVec2 status_padding{ 10, 10 };
    ImVec2 color_padding{ 5, 5 };

    ImColor status_color =
        type == safe_for_use ? gui->get_clr({ 103 / 255.f, 255 / 255.f, 100 / 255.f, 255 / 255.f }) :
        type == at_own_risk ? gui->get_clr({ 255 / 255.f, 150 / 255.f, 100 / 255.f, 255 / 255.f }) :
        type == on_update ? gui->get_clr({ 100 / 255.f, 160 / 255.f, 255 / 255.f, 255 / 255.f }) :
        type == totally_unsafe ? gui->get_clr({ 255 / 255.f, 100 / 255.f, 100 / 255.f, 255 / 255.f }) : gui->get_clr({0, 0, 0, 0});

    ImGui::ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_product* state{ gui->anim_container(&state, id) };

    state->fade_background = ImLerp(state->fade_background, IsItemActive() || hovered ? clr->c_other_clr.accent_clr : clr->c_other_clr.accent_fade_clr, gui->fixed_speed(set->c_product.speed_animation));
    state->button_offset = ImLerp(state->button_offset, IsItemActive() || hovered ? SCALE(2, 2) : SCALE(0, 0), gui->fixed_speed(set->c_product.speed_animation));

    state->offset_stauts = ImLerp(state->offset_stauts, IsItemActive() || hovered ? CalcTextSize(label.data()).x + SCALE(21 + (status_padding.x * 2)) : SCALE(35.f), gui->fixed_speed(set->c_product.speed_animation));
    state->alpha = ImLerp(state->alpha, IsItemActive() || hovered ? 1.f : 0.f, gui->fixed_speed(set->c_product.speed_animation));

    draw->add_image_rounded(window->DrawList, texture, rect.Min, rect.Max, { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.white_clr), SCALE(set->c_product.rounding));

    {
        draw->add_rect_filled(window->DrawList, rect.Min + SCALE(status_padding), rect.Min + ImVec2(state->offset_stauts, SCALE(21 + status_padding.y)), gui->get_clr(clr->c_product.status_bar, state->alpha), SCALE(100.f));

        draw->push_clip_rect(window->DrawList, rect.Min + SCALE(status_padding), rect.Min + ImVec2(state->offset_stauts, SCALE(21 + status_padding.y)), true);
        {

            draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min + SCALE(15, 0) + SCALE(status_padding), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(22 + (status_padding.x * 2)), SCALE(21 + status_padding.y)), gui->get_clr(clr->c_other_clr.white_clr, state->alpha), label.data(), NULL, NULL, { 0.5, 0.5 });

        }
        draw->pop_clip_rect(window->DrawList);

    }

    draw->add_circle_filled(window->DrawList, rect.Min + SCALE(status_padding * 2.05f), SCALE(10.4f), gui->get_clr(clr->c_product.status_bar, 0.8f - state->alpha), SCALE(100.f));

    draw->add_circle_filled(window->DrawList, rect.Min + SCALE(status_padding * 2.05f), SCALE(4.f), gui->get_clr(status_color), SCALE(100.f));

    {
        draw->render_text(window->DrawList, set->c_font.icons[0], info_bar.Min + SCALE(button_padding), info_bar.Max, gui->get_clr(clr->c_other_clr.accent_clr), "L", NULL, NULL, { 0.0, 0.0 });
        draw->render_text(window->DrawList, set->c_font.poppins_medium[0], info_bar.Min + SCALE(button_padding) + SCALE(20, 0), info_bar.Max, gui->get_clr(clr->c_element.text_active), "Last update", NULL, NULL, {0.0, 0.0});

        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], info_bar.Min + SCALE(button_padding), info_bar.Max, gui->get_clr(clr->c_element.text), update.data(), NULL, NULL, {0.0, 1.0});
    }

    state->offset_icons = easing->easing_in_out(id, IsItemActive() || hovered, 65, 0, gui->fixed_speed(set->c_product.speed_animation * 3), back);

    {
        draw->fade_rect_filled(window->DrawList, rect.Max - (SCALE(button_padding) + SCALE(30, 30)) - state->button_offset, rect.Max - SCALE(button_padding) + state->button_offset, gui->get_clr(state->fade_background), gui->get_clr(clr->c_other_clr.accent_clr), horizontally, SCALE(set->c_product.button_rounding));

        draw->push_clip_rect(window->DrawList, rect.Max - (SCALE(button_padding) + SCALE(30, 30)), rect.Max - SCALE(button_padding), true);
        {

            draw->render_text(window->DrawList, set->c_font.icons[1], rect.Max - (SCALE(button_padding) + SCALE(30 - (65 - state->offset_icons), 30)), rect.Max - SCALE(button_padding), gui->get_clr(clr->c_other_clr.black_clr), "C", NULL, NULL, { 0.5, 0.5 });
            draw->render_text(window->DrawList, set->c_font.icons[1], rect.Max - (SCALE(button_padding) + SCALE(30 + state->offset_icons, 30)), rect.Max - SCALE(button_padding), gui->get_clr(clr->c_other_clr.black_clr), "C", NULL, NULL, { 0.5, 0.5 });

        }
        draw->pop_clip_rect(window->DrawList);

    }

    return pressed;
}

bool c_widget::product_info(std::string_view update, std::string_view status)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(update.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + ImVec2(GetContentRegionAvail().x, SCALE(32)));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    {
        draw->render_text(window->DrawList, set->c_font.icons[0], rect.Min, rect.Max, gui->get_clr(clr->c_other_clr.accent_clr), "L", NULL, NULL, { 0.0, 0.0 }, NULL);
        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min + SCALE(25, 0), rect.Max, gui->get_clr(clr->c_element.text_active), "UPDATE", NULL, NULL, { 0.0, 0.0 }, NULL);

        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min, rect.Max, gui->get_clr(clr->c_element.text), update.data(), NULL, NULL, { 0.0, 1.0 }, NULL);
    }

    {
        draw->render_text(window->DrawList, set->c_font.icons[0], rect.Min, rect.Max, gui->get_clr(clr->c_other_clr.accent_clr), "K", NULL, NULL, { 1.0, 0.0 }, NULL);
        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min, rect.Max - SCALE(25, 0), gui->get_clr(clr->c_element.text_active), "STATUS", NULL, NULL, {1.0, 0.0}, NULL);

        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min, rect.Max, gui->get_clr(clr->c_element.text), status.data(), NULL, NULL, { 1.0, 1.0 }, NULL);
    }

    return pressed;
}