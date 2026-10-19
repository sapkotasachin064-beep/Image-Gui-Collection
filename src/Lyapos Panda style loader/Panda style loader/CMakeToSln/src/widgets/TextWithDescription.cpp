//
// Created by n08i40k on 14.04.2024.
//

#include "TextWithDescription.h"

#include <string>
#include <vector>

#include "imgui_internal.h"
#include "dynamic_size/imgui_dynamic_size.h"

void
ImGui::TextWithDescription(
	const char* fmt,
	...) {
	va_list args;
	va_start(args, fmt);
	TextWithDescriptionV(fmt, args);
	va_end(args);
}

void
ImGui::TextWithDescriptionV(const char* fmt,
                            const va_list args) {
	if (GetCurrentWindow()->SkipItems)
		return;

	if (!std::string(fmt).contains('\n')) {
		TextV(fmt, args);
		return;
	}

	const char* text;
	const char* text_end;
	ImFormatStringToTempBufferV(&text, &text_end, fmt, args);

	const std::string input(text, text_end);

	const auto next_line_index = input.find('\n');
	const auto main_text = input.substr(0, next_line_index);
	const auto description_text = input.substr(next_line_index);

	const float cursor_x = GetCursorPosX();
	Text("%s", main_text.c_str());
	SetCursorPosX(cursor_x);

	const float main_line_height = GetTextLineHeight();

	PushStyleColor(ImGuiCol_Text, GetColorU32(ImGuiCol_TextDescription));
	PushFont(imgui_dynamic_size::medium_fonts[3]);
	SetCursorPosY(GetCursorPosY() - (main_line_height - GetTextLineHeight()) - GetStyle().ItemSpacing.y * 2);
	Text("%s", description_text.c_str());
	PopFont();
	PopStyleColor();
}
