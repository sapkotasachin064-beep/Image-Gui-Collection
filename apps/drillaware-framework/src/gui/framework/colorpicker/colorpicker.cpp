#include "gui/framework/colorpicker/colorpicker.hpp"

#include <unordered_map>
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/slider/slider.hpp"

namespace ksd
{
	using namespace ImGui;

	struct PickerState
	{
		bool rainbow = false;
		float speed = 1.f;
	};

	static std::unordered_map<float*, float> activeRainbows;

	void UpdateRainbows()
	{
		if (!GImGui)
		{
			return;
		}

		const float deltaTime = GImGui->IO.DeltaTime;
		for (auto& entry : activeRainbows)
		{
			float* color = entry.first;
			float hue, saturation, brightness;
			ColorConvertRGBtoHSV(color[0], color[1], color[2], hue, saturation, brightness);
			hue += entry.second * deltaTime * 0.15f;
			if (hue >= 1.f)
			{
				hue -= 1.f;
			}
			ColorConvertHSVtoRGB(hue, saturation, brightness, color[0], color[1], color[2]);
		}
	}

	static void DrawBarHandle(ImDrawList* drawList, float centerX, float barTop, float barBottom)
	{
		const float halfWidth = 4.5f;
		const float overhang = 2.5f;
		drawList->AddRectFilled(ImVec2(centerX - halfWidth - 1.f, barTop - overhang - 1.f), ImVec2(centerX + halfWidth + 1.f, barBottom + overhang + 1.f), IM_COL32(0, 0, 0, 140), 3.f);
		drawList->AddRectFilled(ImVec2(centerX - halfWidth, barTop - overhang), ImVec2(centerX + halfWidth, barBottom + overhang), IM_COL32(255, 255, 255, 230), 2.f);
	}

	static bool ColorPickerBody(const char* id, float rgba[4], bool hideAlpha)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (!window || window->SkipItems)
		{
			return false;
		}

		PushID(id);
		BeginGroup();

		ImGuiStyle& styleRef = GetStyle();
		ImGuiIO& io = GetIO();
		ImDrawList* drawList = window->DrawList;

		const float svSize = GetContentRegionAvail().x;
		const float barHeight = 10.f;
		const float barGap = 8.f;
		const float barRounding = 0.f;

		float hue = 0.f, saturation = 0.f, brightness = 0.f;
		ColorConvertRGBtoHSV(rgba[0], rgba[1], rgba[2], hue, saturation, brightness);

		bool isChanged = false;

		InvisibleButton("##sv", ImVec2(svSize, svSize));
		const bool svActive = IsItemActive();
		const ImVec2 svMin = GetItemRectMin();
		const ImVec2 svMax = GetItemRectMax();

		SetCursorScreenPos(ImVec2(svMin.x, svMax.y + barGap));
		InvisibleButton("##hue", ImVec2(svSize, barHeight));
		const bool hueActive = IsItemActive();
		const ImVec2 hueMin = GetItemRectMin();
		const ImVec2 hueMax = GetItemRectMax();

		ImVec2 alphaMin(0.f, 0.f);
		ImVec2 alphaMax(0.f, 0.f);
		bool alphaActive = false;
		if (!hideAlpha)
		{
			SetCursorScreenPos(ImVec2(svMin.x, hueMax.y + barGap));
			InvisibleButton("##alpha", ImVec2(svSize, barHeight));
			alphaActive = IsItemActive();
			alphaMin = GetItemRectMin();
			alphaMax = GetItemRectMax();
		}

		if (svActive)
		{
			saturation = ImSaturate((io.MousePos.x - svMin.x) / (svSize - 1.f));
			brightness = 1.f - ImSaturate((io.MousePos.y - svMin.y) / (svSize - 1.f));
			ColorConvertHSVtoRGB(hue, saturation, brightness, rgba[0], rgba[1], rgba[2]);
			isChanged = true;
		}
		if (hueActive)
		{
			hue = ImMin(ImSaturate((io.MousePos.x - hueMin.x) / (hueMax.x - hueMin.x)), 0.9999f);
			ColorConvertHSVtoRGB(hue, saturation, brightness, rgba[0], rgba[1], rgba[2]);
			isChanged = true;
		}
		if (alphaActive)
		{
			rgba[3] = ImSaturate((io.MousePos.x - alphaMin.x) / (alphaMax.x - alphaMin.x));
			isChanged = true;
		}

