#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/combo/combo.hpp"
#include "gui/framework/inputtext/inputtext.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"

namespace vars
{
	const char* page5ItemNames[6] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6" };
	int page5Index = 2;
	bool page5MultiSelection[6] = { false, false, true, false, false, false };

	char page5Text1[128] = "";
	char page5Text2[128] = "";
	bool page5Check1 = false;
	bool page5Check2 = true;

	float page5RangeMin = 0.2f;
	float page5RangeMax = 0.8f;
	int page5IntRangeMin = 4;
	int page5IntRangeMax = 18;
	int page5Int = 12;

	float page5Color1[4] = { 0.36f, 0.55f, 0.80f, 1.f };
	float page5Color2[4] = { 0.90f, 0.72f, 0.35f, 1.f };
}

namespace pages
{
	void RenderPage5() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page5Left", ImVec2(224, 435)))
		{
			ksd::combo("Placeholder 1", &vars::page5Index, vars::page5ItemNames, IM_ARRAYSIZE(vars::page5ItemNames), 5);
			ksd::InputText("Placeholder 2", vars::page5Text1, IM_ARRAYSIZE(vars::page5Text1));
			ksd::InputText("Placeholder 3", vars::page5Text2, IM_ARRAYSIZE(vars::page5Text2));
			ksd::Checkbox("Placeholder 4", &vars::page5Check1);
			ksd::Checkbox("Placeholder 5", &vars::page5Check2);
			ksd::ColorEdit4x2("Placeholder 6", vars::page5Color1, vars::page5Color2);
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page5Right", ImVec2(224, 435)))
		{
			ksd::SliderFloatRange2("Placeholder 7", &vars::page5RangeMin, &vars::page5RangeMax, 0.f, 1.f, 0.05f, "%.2f");
			ksd::SliderIntRange2("Placeholder 8", &vars::page5IntRangeMin, &vars::page5IntRangeMax, 0, 30, 2);
			ksd::SliderInt("Placeholder 9", &vars::page5Int, 0, 60);
			ksd::MultiCombo("Placeholder 10", vars::page5MultiSelection, vars::page5ItemNames, IM_ARRAYSIZE(vars::page5ItemNames));
		}
		ksd::EndChild();
	}
}
