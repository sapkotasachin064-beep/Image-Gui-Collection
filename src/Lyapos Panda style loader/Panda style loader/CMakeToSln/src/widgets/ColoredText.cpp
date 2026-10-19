#include "ColoredText.h"

#include <array>

#include "ext_main.h"
#include "imgui_internal.h"

#include <regex>

namespace ImGui {
void
ColoredText(const char* fmt,
	...) {
	va_list args;
	va_start(args, fmt);
	ColoredTextV(ColoredTextFlags_Align_Left, fmt, args);
	va_end(args);
}

void
ColoredTextEx(const ColoredTextFlags flags,
              const char* fmt,
              ...) {
	va_list args;
	va_start(args, fmt);
	ColoredTextV(flags, fmt, args);
	va_end(args);
}

void
ColoredTextV(ColoredTextFlags flags,
             const char* fmt,
             va_list args) {
	if (GetCurrentWindow()->SkipItems)
		return;

	std::string input; {
		const char* text;
		const char* text_end;
		ImFormatStringToTempBufferV(&text, &text_end, fmt, args);

		input = std::string(text, text_end);
	}

	std::vector<std::string> text_parts;
	std::vector<std::string> text_prefixes;

	// Алгоритм по получению префиксов и частей текста
	{
		struct prefix_match {
			size_t begin;
			size_t end;

			std::string match;
		};

		std::vector<prefix_match> matches{};

		// Получение списка цветовых кодов и их позиций в строке
		{
			static std::regex pattern("#[0-9a-fA-F]{6}");

			std::sregex_iterator iterator(input.cbegin(), input.cend(), pattern);
			std::sregex_iterator end;

			while (iterator != end) {
				const auto& match = *iterator;

				matches.emplace_back(
					match.position(),
					match.position() + match.length(),
					match.str()
				);

				++iterator;
			}
		}

		// Получение частей текста и отфильтровка префиксов для них
		{
			// Если в начале текста нету префикса - установка дефолтного
			if (matches.empty()) {
				text_prefixes.emplace_back("#000000");
				text_parts.emplace_back(input.begin(), input.end());
			}
			else {
				if (matches[0].begin > 0)
					text_prefixes.emplace_back("#000000");

				size_t offset = 0;
				for (auto& [begin, end, match] : matches) {
					if (begin > offset) {
						// Если перед префиксом есть текст (обычный, не другого префикса)
						text_prefixes.push_back(match);
						text_parts.emplace_back(
							std::offset(input.data(), offset),
							begin - offset);
					}
					else {
						if (begin == 0) // Если префикс в начале строки, то он добавляется в список
							text_prefixes.push_back(match);
						else // Иначе прошлый префикс заменяется новым (для случаев где 2 префикса подряд)
							text_prefixes[text_prefixes.size() - 1] = match;
					}

					offset = end;
				}

				if (input.size() > offset) // Если после последнего префикса ещё есть текст
					text_parts.emplace_back(
						std::offset(input.data(), offset),
						input.size() - offset);
			}
		}
	}

	if (flags != ColoredTextFlags_Align_Left) {
		std::string clear_string;

		std::ranges::for_each(text_parts,
		                      [&clear_string](const std::string& part) { clear_string.append(part); });

		float text_width = CalcTextSize(clear_string.c_str()).x;
		SetCursorPosX(
			(GetContentRegionMax().x - text_width) * (flags & ColoredTextFlags_Align_Center
				                                          ? 1.F / 2
				                                          : 1.F));
	}

	std::array<float, 3> source_color{};

	for (size_t i{}; i < text_parts.size(); ++i) {
		const char* prefixStart = text_prefixes[i].c_str();

		for (uint8_t j{}; j < 3; ++j) {
			std::array<char, 3> source_color_buffer{};
			memcpy(source_color_buffer.data(), std::offset(prefixStart, j * 2 + 1), 2);

			constexpr int radix = 16;
			constexpr int pixel_color = 256;

			source_color[j] = static_cast<float>(strtol(source_color_buffer.data(), nullptr, radix)) / pixel_color;
		}

		ImVec4 color;

		if ((source_color[0] != 0 || source_color[1] != 0 || source_color[2] != 0) &&
			(source_color[0] != 1 || source_color[1] != 1 || source_color[2] != 1))
			color = {source_color[0], source_color[1], source_color[2], 1};
		else
			color = GetStyle().Colors[ImGuiCol_Text];

		PushStyleColor(ImGuiCol_Text, color);
		TextEx(text_parts[i].c_str(), nullptr, ImGuiTextFlags_NoWidthForLargeClippedText);
		PopStyleColor();

		if (i + 1 != text_parts.size())
			SameLine(0.F, 0.F);
	}
}
}