		ColorConvertRGBtoHSV(rgba[0], rgba[1], rgba[2], hue, saturation, brightness);

		const int styleAlpha8 = IM_F32_TO_INT8_SAT(styleRef.Alpha);
		const ImU32 whiteOpaque = IM_COL32(255, 255, 255, styleAlpha8);
		const ImU32 blackOpaque = IM_COL32(0, 0, 0, styleAlpha8);

		ImVec4 hueRgb(1.f, 1.f, 1.f, styleRef.Alpha);
		ColorConvertHSVtoRGB(hue, 1.f, 1.f, hueRgb.x, hueRgb.y, hueRgb.z);
		const ImU32 hueRgbPacked = GetColorU32(hueRgb);

		drawList->AddRectFilledMultiColor(svMin, svMax, whiteOpaque, hueRgbPacked, hueRgbPacked, whiteOpaque);
		drawList->AddRectFilledMultiColor(svMin, svMax, 0, 0, blackOpaque, blackOpaque);
		drawList->AddRect(svMin, svMax, GetColorU32(style::stroke), 0.f);

		const ImVec2 svCursor(
			ImClamp(IM_ROUND(svMin.x + saturation * svSize), svMin.x + 2.f, svMax.x - 2.f),
			ImClamp(IM_ROUND(svMin.y + (1.f - brightness) * svSize), svMin.y + 2.f, svMax.y - 2.f));
		const float svRadius = 6.f;
		const ImU32 selectedPacked = GetColorU32(ImVec4(rgba[0], rgba[1], rgba[2], styleRef.Alpha));
		const int circleSegments = drawList->_CalcCircleAutoSegmentCount(svRadius);
		drawList->AddCircleFilled(svCursor, svRadius, selectedPacked, circleSegments);
		drawList->AddCircle(svCursor, svRadius + 1.f, whiteOpaque, circleSegments);

		static const ImU32 hueColors[7] = {
			IM_COL32(255, 0, 0, 255),
			IM_COL32(255, 255, 0, 255),
			IM_COL32(0, 255, 0, 255),
			IM_COL32(0, 255, 255, 255),
			IM_COL32(0, 0, 255, 255),
			IM_COL32(255, 0, 255, 255),
			IM_COL32(255, 0, 0, 255),
		};
		const float segmentWidth = (hueMax.x - hueMin.x) / 6.f;
		for (int i = 0; i < 6; ++i)
		{
			drawList->AddRectFilledMultiColor(
				ImVec2(hueMin.x + i * segmentWidth, hueMin.y),
				ImVec2(hueMin.x + (i + 1) * segmentWidth, hueMax.y),
				hueColors[i], hueColors[i + 1], hueColors[i + 1], hueColors[i]);
		}
		drawList->AddRect(hueMin, hueMax, IM_COL32(0, 0, 0, 80), barRounding, 0, 1.2f);

		const float hueHandleX = ImClamp(IM_ROUND(hueMin.x + hue * (hueMax.x - hueMin.x)), hueMin.x + 5.f, hueMax.x - 5.f);
		DrawBarHandle(drawList, hueHandleX, hueMin.y, hueMax.y);

