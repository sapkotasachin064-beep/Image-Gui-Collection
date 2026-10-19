#pragma once

#include "imgui.h"

namespace style
{
    inline ImVec4 accent = ImColor(240, 110, 30);
    inline ImVec4 accentDim = ImColor(110, 52, 16);
    inline ImVec4 accentBarDark = ImColor(85, 32, 6);
    inline ImVec4 accentBar = ImColor(165, 72, 18);
    inline ImVec4 accentBarBright = ImColor(240, 110, 30);
    inline ImVec4 toggleOn = ImColor(110, 158, 75);

    inline ImVec4 headerBg = ImColor(22, 22, 22);
    inline ImVec4 windowBg = ImColor(31, 31, 31);
    inline ImVec4 panelBg = ImColor(31, 31, 31);
    inline ImVec4 controlBg = ImColor(16, 16, 16);
    inline ImVec4 checkboxBg = ImColor(28, 28, 28);
    inline ImVec4 listRowEven = ImColor(28, 28, 28);
    inline ImVec4 listRowOdd = ImColor(36, 36, 36);

    inline ImVec4 windowBorder = ImColor(13, 13, 13);
    inline ImVec4 stroke = ImColor(62, 62, 62);
    inline ImVec4 strokeHover = ImColor(108, 108, 108);
    inline ImVec4 tabBorder = ImColor(65, 65, 65);

    inline ImVec4 tabHighlight = ImColor(162, 144, 124);
    inline ImVec4 sliderTrack = ImColor(26, 26, 26);
    inline ImVec4 sliderFill = ImColor(240, 110, 30);
    inline ImVec4 sliderKnob = ImColor(155, 155, 155);
    inline ImVec4 bevelDark = ImColor(14, 14, 14);
    inline ImVec4 bevelLight = ImColor(66, 66, 66);

    inline ImVec4 text = ImColor(205, 205, 205);
    inline ImVec4 textStrong = ImColor(255, 255, 255);
    inline ImVec4 textDisabled = ImColor(112, 112, 112);
    inline ImVec4 titleText = ImColor(240, 240, 240);

    inline float scale = 1.f;

    inline float textSize = 11.f;
    inline float titleSize = 10.f;
    inline float titleBarHeight = 18.f;
    inline float tabBarHeight = 15.f;

    inline float Scaled(float value) noexcept
    {
        return value * scale;
    }

    inline void ApplyAccent(const float accentColor[4]) noexcept
    {
        accent = ImVec4(accentColor[0], accentColor[1], accentColor[2], 1.f);
        accentBarBright = accent;
        sliderFill = accent;
        accentBar = ImVec4(accent.x * 0.67f, accent.y * 0.67f, accent.z * 0.67f, 1.f);
        accentDim = ImVec4(accent.x * 0.47f, accent.y * 0.47f, accent.z * 0.47f, 1.f);
        accentBarDark = ImVec4(accent.x * 0.31f, accent.y * 0.31f, accent.z * 0.31f, 1.f);
    }

    inline void ResetToDefault() noexcept
    {
        accent = ImColor(240, 110, 30);
        accentDim = ImColor(110, 52, 16);
        accentBarDark = ImColor(85, 32, 6);
        accentBar = ImColor(165, 72, 18);
        accentBarBright = ImColor(240, 110, 30);
        toggleOn = ImColor(110, 158, 75);
        headerBg = ImColor(22, 22, 22);
        windowBg = ImColor(31, 31, 31);
        panelBg = ImColor(31, 31, 31);
        controlBg = ImColor(16, 16, 16);
        checkboxBg = ImColor(28, 28, 28);
        listRowEven = ImColor(28, 28, 28);
        listRowOdd = ImColor(36, 36, 36);
        windowBorder = ImColor(13, 13, 13);
        stroke = ImColor(62, 62, 62);
        strokeHover = ImColor(108, 108, 108);
        tabBorder = ImColor(65, 65, 65);
        tabHighlight = ImColor(162, 144, 124);
        sliderTrack = ImColor(26, 26, 26);
        sliderFill = ImColor(240, 110, 30);
        sliderKnob = ImColor(155, 155, 155);
        bevelDark = ImColor(14, 14, 14);
        bevelLight = ImColor(66, 66, 66);
        text = ImColor(205, 205, 205);
        textStrong = ImColor(255, 255, 255);
        textDisabled = ImColor(112, 112, 112);
        titleText = ImColor(240, 240, 240);
    }
}
