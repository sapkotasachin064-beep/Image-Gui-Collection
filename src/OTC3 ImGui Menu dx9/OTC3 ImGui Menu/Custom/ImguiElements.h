#pragma once
#include <iostream>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include "../Fonts/Icons.h"

int active_tab = 0;
int active_subtab = 0;

ImFont* Logo;
ImFont* Tabs;
ImFont* Icons;
ImFont* Elements;
ImFont* Subtabs;

namespace LamantineSDK {
    ImVec4 rgb_to_imvec4(float r, float g, float b, float a)
    {
        return ImVec4(r / 255, g / 255, b / 255, a / 255);
    }
    void InitFonts() {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguisb.ttf", 20);
        Logo = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuib.ttf", 38);
        Tabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuib.ttf", 20);
        Elements = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguisb.ttf", 17);
        Subtabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguisb.ttf", 16);
        Icons = io.Fonts->AddFontFromMemoryTTF(&IconFont, sizeof IconFont, 28, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }

    void InitStyles() {
        ImGui::GetStyle().WindowRounding = 0.0f;
        ImGui::GetStyle().FrameRounding = 2.5f;
        ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
        ImGui::GetStyle().FramePadding = ImVec2(0.0f, 0.0f);
        ImGui::GetStyle().ChildRounding = 0.0f;
        ImGui::GetStyle().WindowBorderSize = 0.0f;
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
        ImGui::GetStyle().PopupBorderSize = 0.0f;
    }

    void ColorStyles() {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.f);
        colors[ImGuiCol_WindowBg] = ImColor(51, 68, 88);
        colors[ImGuiCol_ChildBg] = ImVec4(0.39f, 0.00f, 0.63f, 0.f);
        colors[ImGuiCol_PopupBg] = ImColor(35, 35, 54);
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
        colors[ImGuiCol_Button] = ImColor(1, 1, 1, 0);
        colors[ImGuiCol_ButtonHovered] = ImColor(1, 1, 1, 45);
        colors[ImGuiCol_ButtonActive] = ImColor(1, 1, 1, 55);
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
    void Tab(const char* label, int item_id) {
        ImVec2 pos = ImGui::GetCursorPos();
        if (active_tab == item_id)
            ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + 95, pos.y + 25), ImColor(1, 1, 1, 85), 2.5f);
        ImGui::SetCursorPos(pos);
        if (ImGui::Button(label, ImVec2(95, 25)))
            active_tab = item_id;
    }
    void SubTab(const char* label, const char* icon, int _id) {
        ImVec2 pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(pos);
        if (ImGui::InvisibleButton(label, ImGui::CalcTextSize(label)))
            active_subtab = _id;
        if (active_subtab == _id) {
            ImGui::SetCursorPos(pos);
            ImGui::PushFont(Subtabs);
            ImGui::TextColored(ImColor(255, 255, 255), label);
            ImGui::SetCursorPos(ImVec2(pos.x + ImGui::CalcTextSize(label).x / 2 - ImGui::CalcTextSize(icon).x - ImGui::CalcTextSize(icon).x, pos.y - 25));
            ImGui::PushFont(Icons);
            ImGui::TextColored(ImColor(255, 255, 255), icon);
            ImGui::PushFont(Subtabs);
        }
        else if (ImGui::IsItemHovered()) {
            ImGui::SetCursorPos(pos);
            ImGui::PushFont(Subtabs);
            ImGui::TextColored(ImColor(255, 255, 255), label);
            ImGui::SetCursorPos(ImVec2(pos.x + ImGui::CalcTextSize(label).x / 2 - ImGui::CalcTextSize(icon).x - ImGui::CalcTextSize(icon).x, pos.y - 25));
            ImGui::PushFont(Icons);
            ImGui::TextColored(ImColor(255, 255, 255), icon);
            ImGui::PushFont(Subtabs);
        }
        else {
            ImGui::SetCursorPos(pos);
            ImGui::PushFont(Subtabs);
            ImGui::TextColored(ImColor(105, 111, 127), label);
            ImGui::SetCursorPos(ImVec2(pos.x + ImGui::CalcTextSize(label).x / 2 - ImGui::CalcTextSize(icon).x * 2, pos.y - 25));
            ImGui::PushFont(Icons);
            ImGui::TextColored(ImColor(105, 111, 127), icon);
            ImGui::PushFont(Subtabs);
        }
    }
    void Child(const char* label, ImVec2 size) {
        ImVec2 pos = ImGui::GetCursorPos();
        ImVec2 textpos = ImVec2(pos.x + ImGui::CalcTextSize(label).x / 2 + size.x / 2, pos.y);
        ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(62, 79, 99), 4);
        ImGui::GetWindowDrawList()->AddLine(pos, ImVec2(pos.x + size.x, pos.y), ImColor(255, 205, 126), 2);
        ImGui::SetCursorPos(ImVec2(pos.x + size.x / 2 - ImGui::CalcTextSize(label).x / 2, pos.y - 10));
        ImGui::PushFont(Elements);
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(textpos.x + 10, textpos.y), ImVec2(textpos.x - 10 - ImGui::CalcTextSize(label).x, pos.y - 5), ImColor(51, 68, 88), 4);
        ImGui::Text(label);
    }
}