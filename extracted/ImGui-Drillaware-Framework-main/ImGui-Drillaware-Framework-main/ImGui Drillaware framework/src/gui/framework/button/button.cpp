#include "gui/framework/button/button.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	struct ButtonState
	{
		ImVec4 bg;
		ImVec4 border;
		ImVec4 text;
		int lastFrame;
	};

	bool button(const char* label, const ImVec2& size)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& styleRef = g.Style;
		const ImGuiID id = window->GetID(label);

		PushFont(fonts::body, style::textSize);
		const ImVec2 labelSize = CalcTextSize(label, nullptr, true);
		PopFont();

		ImVec2 pos = window->DC.CursorPos;
		if (styleRef.FramePadding.y < window->DC.CurrLineTextBaseOffset)
		{
			pos.y += window->DC.CurrLineTextBaseOffset - styleRef.FramePadding.y;
		}

		const ImVec2 frameSize = CalcItemSize(size, labelSize.x + styleRef.FramePadding.x * 2.f, labelSize.y + styleRef.FramePadding.y * 2.f);
		const ImRect rect(pos, pos + frameSize);
		ItemSize(frameSize, styleRef.FramePadding.y);
		if (!ItemAdd(rect, id))
		{
			return false;
		}

		bool hovered = false;
		bool held = false;
		const bool pressed = ButtonBehavior(rect, id, &hovered, &held);

		ButtonState& state = anim::State<ButtonState>(id);
		const ImVec4 bgTarget = held ? style::buttonHeld : hovered ? style::buttonBgHover : style::buttonBg;
		const ImVec4 borderTarget = hovered ? style::buttonBorderHover : style::buttonBorder;
		const ImVec4 textTarget = hovered ? style::accent : style::buttonText;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.bg = bgTarget;
			state.border = borderTarget;
			state.text = textTarget;
		}
		else if (hovered)
		{
			state.bg = bgTarget;
			state.border = borderTarget;
			state.text = textTarget;
		}
		else
		{
			anim::EaseLerp(state.bg, bgTarget, 14.f);
			anim::EaseLerp(state.border, borderTarget, 14.f);
			anim::EaseLerp(state.text, textTarget, 14.f);
		}

		if (hovered)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		RenderNavCursor(rect, id);
		RenderFrame(rect.Min, rect.Max, GetColorU32(state.bg), false, 5);
		window->DrawList->AddRect(rect.Min, rect.Max, GetColorU32(state.border), 5);
		window->DrawList->AddText(fonts::body, style::textSize, rect.GetCenter() - labelSize / 2, GetColorU32(state.text), label);

		return pressed;
	}
}
