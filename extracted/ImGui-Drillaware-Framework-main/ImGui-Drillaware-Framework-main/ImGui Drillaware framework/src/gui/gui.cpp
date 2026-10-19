#include "gui/gui.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/tab/tab.hpp"
#include "gui/framework/textline/textline.hpp"
#include "gui/framework/panel/panel.hpp"
#include "gui/framework/colorpicker/colorpicker.hpp"
#include "gui/pages/pages.hpp"

static void ApplyTheme() noexcept
{
	style::accent = ImVec4(vars::menuAccentColor[0], vars::menuAccentColor[1], vars::menuAccentColor[2], vars::menuAccentColor[3]);
	style::selectTextOn = style::accent;
	style::selectTextHover = ImLerp(style::textWhite, style::accent, 0.75f);

	anim::speed = vars::menuAnimationSpeed;
}

void gui::Render() noexcept
{
	if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
	{
		isOpen = !isOpen;
	}

	if (!isOpen)
	{
		return;
	}

	ksd::UpdateRainbows();
	ApplyTheme();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, style::windowBg);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, style::childBg);
	ImGui::PushStyleColor(ImGuiCol_Border, style::border);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, style::frameBg);
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, style::accent);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight));

	if (ImGui::Begin("###Menu", nullptr, windowFlags))
	{
		ImGui::PushFont(fonts::body, style::textSize);

		ImDrawList* draw = ImGui::GetWindowDrawList();
		const ImVec2 p = ImGui::GetWindowPos();
		const float logoY = (titleBarHeight - style::logoSize) * 0.5f;

		draw->AddRectFilled(p, ImVec2(p.x + menuWidth, p.y + titleBarHeight), ImColor(style::childBg), 5, ImDrawFlags_RoundCornersTop);
		draw->AddRectFilled(ImVec2(p.x, p.y + 50), ImVec2(p.x + menuWidth, p.y + 51), ImColor(style::frameBg), 5);
		draw->AddRectFilled(ImVec2(p.x, p.y + 51), ImVec2(p.x + navWidth, p.y + menuHeight), ImColor(style::childBg), 5, ImDrawFlags_RoundCornersBottomLeft);
		draw->AddText(fonts::title, style::logoSize, ImVec2(p.x + 14, p.y + logoY), ImColor(style::textWhite), ".");
		draw->AddText(fonts::title, style::logoSize, ImVec2(p.x + 19, p.y + logoY), ImColor(style::accent), "DRILLAWARE");

		ImGui::SetCursorPos(ImVec2(1, 55));
		ImGui::BeginGroup();

		ksd::TextLine("Main");
		if (ksd::tab("Crimson", activeTab == 0))
		{
			activeTab = 0;
		}
		if (ksd::tab("Magenta", activeTab == 1))
		{
			activeTab = 1;
		}
		if (ksd::tab("Emerald", activeTab == 2))
		{
			activeTab = 2;
		}
		if (ksd::tab("Sapphire", activeTab == 3))
		{
			activeTab = 3;
		}
		if (ksd::tab("Obsidian", activeTab == 4))
		{
			activeTab = 4;
		}

		ksd::TextLine("Misc");
		if (ksd::tab("Lavender", activeTab == 5))
		{
			activeTab = 5;
		}
		if (ksd::tab("Chestnut", activeTab == 6))
		{
			activeTab = 6;
		}
		if (ksd::tab("Marigold", activeTab == 7))
		{
			activeTab = 7;
		}
		if (ksd::tab("Settings", activeTab == 8))
		{
			activeTab = 8;
		}

		ImGui::EndGroup();

		static int visibleTab = activeTab;
		static float fadeProgress = 1.f;

		const bool isSwapping = visibleTab != activeTab;
		float fadeSeconds = 0.14f;
		if (isSwapping)
		{
			fadeSeconds = 0.10f;
		}

		const float fadeStep = ImGui::GetIO().DeltaTime * anim::speed / fadeSeconds;
		if (isSwapping)
		{
			fadeProgress -= fadeStep;
		}
		else
		{
			fadeProgress += fadeStep;
		}
		fadeProgress = ImClamp(fadeProgress, 0.f, 1.f);

		if (isSwapping && fadeProgress <= 0.f)
		{
			visibleTab = activeTab;
		}

		const float fadeEase = fadeProgress * fadeProgress * (3.f - 2.f * fadeProgress);
		const float pageAlpha = ImMax(fadeEase, 0.02f);

		const float slideDistance = 12.f;
		const float slideY = (1.f - fadeEase) * slideDistance;
		ksd::panelOffset = ImVec2(0.f, slideY);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, pageAlpha);

		if (visibleTab == 0)
		{
			pages::RenderPage1();
		}
		if (visibleTab == 1)
		{
			pages::RenderPage2();
		}
		if (visibleTab == 2)
		{
			pages::RenderPage3();
		}
		if (visibleTab == 3)
		{
			pages::RenderPage4();
		}
		if (visibleTab == 4)
		{
			pages::RenderPage5();
		}
		if (visibleTab == 5)
		{
			pages::RenderPage6();
		}
		if (visibleTab == 6)
		{
			pages::RenderPage7();
		}
		if (visibleTab == 7)
		{
			pages::RenderPage8();
		}
		if (visibleTab == 8)
		{
			pages::RenderSettings();
		}

		ImGui::PopStyleVar();
		ksd::panelOffset = ImVec2(0.f, 0.f);

		ImGui::PopFont();
	}
	ImGui::End();

	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(5);
}
