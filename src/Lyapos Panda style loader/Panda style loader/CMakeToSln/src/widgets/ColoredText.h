#pragma once

#include <cstdarg>

namespace ImGui {
	enum ColoredTextFlags {
		ColoredTextFlags_Align_Left = 0,
		ColoredTextFlags_Align_Center = 1 << 0,
		ColoredTextFlags_Align_Right = 1 << 1
	};
	void ColoredTextEx(ColoredTextFlags flags, const char* fmt, ...);
	void ColoredText(const char* fmt, ...);

    void ColoredTextV(ColoredTextFlags flags, const char* fmt, va_list args);
}