#include "gui/framework/textline/textline.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	void TextLine(const char* label)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			return;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& styleRef = g.Style;
		const float rowHeight = 26.f;
		const float insetX = 10.f;
		const float bottomPad = 2.f;

		ImFontBaked* baked = fonts::title->GetFontBaked(style::headingSize);
		float textWidth = 0.f;
		for (const char* c = label; *c != 0; c++)
		{
			textWidth += baked->GetCharAdvance((ImWchar)ImToUpper(*c)) + style::headingTrackingPx;
		}

		const ImVec2 pos = window->DC.CursorPos;
		const ImRect rect(pos, pos + ImVec2(insetX + textWidth, rowHeight));
		ItemSize(rect, styleRef.FramePadding.y);

		if (!ItemAdd(rect, 0))
		{
			return;
		}

		char upperChar[2] = { 0, 0 };
		float glyphX = rect.Min.x + insetX;
		const float glyphY = rect.Min.y + rowHeight - style::headingSize - bottomPad;
		for (const char* c = label; *c != 0; c++)
		{
			upperChar[0] = (char)ImToUpper(*c);
			window->DrawList->AddText(fonts::title, style::headingSize, ImVec2(IM_TRUNC(glyphX), IM_TRUNC(glyphY)), ImColor(style::headingText), upperChar);
			glyphX += baked->GetCharAdvance((ImWchar)upperChar[0]) + style::headingTrackingPx;
		}
	}
}
