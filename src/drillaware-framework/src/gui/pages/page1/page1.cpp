#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/combo/combo.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"

namespace vars
{
	bool page1Check1 = true;
	bool page1Check2 = false;
	bool page1Check3 = false;
	bool page1Check4 = true;
	float page1Value1 = 40.f;
	int page1Value2 = 6;

	const char* page1ItemNames[5] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
	int page1Index = 0;
	bool page1MultiSelection[5] = { true, false, false, false, false };

	float page1Color[4] = { 0.80f, 0.36f, 0.36f, 1.f };
	bool page1Check5 = false;
}

namespace pages
{
	void RenderPage1() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page1Left", ImVec2(224, 435)))
		{
			ksd::Checkbox("Placeholder 1", &vars::page1Check1);
			ksd::Checkbox("Placeholder 2", &vars::page1Check2);
			ksd::Checkbox("Placeholder 3", &vars::page1Check3);
			ksd::SliderFloat("Placeholder 4", &vars::page1Value1, 0.f, 100.f, "%.1f");
			ksd::SliderInt("Placeholder 5", &vars::page1Value2, 0, 24);
			ksd::Checkbox("Placeholder 6", &vars::page1Check4);
			ksd::button("Placeholder 7", ImVec2(200, 25));
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page1Right", ImVec2(224, 435)))
		{
			ksd::combo("Placeholder 8", &vars::page1Index, vars::page1ItemNames, IM_ARRAYSIZE(vars::page1ItemNames), 5);
			ksd::MultiCombo("Placeholder 9", vars::page1MultiSelection, vars::page1ItemNames, IM_ARRAYSIZE(vars::page1ItemNames));
			ksd::ColorEdit4("Placeholder 10", vars::page1Color);
			ksd::Checkbox("Placeholder 11", &vars::page1Check5);
			ksd::button("Placeholder 12", ImVec2(95, 25));
			ImGui::SameLine();
			ksd::button("Placeholder 13", ImVec2(95, 25));
		}
		ksd::EndChild();
	}
}
