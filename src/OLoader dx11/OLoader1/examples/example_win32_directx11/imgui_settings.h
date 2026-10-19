#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "imgui_internal.h"

#include <iostream>

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h> 
#include <conio.h>
#include <Windows.h>

namespace c {

    inline ImColor background(24, 25, 34, 255);
    inline ImColor stroke(27, 28, 37, 255);
    inline ImColor input(24, 25, 34, 230);
    inline ImColor input_icon(92, 138, 255, 255);
    inline ImColor Image_heart(235, 218, 63, 255);
    inline ImColor Image_update(63, 235, 80, 255);
    inline ImColor Image_docs(235, 63, 135, 255);
    inline ImColor Image_save(63, 80, 235, 255);
    inline ImColor Switch(24, 25, 34, 255);
    inline ImColor button(92, 138, 255, 255);
    inline ImColor in_button(92, 138, 255, 195);
    inline ImColor In_Switch(21, 22, 29, 255);
    inline ImColor main_color(92, 138, 255, 255);
    inline ImColor text_activite(255, 255, 255, 255);
    inline ImColor text_grey(255, 255, 255, 220);
    inline ImColor text_grey1(103, 103, 105, 255);
    inline ImColor text_red(255, 92, 92, 255);
    inline ImColor text_version(130, 143, 189, 230);
    inline ImColor text_autch(130, 143, 189, 255);
    inline ImColor switch_text(130, 143, 189, 127);
    inline ImColor rect_main(36, 38, 51, 255);
    inline ImColor red_text(176, 89, 89, 255);
    inline ImColor green_text(101, 176, 89, 255);
    inline ImColor combo(32, 33, 44, 64);
}

namespace c1 {

    namespace other
    {
        inline ImColor background(24, 25, 34, 255);
        inline ImColor stroke(27, 28, 37, 255);
        inline ImColor input(24, 25, 34, 230);
        inline ImColor input_icon(92, 138, 255, 255);
        inline ImColor Image_heart(235, 218, 63, 255);
        inline ImColor Image_update(63, 235, 80, 255);
        inline ImColor Image_docs(235, 63, 135, 255);
        inline ImColor Image_save(63, 80, 235, 255);
        inline ImColor Switch(24, 25, 34, 255);
        inline ImColor button(92, 138, 255, 255);
        inline ImColor in_button(92, 138, 255, 195);
        inline ImColor In_Switch(21, 22, 29, 255);
        inline ImColor main_color(92, 138, 255, 255);
        inline ImColor text_activite(255, 255, 255, 255);
        inline ImColor text_grey(255, 255, 255, 220);
        inline ImColor text_grey1(103, 103, 105, 255);
        inline ImColor text_red(255, 92, 92, 255);
        inline ImColor text_version(130, 143, 189, 230);
        inline ImColor text_autch(130, 143, 189, 255);
        inline ImColor switch_text(130, 143, 189, 127);
        inline ImColor rect_main(36, 38, 51, 255);
        inline ImColor red_text(176, 89, 89, 255);
        inline ImColor green_text(101, 176, 89, 255);
        inline ImColor combo(32, 33, 44, 64);
    }

}


inline int rotation_start_index;
inline void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}

inline ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}


inline void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

inline float anim_speed;
