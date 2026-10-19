#pragma once
#include <iostream>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include "../Fonts/gilroy.h"

#include <map>

int active_tab = 0;

ImFont* _main;
ImFont* _icon;
ImFont* logo;

using namespace ImGui;

namespace LamantineSDK {
    ImVec4 rgb_to_imvec4(float r, float g, float b, float a)
    {
        return ImVec4(r / 255, g / 255, b / 255, a / 255);
    }
    void InitFonts() {
        ImGuiIO& io = ImGui::GetIO();
        _main = io.Fonts->AddFontFromMemoryTTF(&gilroy, sizeof gilroy, 14, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        _icon = io.Fonts->AddFontFromMemoryTTF(&gilroy, sizeof gilroy, 22, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        logo = io.Fonts->AddFontFromMemoryTTF(&gilroy, sizeof gilroy, 52, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }

    void InitStyles() {
        ImGui::GetStyle().WindowRounding = 15.0f;
        ImGui::GetStyle().FrameRounding = 10.f;
        ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
        ImGui::GetStyle().FramePadding = ImVec2(0.0f, 0.0f);
        ImGui::GetStyle().ChildRounding = 0.0f;
        ImGui::GetStyle().WindowBorderSize = 0.0f;
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
        ImGui::GetStyle().PopupBorderSize = 0.0f;
        ImGui::GetStyle().AntiAliasedLinesUseTex = true;
    }

    void ColorStyles() {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImColor(104, 104, 104);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.f);
        colors[ImGuiCol_WindowBg] = ImColor(255, 255, 255);
        colors[ImGuiCol_ChildBg] = ImVec4(1, 1.0f, 1, 0.f);
        colors[ImGuiCol_PopupBg] = ImColor(239, 239, 239);
        colors[ImGuiCol_Border] = ImColor(35, 35, 54);
        colors[ImGuiCol_BorderShadow] = ImColor(35, 35, 54);
        colors[ImGuiCol_FrameBg] = ImVec4(0.239f, 0.239f, 0.349f, 0.5);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.216f, 0.216f, 0.376f, 0.5f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.216f, 0.216f, 0.376f, 0.6f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.75f, 0.01f, 0.94f, 1.f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.39f, 0.00f, 0.63f, 1.f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.39f, 0.00f, 0.63f, 1.f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.063f, 0.349f, 0.851f, 1.f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.325f, 0.451f, 0.839f, 1.f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.325f, 0.451f, 0.839f, 1.f);
        colors[ImGuiCol_Button] = ImColor(10, 10, 10, 25);
        colors[ImGuiCol_ButtonHovered] = ImColor(10, 10, 10, 45);
        colors[ImGuiCol_ButtonActive] = ImColor(10, 10, 10, 70);
        colors[ImGuiCol_Header] = ImColor(54, 54, 79);
        colors[ImGuiCol_HeaderHovered] = ImColor(54, 54, 90);
        colors[ImGuiCol_HeaderActive] = ImColor(54, 54, 90);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 1.f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 1.f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.f);
        colors[ImGuiCol_TextSelectedBg] = ImColor(35, 35, 54);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 1.f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.f);
    }

    bool tabs(const char* tab, int _id)
    {
        ImGuiWindow* wnd = GetCurrentWindow();
        ImGuiID id = wnd->GetID(tab);

        static std::map<ImGuiID, float> alpha_anim;
        auto alpha = alpha_anim.find(id);

        if (alpha == alpha_anim.end())
        {
            alpha_anim.insert({ id, 0.f });
            alpha = alpha_anim.find(id);
        }

        static std::map<ImGuiID, float> alpha_anim2;
        auto alpha2 = alpha_anim2.find(id);

        if (alpha2 == alpha_anim2.end())
        {
            alpha_anim2.insert({ id, 0.f });
            alpha2 = alpha_anim2.find(id);
        }

        ImVec2 pos = GetCursorPos();
        ImVec2 _t = CalcTextSize(tab);

        SetCursorPos(pos);
        if (InvisibleButton(tab, _t))
        {
            active_tab = _id;
        }
        if (_id == active_tab)
        {
            if (alpha->second < 255)
                alpha->second += 15;
            if (alpha2->second < 255)
                alpha2->second += 25;
        }
        else if (IsItemHovered())
        {
            if (alpha->second < 255)
                alpha->second += 15;
        }
        else
        {
            if (alpha->second > 0)
                alpha->second -= 15;
            if (alpha2->second > 0)
                alpha2->second -= 15;
        }

        SetCursorPos(pos);
        TextColored(ImColor(104, 104, 104), tab);

        SetCursorPos(pos);
        TextColored(ImColor(0, 26, 255, (int)alpha->second), tab);
        /*GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - 23, pos.y + 30), ImVec2(pos.x + _t.x + 23, pos.y + _t.y + 13), ImColor(126, 88, 181, (int)alpha2->second));*/
        return false;
    }
}