		if (!hideAlpha)
		{
			RenderColorRectWithAlphaCheckerboard(drawList, alphaMin, alphaMax, 0, 5.f, ImVec2(0.f, 0.f), barRounding);

			const int red8 = IM_F32_TO_INT8_SAT(rgba[0]);
			const int green8 = IM_F32_TO_INT8_SAT(rgba[1]);
			const int blue8 = IM_F32_TO_INT8_SAT(rgba[2]);
			drawList->AddRectFilledMultiColor(alphaMin, alphaMax,
				IM_COL32(red8, green8, blue8, 0),
				IM_COL32(red8, green8, blue8, 255),
				IM_COL32(red8, green8, blue8, 255),
				IM_COL32(red8, green8, blue8, 0));
			drawList->AddRect(alphaMin, alphaMax, IM_COL32(0, 0, 0, 80), barRounding, 0, 1.2f);

			const float alphaHandleX = ImClamp(IM_ROUND(alphaMin.x + rgba[3] * (alphaMax.x - alphaMin.x)), alphaMin.x + 5.f, alphaMax.x - 5.f);
			DrawBarHandle(drawList, alphaHandleX, alphaMin.y, alphaMax.y);
		}

		EndGroup();
		PopID();
		return isChanged;
	}

	static bool SwatchPickerPopup(const char* popupName, const ImVec2& anchor, float color[4], bool hasAlpha, bool* rainbowOn, float* rainbowSpeed)
	{
		if (!IsPopupOpen(popupName))
		{
			return false;
		}

		SetNextWindowPos(anchor);
		SetNextWindowSize(ImVec2(220.f, 0.f), ImGuiCond_Always);
		PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 8.f));
		PushStyleVar(ImGuiStyleVar_PopupRounding, 6.f);
		PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
		PushStyleColor(ImGuiCol_PopupBg, style::popupBg);
		PushStyleColor(ImGuiCol_Border, style::stroke);

		bool isChanged = false;
		if (BeginPopup(popupName, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			if (ColorPickerBody("##pick", color, !hasAlpha))
			{
				isChanged = true;
			}

			Checkbox("Rainbow", rainbowOn);
			if (*rainbowOn)
			{
				SliderFloat("Speed", rainbowSpeed, 0.2f, 4.f, "%.1fx");
			}

			EndPopup();
		}
		PopStyleColor(2);
		PopStyleVar(4);

		return isChanged;
	}

	static void DrawSwatch(ImDrawList* drawList, const ImRect& frameRect, const float color[4], bool hasAlpha, float rounding)
	{
		drawList->AddRectFilled(frameRect.Min, frameRect.Max, GetColorU32(style::checkFrameIdle), rounding);
		const ImRect innerRect(frameRect.Min + ImVec2(1.f, 1.f), frameRect.Max - ImVec2(1.f, 1.f));
		RenderColorRectWithAlphaCheckerboard(drawList, innerRect.Min, innerRect.Max, GetColorU32(ImVec4(color[0], color[1], color[2], hasAlpha ? color[3] : 1.f)), 4.f, ImVec2(0.f, 0.f), rounding - 1.f);
		drawList->AddRect(frameRect.Min, frameRect.Max, GetColorU32(style::stroke), rounding);
	}

	bool ColorEdit4(const char* label, float color[4], ImGuiColorEditFlags flags, bool* rainbow, float* speed)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& styleRef = g.Style;
		const bool hasAlpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
		const ImGuiID id = window->GetID(label);

		PickerState& state = anim::State<PickerState>(id);
		bool* rainbowOn = rainbow ? rainbow : &state.rainbow;
		float* rainbowSpeed = speed ? speed : &state.speed;

		if (*rainbowOn)
		{
			activeRainbows[color] = *rainbowSpeed;
		}
		else
		{
			activeRainbows.erase(color);
		}

		PushFont(fonts::body, style::textSize);

		const float squareSize = 16.f;
		const float rounding = 4.f;
		const ImVec2 labelSize = CalcTextSize(label, nullptr, true);
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect frameRect(pos, pos + ImVec2(squareSize, squareSize));
		const ImRect totalRect(pos, pos + ImVec2(squareSize + labelSize.x + styleRef.ItemInnerSpacing.x + 2, squareSize));

		ItemAdd(totalRect, id, &frameRect);
		ItemSize(totalRect, styleRef.FramePadding.y);

		bool hovered = false;
		bool held = false;
		const bool pressed = ButtonBehavior(totalRect, id, &hovered, &held);

		char popupName[40];
		ImFormatString(popupName, IM_ARRAYSIZE(popupName), "##ColorPicker_%08X", id);
		if (pressed)
		{
			OpenPopup(popupName);
		}
		if (hovered)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		ImDrawList* drawList = window->DrawList;
		DrawSwatch(drawList, frameRect, color, hasAlpha, rounding);

		if (labelSize.x > 0.f)
		{
			const ImVec2 labelPos(frameRect.Max.x + styleRef.ItemInnerSpacing.x + 3, frameRect.GetCenter().y - labelSize.y * 0.5f);
			RenderText(labelPos, label);
		}
		PopFont();

		const ImVec2 popupAnchor = g.LastItemData.Rect.GetBL() + ImVec2(0.f, styleRef.ItemSpacing.y + 4.f);
		return SwatchPickerPopup(popupName, popupAnchor, color, hasAlpha, rainbowOn, rainbowSpeed);
	}

	static bool MultiColorEdit(const char* label, float* const swatchColors[], int swatchCount, ImGuiColorEditFlags flags)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			return false;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& styleRef = g.Style;
		const bool hasAlpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;

		PushFont(fonts::body, style::textSize);

		const float squareSize = 16.f;
		const float rounding = 4.f;
		const float swatchGap = 6.f;
		const ImVec2 labelSize = CalcTextSize(label, nullptr, true);
		const ImVec2 pos = window->DC.CursorPos;
		const float swatchesWidth = swatchCount * squareSize + (swatchCount - 1) * swatchGap;
		const ImRect totalRect(pos, pos + ImVec2(swatchesWidth + labelSize.x + styleRef.ItemInnerSpacing.x + 2, squareSize));
		ItemSize(totalRect, styleRef.FramePadding.y);

		ImDrawList* drawList = window->DrawList;
		bool isChanged = false;

		PushID(label);
		for (int i = 0; i < swatchCount; i++)
		{
			PushID(i);

			float* color = swatchColors[i];
			const float swatchX = i * (squareSize + swatchGap);
			const ImRect swatchRect(pos + ImVec2(swatchX, 0.f), pos + ImVec2(swatchX + squareSize, squareSize));
			const ImGuiID swatchId = window->GetID("swatch");

			if (ItemAdd(swatchRect, swatchId))
			{
				bool hovered = false;
				bool held = false;
				const bool pressed = ButtonBehavior(swatchRect, swatchId, &hovered, &held);

				char popupName[40];
				ImFormatString(popupName, IM_ARRAYSIZE(popupName), "##ColorPicker_%08X", swatchId);
				if (pressed)
				{
					OpenPopup(popupName);
				}
				if (hovered)
				{
					SetMouseCursor(ImGuiMouseCursor_Hand);
				}

				PickerState& state = anim::State<PickerState>(swatchId);
				if (state.rainbow)
				{
					activeRainbows[color] = state.speed;
				}
				else
				{
					activeRainbows.erase(color);
				}

				DrawSwatch(drawList, swatchRect, color, hasAlpha, rounding);

				const ImVec2 popupAnchor = swatchRect.GetBL() + ImVec2(0.f, styleRef.ItemSpacing.y + 4.f);
				if (SwatchPickerPopup(popupName, popupAnchor, color, hasAlpha, &state.rainbow, &state.speed))
				{
					isChanged = true;
				}
			}

			PopID();
		}
		PopID();

		if (labelSize.x > 0.f)
		{
			const ImVec2 labelPos(pos.x + swatchesWidth + styleRef.ItemInnerSpacing.x + 3, pos.y + (squareSize - labelSize.y) * 0.5f);
			RenderText(labelPos, label);
		}
		PopFont();

		return isChanged;
	}

	bool ColorEdit4x2(const char* label, float color1[4], float color2[4], ImGuiColorEditFlags flags)
	{
		float* const swatchColors[2] = { color1, color2 };
		return MultiColorEdit(label, swatchColors, 2, flags);
	}

	bool ColorEdit4x3(const char* label, float color1[4], float color2[4], float color3[4], ImGuiColorEditFlags flags)
	{
		float* const swatchColors[3] = { color1, color2, color3 };
		return MultiColorEdit(label, swatchColors, 3, flags);
	}
}
