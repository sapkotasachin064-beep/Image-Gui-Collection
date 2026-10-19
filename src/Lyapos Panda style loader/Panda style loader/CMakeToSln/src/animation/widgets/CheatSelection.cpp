//
// Created by Lyapos on 30.04.2024.
//

#include "CheatSelection.h"

#include <Windows.h>
#include <iostream>
#include <imgui.h>
#include <imgui_internal.h>

#include "animation/imgui_animation_manager.h"
#include "animation/linear/linear_button_color_animation.h"
#include "animation/linear/linear_color_animation.h"
#include "render/RenderCheckMarkRotated.h"
#include "widgets/TextWithDescription.h"
#include "widgets/ColoredText.h"
#include "widgets/CenteredText.h"
#include "dynamic_size/imgui_dynamic_size.h"
#include "fonts/icomoon_icons.h"

#include "animation/linear/linear_with_target_2d_animation.h"
#include "animation/linear/linear_with_target_animation.h"

bool ImGui::CheatSelection(const char *label, const char *subscription, const char *update, ImTextureID logotype, bool active) {
	auto *window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	auto &g = *GImGui;
	const auto &style = g.Style;
	const auto id = window->GetID(label);
	const auto label_size = CalcTextSize(label, nullptr, true);
	const char *label_end = FindRenderedTextEnd(label);

	const auto pos = window->DC.CursorPos;
	const ImRect total_bb(pos, pos + DynImVec2(640, 130));
	const ImRect button_bb(total_bb.Min + DynImVec2(580, 45), total_bb.Max - DynImVec2(20, 45));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

	bool hovered;
	bool held;
	bool pressed = ButtonBehavior(button_bb, id, &hovered, &held);

	ImColor buttonColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
	ImColor bgColor = GetColorU32(ImGuiCol_Header);

    auto *const p_button_col = imgui_animation_manager::get_instance()->get_animation<linear_color_animation>(
            window, id, "p_button_col"_sh, 2.3f);

    p_button_col->set_target_color(hovered ? ImColor(255, 255, 255, 55) : ImColor(255, 255, 255, 0));


    window->DrawList->AddImageRounded(logotype, total_bb.Min, total_bb.Max,
                               ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), style.FrameRounding, 0);

    window->DrawList->AddRectFilled(total_bb.Min - DynImVec2(0.5f, 0.5f), total_bb.Max + DynImVec2(0.5f, 0.5f), bgColor, style.FrameRounding, 0);

    window->DrawList->AddRectFilled(button_bb.Min - DynImVec2(0.5f, 0.5f), button_bb.Max + DynImVec2(0.5f, 0.5f), buttonColor, style.FrameRounding, 0);

    window->DrawList->AddRectFilled(button_bb.Min - DynImVec2(0.5f, 0.5f), button_bb.Max + DynImVec2(0.5f, 0.5f), p_button_col->get_current_u32_color(), style.FrameRounding, 0);

    window->DrawList->AddText(button_bb.GetCenter() - CalcTextSize(ICON_MATERIAL_SYMBOLS_NOT_STARTED_ROUNDED) / 2, ImColor(255, 255, 255, 255), ICON_MATERIAL_SYMBOLS_NOT_STARTED_ROUNDED);

    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();;

    ImGui::SetCursorScreenPos(total_bb.Min + DynImVec2(20.f, 49.f));
    ImGui::TextWithDescription(label);

    ImGui::SetCursorScreenPos(total_bb.Min + DynImVec2(225.f, 50.f));
    ImGui::TextWithDescription(subscription);

    ImGui::SetCursorScreenPos(total_bb.Min + DynImVec2(385.f, 50.f));
    ImGui::TextWithDescription(update);

    ImGui::SetCursorScreenPos(cursor_pos);

    return pressed;
}

int rotation_start_index;
void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}


void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}


struct spoof_button_state {
    int status;
    std::string text;
    float timer, rotation;
    float timer_temp;
    bool loading_state;
};

int ImGui::SpoofButton(const char *label, bool succes)
{
    auto *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    auto &g = *GImGui;
    const auto &style = g.Style;
    const auto id = window->GetID(label);
    const auto label_size = CalcTextSize(label, nullptr, true);
    const char *label_end = FindRenderedTextEnd(label);

    const auto pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + DynImVec2(227, 40));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    static std::map<ImGuiID, spoof_button_state> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, spoof_button_state() });
        it_anim = anim.find(id);
    }

    bool hovered;
    bool held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    if(pressed) {
        it_anim->second.loading_state = true;
        it_anim->second.status = 1;
    }
    ImColor buttonColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

    auto *const p_text_col = imgui_animation_manager::get_instance()->get_animation<linear_color_animation>(
            window, id, "spoof_text_col"_sh, 1.8f);

    auto *const p_button_col = imgui_animation_manager::get_instance()->get_animation<linear_color_animation>(
            window, id, "p_button_col"_sh, 2.3f);

    if(succes)
    p_button_col->set_target_color(it_anim->second.status == 0 ? buttonColor : it_anim->second.status == 1 ? buttonColor.SetAlpha(0.15f) : ImColor(65, 78, 154, 38));
    else
        p_button_col->set_target_color(it_anim->second.status == 0 ? buttonColor : it_anim->second.status == 1 ? buttonColor.SetAlpha(0.15f) : ImColor(65, 78, 154, 38));

    if(it_anim->second.timer_temp > 150.f && it_anim->second.timer_temp < 250.f)
        it_anim->second.timer = 2;
    if(it_anim->second.timer_temp > 250.f)
        it_anim->second.timer = 3;

    if(it_anim->second.loading_state) {
        it_anim->second.timer_temp += ImGui::GetIO().DeltaTime * 30.f;

        if (it_anim->second.timer == 1)
            it_anim->second.status = 1;
        else if (it_anim->second.timer == 2)
            it_anim->second.status = succes ? 2 : 3;
        else if (it_anim->second.timer == 3) {
            it_anim->second.status = 0;
            it_anim->second.timer = 0;
            it_anim->second.timer_temp = 0;
            it_anim->second.loading_state = false;
        } // reset

        if (it_anim->second.status == 0)
            it_anim->second.text = label;
        else if (it_anim->second.status == 1)
            it_anim->second.text = "1";
        else if (it_anim->second.status == 3)
            it_anim->second.text = "1";
        else if (it_anim->second.status == 2)
            it_anim->second.text = "1";
    }
    else
        it_anim->second.text = label;

    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, p_button_col->get_current_u32_color(), style.FrameRounding, 0);

    if(it_anim->second.status == 1) {
        ImRotateStart();
        it_anim->second.rotation += ImGui::GetIO().DeltaTime * 10.f;
    }

    window->DrawList->AddText(total_bb.GetCenter() - CalcTextSize(it_anim->second.text.c_str()) / 2, ImColor(1.f, 1.f, 1.f, 1.f),  it_anim->second.text.c_str());

    if(it_anim->second.status == 1)
        ImRotateEnd(IM_PI / 2 + it_anim->second.rotation);

    return it_anim->second.status;
}