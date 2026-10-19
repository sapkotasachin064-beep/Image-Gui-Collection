#include "gui/framework/slider/slider.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const float kTrackGap = 5.f;
    static const float kTrackHeight = 8.f;
    static const float kFillBand = 2.f;
    static const float kKnobWidth = 5.f;
    static const float kKnobOverhang = 2.f;
    static const float kKnobPoint = 2.f;

    static bool SliderRow(const char* label, const char* valueText, float* fraction)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }
        if (!MatchesSearch(label))
        {
            return false;
        }

        const ImGuiID id = window->GetID(label);
        const char* labelEnd = FindRenderedTextEnd(label);
        const ImVec2 labelSize = CalcTextSize(label, labelEnd);

        const float trackGap = style::Scaled(kTrackGap);
        const float trackHeight = style::Scaled(kTrackHeight);

        const ImVec2 pos = window->DC.CursorPos;
        const float rowWidth = ImMax(window->WorkRect.Max.x - pos.x, 16.f);
        const ImRect bb(pos, pos + ImVec2(rowWidth, labelSize.y + trackGap + trackHeight));

        ItemSize(bb);
        if (!ItemAdd(bb, id))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        ButtonBehavior(bb, id, &isHovered, &isHeld);

        const float trackLeft = pos.x;
        const float trackRight = pos.x + rowWidth;

        bool hasChanged = false;
        if (isHeld)
        {
            const float grabbed = ImClamp((GetIO().MousePos.x - trackLeft) / (trackRight - trackLeft), 0.f, 1.f);
            if (grabbed != *fraction)
            {
                *fraction = grabbed;
                hasChanged = true;
                MarkItemEdited(id);
            }
        }

        const ImVec2 valueSize = CalcTextSize(valueText);

        ImDrawList* drawList = window->DrawList;
        drawList->AddText(pos, ImColor(style::textStrong), label, labelEnd);
        drawList->AddText(ImVec2(trackRight - valueSize.x, pos.y), ImColor(style::textStrong), valueText);

        const float trackTop = pos.y + labelSize.y + trackGap;
        const float trackBottom = trackTop + trackHeight;
        const float fillRight = trackLeft + (trackRight - trackLeft) * *fraction;

        drawList->AddRectFilled(ImVec2(trackLeft, trackTop), ImVec2(trackRight, trackBottom), ImColor(style::sliderTrack));

        const float fillBand = style::Scaled(kFillBand);
        if (fillRight > trackLeft + 1.f)
        {
            const float fillTop = trackTop + (trackHeight - fillBand) * 0.5f;

            drawList->AddRectFilled(
                ImVec2(trackLeft + 1.f, fillTop),
                ImVec2(ImMin(fillRight, trackRight - 1.f), fillTop + fillBand),
                ImColor(style::sliderFill));
        }

        drawList->AddRectFilled(ImVec2(trackLeft, trackTop), ImVec2(trackRight, trackTop + 1.f), ImColor(style::bevelDark));
        drawList->AddRectFilled(ImVec2(trackLeft, trackTop), ImVec2(trackLeft + 1.f, trackBottom), ImColor(style::bevelDark));
        drawList->AddRectFilled(ImVec2(trackLeft, trackBottom - 1.f), ImVec2(trackRight, trackBottom), ImColor(style::bevelLight));
        drawList->AddRectFilled(ImVec2(trackRight - 1.f, trackTop), ImVec2(trackRight, trackBottom), ImColor(style::bevelLight));

        const float knobHalfWidth = style::Scaled(kKnobWidth) * 0.5f;
        const float knobOverhang = style::Scaled(kKnobOverhang);
        const float knobCenterX = ImClamp(fillRight, trackLeft + knobHalfWidth, trackRight - knobHalfWidth);
        const float knobTop = trackTop - knobOverhang;
        const float knobBottom = trackBottom + knobOverhang;
        const float knobShoulder = knobBottom - style::Scaled(kKnobPoint);

        drawList->PathLineTo(ImVec2(knobCenterX - knobHalfWidth, knobTop));
        drawList->PathLineTo(ImVec2(knobCenterX + knobHalfWidth, knobTop));
        drawList->PathLineTo(ImVec2(knobCenterX + knobHalfWidth, knobShoulder));
        drawList->PathLineTo(ImVec2(knobCenterX, knobBottom));
        drawList->PathLineTo(ImVec2(knobCenterX - knobHalfWidth, knobShoulder));
        drawList->PathFillConvex(ImColor(style::sliderKnob));

        return hasChanged;
    }

    bool SliderInt(const char* label, int* value, int minValue, int maxValue, const char* format)
    {
        char valueText[64];
        ImFormatString(valueText, sizeof(valueText), format, *value);

        const float range = (float)(maxValue - minValue);
        float fraction = range > 0.f ? (float)(*value - minValue) / range : 0.f;

        if (!SliderRow(label, valueText, &fraction))
        {
            return false;
        }

        const int nextValue = minValue + (int)(fraction * range + 0.5f);
        if (nextValue == *value)
        {
            return false;
        }

        *value = nextValue;
        return true;
    }

    bool SliderFloat(const char* label, float* value, float minValue, float maxValue, const char* format)
    {
        char valueText[64];
        ImFormatString(valueText, sizeof(valueText), format, *value);

        const float range = maxValue - minValue;
        float fraction = range > 0.f ? (*value - minValue) / range : 0.f;

        if (!SliderRow(label, valueText, &fraction))
        {
            return false;
        }

        *value = minValue + fraction * range;
        return true;
    }

    bool SliderDistance(const char* label, int* value, int maxValue)
    {
        char valueText[64];
        if (*value >= maxValue)
        {
            ImStrncpy(valueText, "Unlimited", sizeof(valueText));
        }
        else
        {
            ImFormatString(valueText, sizeof(valueText), "%dm", *value);
        }

        float fraction = maxValue > 0 ? (float)*value / (float)maxValue : 0.f;

        if (!SliderRow(label, valueText, &fraction))
        {
            return false;
        }

        const int nextValue = (int)(fraction * (float)maxValue + 0.5f);
        if (nextValue == *value)
        {
            return false;
        }

        *value = nextValue;
        return true;
    }
}
