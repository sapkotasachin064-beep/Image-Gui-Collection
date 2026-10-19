#include "gui/framework/tab/tab.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const int kCapRows = 3;
    static const float kCornerCut = 2.f;

    bool TopTab(const char* label, bool isActive, float padX)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        const ImGuiID id = window->GetID(label);
        const char* labelEnd = FindRenderedTextEnd(label);
        const ImVec2 labelSize = CalcTextSize(label, labelEnd);

        const float bodyHeight = style::Scaled(style::tabBarHeight);
        const float pad = style::Scaled(padX);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + ImVec2(labelSize.x + pad * 2.f, bodyHeight));

        ItemSize(bb);
        if (!ItemAdd(bb, id))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        const bool isPressed = ButtonBehavior(bb, id, &isHovered, &isHeld);

        ImDrawList* drawList = window->DrawList;
        const float edge = style::Scaled(1.f);
        const ImU32 fillColor = ImColor(style::windowBg);

        const ImDrawListFlags savedFlags = drawList->Flags;
        drawList->Flags &= ~(ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill);

        if (isActive)
        {
            const ImVec4 capColors[kCapRows] = { style::accentBarDark, style::accentBar, style::accentBarBright };
            for (int row = 0; row < kCapRows; row++)
            {
                const float inset = style::Scaled(float(kCapRows - row));
                const float rowTop = bb.Min.y - edge * float(kCapRows - row);
                drawList->AddRectFilled(
                    ImVec2(bb.Min.x + inset, rowTop),
                    ImVec2(bb.Max.x - inset, rowTop + edge),
                    ImColor(capColors[row]));
            }

            drawList->AddRectFilled(bb.Min, ImVec2(bb.Max.x, bb.Max.y + 1.f), fillColor);

            drawList->AddRectFilled(bb.Min, ImVec2(bb.Max.x - edge, bb.Min.y + edge), ImColor(style::tabHighlight));
            drawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + edge, bb.Max.y + 1.f), ImColor(style::tabBorder));
        }
        else
        {
            const float cornerCut = style::Scaled(kCornerCut);

            drawList->AddRectFilled(bb.Min, bb.Max, fillColor, cornerCut, ImDrawFlags_RoundCornersTopLeft);

            drawList->AddRectFilled(
                ImVec2(bb.Min.x + cornerCut, bb.Min.y),
                ImVec2(bb.Max.x, bb.Min.y + edge), ImColor(style::tabBorder));
            drawList->AddRectFilled(
                ImVec2(bb.Min.x, bb.Min.y + cornerCut),
                ImVec2(bb.Min.x + edge, bb.Max.y), ImColor(style::tabBorder));
            drawList->AddRectFilled(
                ImVec2(bb.Min.x + edge, bb.Min.y + edge),
                ImVec2(bb.Min.x + cornerCut, bb.Min.y + cornerCut), ImColor(style::tabBorder));
        }

        drawList->AddRectFilled(
            ImVec2(bb.Max.x - edge, bb.Min.y + edge),
            ImVec2(bb.Max.x, bb.Max.y), ImColor(style::windowBorder));

        drawList->Flags = savedFlags;

        ImVec4 labelColor = style::textStrong;
        if (isActive)
        {
            labelColor = style::accentBarBright;
        }

        drawList->AddText(
            ImVec2(bb.Min.x + pad, bb.Min.y + (bodyHeight - labelSize.y) * 0.5f),
            ImColor(labelColor), label, labelEnd);

        return isPressed;
    }
}
