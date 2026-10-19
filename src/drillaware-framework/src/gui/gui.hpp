#pragma once
#include "imgui.h"

namespace vars
{
	inline float menuAccentColor[4] = { 205.f / 255.f, 92.f / 255.f, 92.f / 255.f, 1.f };
	inline bool menuAccentRainbow = false;
	inline float menuAccentRainbowSpeed = 1.f;
	inline float menuAnimationSpeed = 1.f;
}

namespace gui
{
	inline bool isOpen = true;
	inline int activeTab = 0;

	inline constexpr float menuWidth = 620.f;
	inline constexpr float menuHeight = 470.f;
	inline constexpr float navWidth = 140.f;
	inline constexpr float titleBarHeight = 50.f;

	void Render() noexcept;
}
