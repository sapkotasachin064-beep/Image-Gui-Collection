#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/combo/combo.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"

namespace vars
{
	float page4Value1 = 75.f;
	float page4Value2 = 0.4f;
	int page4Value3 = 15;
	bool page4Check1 = true;
	bool page4Check2 = false;
	bool page4Check3 = false;

	float page4Color1[4] = { 0.45f, 0.75f, 0.45f, 1.f };
	float page4Color2[4] = { 0.85f, 0.55f, 0.30f, 1.f };

	const char* page4ItemNames[4] = { "Item 1", "Item 2", "Item 3", "Item 4" };
	int page4Index = 1;
}

namespace pages
{
	void RenderPage4() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page4Left", ImVec2(224, 435)))
		{
			ksd::button("Placeholder 1", ImVec2(95, 25));
			ImGui::SameLine();
			ksd::button("Placeholder 2", ImVec2(95, 25));
			ksd::button("Placeholder 3", ImVec2(95, 25));
			ImGui::SameLine();
			ksd::button("Placeholder 4", ImVec2(95, 25));
			ksd::button("Placeholder 5", ImVec2(95, 25));
			ImGui::SameLine();
			ksd::button("Placeholder 6", ImVec2(95, 25));
			ksd::button("Placeholder 7", ImVec2(200, 25));
			ksd::SliderFloat("Placeholder 8", &vars::page4Value1, 0.f, 100.f, "%.1f");
			ksd::SliderInt("Placeholder 9", &vars::page4Value3, 0, 60);
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page4Right", ImVec2(224, 435)))
		{
			ksd::combo("Placeholder 10", &vars::page4Index, vars::page4ItemNames, IM_ARRAYSIZE(vars::page4ItemNames), 4);
			ksd::SliderFloat("Placeholder 11", &vars::page4Value2, 0.f, 1.f, "%.2f");
			ksd::Checkbox("Placeholder 12", &vars::page4Check1);
			ksd::Checkbox("Placeholder 13", &vars::page4Check2);
			ksd::Checkbox("Placeholder 14", &vars::page4Check3);
			ksd::ColorEdit4x2("Placeholder 15", vars::page4Color1, vars::page4Color2);
		}
		ksd::EndChild();
	}
}
