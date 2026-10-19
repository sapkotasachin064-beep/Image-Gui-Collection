//
// Created by n08i40k on 19.04.2024.
//

#include "SmoothColoredButton.h"

bool
ImGui::SmoothColoredButton(const char* label,
	const ImVec4& active_color,
	const ImVec4& default_color,
	const ImVec4& hovered_color,
	const ImVec2& size) {
	PushStyleColor(ImGuiCol_ButtonActive, active_color);
	PushStyleColor(ImGuiCol_Button, default_color);
	PushStyleColor(ImGuiCol_ButtonHovered, hovered_color);

	bool result = Button(label, size);

	PopStyleColor(3);

	return result;
}
