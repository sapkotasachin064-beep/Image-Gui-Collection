#include "gui/framework/combo/combo.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	struct ComboState
	{
		ImVec4 background;
		ImVec4 text;
		float openAnim;
		float arrowAnim;
		int lastFrame;
	};

	struct ComboItemState
	{
		ImVec4 text;
		ImVec4 mark;
		float slide;
		int lastFrame;
	};

	static float CalcMaxPopupHeightFromItemCount(int itemCount)
	{
		ImGuiContext& g = *GImGui;
		if (itemCount <= 0)
		{
			return FLT_MAX;
		}
		return (g.FontSize + g.Style.ItemSpacing.y) * itemCount - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}

	static bool ItemsArrayGetter(void* itemsSource, int index, const char** outText)
	{
		const char* const* items = (const char* const*)itemsSource;
		if (outText)
		{
			*outText = items[index];
		}
		return true;
	}

	static bool SelectableItem(const char* label, bool isSelected, bool shouldCloseOnPress = true)
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
		pos.y += window->DC.CurrLineTextBaseOffset;
		const ImVec2 size(ImMax(labelSize.x, window->WorkRect.Max.x - pos.x), labelSize.y);
		ItemSize(size, 0.f);

		const ImVec2 textMin = pos;
		const ImVec2 textMax(pos.x + size.x, pos.y + size.y);

		ImRect rect(pos, pos + size);
		const float spacingX = styleRef.ItemSpacing.x;
		const float spacingY = styleRef.ItemSpacing.y;
		const float spacingLeft = IM_TRUNC(spacingX * 0.5f);
		const float spacingUp = IM_TRUNC(spacingY * 0.5f);
		rect.Min.x -= spacingLeft;
		rect.Min.y -= spacingUp;
		rect.Max.x += spacingX - spacingLeft;
		rect.Max.y += spacingY - spacingUp;

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

		ComboItemState& state = anim::State<ComboItemState>(id);
		const ImVec4 textTarget = isSelected ? style::selectTextOn : hovered ? style::selectTextHover : style::selectTextIdle;
		const ImVec4 markTarget = isSelected ? style::accent : style::checkMarkOff;
		const float slideTarget = hovered ? 6.f : 0.f;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.text = textTarget;
			state.mark = markTarget;
			state.slide = slideTarget;
		}
		else if (hovered)
		{
			state.text = textTarget;
			anim::EaseLerp(state.mark, markTarget, 10.f);
			anim::EaseLerp(state.slide, slideTarget, 16.f);
		}
		else
		{
			anim::EaseLerp(state.text, textTarget, 14.f);
			anim::EaseLerp(state.mark, markTarget, 10.f);
			anim::EaseLerp(state.slide, slideTarget, 16.f);
		}

		if (hovered)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		const float contentMaxX = window->ContentRegionRect.Max.x - window->Pos.x;
		PushFont(fonts::body, style::textSize);
		PushStyleColor(ImGuiCol_Text, GetColorU32(state.text));
		RenderTextClipped(textMin + ImVec2(5 + state.slide, 0), textMax + ImVec2(25, 0), label, nullptr, &labelSize, styleRef.SelectableTextAlign, &rect);
		RenderCheckMark(window->DrawList, ImVec2(textMin.x + contentMaxX - 23, textMax.y - 14), GetColorU32(state.mark), 10.f);
		PopStyleColor();
		PopFont();

		if (pressed && shouldCloseOnPress && (window->Flags & ImGuiWindowFlags_Popup))
		{
			CloseCurrentPopup();
		}

		return pressed;
	}

	bool BeginCombo(const char* label, const char* previewValue)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();

		const ImGuiNextWindowDataFlags backupNextWindowDataFlags = g.NextWindowData.HasFlags;
		g.NextWindowData.ClearFlags();
		if (window->SkipItems)
		{
			return false;
		}

		const ImGuiStyle& styleRef = g.Style;
		const ImGuiID id = window->GetID(label);

		const float boxWidth = 200.f;
		const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(boxWidth, 48));
		const ImRect clickable(window->DC.CursorPos + ImVec2(0, 22), window->DC.CursorPos + ImVec2(boxWidth, 48));
		ItemSize(rect, styleRef.FramePadding.y);
		if (!ItemAdd(clickable, id, &rect))
		{
			return false;
		}

		bool hovered = false;
		bool held = false;
		const bool pressed = ButtonBehavior(clickable, id, &hovered, &held);
		const ImGuiID popupId = ImHashStr("##ComboPopup", 0, id);
		bool isPopupOpen = IsPopupOpen(popupId, ImGuiPopupFlags_None);
		if (pressed && !isPopupOpen)
		{
			OpenPopupEx(popupId, ImGuiPopupFlags_None);
			isPopupOpen = true;
		}

		ComboState& state = anim::State<ComboState>(id);
		const ImVec4 backgroundTarget = isPopupOpen ? style::comboBgOpen : hovered ? style::comboBgHover : style::comboBgIdle;
		const ImVec4 textTarget = isPopupOpen ? style::comboTextOpen : hovered ? style::comboTextHover : style::comboTextIdle;
		const float openTarget = isPopupOpen ? 1.f : 0.f;
		const float arrowTarget = isPopupOpen ? 0.3f : 0.f;

		const bool isStale = (g.FrameCount - state.lastFrame) > 1;
		state.lastFrame = g.FrameCount;

		if (isStale)
		{
			state.background = backgroundTarget;
			state.text = textTarget;
			state.openAnim = openTarget;
			state.arrowAnim = arrowTarget;
		}
		else if (hovered)
		{
			state.background = backgroundTarget;
			state.text = textTarget;
			anim::EaseLerp(state.openAnim, openTarget, 14.f);
			anim::EaseLerp(state.arrowAnim, arrowTarget, 14.f);
		}
		else
		{
			anim::EaseLerp(state.background, backgroundTarget, 14.f);
			anim::EaseLerp(state.text, textTarget, 14.f);
			anim::EaseLerp(state.openAnim, openTarget, 14.f);
			anim::EaseLerp(state.arrowAnim, arrowTarget, 14.f);
		}

		window->DrawList->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(state.background), 3.f);
		window->DrawList->AddText(fonts::body, style::textSize, rect.Min, GetColorU32(ImGuiCol_Text), label);
		if (hovered)
		{
			SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		PushFont(fonts::body, style::textSize);
		RenderTextClipped(clickable.Min + ImVec2(8, 0), clickable.Max - ImVec2(24, 0), previewValue, nullptr, nullptr, ImVec2(0.f, 0.5f));
		PopFont();

		const float arrowSize = GetFrameHeight() + 2;
		const float arrowX = ImMax(clickable.Min.x, clickable.Max.x - arrowSize);
		const ImU32 arrowColor = GetColorU32(ImVec4(1.f, 1.f, 1.f, 0.6f + state.arrowAnim));
		window->DrawList->AddLine(ImVec2(arrowX + 5, clickable.Min.y + 10), ImVec2(arrowX + 8, clickable.Min.y + 14), arrowColor);
		window->DrawList->AddLine(ImVec2(arrowX + 8, clickable.Min.y + 14), ImVec2(arrowX + 12, clickable.Min.y + 9), arrowColor);
		window->DrawList->AddLine(ImVec2(arrowX + 4, clickable.Min.y + 10), ImVec2(arrowX + 8, clickable.Min.y + 15), arrowColor);
		window->DrawList->AddLine(ImVec2(arrowX + 8, clickable.Min.y + 15), ImVec2(arrowX + 13, clickable.Min.y + 9), arrowColor);

		if (!isPopupOpen)
		{
			return false;
		}

		g.NextWindowData.HasFlags = backupNextWindowDataFlags;
		if (!IsPopupOpen(popupId, ImGuiPopupFlags_None))
		{
			g.NextWindowData.ClearFlags();
			return false;
		}

		const float popupWidth = clickable.GetWidth();
		if (g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint)
		{
			g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, popupWidth);
		}
		else
		{
			SetNextWindowSizeConstraints(ImVec2(popupWidth, 0.f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(8) * state.openAnim));
		}

		char popupName[16];
		ImFormatString(popupName, IM_ARRAYSIZE(popupName), "##Combo_%02d", g.BeginPopupStack.Size);
		if (ImGuiWindow* popupWindow = FindWindowByName(popupName))
		{
			if (popupWindow->WasActive)
			{
				const ImVec2 sizeExpected = CalcWindowNextAutoFitSize(popupWindow);
				popupWindow->AutoPosLastDirection = ImGuiDir_Down;
				const ImRect allowedRect = GetPopupAllowedExtentRect(popupWindow);
				const ImVec2 popupPos = FindBestWindowPosForPopupEx(clickable.GetBL(), sizeExpected, &popupWindow->AutoPosLastDirection, allowedRect, clickable, ImGuiPopupPositionPolicy_ComboBox);
				SetNextWindowPos(popupPos + ImVec2(0, 3));
			}
		}

		const ImGuiWindowFlags popupFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
		PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 10));
		PushStyleVar(ImGuiStyleVar_PopupRounding, 6.f);
		PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.f);
		PushStyleColor(ImGuiCol_PopupBg, style::popupBg);

		const bool isPopupVisible = Begin(popupName, nullptr, popupFlags);

		PopStyleVar(3);
		PopStyleColor();

		if (!isPopupVisible)
		{
			EndPopup();
			IM_ASSERT(0);
			return false;
		}

		return true;
	}

	void EndCombo()
	{
		EndPopup();
	}

	bool Combo(const char* label, int* currentItem, bool (*itemsGetter)(void*, int, const char**), void* itemsSource, int itemCount, int popupMaxHeightInItems)
	{
		ImGuiContext& g = *GImGui;

		const char* previewValue = nullptr;
		if (*currentItem >= 0 && *currentItem < itemCount)
		{
			itemsGetter(itemsSource, *currentItem, &previewValue);
		}

		if (popupMaxHeightInItems != -1 && !(g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint))
		{
			SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popupMaxHeightInItems)));
		}

		if (!BeginCombo(label, previewValue))
		{
			return false;
		}

		bool valueChanged = false;
		for (int i = 0; i < itemCount; i++)
		{
			PushID(i);
			const bool isItemSelected = (i == *currentItem);
			const char* itemText;
			if (!itemsGetter(itemsSource, i, &itemText))
			{
				itemText = "*Unknown item*";
			}
			if (SelectableItem(itemText, isItemSelected))
			{
				valueChanged = true;
				*currentItem = i;
			}
			if (isItemSelected)
			{
				SetItemDefaultFocus();
			}
			PopID();
		}

		EndCombo();

		if (valueChanged)
		{
			MarkItemEdited(g.LastItemData.ID);
		}

		return valueChanged;
	}

	bool combo(const char* label, int* currentItem, const char* const items[], int itemCount, int heightInItems)
	{
		return Combo(label, currentItem, ItemsArrayGetter, (void*)items, itemCount, heightInItems);
	}

	bool MultiCombo(const char* label, bool selection[], const char* const items[], int itemCount)
	{
		ImGuiContext& g = *GImGui;

		char preview[128];
		int previewLength = 0;
		int selectedCount = 0;
		for (int i = 0; i < itemCount; i++)
		{
			if (!selection[i])
			{
				continue;
			}
			if (selectedCount > 0)
			{
				previewLength += ImFormatString(preview + previewLength, sizeof(preview) - (size_t)previewLength, ", ");
			}
			previewLength += ImFormatString(preview + previewLength, sizeof(preview) - (size_t)previewLength, "%s", items[i]);
			selectedCount++;
		}
		if (selectedCount == 0)
		{
			ImFormatString(preview, sizeof(preview), "...");
		}

		if (!(g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint))
		{
			SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(6)));
		}

		if (!BeginCombo(label, preview))
		{
			return false;
		}

		bool valueChanged = false;
		for (int i = 0; i < itemCount; i++)
		{
			PushID(i);
			if (SelectableItem(items[i], selection[i], false))
			{
				selection[i] = !selection[i];
				valueChanged = true;
			}
			PopID();
		}

		EndCombo();

		if (valueChanged)
		{
			MarkItemEdited(g.LastItemData.ID);
		}

		return valueChanged;
	}
}
