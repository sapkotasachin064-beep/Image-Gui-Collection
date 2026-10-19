#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/inputtext/inputtext.hpp"
#include "gui/framework/combo/combo.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"

namespace vars
{
	bool page8Check1 = false;
	bool page8Check2 = true;
	bool page8Check3 = false;
	int page8Int = 90;
	float page8Float = 12.5f;
	char page8Text1[128] = "";
	char page8Text2[128] = "";

	float page8Color1[4] = { 0.85f, 0.55f, 0.30f, 1.f };
	float page8Color2[4] = { 0.30f, 0.70f, 0.65f, 1.f };
	float page8Color3[4] = { 0.75f, 0.30f, 0.55f, 1.f };

	const char* page8ItemNames[5] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
	int page8Index = 0;
	int page8RangeMin = 25;
	int page8RangeMax = 75;
}

namespace pages
{
	void RenderPage8() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page8Left", ImVec2(224, 435)))
		{
			ksd::Checkbox("Placeholder 1", &vars::page8Check1);
			ksd::Checkbox("Placeholder 2", &vars::page8Check2);
			ksd::Checkbox("Placeholder 3", &vars::page8Check3);
			ksd::SliderInt("Placeholder 4", &vars::page8Int, 0, 240);
			ksd::SliderFloat("Placeholder 5", &vars::page8Float, 0.f, 25.f, "%.1f");
			ksd::SliderIntRange2("Placeholder 6", &vars::page8RangeMin, &vars::page8RangeMax, 0, 100, 5);
			ksd::button("Placeholder 7", ImVec2(200, 25));
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page8Right", ImVec2(224, 435)))
		{
			ksd::ColorEdit4x3("Placeholder 8", vars::page8Color1, vars::page8Color2, vars::page8Color3);
			ksd::combo("Placeholder 9", &vars::page8Index, vars::page8ItemNames, IM_ARRAYSIZE(vars::page8ItemNames), 5);
			ksd::InputText("Placeholder 10", vars::page8Text1, IM_ARRAYSIZE(vars::page8Text1));
			ksd::InputText("Placeholder 11", vars::page8Text2, IM_ARRAYSIZE(vars::page8Text2));
			ksd::button("Placeholder 12", ImVec2(200, 25));
		}
		ksd::EndChild();
	}
}
