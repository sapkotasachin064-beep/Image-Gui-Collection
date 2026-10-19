#include "../settings/functions.h"

bool c_widget::checkbox(std::string_view label, bool* callback)
{
    struct checkbox_state
    {
        ImVec4 layout{ clr->c_element.element_inactive };
        ImVec4 circle{ clr->c_checkbox.layout_inactive };
        ImVec4 text{ clr->c_element.text };

        float circle_offset;
        float alpha;
    };

    ImGuiWindow* window = GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label.data());
        
    const ImVec2 pos = window->DC.CursorPos;

    const float w = CalcTextSize(label.data()).x + SCALE(25);
    const float h = SCALE(15);

    const ImRect clickable_rect(pos, pos + ImVec2(w, h));
    const ImRect rect(pos + ImVec2(w - SCALE(20), (h - SCALE(10)) / 2), pos + ImVec2(w, (h + SCALE(10)) / 2));

    ItemSize(clickable_rect, 0);
    if (!ItemAdd(clickable_rect, id))  return false;

    bool hovered, held, pressed = ButtonBehavior(clickable_rect, id, &hovered, &held);

    if (pressed)
        *callback = !(*callback);

    checkbox_state* state = gui->anim_container(&state, id);

    state->layout = ImLerp(state->layout, *callback ? clr->c_checkbox.layout_active : clr->c_checkbox.layout_inactive, gui->fixed_speed(12.f));
    state->circle = ImLerp(state->circle, *callback ? clr->c_other_clr.accent_clr : clr->c_checkbox.circle_inactive, gui->fixed_speed(12.f));

    state->text = ImLerp(state->text, *callback ? clr->c_element.text_active : hovered ? clr->c_element.text_hov : clr->c_element.text, gui->fixed_speed(12.f));

    state->circle_offset = ImLerp(state->circle_offset, *callback ? SCALE(13.f) : SCALE(7.f), gui->fixed_speed(12.f));

    {
        {
            draw->add_rect_filled(window->DrawList, rect.Min, rect.Max, gui->get_clr(state->layout), SCALE(100.f));
            draw->add_circle_filled(window->DrawList, rect.Min + ImVec2(state->circle_offset, SCALE(5.f)), SCALE(7.f), gui->get_clr(state->circle), SCALE(100.f));
        }

        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], clickable_rect.Min, clickable_rect.Max, gui->get_clr(clr->c_element.text_active), label.data(), NULL, NULL, { 0.f, 0.47 });
    }

    return pressed;
}