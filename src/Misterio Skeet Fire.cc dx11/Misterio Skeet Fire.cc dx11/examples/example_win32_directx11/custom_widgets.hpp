#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>
#include "widgets_colors.h"
#include <map>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <wtypes.h>
#include "imstb_textedit.h"

namespace custom
{
    bool            BeginChildEx(ImTextureID icon, const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool            BeginChild(ImTextureID icon, const char* str_id, const ImVec2& size_arg, bool border = false, ImGuiWindowFlags extra_flags = 0);
    bool            BeginChild(ImTextureID icon, ImGuiID id, const ImVec2& size_arg, bool border = false, ImGuiWindowFlags extra_flags = 0);
    void            EndChild();
    bool            Tab(const char* label, ImTextureID icon, bool active);
    bool            Checkbox(const char* label, bool* v);
        
    bool            SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool            SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool            SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 0);

    bool            ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    bool            ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    bool            ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

    bool		    Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool		    Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool		    BeginCombo(const char* label, const char* preview_value, int val = 0, bool multi = false, ImGuiComboFlags flags = 0);
    void		    EndCombo();
    void		    MultiCombo(const char* label, bool variable[], const char* labels[], int count);
    bool		    Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);
    bool		    Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool		    Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);

    
    bool            Keybind(const char* label, int* key, int* variable, bool widget_label);
    void            KeybindBox(const char* label, bool* v, int* variable, int* key);
    bool            Button(const char* label, const ImVec2& size = ImVec2(0, 0));
    bool            ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);

    void            Spacing(float h = 0.0f);

    void            GradientRect(ImDrawList* draw_list, const ImVec2 min, ImVec2 max, ImColor col_top_left, ImColor col_top_right, ImColor col_bot_right, ImColor col_bot_left, float thickness = 0, float rounding = 0);
}
