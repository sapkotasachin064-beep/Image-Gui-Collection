#pragma once

#include "imgui_internal.h"
#include "examples/example_win32_directx11/imgui_settings.h"
void CustomStyleColor() // Отрисовка цветов
{
    ImGuiStyle& s = ImGui::GetStyle();
    ImGuiContext& g = *GImGui;

    // s.Colors[ImGuiCol_PopupBg] = ImColor(11, 12, 13, 255);
    s.Colors[ImGuiCol_ChildBg] = ImColor(11, 12, 13, 127);
    s.Colors[ImGuiCol_WindowBg] = ImColor(21, 22, 29, 255);
    s.Colors[ImGuiCol_Border] = ImColor(52, 52, 52, 255);
    s.ChildRounding = 10.f;
    s.WindowRounding = 12.f;
    s.WindowPadding = ImVec2(0, 0);
}

ImFont* Lexend_Medium = nullptr;
ImFont* Lexend_Bold = nullptr;
ImFont* Lexend_Bold_1 = nullptr;
ImFont* Lexend_Medium_1 = nullptr;
ImFont* Lexend_Medium_2 = nullptr;
ImFont* Lexend_Regular = nullptr;
ImFont* Lexend_Regular_1 = nullptr;
ImFont* Lexend_Regular_2 = nullptr;
ImFont* Lexend_Semibold = nullptr;
ImFont* Lexend_Semibold_1 = nullptr;
ImFont* Lexend_Semibold_2 = nullptr;

ID3D11ShaderResourceView* Logotip = nullptr;
ID3D11ShaderResourceView* screen = nullptr;
ID3D11ShaderResourceView* screen1 = nullptr;
ID3D11ShaderResourceView* screen2 = nullptr;
ID3D11ShaderResourceView* screen3 = nullptr;
ID3D11ShaderResourceView* screen4 = nullptr;
ID3D11ShaderResourceView* screen5 = nullptr;
ID3D11ShaderResourceView* screen6 = nullptr;
ID3D11ShaderResourceView* Username = nullptr;

ID3D11ShaderResourceView* logo = nullptr;

ID3D11ShaderResourceView* bg = nullptr;

ID3D11ShaderResourceView* succes = nullptr;

static int page_switch = 0;

char input[64] = { "" };
char input1[64] = { "" };
char input2[64] = { "" };
char input3[64] = { "" };
char input4[64] = { "" };
char input5[64] = { "" };

static int page_inject = 0;

static int timer = 0;

float progress = 0.0f;
float progress6 = 0.0f;
int i = 0;
int i6 = 0;
