#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/gui.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/slider/slider.hpp"
#include "config/config.hpp"

namespace pages
{
	void RenderSettings() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Theme", ImVec2(224, 435)))
		{
			ksd::ColorEdit4("Accent Color", vars::menuAccentColor, 0, &vars::menuAccentRainbow, &vars::menuAccentRainbowSpeed);
			ksd::SliderFloat("Animation Speed", &vars::menuAnimationSpeed, 0.25f, 3.f, "%.2fx");

			if (ksd::button("Reset Theme", ImVec2(200, 25)))
			{
				vars::menuAccentColor[0] = 205.f / 255.f;
				vars::menuAccentColor[1] = 92.f / 255.f;
				vars::menuAccentColor[2] = 92.f / 255.f;
				vars::menuAccentColor[3] = 1.f;
				vars::menuAccentRainbow = false;
				vars::menuAccentRainbowSpeed = 1.f;
				vars::menuAnimationSpeed = 1.f;
			}
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Config", ImVec2(224, 435)))
		{
			static const char* configStatus = "no config action yet";

			if (ksd::button("Save Config", ImVec2(200, 25)))
			{
				configStatus = config::TrySave() ? "config saved" : "save failed";
			}
			if (ksd::button("Load Config", ImVec2(200, 25)))
			{
				configStatus = config::TryLoad() ? "config loaded" : "load failed";
			}

			ImGui::Text("%s", configStatus);
		}
		ksd::EndChild();
	}
}
