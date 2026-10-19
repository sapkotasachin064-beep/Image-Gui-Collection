#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/combo/combo.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/inputtext/inputtext.hpp"

namespace vars
{
	int page3Int1 = 30;
	int page3Int2 = 8;
	float page3Float1 = 0.6f;
	float page3Float2 = 45.f;
	int page3RangeMin = 10;
	int page3RangeMax = 70;

	const char* page3ItemNames[5] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
	bool page3MultiSelection[5] = { false, true, false, false, false };
	int page3Index = 3;

	bool page3Check1 = true;
	bool page3Check2 = false;
	char page3Text[128] = "";
}

namespace pages
{
	void RenderPage3() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page3Left", ImVec2(224, 435)))
		{
			ksd::SliderInt("Placeholder 1", &vars::page3Int1, 0, 100);
			ksd::SliderFloat("Placeholder 2", &vars::page3Float1, 0.f, 1.f, "%.2f");
			ksd::SliderIntRange2("Placeholder 3", &vars::page3RangeMin, &vars::page3RangeMax, 0, 100, 5);
			ksd::SliderInt("Placeholder 4", &vars::page3Int2, 0, 32);
			ksd::SliderFloat("Placeholder 5", &vars::page3Float2, 0.f, 180.f, "%.1f");
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page3Right", ImVec2(224, 435)))
		{
			ksd::MultiCombo("Placeholder 6", vars::page3MultiSelection, vars::page3ItemNames, IM_ARRAYSIZE(vars::page3ItemNames));
			ksd::combo("Placeholder 7", &vars::page3Index, vars::page3ItemNames, IM_ARRAYSIZE(vars::page3ItemNames), 5);
			ksd::InputText("Placeholder 8", vars::page3Text, IM_ARRAYSIZE(vars::page3Text));
			ksd::Checkbox("Placeholder 9", &vars::page3Check1);
			ksd::Checkbox("Placeholder 10", &vars::page3Check2);
			ksd::button("Placeholder 11", ImVec2(200, 25));
		}
		ksd::EndChild();
	}
}
