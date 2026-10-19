//
// Created by n08i40k on 07.05.2024.
//

#include "HeaderedChild.h"

#include "imgui_internal.h"

bool
ImGui::BeginHeaderedChild(const char* label,
                          const ImVec2& size,
                          const ImGuiChildFlags child_flags,
                          const ImGuiWindowFlags window_flags) {
	if (!BeginChild(label, size, child_flags, window_flags)) {
		EndChild();
		return false;
	}

	auto* const p_window = GetCurrentWindow();
	auto* const p_storage = p_window->DC.StateStorage;

	const auto state_key_id = ImHashStr("headered_group_state");
	assert(p_storage->GetInt(state_key_id, ImGuiHeaderedGroupState_None) == ImGuiHeaderedGroupState_None);
	p_storage->SetInt(state_key_id, ImGuiHeaderedGroupState_Header);

	if (const float y_size = p_storage->GetFloat(ImHashStr("headered_group_y"), -FLT_MAX);
		y_size != -FLT_MAX) {
		const auto& child_pos = p_window->Rect().Min;
		p_window->DrawList->AddRectFilled(child_pos,
		                                  child_pos + ImVec2{p_window->Size.x, y_size},
		                                  0xFF008080,
		                                  GetStyle().ChildRounding,
		                                  ImDrawFlags_RoundCornersTop);
	}

	return true;
}

void
ImGui::BeginHeaderedChildData() {
	auto* const p_window = GetCurrentWindow();
	auto* const p_storage = p_window->DC.StateStorage;

	const auto state_key_id = ImHashStr("headered_group_state");
	assert(p_storage->GetInt(state_key_id, ImGuiHeaderedGroupState_None) == ImGuiHeaderedGroupState_Header);
	p_storage->SetInt(state_key_id, ImGuiHeaderedGroupState_Data);

	p_storage->SetFloat(ImHashStr("headered_group_y"), GetCursorScreenPos().y - p_window->Pos.y);
}

void
ImGui::EndHeaderedChild() {
	auto* const p_window = GetCurrentWindow();
	auto* const p_storage = p_window->DC.StateStorage;

	const auto state_key_id = ImHashStr("headered_group_state");
	assert(p_storage->GetInt(state_key_id, ImGuiHeaderedGroupState_None) == ImGuiHeaderedGroupState_Data);
	p_storage->SetInt(state_key_id, ImGuiHeaderedGroupState_None);

	EndChild();
}
