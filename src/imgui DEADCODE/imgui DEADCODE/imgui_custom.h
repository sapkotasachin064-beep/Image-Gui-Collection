#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui_internal.h>
#include "imgui_settings.h"
#include "imstb_textedit.h"

#include <cstdlib>
#include <imgui.h>

#include <map>

using namespace ImGui;

static const float          DRAG_MOUSE_THRESHOLD_FACTOR = 0.50f;

extern bool scroll_active;

namespace custom
{
	bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiWindowFlags flags = 0);
	bool BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), ImGuiWindowFlags flags = 0);
	bool BeginChild(const char* str_id, const ImVec2& size = ImVec2(0, 0), ImGuiWindowFlags flags = 0);
	void EndChild();

	void BeginGroup();
	void EndGroup();

	bool Checkbox(const char* name, bool *v);

	bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
	bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

	bool Page(bool selected, const char* font_icon, const char* name, const ImVec2& size = ImVec2(0, 0));
	bool SubPage(bool selected, const char* label, const char* information, const ImVec2& size = ImVec2(0, 0));

	bool Keybind(const char* label, int* key, int* mode);

	bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL, bool popup = false);
	bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);

	bool HueSlider(const char* label, float col[4], ImGuiColorEditFlags flags = 0);

	bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

	bool  BeginCombo(const char* label, const char* preview_value, int val = 0, bool multi = false, ImGuiComboFlags flags = 0);
	void  EndCombo();
	void  MultiCombo(const char* label, bool variable[], const char* labels[], int count);
	bool  Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	bool  Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
	bool  Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);

    bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));     

	bool QuandScalar(const char* label, ImGuiDataType data_type, void* p_data0, void* p_data1, const void* p_min0, const void* p_max0, const void* p_min1, const void* p_max1, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);
	bool QuandFloat(const char* label, float* v0, float* v1, float v_min0 = 0.0f, float v_max0 = 0.0f, float v_min1 = 0.0f, float v_max1 = 0.0f, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);
	bool QuandInt(const char* label, int* v0, int* v1, int v_min0 = 0, int v_max0 = 0, int v_min1 = 0, int v_max1 = 0, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);

	bool KnobScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool KnobFloat(const char* label, float* v, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%d", ImGuiSliderFlags flags = 0);
	bool KnobInt(const char* label, int* v, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
}