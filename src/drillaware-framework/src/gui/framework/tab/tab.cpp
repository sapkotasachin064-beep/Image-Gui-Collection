#include "gui/framework/tab/tab.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	struct TabState
	{
		ImVec4 text;
		ImVec4 marker;
		int lastFrame;
	};

	bool tab(const char* label, bool isActive)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& styleRef = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect rect(pos, pos + ImVec2(132, 35));
		ItemSize(rect, styleRef.FramePadding.y);
		if (!ItemAdd(rect, id))
		{
			return false;
		}

		bool hovered = false;
		bool held = false;
		const bool pressed = ButtonBehavior(rect, id, &hovered, &held);
		if (pressed)
		{
			MarkItemEdited(id);
		}

		TabState& state = anim::State<TabState>(id);
		const ImVec4 textTarget = isActive ? style::tabTextActive : hovered ? style::tabTextHover : style::tabTextIdle;
		const ImVec4 markerTarget = isActive ? style::accent : style::markerOff;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.text = textTarget;
			state.marker = markerTarget;
		}
		else if (hovered)
		{
			state.text = textTarget;
			anim::EaseLerp(state.marker, markerTarget, 14.f);
		}
		else
		{
			anim::EaseLerp(state.text, textTarget, 14.f);
			anim::EaseLerp(state.marker, markerTarget, 14.f);
		}

		if (hovered)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		window->DrawList->AddRectFilled(rect.Min + ImVec2(0, 10), rect.Min + ImVec2(2.5f, 30), ImColor(state.marker), 10, ImDrawFlags_RoundCornersRight);

		window->DrawList->AddText(fonts::title, style::tabTextSize, rect.Min + ImVec2(20, 20 - style::tabTextSize * 0.5f), ImColor(state.text), label);

		return pressed;
	}
}
