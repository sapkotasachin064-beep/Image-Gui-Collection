#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	struct CheckboxState
	{
		ImVec4 frame;
		ImVec4 checkmark;
		ImVec4 stroke;
		int lastFrame;
	};

	bool Checkbox(const char* label, bool* value)
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

		const float squareSize = 16.f;
		const float rounding = 4.f;
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect frameRect(pos, pos + ImVec2(squareSize, squareSize));
		const ImRect totalRect(pos, pos + ImVec2(squareSize + labelSize.x + styleRef.ItemInnerSpacing.x + 2, squareSize));
		ItemAdd(totalRect, id, &frameRect);
		ItemSize(totalRect, styleRef.FramePadding.y);

		bool hovered = false;
		bool held = false;
		const bool pressed = ButtonBehavior(totalRect, id, &hovered, &held);
		if (pressed)
		{
			*value = !(*value);
			MarkItemEdited(id);
		}

		CheckboxState& state = anim::State<CheckboxState>(id);
		const ImVec4 checkmarkTarget = *value ? style::checkMarkOn : hovered ? style::checkMarkHover : style::checkMarkOff;
		const ImVec4 frameTarget = *value ? style::accent : hovered ? style::checkFrameHover : style::checkFrameIdle;
		const ImVec4 strokeTarget = hovered ? style::strokeHover : style::stroke;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.checkmark = checkmarkTarget;
			state.frame = frameTarget;
			state.stroke = strokeTarget;
		}
		else
		{
			anim::EaseLerp(state.checkmark, checkmarkTarget, 16.f);
			anim::EaseLerp(state.frame, frameTarget, 16.f);

			if (hovered)
			{
				state.stroke = strokeTarget;
			}
			else
			{
				anim::EaseLerp(state.stroke, strokeTarget, 14.f);
			}
		}

		RenderFrame(frameRect.Min, frameRect.Max, GetColorU32(state.frame), false, rounding);
		window->DrawList->AddRect(frameRect.Min, frameRect.Max, GetColorU32(state.stroke), rounding);
		RenderCheckMark(window->DrawList, frameRect.GetCenter() - ImVec2((squareSize / 2 + 1) / 2, (squareSize / 2 + 1) / 2), GetColorU32(state.checkmark), squareSize / 2 + 1);

		if (labelSize.x > 0.f)
		{
			const ImVec2 labelPos = ImVec2(frameRect.Max.x + styleRef.ItemInnerSpacing.x + 3, frameRect.GetCenter().y - labelSize.y * 0.5f);
			PushFont(fonts::body, style::textSize);
			RenderText(labelPos, label);
			PopFont();
		}

		return pressed;
	}
}
