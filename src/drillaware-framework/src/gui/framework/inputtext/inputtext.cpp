#include "gui/framework/inputtext/inputtext.hpp"
#include "gui/framework/common.hpp"
#include "gui/framework/anim.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/fonts/fonts.hpp"

namespace ksd
{
	using namespace ImGui;

	struct CaretState
	{
		float x;
		int lastFrame;
	};

	bool InputText(const char* label, char* buffer, int bufferSize)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
		{
			return false;
		}

		ImGuiContext& g = *GImGui;
		const float boxWidth = 200.f;
		const float boxHeight = 26.f;
		const char* labelEnd = FindRenderedTextEnd(label);

		PushFont(fonts::body, style::textSize);

		window->DrawList->AddText(window->DC.CursorPos, GetColorU32(ImGuiCol_Text), label, labelEnd);
		SetCursorPos(GetCursorPos() + ImVec2(0, 22));

		SetNextItemWidth(boxWidth);
		PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, (boxHeight - style::textSize) * 0.5f));
		PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(style::accent.x, style::accent.y, style::accent.z, 0.30f));
		PushStyleColor(ImGuiCol_InputTextCursor, ImVec4(0, 0, 0, 0));

		PushID(label);
		const bool isChanged = ImGui::InputText("##field", buffer, bufferSize);
		const ImGuiID fieldId = GetItemID();
		PopID();

		PopStyleColor(2);
		PopStyleVar(2);

		if (g.ActiveId == fieldId)
		{
			ImGuiInputTextState* editState = GetInputTextState(fieldId);
			if (editState != nullptr)
			{
				const char* editText = editState->GetText();
				const float textWidth = CalcTextSize(editText, editText + editState->GetCursorPos()).x;
				const ImRect frameRect = g.LastItemData.Rect;
				const float targetX = frameRect.Min.x + 8.f + textWidth - editState->Scroll.x;

				CaretState& caret = anim::State<CaretState>(fieldId);
				const bool isStale = (g.FrameCount - caret.lastFrame) > 1;
				caret.lastFrame = g.FrameCount;

				if (isStale)
				{
					caret.x = targetX;
				}
				else
				{
					anim::EaseLerp(caret.x, targetX, 25.f);
				}

				const bool isCaretVisible = editState->CursorAnim <= 0.f || ImFmod(editState->CursorAnim, 1.20f) <= 0.80f;
				if (isCaretVisible)
				{
					const float caretTop = frameRect.Min.y + (frameRect.GetHeight() - g.FontSize) * 0.5f;
					window->DrawList->AddRectFilled(ImVec2(caret.x, caretTop), ImVec2(caret.x + 1.5f, caretTop + g.FontSize), GetColorU32(style::textWhite), 1.f);
				}
			}
		}

		PopFont();
		return isChanged;
	}
}
