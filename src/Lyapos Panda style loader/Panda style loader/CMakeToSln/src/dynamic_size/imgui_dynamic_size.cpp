//
// Created by n08i40k on 08.04.2024.
//

#include "imgui_dynamic_size.h"

#include <algorithm>
#include <imgui.h>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>
#include <backends/imgui_impl_dx11.h>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "misc/freetype/imgui_freetype.h"
#include "fonts/icomoon_font.h"
#include "fonts/Font.h"

#include <windows.h>
#include <iostream>

ImGuiStyle *default_style = nullptr;
float imgui_dynamic_size::current_scale_factor = 1.f;
float previous_x = 1920;

float imgui_dynamic_size::calculate_scale_factor() {
//	 return 1.F;

    float x = ImGui::GetMainViewport()->Size.x;

    return min(2.F, max(current_scale_factor, x / 1920.F));
}

void
ScaleAllSizes(ImGuiStyle *p_style,
              const float scale_factor) {
    p_style->WindowPadding = p_style->WindowPadding * scale_factor;
    p_style->WindowRounding = p_style->WindowRounding * scale_factor;
    p_style->WindowMinSize = p_style->WindowMinSize * scale_factor;
    p_style->ChildRounding = p_style->ChildRounding * scale_factor;
    p_style->PopupRounding = p_style->PopupRounding * scale_factor;
    p_style->FramePadding = p_style->FramePadding * scale_factor;
    p_style->FrameRounding = p_style->FrameRounding * scale_factor;
    p_style->ItemSpacing = p_style->ItemSpacing * scale_factor;
    p_style->ItemInnerSpacing = p_style->ItemInnerSpacing * scale_factor;
    p_style->CellPadding = p_style->CellPadding * scale_factor;
    p_style->TouchExtraPadding = p_style->TouchExtraPadding * scale_factor;
    p_style->IndentSpacing = p_style->IndentSpacing * scale_factor;
    p_style->ColumnsMinSpacing = p_style->ColumnsMinSpacing * scale_factor;
    p_style->ScrollbarSize = p_style->ScrollbarSize * scale_factor;
    p_style->ScrollbarRounding = p_style->ScrollbarRounding * scale_factor;
    p_style->GrabMinSize = p_style->GrabMinSize * scale_factor;
    p_style->GrabRounding = p_style->GrabRounding * scale_factor;
    p_style->LogSliderDeadzone = p_style->LogSliderDeadzone * scale_factor;
    p_style->TabRounding = p_style->TabRounding * scale_factor;
    p_style->TabMinWidthForCloseButton = p_style->TabMinWidthForCloseButton != FLT_MAX
                                         ? p_style->TabMinWidthForCloseButton * scale_factor
                                         : FLT_MAX;
    p_style->SeparatorTextPadding = p_style->SeparatorTextPadding * scale_factor;
    p_style->DisplayWindowPadding = p_style->DisplayWindowPadding * scale_factor;
    p_style->DisplaySafeAreaPadding = p_style->DisplaySafeAreaPadding * scale_factor;
    p_style->MouseCursorScale = p_style->MouseCursorScale * scale_factor;
}

void
imgui_dynamic_size::scale_style() {
    if (default_style == nullptr) {
        default_style = new ImGuiStyle;
        memcpy(default_style, &ImGui::GetStyle(), sizeof(ImGuiStyle));
    }

    auto &style = ImGui::GetStyle();
    memcpy(&style, default_style, sizeof(ImGuiStyle));

    ScaleAllSizes(&style, current_scale_factor);
}

static bool font_modified = false;

void
imgui_dynamic_size::scale_font() {
    const auto &io = ImGui::GetIO();
    ImFontConfig fontConfig;
    fontConfig.OversampleH = 1; // Увеличение оверсэмплинга
    fontConfig.OversampleV = 1;
    fontConfig.PixelSnapH = true;
    fontConfig.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting | ImGuiFreeTypeBuilderFlags_LightHinting |  ImGuiFreeTypeBuilderFlags_LoadColor;

    const auto font_size = 17.0F * current_scale_factor;

    // const auto* p_old_font = io.Fonts->Fonts[0];
    // if (!p_old_font->IsLoaded())
    // 	return;

    io.Fonts->Clear();

    const auto *main_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(InterMedium_compressed_data_base85, font_size, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

    assert(main_font != nullptr);

    static ImWchar icomoon_ranges[] = {0x1, 0x10FFFD, 0};

    static ImFontConfig icomoon_config;
    icomoon_config.OversampleH = icomoon_config.OversampleV = 1;
    icomoon_config.MergeMode = true;
    icomoon_config.GlyphOffset.y = 2.5f;
    icomoon_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85,
                                                   font_size,
                                                   &icomoon_config,
                                                   icomoon_ranges);

    for (int i = 0; i < font_types.size(); ++i) {
        for (int j = 0; j < font_sizes.size(); ++j) {

            if (font_types[i] == "bold")
                bold_fonts[j] = io.Fonts->AddFontFromMemoryCompressedBase85TTF(InterBold_compressed_data_base85, font_sizes[j] * current_scale_factor, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

            if (font_types[i] == "semibold")
                semibold_fonts[j] = io.Fonts->AddFontFromMemoryCompressedBase85TTF(InterSemibold_compressed_data_base85, font_sizes[j] * current_scale_factor, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

            if (font_types[i] == "medium")
                medium_fonts[j] = io.Fonts->AddFontFromMemoryCompressedBase85TTF(InterMedium_compressed_data_base85, font_sizes[j] * current_scale_factor, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

            if (font_types[i] == "regular")
                regular_fonts[j] = io.Fonts->AddFontFromMemoryCompressedBase85TTF(InterRegular_base85, font_sizes[j] * current_scale_factor, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

            static ImWchar icomoon_ranges[] = {0x1, 0x10FFFD, 0};
            static ImFontConfig icomoon_config;
            icomoon_config.OversampleH = icomoon_config.OversampleV = 1;
            icomoon_config.MergeMode = true;
            icomoon_config.GlyphOffset.y = 4.5f;
            icomoon_config.GlyphExtraSpacing.y = 4.5f;
            icomoon_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
            io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85,
                                                           font_sizes[j] * current_scale_factor * 1.25f,
                                                           &icomoon_config,
                                                           icomoon_ranges);
        }
    }



    ImGui::GetIO().Fonts->Build();
    ImGui_ImplDX11_InvalidateDeviceObjects();

    // delete p_old_font;

    font_modified = true;
}

void
imgui_dynamic_size::scale_imgui() {
    const float current_x = ImGui::GetMainViewport()->Size.x;
    if (current_x == previous_x)
        return;
    previous_x = current_x;

    current_scale_factor = calculate_scale_factor();

    scale_style();
    scale_font();
}

std::vector<std::string> imgui_dynamic_size::font_types = {"regular", "semibold", "medium", "bold", "black"};
std::vector<float> imgui_dynamic_size::font_sizes = {11.0f, 13.0f, 15.0f, 17.0f, 20.0f, 22.0f, 24.0f, 26.0f, 28.0f,
                                                     30.0f};
