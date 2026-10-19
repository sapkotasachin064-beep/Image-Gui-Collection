#include "imgui.h"
#include "gui/pages/pages.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/combo/combo.hpp"

namespace vars
{
	float page6Color1[4] = { 0.80f, 0.36f, 0.36f, 1.f };
	float page6Color2[4] = { 0.90f, 0.72f, 0.35f, 1.f };
	float page6Color3[4] = { 0.36f, 0.55f, 0.80f, 1.f };
	float page6Color4[4] = { 0.62f, 0.42f, 0.78f, 0.5f };
	float page6Color5[4] = { 0.45f, 0.75f, 0.45f, 1.f };
	float page6Color6[4] = { 0.30f, 0.70f, 0.65f, 1.f };

	bool page6Check1 = false;
	bool page6Check2 = true;
	bool page6Check3 = false;
	bool page6Check4 = false;

	float page6Value = 55.f;
	const char* page6ItemNames[4] = { "Item 1", "Item 2", "Item 3", "Item 4" };
	int page6Index = 0;
}

namespace pages
{
	void RenderPage6() noexcept
	{
		ImGui::SetCursorPos(ImVec2(150, 25));
		if (ksd::BeginChild("Placeholder##page6Left", ImVec2(224, 435)))
		{
			ksd::ColorEdit4x3("Placeholder 1", vars::page6Color1, vars::page6Color2, vars::page6Color3);
			ksd::ColorEdit4x2("Placeholder 2", vars::page6Color5, vars::page6Color6);
			ksd::ColorEdit4("Placeholder 3", vars::page6Color4);
			ksd::Checkbox("Placeholder 4", &vars::page6Check1);
			ksd::Checkbox("Placeholder 5", &vars::page6Check2);
			ksd::SliderFloat("Placeholder 6", &vars::page6Value, 0.f, 100.f, "%.1f");
		}
		ksd::EndChild();

		ImGui::SetCursorPos(ImVec2(384, 25));
		if (ksd::BeginChild("Placeholder##page6Right", ImVec2(224, 435)))
		{
			ksd::combo("Placeholder 7", &vars::page6Index, vars::page6ItemNames, IM_ARRAYSIZE(vars::page6ItemNames), 4);
			ksd::Checkbox("Placeholder 8", &vars::page6Check3);
			ksd::Checkbox("Placeholder 9", &vars::page6Check4);
			ksd::button("Placeholder 10", ImVec2(200, 25));
			ksd::button("Placeholder 11", ImVec2(95, 25));
			ImGui::SameLine();
			ksd::button("Placeholder 12", ImVec2(95, 25));
		}
		ksd::EndChild();
	}
}
