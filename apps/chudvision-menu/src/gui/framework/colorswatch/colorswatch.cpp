#include "gui/framework/colorswatch/colorswatch.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const ImVec2 kSwatchSize(15.f, 6.f);
    static const float kPickerWidth = 150.f;
    static const float kBarHeight = 8.f;
    static const float kBarGap = 6.f;

    static void DrawBarHandle(ImDrawList* drawList, float centerX, float top, float bottom)
    {
        drawList->AddRectFilled(
            ImVec2(centerX - 2.f, top - 1.f),
            ImVec2(centerX + 2.f, bottom + 1.f), ImColor(style::bevelDark));
        drawList->AddRectFilled(
            ImVec2(centerX - 1.f, top),
            ImVec2(centerX + 1.f, bottom), ImColor(style::textStrong));
    }

    static bool ColorPickerBody(float rgba[4])
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        BeginGroup();

        ImGuiIO& io = GetIO();
        ImDrawList* drawList = window->DrawList;

        const float svSize = GetContentRegionAvail().x;
        const float barHeight = style::Scaled(kBarHeight);
        const float barGap = style::Scaled(kBarGap);

        float hue = 0.f;
        float sat = 0.f;
        float val = 0.f;
        ColorConvertRGBtoHSV(rgba[0], rgba[1], rgba[2], hue, sat, val);

        static ImGuiID activePickerId = 0;
        static float savedHue = 0.f;
        static float savedSat = 0.f;
        if (activePickerId != window->ID)
        {
            activePickerId = window->ID;
            savedHue = hue;
            savedSat = sat;
        }
        if (sat <= 0.f)
        {
            hue = savedHue;
        }
        if (val <= 0.f)
        {
            sat = savedSat;
        }

        bool hasChanged = false;

        InvisibleButton("##sv", ImVec2(svSize, svSize));
        const bool isSvActive = IsItemActive();
        const ImVec2 svMin = GetItemRectMin();
        const ImVec2 svMax = GetItemRectMax();

        SetCursorScreenPos(ImVec2(svMin.x, svMax.y + barGap));
        InvisibleButton("##hue", ImVec2(svSize, barHeight));
        const bool isHueActive = IsItemActive();
        const ImVec2 hueMin = GetItemRectMin();
        const ImVec2 hueMax = GetItemRectMax();

        SetCursorScreenPos(ImVec2(svMin.x, hueMax.y + barGap));
        InvisibleButton("##alpha", ImVec2(svSize, barHeight));
        const bool isAlphaActive = IsItemActive();
        const ImVec2 alphaMin = GetItemRectMin();
        const ImVec2 alphaMax = GetItemRectMax();

        if (isSvActive)
        {
            sat = ImSaturate((io.MousePos.x - svMin.x) / (svSize - 1.f));
            val = 1.f - ImSaturate((io.MousePos.y - svMin.y) / (svSize - 1.f));
            savedSat = sat;
            ColorConvertHSVtoRGB(hue, sat, val, rgba[0], rgba[1], rgba[2]);
            hasChanged = true;
        }
        if (isHueActive)
        {
            hue = ImMin(ImSaturate((io.MousePos.x - hueMin.x) / (hueMax.x - hueMin.x)), 0.9999f);
            savedHue = hue;
            ColorConvertHSVtoRGB(hue, sat, val, rgba[0], rgba[1], rgba[2]);
            hasChanged = true;
        }
        if (isAlphaActive)
        {
            rgba[3] = ImSaturate((io.MousePos.x - alphaMin.x) / (alphaMax.x - alphaMin.x));
            hasChanged = true;
        }

        ImVec4 hueColor(1.f, 1.f, 1.f, 1.f);
        ColorConvertHSVtoRGB(hue, 1.f, 1.f, hueColor.x, hueColor.y, hueColor.z);
        const ImU32 huePacked = GetColorU32(hueColor);

        drawList->AddRectFilledMultiColor(svMin, svMax, IM_COL32_WHITE, huePacked, huePacked, IM_COL32_WHITE);
        drawList->AddRectFilledMultiColor(svMin, svMax, 0, 0, IM_COL32_BLACK, IM_COL32_BLACK);
        drawList->AddRect(svMin, svMax, ImColor(style::bevelDark));

        const ImVec2 svCursor(
            ImClamp(IM_ROUND(svMin.x + sat * svSize), svMin.x + 3.f, svMax.x - 3.f),
            ImClamp(IM_ROUND(svMin.y + (1.f - val) * svSize), svMin.y + 3.f, svMax.y - 3.f));
        drawList->AddRect(svCursor - ImVec2(3.f, 3.f), svCursor + ImVec2(3.f, 3.f), ImColor(style::bevelDark));
        drawList->AddRect(svCursor - ImVec2(2.f, 2.f), svCursor + ImVec2(2.f, 2.f), ImColor(style::textStrong));

        static const ImU32 hueColors[7] = {
            IM_COL32(255, 0, 0, 255),
            IM_COL32(255, 255, 0, 255),
            IM_COL32(0, 255, 0, 255),
            IM_COL32(0, 255, 255, 255),
            IM_COL32(0, 0, 255, 255),
            IM_COL32(255, 0, 255, 255),
            IM_COL32(255, 0, 0, 255),
        };
        const float segmentWidth = (hueMax.x - hueMin.x) / 6.f;
        for (int segment = 0; segment < 6; segment++)
        {
            drawList->AddRectFilledMultiColor(
                ImVec2(hueMin.x + segment * segmentWidth, hueMin.y),
                ImVec2(hueMin.x + (segment + 1) * segmentWidth, hueMax.y),
                hueColors[segment], hueColors[segment + 1], hueColors[segment + 1], hueColors[segment]);
        }
        drawList->AddRect(hueMin, hueMax, ImColor(style::bevelDark));

        const float hueCenterX = ImClamp(IM_ROUND(hueMin.x + hue * (hueMax.x - hueMin.x)), hueMin.x + 3.f, hueMax.x - 3.f);
        DrawBarHandle(drawList, hueCenterX, hueMin.y, hueMax.y);

        RenderColorRectWithAlphaCheckerboard(drawList, alphaMin, alphaMax, 0, style::Scaled(4.f), ImVec2(0.f, 0.f));

        const int red8 = IM_F32_TO_INT8_SAT(rgba[0]);
        const int green8 = IM_F32_TO_INT8_SAT(rgba[1]);
        const int blue8 = IM_F32_TO_INT8_SAT(rgba[2]);
        drawList->AddRectFilledMultiColor(alphaMin, alphaMax,
            IM_COL32(red8, green8, blue8, 0),
            IM_COL32(red8, green8, blue8, 255),
            IM_COL32(red8, green8, blue8, 255),
            IM_COL32(red8, green8, blue8, 0));
        drawList->AddRect(alphaMin, alphaMax, ImColor(style::bevelDark));

        const float alphaCenterX = ImClamp(IM_ROUND(alphaMin.x + rgba[3] * (alphaMax.x - alphaMin.x)), alphaMin.x + 3.f, alphaMax.x - 3.f);
        DrawBarHandle(drawList, alphaCenterX, alphaMin.y, alphaMax.y);

        EndGroup();
        return hasChanged;
    }

    bool ColorSwatch(const char* id, float color[4])
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        if (IsRowFiltered())
        {
            return false;
        }

        const ImVec2 swatchSize(style::Scaled(kSwatchSize.x), style::Scaled(kSwatchSize.y));
        PlaceRight(swatchSize);

        const ImGuiID itemId = window->GetID(id);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + swatchSize);

        ItemSize(bb);
        if (!ItemAdd(bb, itemId))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        if (ButtonBehavior(bb, itemId, &isHovered, &isHeld))
        {
            OpenPopup(id);
        }

        ImDrawList* drawList = window->DrawList;
        RenderColorRectWithAlphaCheckerboard(
            drawList, bb.Min, bb.Max,
            ImColor(color[0], color[1], color[2], color[3]),
            style::Scaled(3.f), ImVec2(0.f, 0.f));

        bool hasChanged = false;
        if (IsPopupOpen(id))
        {
            SetNextWindowPos(ImVec2(bb.Max.x, bb.Max.y + style::Scaled(2.f)), ImGuiCond_Always, ImVec2(1.f, 0.f));
            SetNextWindowSize(ImVec2(style::Scaled(kPickerWidth), 0.f), ImGuiCond_Always);
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style::Scaled(6.f), style::Scaled(6.f)));
            if (BeginPopup(id, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
            {
                hasChanged = ColorPickerBody(color);
                EndPopup();
            }
            PopStyleVar();
        }
        return hasChanged;
    }
}
