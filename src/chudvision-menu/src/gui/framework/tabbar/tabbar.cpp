#include "gui/framework/tabbar/tabbar.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/tab/tab.hpp"

namespace ksd
{
    using namespace ImGui;

    static const float kCapReserve = 3.f;
    static const float kBladePad = 5.f;

    struct TabBarFrame
    {
        ImGuiID id;
        int selected;
        int index;
        ImVec2 stripStart;
        float bladeX;
        ImVec2 contentStart;
        ImVec2 contentEnd;
        bool hasContent;
    };

    static ImVector<TabBarFrame> tabBarStack;

    static void CommitCursor()
    {
        ImGuiWindow* window = GetCurrentWindow();
        window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
        window->DC.IsSetPos = false;
    }

    bool BeginTabBar(const char* id)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        TabBarFrame frame;
        frame.id = window->GetID(id);
        frame.selected = GetStateStorage()->GetInt(frame.id, 0);
        frame.index = 0;
        frame.bladeX = 0.f;
        frame.hasContent = false;

        SetCursorPosY(GetCursorPosY() + style::Scaled(kCapReserve));
        frame.stripStart = window->DC.CursorPos;

        const float stripWidth = GetContentRegionAvail().x;
        const float baselineY = frame.stripStart.y + style::Scaled(style::tabBarHeight);
        window->DrawList->AddLine(
            ImVec2(frame.stripStart.x, baselineY),
            ImVec2(frame.stripStart.x + stripWidth, baselineY),
            ImColor(style::stroke));

        frame.contentStart = ImVec2(
            frame.stripStart.x,
            baselineY + GetStyle().ItemSpacing.y);
        frame.contentEnd = frame.contentStart;

        tabBarStack.push_back(frame);
        return true;
    }

    bool BeginTabItem(const char* label)
    {
        if (tabBarStack.empty())
        {
            return false;
        }

        TabBarFrame& frame = tabBarStack.back();
        ImGuiWindow* window = GetCurrentWindow();
        const ImVec2 savedCursor = window->DC.CursorPos;
        const int itemIndex = frame.index++;

        SetCursorScreenPos(ImVec2(frame.stripStart.x + frame.bladeX, frame.stripStart.y));
        CommitCursor();

        if (TopTab(label, frame.selected == itemIndex, kBladePad))
        {
            frame.selected = itemIndex;
            GetStateStorage()->SetInt(frame.id, itemIndex);
        }
        frame.bladeX += GetItemRectSize().x;

        if (frame.selected == itemIndex)
        {
            SetCursorScreenPos(frame.contentStart);
            CommitCursor();
            return true;
        }

        SetCursorScreenPos(savedCursor);
        CommitCursor();
        return false;
    }

    void EndTabItem()
    {
        if (tabBarStack.empty())
        {
            return;
        }

        TabBarFrame& frame = tabBarStack.back();
        frame.contentEnd = GetCurrentWindow()->DC.CursorPos;
        frame.hasContent = true;
    }

    void EndTabBar()
    {
        if (tabBarStack.empty())
        {
            return;
        }

        const TabBarFrame frame = tabBarStack.back();
        tabBarStack.pop_back();

        SetCursorScreenPos(frame.hasContent ? frame.contentEnd : frame.contentStart);
        CommitCursor();
    }
}
