#include "CenteredText.h"

#include "imgui_internal.h"

namespace ImGui {
void CenteredText(const char *fmt, ...) {
	if (const auto *const window = GetCurrentWindow(); window->SkipItems)
		return;

	const char *text;
	const char *text_end;
	{
		va_list args;
			va_start(args, fmt);
		ImFormatStringToTempBufferV(&text, &text_end, fmt, args);
			va_end(args);
	}
	constexpr float delimier = 0.5F;
	SetCursorPosX((GetWindowSize().x - CalcTextSize(text, text_end).x) * delimier);

	TextEx(text, text_end, ImGuiTextFlags_NoWidthForLargeClippedText);

	SameLine(0, 0);
	SetCursorPosX(GetWindowSize().x);
	NewLine();
	SetCursorPosX(0);
}
}
