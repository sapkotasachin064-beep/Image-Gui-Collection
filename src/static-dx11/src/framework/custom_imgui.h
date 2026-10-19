#pragma once
#include "imgui.h"

namespace ksd
{
void Initialize(float fDpiScale);
void ApplyStyle();
ImFont* GetBoldFont();
ImFont* GetMediumFont();
ImFont* GetIconFont();
ImFont* GetLogoFont();
ImFont* GetStaticTitleFont();
ImFont* GetRegularFont();
void RenderMenuStrip(const char* const* ppText, const char* const* ppIcon, int iItemCount, int& iCurrentIndex, const ImVec2& vButtonSize, float fSpacing);
bool BeginCollapsibleSection(const char* strId, const char* label, float fX, float fY, float fHeaderHeight, float fWidth, bool bDefaultOpen = true);
void EndCollapsibleSection();
bool BeginGroupBox(const char* strId, const char* label, float fX, float fY, float fHeaderHeight, float fWidth, float fHeight = 0.f);
void EndGroupBox();
bool ButtonEx(const char* label, const ImVec2& sizeArg, ImGuiButtonFlags flags);
bool Button(const char* label, const ImVec2& sizeArg, ImGuiButtonFlags flags);
bool Button(const char* label, const ImVec2& sizeArg = ImVec2(0.f, 0.f));
bool BeginCombo(const char* label, const char* previewValue, ImGuiComboFlags flags = ImGuiComboFlags_None);
bool Selectable(const char* label, bool selected, const ImVec2& sizeArg = ImVec2(0.f, 0.f));
bool Combo(const char* label, int* currentItem, const char* (*getter)(void* userData, int iIndex), void* userData, int iItemCount, int iPopupMaxHeightInItems = -1);
bool Combo(const char* label, int* currentItem, const char* const items[], int iItemCount, int iHeightInItems = -1);
bool Combo(const char* label, int* currentItem, const char* itemsSeparatedByZeros, float fWidth, int iHeightInItems = -1);
bool Checkbox(const char* label, bool* pbValue);
bool SliderFloat(const char* displayName, float* pfValue, float fMin, float fMax, const char* fmt = "%.3f");
bool SliderInt(const char* displayName, int* piValue, int iMin, int iMax, const char* fmt = "%d");
bool InputText(const char* label, char* buf, int iBufSize, ImGuiInputTextFlags flags = 0);
bool ColorPickerCompact3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
bool ColorPicker(const char* label, float* pr, float* pg, float* pb);
bool ColorPicker2(
    const char* label,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1);
bool ColorPicker3(
    const char* label,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1,
    float* pr2, float* pg2, float* pb2);
bool CheckboxColorPicker1(const char* label, bool* pbValue, float* pr, float* pg, float* pb);
bool CheckboxColorPicker2(
    const char* label,
    bool* pbValue,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1);
bool CheckboxColorPicker3(
    const char* label,
    bool* pbValue,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1,
    float* pr2, float* pg2, float* pb2);
bool KeyBindRow(const char* label, int* piKey);
}
