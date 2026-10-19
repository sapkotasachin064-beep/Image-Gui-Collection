
#include "HelpMarker.h"
#include "imgui_internal.h"

namespace ImGui {
void HelpMarker(const char *fmt, ...) {
	TextDisabled("(?)");
	if (IsItemHovered()) {
		BeginTooltip();
		PushTextWrapPos(GetFontSize() * 35.F); { // NOLINT(*-magic-numbers)
			const char *text;
			const char *text_end;
			va_list args;
			va_start(args, fmt);
			ImFormatStringToTempBufferV(&text, &text_end, fmt, args);
			va_end(args);

			TextUnformatted(text, text_end);
		}
		PopTextWrapPos();
		EndTooltip();
	}
}

/*ImGuiCol GetStyleColorId(const char* name)
{
	if (strcmp(name, "Text") == 0) return ImGuiCol_Text;
	if (strcmp(name, "TextDisabled") == 0) return ImGuiCol_TextDisabled;
	if (strcmp(name, "WindowBg") == 0) return ImGuiCol_WindowBg;
	if (strcmp(name, "ChildBg") == 0) return ImGuiCol_ChildBg;
	if (strcmp(name, "PopupBg") == 0) return ImGuiCol_PopupBg;
	if (strcmp(name, "Border") == 0) return ImGuiCol_Border;
	if (strcmp(name, "BorderShadow") == 0) return ImGuiCol_BorderShadow;
	if (strcmp(name, "FrameBg") == 0) return ImGuiCol_FrameBg;
	if (strcmp(name, "FrameBgHovered") == 0) return ImGuiCol_FrameBgHovered;
	if (strcmp(name, "FrameBgActive") == 0) return ImGuiCol_FrameBgActive;
	if (strcmp(name, "TitleBg") == 0) return ImGuiCol_TitleBg;
	if (strcmp(name, "TitleBgActive") == 0) return ImGuiCol_TitleBgActive;
	if (strcmp(name, "TitleBgCollapsed") == 0) return ImGuiCol_TitleBgCollapsed;
	if (strcmp(name, "MenuBarBg") == 0) return ImGuiCol_MenuBarBg;
	if (strcmp(name, "ScrollbarBg") == 0) return ImGuiCol_ScrollbarBg;
	if (strcmp(name, "ScrollbarGrab") == 0) return ImGuiCol_ScrollbarGrab;
	if (strcmp(name, "ScrollbarGrabHovered") == 0) return ImGuiCol_ScrollbarGrabHovered;
	if (strcmp(name, "ScrollbarGrabActive") == 0) return ImGuiCol_ScrollbarGrabActive;
	if (strcmp(name, "CheckMark") == 0) return ImGuiCol_CheckMark;
	if (strcmp(name, "SliderGrab") == 0) return ImGuiCol_SliderGrab;
	if (strcmp(name, "SliderGrabActive") == 0) return ImGuiCol_SliderGrabActive;
	if (strcmp(name, "Button") == 0) return ImGuiCol_Button;
	if (strcmp(name, "ButtonHovered") == 0) return ImGuiCol_ButtonHovered;
	if (strcmp(name, "ButtonActive") == 0) return ImGuiCol_ButtonActive;
	if (strcmp(name, "Header") == 0) return ImGuiCol_Header;
	if (strcmp(name, "HeaderHovered") == 0) return ImGuiCol_HeaderHovered;
	if (strcmp(name, "HeaderActive") == 0) return ImGuiCol_HeaderActive;
	if (strcmp(name, "Separator") == 0) return ImGuiCol_Separator;
	if (strcmp(name, "SeparatorHovered") == 0) return ImGuiCol_SeparatorHovered;
	if (strcmp(name, "SeparatorActive") == 0) return ImGuiCol_SeparatorActive;
	if (strcmp(name, "ResizeGrip") == 0) return ImGuiCol_ResizeGrip;
	if (strcmp(name, "ResizeGripHovered") == 0) return ImGuiCol_ResizeGripHovered;
	if (strcmp(name, "ResizeGripActive") == 0) return ImGuiCol_ResizeGripActive;
	if (strcmp(name, "Tab") == 0) return ImGuiCol_Tab;
	if (strcmp(name, "TabHovered") == 0) return ImGuiCol_TabHovered;
	if (strcmp(name, "TabActive") == 0) return ImGuiCol_TabActive;
	if (strcmp(name, "TabUnfocused") == 0) return ImGuiCol_TabUnfocused;
	if (strcmp(name, "TabUnfocusedActive") == 0) return ImGuiCol_TabUnfocusedActive;
	if (strcmp(name, "PlotLines") == 0) return ImGuiCol_PlotLines;
	if (strcmp(name, "PlotLinesHovered") == 0) return ImGuiCol_PlotLinesHovered;
	if (strcmp(name, "PlotHistogram") == 0) return ImGuiCol_PlotHistogram;
	if (strcmp(name, "PlotHistogramHovered") == 0) return ImGuiCol_PlotHistogramHovered;
	if (strcmp(name, "TableHeaderBg") == 0) return ImGuiCol_TableHeaderBg;
	if (strcmp(name, "TableBorderStrong") == 0) return ImGuiCol_TableBorderStrong;
	if (strcmp(name, "TableBorderLight") == 0) return ImGuiCol_TableBorderLight;
	if (strcmp(name, "TableRowBg") == 0) return ImGuiCol_TableRowBg;
	if (strcmp(name, "TableRowBgAlt") == 0) return ImGuiCol_TableRowBgAlt;
	if (strcmp(name, "TextSelectedBg") == 0) return ImGuiCol_TextSelectedBg;
	if (strcmp(name, "DragDropTarget") == 0) return ImGuiCol_DragDropTarget;
	if (strcmp(name, "NavHighlight") == 0) return ImGuiCol_NavHighlight;
	if (strcmp(name, "NavWindowingHighlight") == 0) return ImGuiCol_NavWindowingHighlight;
	if (strcmp(name, "NavWindowingDimBg") == 0) return ImGuiCol_NavWindowingDimBg;
	if (strcmp(name, "ModalWindowDimBg") == 0) return ImGuiCol_ModalWindowDimBg;
	IM_ASSERT(0);
	return 0;
}*/
}
