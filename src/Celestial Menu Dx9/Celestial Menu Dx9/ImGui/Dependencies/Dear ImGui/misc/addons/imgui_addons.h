#pragma once

#include "imgui.h"
#include "imgui_internal.h"





namespace ImAdd
{
    // Configs
    inline float fAnimationSpeed = 0.09f;

    // Helpers
    ImVec4  HexToColorVec4(unsigned int hex_color, float alpha);

    // Clickables
	bool    Keybind(const char* label, int* key);
	bool    Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
	bool    TextButton(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
	bool    AcentButton(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
	bool    RadioButton(const char* label, int* v, int current_id, const ImVec2& size_arg = ImVec2(0, 0));
    bool    RadioButtonIcon(const char* str_id, const char* icon, const char* label, int* v, int current_id, const ImVec2& size_arg = ImVec2(0, 0));

    // Toggles
	bool    Checkbox(const char* label, bool* v);
	bool    SmallCheckbox(const char* label, bool* v);

	// Color Picker
	//bool    ColorEdit3(const char* label, float col[3]);
	bool    ColorEdit4(const char* label, float col[4]);

	// Separators
	void    SeparatorText(const char* label);

	// Inputs
	bool    InputText(const char* label, const char* text, char* buf, size_t buf_size, float width = NULL, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

	// Combo

	bool			Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool		    Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));


	bool		    BeginCombo(const char* label, const char* description, const char* preview_value, int val = 0, bool multi = false, ImGuiComboFlags flags = 0);
	void		    EndCombo();
	void		    MultiCombo(const char* label, const char* description, bool variable[], const char* labels[], int count);
	bool		    Combo(const char* label, const char* description, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);
	bool		    Combo(const char* label, const char* description, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	bool		    Combo(const char* label, const char* description, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);

	// Sliders
	bool    SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL);
	bool    SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f");
	bool    SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d");

}
