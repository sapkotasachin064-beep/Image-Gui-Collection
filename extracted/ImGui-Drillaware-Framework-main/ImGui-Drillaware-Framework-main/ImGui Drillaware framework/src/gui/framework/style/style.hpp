#pragma once
#include "imgui.h"

namespace style
{
	inline ImVec4 accent = ImColor(205, 92, 92);

	inline ImVec4 childBg = ImColor(10, 10, 12);
	inline ImVec4 windowBg = ImColor(15, 15, 17);
	inline ImVec4 frameBg = ImColor(24, 24, 28);
	inline ImVec4 popupBg = ImColor(34, 34, 40);
	inline ImVec4 border = ImColor(0, 0, 0, 0);
	inline ImVec4 stroke = ImColor(46, 46, 55);
	inline ImVec4 strokeHover = ImColor(72, 72, 84);
	inline ImVec4 panelDivider = ImColor(32, 32, 38);

	inline ImVec4 textWhite = ImColor(237, 237, 240);
	inline ImVec4 headingText = ImColor(135, 135, 145);

	inline ImVec4 tabTextActive = ImColor(237, 237, 240);
	inline ImVec4 tabTextHover = ImColor(175, 175, 185);
	inline ImVec4 tabTextIdle = ImColor(120, 120, 130);
	inline ImVec4 markerOff = ImColor(0, 0, 0, 0);

	inline ImVec4 checkMarkOn = ImColor(240, 240, 245);
	inline ImVec4 checkMarkHover = ImColor(120, 120, 130);
	inline ImVec4 checkMarkOff = ImColor(0, 0, 0, 0);
	inline ImVec4 checkFrameHover = ImColor(36, 36, 42);
	inline ImVec4 checkFrameIdle = ImColor(28, 28, 33);

	inline ImVec4 sliderTrack = ImColor(22, 22, 27);
	inline ImVec4 sliderKnob = ImColor(237, 237, 240);
	inline ImVec4 sliderTextActive = ImColor(237, 237, 240);
	inline ImVec4 sliderTextHover = ImColor(175, 175, 185);
	inline ImVec4 sliderTextIdle = ImColor(120, 120, 130);

	inline ImVec4 buttonBg = ImColor(24, 24, 28);
	inline ImVec4 buttonBgHover = ImColor(36, 36, 42);
	inline ImVec4 buttonHeld = ImColor(48, 48, 55);
	inline ImVec4 buttonBorder = ImColor(38, 38, 44);
	inline ImVec4 buttonBorderHover = ImColor(56, 56, 64);
	inline ImVec4 buttonText = ImColor(200, 200, 210);

	inline ImVec4 comboBgOpen = ImColor(20, 20, 24);
	inline ImVec4 comboBgHover = ImColor(30, 30, 35);
	inline ImVec4 comboBgIdle = ImColor(24, 24, 28);
	inline ImVec4 comboTextOpen = ImColor(237, 237, 240);
	inline ImVec4 comboTextHover = ImColor(185, 185, 195);
	inline ImVec4 comboTextIdle = ImColor(140, 140, 150);

	inline ImVec4 selectTextOn = accent;
	inline ImVec4 selectTextHover = ImColor(217, 133, 133);
	inline ImVec4 selectTextIdle = ImColor(220, 220, 228);

	inline float headingSize = 16.f;
	inline float textSize = 13.f;
	inline float tabTextSize = 16.f;
	inline float panelTitleSize = 16.f;
	inline float logoSize = 28.f;
	inline float headingTrackingPx = 1.5f;
}
