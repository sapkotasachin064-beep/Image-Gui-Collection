#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/inputtext/inputtext.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"

namespace vars
{
	char page2Text1[128] = "";
	char page2Text2[128] = "";
	char page2Text3[128] = "";
	char page2Text4[128] = "";
	char page2Text5[128] = "";

	float page2Color1[4] = { 0.80f, 0.36f, 0.36f, 1.f };
	float page2Color2[4] = { 0.36f, 0.55f, 0.80f, 1.f };
	float page2Color3[4] = { 0.62f, 0.42f, 0.78f, 1.f };

	bool page2Check1 = false;
	bool page2Check2 = true;
	float page2Value = 0.35f;
}

namespace pages
{
	void RenderPage2() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page2Left", ImVec2(224, 435)))
		{
			ksd::InputText("Placeholder 1", vars::page2Text1, IM_ARRAYSIZE(vars::page2Text1));
			ksd::InputText("Placeholder 2", vars::page2Text2, IM_ARRAYSIZE(vars::page2Text2));
			ksd::InputText("Placeholder 3", vars::page2Text3, IM_ARRAYSIZE(vars::page2Text3));
			ksd::InputText("Placeholder 4", vars::page2Text4, IM_ARRAYSIZE(vars::page2Text4));
			ksd::InputText("Placeholder 5", vars::page2Text5, IM_ARRAYSIZE(vars::page2Text5));
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page2Right", ImVec2(224, 435)))
		{
			ksd::ColorEdit4x2("Placeholder 6", vars::page2Color1, vars::page2Color2);
			ksd::ColorEdit4("Placeholder 7", vars::page2Color3);
			ksd::Checkbox("Placeholder 8", &vars::page2Check1);
			ksd::Checkbox("Placeholder 9", &vars::page2Check2);
			ksd::SliderFloat("Placeholder 10", &vars::page2Value, 0.f, 1.f, "%.2f");
			ksd::button("Placeholder 11", ImVec2(200, 25));
			ksd::button("Placeholder 12", ImVec2(200, 25));
		}
		ksd::EndChild();
	}
}
