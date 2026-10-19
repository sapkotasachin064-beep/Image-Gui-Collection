#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/combo/combo.hpp"
#include "gui/framework/inputtext/inputtext.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"

namespace vars
{
	const char* page7ItemNames[7] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7" };
	bool page7MultiSelection[7] = { false, false, false, false, false, false, false };
	int page7Index1 = 0;
	int page7Index2 = 4;

	char page7Text1[128] = "";
	char page7Text2[128] = "";
	char page7Text3[128] = "";

	bool page7Check1 = true;
	bool page7Check2 = false;
	float page7Value = 22.f;
	float page7Color[4] = { 0.55f, 0.35f, 0.25f, 1.f };
}

namespace pages
{
	void RenderPage7() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page7Left", ImVec2(224, 435)))
		{
			ksd::MultiCombo("Placeholder 1", vars::page7MultiSelection, vars::page7ItemNames, IM_ARRAYSIZE(vars::page7ItemNames));
			ksd::combo("Placeholder 2", &vars::page7Index1, vars::page7ItemNames, IM_ARRAYSIZE(vars::page7ItemNames), 5);
			ksd::combo("Placeholder 3", &vars::page7Index2, vars::page7ItemNames, IM_ARRAYSIZE(vars::page7ItemNames), 5);
			ksd::Checkbox("Placeholder 4", &vars::page7Check1);
			ksd::Checkbox("Placeholder 5", &vars::page7Check2);
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page7Right", ImVec2(224, 435)))
		{
			ksd::InputText("Placeholder 6", vars::page7Text1, IM_ARRAYSIZE(vars::page7Text1));
			ksd::InputText("Placeholder 7", vars::page7Text2, IM_ARRAYSIZE(vars::page7Text2));
			ksd::InputText("Placeholder 8", vars::page7Text3, IM_ARRAYSIZE(vars::page7Text3));
			ksd::SliderFloat("Placeholder 9", &vars::page7Value, 0.f, 60.f, "%.1f");
			ksd::ColorEdit4("Placeholder 10", vars::page7Color);
			ksd::button("Placeholder 11", ImVec2(95, 25));
			ImGui::SameLine();
			ksd::button("Placeholder 12", ImVec2(95, 25));
		}
		ksd::EndChild();
	}
}
