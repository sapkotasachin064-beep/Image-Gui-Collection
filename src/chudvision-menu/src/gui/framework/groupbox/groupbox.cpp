#include "gui/framework/groupbox/groupbox.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/style/style.hpp"

namespace ksd
{
    using namespace ImGui;

    static const float kPadX = 9.f;
    static const float kPadY = 4.f;
    static const float kCaptionInset = 7.f;
    static const float kCaptionGap = 4.f;
    static const ImVec2 kItemSpacing(6.f, 2.f);

    struct GroupBoxFrame
    {
        ImVec2 origin;
        char caption[64];
        bool isCaptionCentered;
    };

    static ImVector<GroupBoxFrame> groupBoxStack;

    bool BeginGroupBox(const char* label, const ImVec2& size, bool isCaptionCentered)
    {
        ImGuiWindow* window = GetCurrentWindow();

        GroupBoxFrame frame;
        frame.origin = window->DC.CursorPos;
        frame.isCaptionCentered = isCaptionCentered;

        const char* captionEnd = FindRenderedTextEnd(label);
        ImStrncpy(frame.caption, label, ImMin((size_t)(captionEnd - label) + 1, sizeof(frame.caption)));
        groupBoxStack.push_back(frame);

        ImGuiChildFlags childFlags = ImGuiChildFlags_AlwaysUseWindowPadding;
        if (size.y == 0.f)
        {
            childFlags |= ImGuiChildFlags_AutoResizeY;
        }

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style::Scaled(kPadX), style::Scaled(kPadY)));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style::Scaled(kItemSpacing.x), style::Scaled(kItemSpacing.y)));

        const bool isOpen = BeginChild(label, size, childFlags, ImGuiWindowFlags_NoScrollbar);

        ImGuiWindow* childWindow = GetCurrentWindow();
        PushClipRect(
            ImVec2(childWindow->ClipRect.Min.x, childWindow->Pos.y + style::Scaled(style::textSize)),
            ImVec2(childWindow->ClipRect.Max.x, childWindow->Pos.y + childWindow->Size.y - style::Scaled(3.f)),
            true);

        if (isOpen)
        {
            SetCursorPosY(GetCursorPosY() + style::Scaled(style::textSize) * 0.5f + style::Scaled(3.f));

            ImGuiWindow* boxWindow = GetCurrentWindow();
            boxWindow->DC.CursorMaxPos.y = ImMax(boxWindow->DC.CursorMaxPos.y, boxWindow->DC.CursorPos.y);
            boxWindow->DC.IsSetPos = false;
        }
        return isOpen;
    }

    bool BeginColumnBox(const char* label, float heightRatio)
    {
        return BeginGroupBox(label, ImVec2(ColumnWidth(), ColumnBoxHeight(heightRatio)));
    }

    void EndGroupBox()
    {
        PopClipRect();
        EndChild();
        PopStyleVar(2);

        const GroupBoxFrame frame = groupBoxStack.back();
        groupBoxStack.pop_back();

        const ImVec2 rectMin = GetItemRectMin();
        const ImVec2 rectMax = GetItemRectMax();
        const float borderTop = frame.origin.y + style::Scaled(style::textSize) * 0.5f;

        ImDrawList* drawList = GetWindowDrawList();
        drawList->AddRectFilled(ImVec2(rectMin.x, borderTop), rectMax, ImColor(style::panelBg));

        const float rounding = style::Scaled(2.f);
        drawList->AddRect(
            ImVec2(rectMin.x + 1.f, borderTop + 1.f), rectMax,
            ImColor(style::stroke), rounding);
        drawList->AddRect(
            ImVec2(rectMin.x, borderTop), ImVec2(rectMax.x - 1.f, rectMax.y - 1.f),
            ImColor(style::bevelDark), rounding);

        PushFont(fonts::bold, style::Scaled(style::textSize));

        const ImVec2 captionSize = CalcTextSize(frame.caption);
        const float captionGap = style::Scaled(kCaptionGap);

        const float captionX = IM_TRUNC(frame.isCaptionCentered
            ? rectMin.x + (rectMax.x - rectMin.x - captionSize.x) * 0.5f
            : rectMin.x + style::Scaled(kCaptionInset));

        drawList->AddRectFilled(
            ImVec2(captionX - captionGap, borderTop - 1.f),
            ImVec2(captionX + captionSize.x + captionGap, borderTop + 2.f),
            ImColor(style::windowBg));
        const ImU32 captionColor = ImColor(style::textStrong);
        const float captionY = IM_TRUNC(frame.origin.y);
        drawList->AddText(ImVec2(captionX, captionY), captionColor, frame.caption);

        PopFont();
    }
}
