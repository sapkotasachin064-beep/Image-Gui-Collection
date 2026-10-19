#include "../settings/functions.h"

bool c_widget::inject_button(std::string_view label, const ImVec2& size)
{
    struct c_button
    {
        ImVec4 invalid_account;
        bool invalid_anim = false;
        float invalid_timer = 0;

        float radius = 0;
        float circle_alpha = 0;
        bool clicked = false;

        float spacing;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + size);

    ImGui::ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_button* state{ gui->anim_container(&state, id) };

    if (pressed && (func->c_sign.login != func->c_sign.login_success || func->c_sign.password != func->c_sign.password_success)) func->c_sign.invalid_account = true;

    if (func->c_sign.invalid_account) state->invalid_timer += 0.2f * gui->fixed_speed(20.f);

    if (state->invalid_timer > 5.f)
    {
        func->c_sign.invalid_account = false;
        state->invalid_timer = 0.f;
    }

    if (pressed) state->clicked = true;

    state->spacing = easing->easing_in_out(id, IsItemActive() || hovered, SCALE(15.f), SCALE(30.f), gui->fixed_speed(set->c_button.speed_animation), elastic);

    state->radius = ImClamp(state->radius + (gui->fixed_speed(rect.GetWidth() * set->c_button.speed_animation / 2) * (state->clicked ? 1.f : -1.f)), 0.f, rect.GetWidth() / 2 + SCALE(5));
    state->circle_alpha = ImClamp(state->circle_alpha + (gui->fixed_speed(2.f) * (state->radius > rect.GetWidth() / 2 - 1 ? 1.f : -1.f)), 0.f, 1.f);


    if (state->circle_alpha > 0.9f)
    {
        state->radius = 0.f;
        state->circle_alpha = 0.f;
        state->clicked = false;
    }

    draw->add_rect_filled(window->DrawList, rect.Min, rect.Max, gui->get_clr(clr->c_other_clr.accent_clr), set->c_button.rounding);

    draw->push_clip_rect(window->DrawList, rect.Min, rect.Max, true);
    {

        draw->add_circle_filled(window->DrawList, rect.GetCenter(), state->radius, gui->get_clr({ 0.f, 0.f, 0.f, 0.4f * (1.f - state->circle_alpha) }));

    }
    draw->pop_clip_rect(window->DrawList);

    draw->render_text(window->DrawList, set->c_font.poppins_medium[0], rect.Min, rect.Max - ImVec2(CalcTextSize(label.data()).x / 2 + SCALE(15), 0), gui->get_clr(clr->c_other_clr.black_clr), label.data(), NULL, NULL, {0.5, 0.5}, NULL);
    draw->render_text(window->DrawList, set->c_font.icons[1], rect.Min + ImVec2(CalcTextSize(label.data()).x / 2 + state->spacing, 0), rect.Max, gui->get_clr(clr->c_other_clr.black_clr), "C", NULL, NULL, {0.5, 0.50}, NULL);

    return pressed;
}

bool c_widget::license_button(std::string_view label, const ImVec2& size)
{
    struct c_button
    {
        ImVec4 invalid_license;
        bool invalid_anim = false;
        float invalid_timer = 0;

        float radius = 0;
        float circle_alpha = 0;
        bool clicked = false;

        float spacing;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + size);

    ImGui::ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_button* state{ gui->anim_container(&state, id) };

    if (pressed && (func->c_inject.license != func->c_inject.license_success))
        func->c_inject.invalid_license = true;

    if (func->c_inject.invalid_license) state->invalid_timer += 0.2f * gui->fixed_speed(20.f);

    if (state->invalid_timer > 5.f)
    {
        func->c_inject.invalid_license = false;
        state->invalid_timer = 0.f;
    }

    if (pressed) state->clicked = true;

    state->spacing = easing->easing_in_out(id, IsItemActive() || hovered, SCALE(15.f), SCALE(30.f), gui->fixed_speed(set->c_button.speed_animation), elastic);

    state->radius = ImClamp(state->radius + (gui->fixed_speed(rect.GetWidth() * set->c_button.speed_animation / 2) * (state->clicked ? 1.f : -1.f)), 0.f, rect.GetWidth() / 2 + SCALE(5));
    state->circle_alpha = ImClamp(state->circle_alpha + (gui->fixed_speed(2.f) * (state->radius > rect.GetWidth() / 2 - 1 ? 1.f : -1.f)), 0.f, 1.f);

    if (state->circle_alpha > 0.9f)
    {
        state->radius = 0.f;
        state->circle_alpha = 0.f;
        state->clicked = false;
    }

    draw->add_rect_filled(window->DrawList, rect.Min, rect.Max, gui->get_clr(clr->c_other_clr.accent_clr), set->c_button.rounding);

    draw->push_clip_rect(window->DrawList, rect.Min, rect.Max, true);
    {

        draw->add_circle_filled(window->DrawList, rect.GetCenter(), state->radius, gui->get_clr({ 0.f, 0.f, 0.f, 0.4f * (1.f - state->circle_alpha) }));

    }
    draw->pop_clip_rect(window->DrawList);

    draw->render_text(window->DrawList, set->c_font.poppins_medium[0], rect.Min, rect.Max - ImVec2(CalcTextSize(label.data()).x / 2 + SCALE(15), 0), gui->get_clr(clr->c_other_clr.black_clr), label.data(), NULL, NULL, {0.5, 0.5}, NULL);
    draw->render_text(window->DrawList, set->c_font.icons[1], rect.Min + ImVec2(CalcTextSize(label.data()).x / 2 + state->spacing, 0), rect.Max, gui->get_clr(clr->c_other_clr.black_clr), "C", NULL, NULL, { 0.5, 0.50 }, NULL);

    return pressed;
}

bool c_widget::ico_button(ImFont* font, std::string_view icon, const ImVec2& size)
{
    struct c_button
    {
        ImVec4 text = clr->c_element.text_active;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(icon.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + SCALE(size));

    ImGui::ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_button* state{ gui->anim_container(&state, id) };
    state->text = ImLerp(state->text, IsItemActive() || hovered ? clr->c_element.text_active : clr->c_other_clr.accent_clr, gui->fixed_speed(set->c_button.speed_animation));

    draw->render_text(window->DrawList, font, rect.Min, rect.Max, gui->get_clr(state->text), icon.data(), NULL, NULL, { 0.5, 0.5 }, NULL);

    return pressed;
}