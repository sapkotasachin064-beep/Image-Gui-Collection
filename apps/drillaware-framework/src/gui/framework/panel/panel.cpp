#include "gui/framework/panel/panel.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	bool BeginChild(const char* title, const ImVec2& size)
	{
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
		PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

		SetCursorPos(GetCursorPos() + ImVec2(0.f, 34.f) + panelOffset);
		const bool isVisible = ImGui::BeginChild(title, size - ImVec2(0, 36), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);

		const ImVec2 panelPos = GetWindowPos();
		const char* titleEnd = FindRenderedTextEnd(title);
		GetWindowDrawList()->AddText(fonts::title, style::panelTitleSize, panelPos + ImVec2(10, 9), GetColorU32(style::accent), title, titleEnd);
		GetWindowDrawList()->AddRectFilled(panelPos + ImVec2(10, 32), ImVec2(panelPos.x + GetWindowWidth() - 10, panelPos.y + 33), GetColorU32(style::panelDivider));

		SetCursorPos(ImVec2(10, 44));
		BeginGroup();

		return isVisible;
	}

	void EndChild()
	{
		EndGroup();
		ImGui::EndChild();
		PopStyleVar(2);
	}
}
