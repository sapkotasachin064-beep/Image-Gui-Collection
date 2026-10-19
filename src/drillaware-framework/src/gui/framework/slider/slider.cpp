#include "gui/framework/slider/slider.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	struct SliderState
	{
		ImVec4 valueText;
		float fillX;
		float knobRadius;
		int lastFrame;
	};

	struct RangeSliderState
	{
		ImVec4 valueText;
		float fillMinX;
		float fillMaxX;
		float knobRadius;
		int activeKnob;
		int lastFrame;
	};

	static const float sliderWidth = 200.f;
	static const float sliderHeight = 38.f;
	static const float trackTop = 22.f;
	static const float trackBottom = 34.f;
	static const float trackCenterY = 28.f;

	static void DrawSliderHeader(ImGuiWindow* window, const ImVec2& pos, const char* label, const char* valueText, ImU32 valueColor)
	{
		const char* labelEnd = FindRenderedTextEnd(label);
		window->DrawList->AddText(fonts::body, style::textSize, pos, GetColorU32(ImGuiCol_Text), label, labelEnd);

		PushFont(fonts::body, style::textSize);
		const float valueWidth = CalcTextSize(valueText).x;
		PopFont();
		window->DrawList->AddText(fonts::body, style::textSize, ImVec2(pos.x + sliderWidth - valueWidth, pos.y), valueColor, valueText);
	}

	static void DrawSliderTrack(ImGuiWindow* window, const ImVec2& pos, float fillStartX, float fillEndX)
	{
		window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + trackCenterY - 2.5f), ImVec2(pos.x + sliderWidth, pos.y + trackCenterY + 2.5f), GetColorU32(style::sliderTrack), 100);
		window->DrawList->AddRectFilled(ImVec2(pos.x + fillStartX, pos.y + trackCenterY - 2.5f), ImVec2(pos.x + fillEndX, pos.y + trackCenterY + 2.5f), GetColorU32(style::accent), 100);
	}

	static bool SliderScalarStyled(const char* label, ImGuiDataType dataType, void* value, const void* min, const void* max, const char* format)
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

		const ImRect totalRect(pos, pos + ImVec2(sliderWidth, sliderHeight));
		const ImRect trackRect(pos + ImVec2(0, trackTop), pos + ImVec2(sliderWidth, trackBottom));
		ItemSize(totalRect, styleRef.FramePadding.y);
		if (!ItemAdd(totalRect, id, &trackRect, ImGuiItemFlags_Inputable))
		{
			return false;
		}

		const bool hovered = ItemHoverable(trackRect, id, g.LastItemData.ItemFlags);
		const bool clicked = hovered && IsMouseClicked(ImGuiMouseButton_Left, ImGuiInputFlags_None, id);
		if (clicked || g.NavActivateId == id)
		{
			if (clicked)
			{
				SetKeyOwner(ImGuiKey_MouseLeft, id);
			}
			SetActiveID(id, window);
			SetFocusID(id, window);
			FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}

		const bool isActive = g.ActiveId == id;
		if (hovered || isActive)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		ImRect grabRect;
		const bool valueChanged = SliderBehavior(trackRect, id, dataType, value, min, max, format, ImGuiSliderFlags_None, &grabRect);
		if (valueChanged)
		{
			MarkItemEdited(id);
		}

		SliderState& state = anim::State<SliderState>(id);
		const ImVec4 valueColorTarget = isActive ? style::sliderTextActive : hovered ? style::sliderTextHover : style::sliderTextIdle;
		const float fillTarget = grabRect.GetCenter().x - pos.x;
		const float radiusTarget = (isActive || hovered) ? 7.5f : 6.f;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.valueText = valueColorTarget;
			state.fillX = fillTarget;
			state.knobRadius = radiusTarget;
		}
		else
		{
			anim::EaseLerp(state.valueText, valueColorTarget, 14.f);
			anim::EaseLerp(state.fillX, fillTarget, 20.f);
			anim::EaseLerp(state.knobRadius, radiusTarget, 20.f);
		}

		char valueText[64];
		DataTypeFormatString(valueText, IM_ARRAYSIZE(valueText), dataType, value, format);

		DrawSliderHeader(window, pos, label, valueText, GetColorU32(state.valueText));
		DrawSliderTrack(window, pos, 0.f, state.fillX);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + state.fillX, pos.y + trackCenterY), state.knobRadius, GetColorU32(style::sliderKnob), 100);

		return valueChanged;
	}

	static bool RangeSliderStyled(const char* label, const char* valueText, float* valueMin, float* valueMax, float min, float max, float minGap, bool shouldSnapToInt)
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

		const ImRect totalRect(pos, pos + ImVec2(sliderWidth, sliderHeight));
		const ImRect trackRect(pos + ImVec2(0, trackTop), pos + ImVec2(sliderWidth, trackBottom));
		ItemSize(totalRect, styleRef.FramePadding.y);
		if (!ItemAdd(totalRect, id, &trackRect, ImGuiItemFlags_Inputable))
		{
			return false;
		}

		RangeSliderState& state = anim::State<RangeSliderState>(id);

		const float range = max - min;
		const float knobMinX = (*valueMin - min) / range * sliderWidth;
		const float knobMaxX = (*valueMax - min) / range * sliderWidth;

		const bool hovered = ItemHoverable(trackRect, id, g.LastItemData.ItemFlags);
		const bool clicked = hovered && IsMouseClicked(ImGuiMouseButton_Left, ImGuiInputFlags_None, id);
		if (clicked)
		{
			SetKeyOwner(ImGuiKey_MouseLeft, id);
			SetActiveID(id, window);
			SetFocusID(id, window);
			FocusWindow(window);

			const float mouseX = g.IO.MousePos.x - pos.x;
			state.activeKnob = ImFabs(mouseX - knobMinX) <= ImFabs(mouseX - knobMaxX) ? 0 : 1;
		}

		const bool isActive = g.ActiveId == id;
		if (hovered || isActive)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		bool valueChanged = false;
		if (isActive)
		{
			if (!g.IO.MouseDown[0])
			{
				ClearActiveID();
			}
			else
			{
				const float mouseRatio = ImClamp((g.IO.MousePos.x - pos.x) / sliderWidth, 0.f, 1.f);
				float draggedValue = min + mouseRatio * range;
				if (shouldSnapToInt)
				{
					draggedValue = (float)(int)(draggedValue + (draggedValue >= 0.f ? 0.5f : -0.5f));
				}

				if (state.activeKnob == 0)
				{
					draggedValue = ImClamp(draggedValue, min, *valueMax - minGap);
					if (draggedValue != *valueMin)
					{
						*valueMin = draggedValue;
						valueChanged = true;
					}
				}
				else
				{
					draggedValue = ImClamp(draggedValue, *valueMin + minGap, max);
					if (draggedValue != *valueMax)
					{
						*valueMax = draggedValue;
						valueChanged = true;
					}
				}
			}
		}
		if (valueChanged)
		{
			MarkItemEdited(id);
		}

		const ImVec4 valueColorTarget = isActive ? style::sliderTextActive : hovered ? style::sliderTextHover : style::sliderTextIdle;
		const float fillMinTarget = (*valueMin - min) / range * sliderWidth;
		const float fillMaxTarget = (*valueMax - min) / range * sliderWidth;
		const float radiusTarget = (isActive || hovered) ? 7.5f : 6.f;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.valueText = valueColorTarget;
			state.fillMinX = fillMinTarget;
			state.fillMaxX = fillMaxTarget;
			state.knobRadius = radiusTarget;
		}
		else
		{
			anim::EaseLerp(state.valueText, valueColorTarget, 14.f);
			anim::EaseLerp(state.fillMinX, fillMinTarget, 20.f);
			anim::EaseLerp(state.fillMaxX, fillMaxTarget, 20.f);
			anim::EaseLerp(state.knobRadius, radiusTarget, 20.f);
		}

		DrawSliderHeader(window, pos, label, valueText, GetColorU32(state.valueText));
		DrawSliderTrack(window, pos, state.fillMinX, state.fillMaxX);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + state.fillMinX, pos.y + trackCenterY), state.knobRadius, GetColorU32(style::sliderKnob), 100);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + state.fillMaxX, pos.y + trackCenterY), state.knobRadius, GetColorU32(style::sliderKnob), 100);

		return valueChanged;
	}

	bool SliderFloat(const char* label, float* value, float min, float max, const char* format)
	{
		return SliderScalarStyled(label, ImGuiDataType_Float, value, &min, &max, format);
	}

	bool SliderInt(const char* label, int* value, int min, int max, const char* format)
	{
		return SliderScalarStyled(label, ImGuiDataType_S32, value, &min, &max, format);
	}

	bool SliderFloatRange2(const char* label, float* valueMin, float* valueMax, float min, float max, float minGap, const char* format)
	{
		char minText[32];
		char maxText[32];
		ImFormatString(minText, IM_ARRAYSIZE(minText), format, *valueMin);
		ImFormatString(maxText, IM_ARRAYSIZE(maxText), format, *valueMax);

		char valueText[64];
		ImFormatString(valueText, IM_ARRAYSIZE(valueText), "%s - %s", minText, maxText);

		return RangeSliderStyled(label, valueText, valueMin, valueMax, min, max, minGap, false);
	}

	bool SliderIntRange2(const char* label, int* valueMin, int* valueMax, int min, int max, int minGap)
	{
		char valueText[64];
		ImFormatString(valueText, IM_ARRAYSIZE(valueText), "%d - %d", *valueMin, *valueMax);

		float draggedMin = (float)*valueMin;
		float draggedMax = (float)*valueMax;
		const bool valueChanged = RangeSliderStyled(label, valueText, &draggedMin, &draggedMax, (float)min, (float)max, (float)minGap, true);
		if (valueChanged)
		{
			*valueMin = (int)draggedMin;
			*valueMax = (int)draggedMax;
		}

		return valueChanged;
	}
}
