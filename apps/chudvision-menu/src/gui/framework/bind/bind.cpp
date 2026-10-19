#include "gui/framework/bind/bind.hpp"

#include "gui/framework/common.hpp"
#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/search/search.hpp"
#include "gui/framework/style/style.hpp"

#include <unordered_map>

namespace ksd
{
    using namespace ImGui;

    static const ImVec2 kPillSize(20.f, 9.f);
    static const float kPillTextSize = 8.f;

    struct BindEntry
    {
        bool* isEnabled;
        int key;
        int mode;
    };

    static std::unordered_map<ImGuiID, BindEntry> registeredBinds;
    static ImGuiID capturingId = 0;
    static int capturingFrame = 0;

    static const char* KeyLabel(int key)
    {
        switch (key)
        {
        case ImGuiKey_None: return "-";
        case ImGuiKey_MouseLeft: return "M1";
        case ImGuiKey_MouseRight: return "M2";
        case ImGuiKey_MouseMiddle: return "M3";
        case ImGuiKey_MouseX1: return "M4";
        case ImGuiKey_MouseX2: return "M5";
        default: return GetKeyName((ImGuiKey)key);
        }
    }

    static bool CaptureKeyPress(int* key)
    {
        if (GetFrameCount() == capturingFrame)
        {
            return false;
        }

        if (IsKeyPressed(ImGuiKey_Escape, false))
        {
            *key = ImGuiKey_None;
            return true;
        }

        for (int scanned = ImGuiKey_NamedKey_BEGIN; scanned < ImGuiKey_NamedKey_END; scanned++)
        {
            if (IsKeyPressed((ImGuiKey)scanned, false))
            {
                *key = scanned;
                return true;
            }
        }
        return false;
    }

    bool Bind(const char* id, bool* isEnabled, int* key, int* mode)
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

        const ImVec2 pillSize(style::Scaled(kPillSize.x), style::Scaled(kPillSize.y));
        PlaceRight(pillSize);

        const ImGuiID itemId = window->GetID(id);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + pillSize);

        ItemSize(bb);
        if (!ItemAdd(bb, itemId))
        {
            return false;
        }

        bool isHovered = false;
        bool isHeld = false;
        const bool isPressed = ButtonBehavior(bb, itemId, &isHovered, &isHeld, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

        bool hasChanged = false;
        if (isPressed && IsMouseReleased(ImGuiMouseButton_Right))
        {
            *mode = (*mode + 1) % 3;
            hasChanged = true;
        }
        else if (isPressed)
        {
            capturingId = itemId;
            capturingFrame = GetFrameCount();
        }

        const bool isCapturing = capturingId == itemId;
        if (isCapturing && CaptureKeyPress(key))
        {
            capturingId = 0;
            hasChanged = true;
        }

        BindEntry& entry = registeredBinds[itemId];
        entry.isEnabled = isEnabled;
        entry.key = *key;
        entry.mode = *mode;

        const bool isAlways = *mode == BindMode_Always;
        const ImVec4 textColor = isAlways ? style::toggleOn : style::textStrong;

        const char* pillText = "...";
        if (!isCapturing)
        {
            pillText = isAlways ? "ON" : KeyLabel(*key);
        }

        ImVec4 borderColor = style::stroke;
        if (isCapturing)
        {
            borderColor = style::strokeHover;
        }

        ImDrawList* drawList = window->DrawList;
        drawList->AddRectFilled(bb.Min, bb.Max, ImColor(style::controlBg));
        drawList->AddRect(bb.Min, bb.Max, ImColor(borderColor));

        PushFont(fonts::body, style::Scaled(kPillTextSize));
        const ImVec2 pillTextSize = CalcTextSize(pillText);
        drawList->PushClipRect(bb.Min, bb.Max, true);
        drawList->AddText(fonts::body, style::Scaled(kPillTextSize),
            ImVec2(bb.Min.x + (pillSize.x - pillTextSize.x) * 0.5f, bb.Min.y + (pillSize.y - pillTextSize.y) * 0.5f),
            ImColor(textColor), pillText);
        drawList->PopClipRect();
        PopFont();

        return hasChanged;
    }

    void UpdateBinds() noexcept
    {
        if (capturingId != 0 || GetIO().WantTextInput)
        {
            return;
        }

        for (auto& bind : registeredBinds)
        {
            const BindEntry& entry = bind.second;
            if (entry.key == ImGuiKey_None || entry.isEnabled == nullptr)
            {
                continue;
            }

            if (entry.mode == BindMode_Hold)
            {
                *entry.isEnabled = IsKeyDown((ImGuiKey)entry.key);
            }
            else if (entry.mode == BindMode_Toggle && IsKeyPressed((ImGuiKey)entry.key, false))
            {
                *entry.isEnabled = !*entry.isEnabled;
            }
        }
    }
}
