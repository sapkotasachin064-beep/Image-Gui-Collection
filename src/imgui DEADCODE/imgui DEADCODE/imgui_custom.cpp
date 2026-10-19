#include "imgui_custom.h"
#include <string>
#include <wtypes.h>

namespace font
{
   extern ImFont* inter_black_name;
   extern ImFont* inter_black_widget;
   extern ImFont* inter_semibold_widget;

   extern ImFont* icomoon[14];
}

namespace custom
{

    const char* keys[] =
    {
        "None",
        "M1",
        "M2",
        "CN",
        "M3",
        "M4",
        "M5",
        "-",
        "Back",
        "Tab",
        "-",
        "-",
        "CLR",
        "Enter",
        "-",
        "-",
        "Shift",
        "CTL",
        "Menu",
        "Pause",
        "Caps",
        "KAN",
        "-",
        "JUN",
        "FIN",
        "KAN",
        "-",
        "CON",
        "NCO",
        "ACC",
        "MAD",
        "Space",
        "PGU",
        "PGD",
        "End",
        "Home",
        "Left",
        "Up",
        "Right",
        "Down",
        "SEL",
        "PRI",
        "EXE",
        "PRI",
        "INS",
        "DEL",
        "HEL",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "WIN",
        "WIN",
        "APP",
        "-",
        "SLE",
        "Num 0",
        "Num 1",
        "Num 2",
        "Num 3",
        "Num 4",
        "Num 5",
        "Num 6",
        "Num 7",
        "Num 8",
        "Num 9",
        "MUL",
        "ADD",
        "SEP",
        "MIN",
        "Delete",
        "DIV",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "NUM",
        "SCR",
        "EQU",
        "MAS",
        "TOY",
        "OYA",
        "OYA",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "-",
        "Shift",
        "Shift",
        "Ctrl",
        "Ctrl",
        "Alt",
        "Alt"
    };

#include <string>

    struct key_state
    {
        ImVec4 background, text;
        bool active = false;
        bool hovered = false;
        float alpha = 0.f, slow;
    };

    bool Keybind(const char* label, int* key, int* mode)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        ImGuiIO& io = g.IO;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID(label);

        const float width = ((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns);

        const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width, 55.f));

        ItemSize(ImRect(rect.Min, rect.Max - ImVec2(0, 0)));

        if (!ImGui::ItemAdd(rect, id)) return false;

        char buf_display[64] = "None";


        bool value_changed = false;
        int k = *key;

        std::string active_key = "";
        active_key += keys[*key];

        if (*key != 0 && g.ActiveId != id) {
            strcpy_s(buf_display, active_key.c_str());
        }
        else if (g.ActiveId == id) {
            strcpy_s(buf_display, "...");
        }

        PushFont(font::inter_semibold_widget);
        const ImVec2 label_size = CalcTextSize(buf_display, NULL, true);

        static std::map<ImGuiID, key_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, key_state() });
            it_anim = anim.find(id);
        }

        ImRect clickable(ImVec2(rect.Max.x - (label_size.x + 25), rect.Min.y + 10), rect.Max - ImVec2(10, 10));

        PopFont();

        bool hovered = ItemHoverable(clickable, id, NULL);

        window->DrawList->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::background_widgets), c::keybind::rounding);

        it_anim->second.slow = ImLerp(it_anim->second.slow, clickable.Min.x - rect.Min.x, g.IO.DeltaTime * 15.f);
        window->DrawList->AddRectFilled(ImVec2(it_anim->second.slow + rect.Min.x, clickable.Min.y), clickable.Max, GetColorU32(c::keybind::background), c::keybind::rounding);

        window->DrawList->AddText(rect.Min + ImVec2(10, 10), GetColorU32(c::text::text_active), label);

        PushFont(font::inter_semibold_widget);

        window->DrawList->AddText(rect.Min + ImVec2(10, 30), GetColorU32(c::text::text), "Press the key to install");

        PushClipRect(ImVec2(it_anim->second.slow + rect.Min.x, clickable.Min.y), clickable.Max, true);
        PushStyleColor(ImGuiCol_Text, c::text::text_active);
        RenderTextClipped(clickable.Min, clickable.Max, buf_display, NULL, &label_size, ImVec2(0.5f, 0.5f));
        PopStyleColor();
        PopClipRect();

        PopFont();


        if (hovered && io.MouseClicked[0])
        {
            if (g.ActiveId != id) {
                // Start edition
                memset(io.MouseDown, 0, sizeof(io.MouseDown));
                memset(io.KeysDown, 0, sizeof(io.KeysDown));
                *key = 0;
            }
            ImGui::SetActiveID(id, window);
            ImGui::FocusWindow(window);
        }
        else if (io.MouseClicked[0]) {
            // Release focus when we click outside
            if (g.ActiveId == id)
                ImGui::ClearActiveID();
        }

        if (g.ActiveId == id) {
            for (auto i = 0; i < 5; i++) {
                if (io.MouseDown[i]) {
                    switch (i) {
                    case 0:
                        k = 0x01;
                        break;
                    case 1:
                        k = 0x02;
                        break;
                    case 2:
                        k = 0x04;
                        break;
                    case 3:
                        k = 0x05;
                        break;
                    case 4:
                        k = 0x06;
                        break;
                    }
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
            if (!value_changed) {
                for (auto i = 0x08; i <= 0xA5; i++) {
                    if (io.KeysDown[i]) {
                        k = i;
                        value_changed = true;
                        ImGui::ClearActiveID();
                    }
                }
            }

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                *key = 0;
                ImGui::ClearActiveID();
            }
            else {
                *key = k;
            }
        }

        return value_changed;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CHILD
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiWindowFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;

        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
        flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

        const ImVec2 content_avail = GetContentRegionAvail();
        ImVec2 size = ImTrunc(size_arg);
        const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
        if (size.x <= 0.0f)
            size.x = ImMax(content_avail.x + size.x, 4.0f);
        if (size.y <= 0.0f)
            size.y = ImMax(content_avail.y + size.y, 4.0f);
        SetNextWindowSize(size);

        GetWindowDrawList()->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + size_arg, GetColorU32(c::child::background), c::child::rounding);
    
        const char* temp_window_name;
        if (name)
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
        else
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

        bool ret = Begin(temp_window_name, NULL, flags | !ImGuiWindowFlags_HorizontalScrollbar);

        ImGuiWindow* child_window = g.CurrentWindow;
        child_window->ChildId = id;
        child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

        if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

        const ImGuiID temp_id_for_activation = ImHashStr("##Child", 0, id);
        if (g.ActiveId == temp_id_for_activation)
            ClearActiveID();
        if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY))
        {
            FocusWindow(child_window);
            NavInitWindow(child_window, false);
            SetActiveID(temp_id_for_activation, child_window);
            g.ActiveIdSource = g.NavInputSource;
        }
        return ret;
    }

    bool BeginChild(const char* str_id, const ImVec2& size_arg, ImGuiWindowFlags extra_flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 20));
        return BeginChildEx(str_id, window->GetID(str_id), size_arg, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding);
    }

    bool BeginChild(ImGuiID id, const ImVec2& size_arg, ImGuiWindowFlags extra_flags)
    {
        IM_ASSERT(id != 0);
        return BeginChildEx(NULL, id, size_arg, extra_flags);
    }

    void EndChild()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        PopStyleVar(2);
        IM_ASSERT(g.WithinEndChild == false);
        IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);

        g.WithinEndChild = true;
        if (window->BeginCount > 1)
        {
            End();
        }
        else
        {
            ImVec2 sz = window->Size;
            if (window->AutoFitChildAxises & (1 << ImGuiAxis_X))   sz.x = ImMax(4.0f, sz.x);
            if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))   sz.y = ImMax(4.0f, sz.y);

            End();

            ImGuiWindow* parent_window = g.CurrentWindow;
            ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
            ItemSize(sz);

            if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavWindowHasScrollY) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
            {
                ItemAdd(bb, window->ChildId);
            }
            else
            {
                ItemAdd(bb, 0);
                if (window->Flags & ImGuiWindowFlags_NavFlattened) parent_window->DC.NavLayersActiveMaskNext |= window->DC.NavLayersActiveMaskNext;

            }
            if (g.HoveredWindow == window) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
        }

        g.WithinEndChild = false;
        g.LogLinePosY = -FLT_MAX;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CHECKBOX
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct check_state
    {
        ImVec4 background, text;
        float alpha, mark_pos;
    };

    bool Checkbox(const char* label, bool* v)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const float square_sz = GetFrameHeight();
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect rect(pos, pos + ImVec2((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns, 55));

        ItemSize(rect, 0.f);
        if (!ItemAdd(rect, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);
        if (IsItemClicked())
        {
            *v = !(*v);
            MarkItemEdited(id);
        }

        static std::map<ImGuiID, check_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, check_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.background = ImLerp(it_anim->second.background, *v ? c::accent : c::checkbox::background, g.IO.DeltaTime * 6.f);
        it_anim->second.alpha = ImLerp(it_anim->second.alpha, *v ? 1.f : 0.f, g.IO.DeltaTime * 6.f);

        it_anim->second.mark_pos = ImClamp(it_anim->second.mark_pos + (35.f * g.IO.DeltaTime * (*v ? -3.f : 3.f)), 0.f, 35.f);

        const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::background_widgets), c::checkbox::rounding);

        GetWindowDrawList()->AddRectFilled(rect.Max - ImVec2(40, 40), rect.Max - ImVec2(15, 15), GetColorU32(it_anim->second.background), c::checkbox::rounding);
        GetWindowDrawList()->AddShadowRect(rect.Max - ImVec2(40, 40), rect.Max - ImVec2(15, 15), GetColorU32(c::accent, it_anim->second.alpha - 0.4f), 20.f, ImVec2(0, 0), 30.f);

        PushClipRect(rect.Max - ImVec2(40, 40), rect.Max - ImVec2(15, 15), true);
        RenderCheckMark(GetWindowDrawList(), rect.Max - ImVec2(40 - (square_sz / 2) / 2, 40 - (square_sz / 2 + it_anim->second.mark_pos) / 2), GetColorU32(c::checkbox::mark, it_anim->second.alpha), square_sz / 2);

        GetWindowDrawList()->AddText(font::icomoon[16], 16.8f, rect.Max - ImVec2(40 - (square_sz / 2) / 2 + 2, 40 - (square_sz / 2 - 37 + it_anim->second.mark_pos) / 2 + 2), GetColorU32(c::accent, 0.2f), "z");

        PopClipRect();

        GetWindowDrawList()->AddText(ImVec2(check_bb.Min.x, check_bb.Min.y - 10) + (ImVec2(45 - CalcTextSize(label).y, 55 - CalcTextSize(label).y)) / 2, GetColorU32(c::text::text_active), label);

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(ImVec2(check_bb.Min.x - 2, check_bb.Min.y + 10) + (ImVec2(45 - CalcTextSize(label).y, 55 - CalcTextSize(label).y)) / 2, GetColorU32(c::text::text), "Click for toggle.");
        PopFont();

        return pressed;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SLIDER
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct slider_state
    {
        ImVec4 text;
        float slow, blick;
    };

//  bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
//  {
//      ImGuiWindow* window = GetCurrentWindow();
//      if (window->SkipItems) return false;
//
//      ImGuiContext& g = *GImGui;
//      const ImGuiStyle& style = g.Style;
//      const ImGuiID id = window->GetID(label);
//      const float w = ((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns);
//
//      const ImVec2 label_size = CalcTextSize(label, NULL, true);
//
//      const ImRect frame_bb(window->DC.CursorPos + ImVec2(10, 58), window->DC.CursorPos + ImVec2(w - 10, 68));
//      const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x, 0.0f));
//
//      const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, 110));
//
//      const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
//      ItemSize(rect, 0.f);
//      if (!ItemAdd(total_bb, id, &frame_bb)) return false;
//
//      if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;
//
//      bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags), held, pressed = ButtonBehavior(frame_bb, id, &hovered, &held, NULL);
//      bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
//
//      static std::map<ImGuiID, slider_state> anim;
//      auto it_anim = anim.find(id);
//
//      if (it_anim == anim.end())
//      {
//          anim.insert({ id, slider_state() });
//          it_anim = anim.find(id);
//      }
//
//      GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::background_widgets), c::slider::rounding);
//
//      GetWindowDrawList()->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(c::slider::background), c::slider::rounding);
//
//      ImRect grab_bb;
//      const bool value_changed = SliderBehavior(ImRect(frame_bb.Min + ImVec2(3, 0), frame_bb.Max + ImVec2(7, 0)), id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
//
//      if (value_changed) MarkItemEdited(id);
//
//      it_anim->second.slow = ImLerp(it_anim->second.slow, grab_bb.Min.x - (frame_bb.Min.x + 5), g.IO.DeltaTime * 25.f);
//      it_anim->second.blick = ImLerp(it_anim->second.blick, IsItemActive() ? 4.f : 5.f, g.IO.DeltaTime * 8.f);
//
//      GetWindowDrawList()->AddRectFilled(frame_bb.Min + ImVec2(2, 2), ImVec2(it_anim->second.slow + (frame_bb.Min.x + 2), grab_bb.Max.y), GetColorU32(c::accent), c::slider::rounding, ImDrawFlags_RoundCornersLeft);
//      GetWindowDrawList()->AddShadowRect(frame_bb.Min + ImVec2(2, 2), ImVec2(it_anim->second.slow + (frame_bb.Min.x + 2), grab_bb.Max.y), GetColorU32(c::accent, 0.7f), 20.f, ImVec2(0, 0), c::slider::rounding, ImDrawFlags_RoundCornersLeft);
//
//      GetWindowDrawList()->AddRectFilled(ImVec2(it_anim->second.slow + frame_bb.Min.x, (grab_bb.Min.y - it_anim->second.blick)), ImVec2(it_anim->second.slow + (frame_bb.Min.x + 12), (grab_bb.Max.y + it_anim->second.blick)), GetColorU32(c::text::text_active), c::slider::rounding);
//
//
//      char value_buf[64];
//      const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
//
//      GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 10), GetColorU32(c::text::text_active), label);
//
//      PushFont(font::inter_semibold_widget);
//      GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 30), GetColorU32(c::text::text), "Drag slider for change value.");
//      PopFont();
//
//      PushFont(font::inter_semibold_widget);
//      GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 80), GetColorU32(c::text::text_active), value_buf);
//      PopFont();
//
//      return value_changed;
//  }

        bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = ((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns);

        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect frame_bb(window->DC.CursorPos + ImVec2(10, 58), window->DC.CursorPos + ImVec2(w - 10, 68));
        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x, 0.0f));

        const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, 110));

        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
        ItemSize(rect, 0.f);
        if (!ItemAdd(total_bb, id, &frame_bb)) return false;

        if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

        bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags), held, pressed = ButtonBehavior(frame_bb, id, &hovered, &held, NULL);
        bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);

        static std::map<ImGuiID, slider_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, slider_state() });
            it_anim = anim.find(id);
        }

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::background_widgets), c::slider::rounding);

        GetWindowDrawList()->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(c::slider::background), c::slider::rounding);

        ImRect grab_bb;
        const bool value_changed = SliderBehavior(ImRect(frame_bb.Min + ImVec2(3, 0), frame_bb.Max + ImVec2(7, 0)), id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);

        if (value_changed) MarkItemEdited(id);

        it_anim->second.slow = ImLerp(it_anim->second.slow, grab_bb.Min.x - (frame_bb.Min.x + 5), g.IO.DeltaTime * 25.f);
        it_anim->second.blick = ImLerp(it_anim->second.blick, IsItemActive() ? 4.f : 5.f, g.IO.DeltaTime * 8.f);

        GetWindowDrawList()->AddRectFilled(frame_bb.Min + ImVec2(2, 2), ImVec2(it_anim->second.slow + (frame_bb.Min.x + 2), grab_bb.Max.y), GetColorU32(c::accent), c::slider::rounding, ImDrawFlags_RoundCornersLeft);
        GetWindowDrawList()->AddShadowRect(frame_bb.Min + ImVec2(2, 2), ImVec2(it_anim->second.slow + (frame_bb.Min.x + 2), grab_bb.Max.y), GetColorU32(c::accent, 0.7f), 20.f, ImVec2(0, 0), c::slider::rounding, ImDrawFlags_RoundCornersLeft);

        GetWindowDrawList()->AddRectFilled(ImVec2(it_anim->second.slow + frame_bb.Min.x, (grab_bb.Min.y - it_anim->second.blick)), ImVec2(it_anim->second.slow + (frame_bb.Min.x + 12), (grab_bb.Max.y + it_anim->second.blick)), GetColorU32(c::text::text_active), c::slider::rounding);

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 10), GetColorU32(c::text::text_active), label);

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 30), GetColorU32(c::text::text), "Drag slider for change value.");
        PopFont();

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 80), GetColorU32(c::text::text_active), value_buf);
        PopFont();

        return value_changed;
    }

    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
    }

    bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GROUP BOX
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BeginGroup()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;

        g.GroupStack.resize(g.GroupStack.Size + 1);
        ImGuiGroupData& group_data = g.GroupStack.back();
        group_data.WindowID = window->ID;
        group_data.BackupCursorPos = window->DC.CursorPos;
        group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
        group_data.BackupIndent = window->DC.Indent;
        group_data.BackupGroupOffset = window->DC.GroupOffset;
        group_data.BackupCurrLineSize = window->DC.CurrLineSize;
        group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
        group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
        group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
        group_data.BackupActiveIdPreviousFrameIsAlive = g.ActiveIdPreviousFrameIsAlive;
        group_data.EmitItem = true;

        window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
        window->DC.Indent = window->DC.GroupOffset;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);

        if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;
    }

    void EndGroup()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        IM_ASSERT(g.GroupStack.Size > 0);

        ImGuiGroupData& group_data = g.GroupStack.back();
        IM_ASSERT(group_data.WindowID == window->ID);

        if (window->DC.IsSetPos)
            ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

        ImRect group_bb(group_data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, group_data.BackupCursorPos));

        window->DC.CursorPos = group_data.BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
        window->DC.Indent = group_data.BackupIndent;
        window->DC.GroupOffset = group_data.BackupGroupOffset;
        window->DC.CurrLineSize = group_data.BackupCurrLineSize;
        window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
        if (g.LogEnabled)
            g.LogLinePosY = -FLT_MAX;

        if (!group_data.EmitItem)
        {
            g.GroupStack.pop_back();
            return;
        }

        window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrLineTextBaseOffset);
        ItemSize(group_bb.GetSize());
        ItemAdd(group_bb, 0, NULL, ImGuiItemFlags_NoTabStop);

        const bool group_contains_curr_active_id = (group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
        const bool group_contains_prev_active_id = (group_data.BackupActiveIdPreviousFrameIsAlive == false) && (g.ActiveIdPreviousFrameIsAlive == true);

        if (group_contains_curr_active_id) g.LastItemData.ID = g.ActiveId;
        else if (group_contains_prev_active_id) g.LastItemData.ID = g.ActiveIdPreviousFrame;
        g.LastItemData.Rect = group_bb;

        const bool group_contains_curr_hovered_id = (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
        if (group_contains_curr_hovered_id) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;

        if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;

        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;

        if (group_contains_prev_active_id && g.ActiveId != g.ActiveIdPreviousFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;

        g.GroupStack.pop_back();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PAGE
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct page_state
    {
        ImVec4 background, text, icon;
        float alpha, size_cap;
    };

    bool Page(bool selected, const char* icon_font, const char* label, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        const ImRect bb(pos, pos + size);

        ItemSize(size, 0.f);
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        static std::map<ImGuiID, page_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, page_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.icon = ImLerp(it_anim->second.icon, selected ? c::accent : c::text::text, g.IO.DeltaTime * 6.f);
        it_anim->second.text = ImLerp(it_anim->second.text, selected ? c::text::text_active : c::text::text, g.IO.DeltaTime * 6.f);
        it_anim->second.alpha = ImLerp(it_anim->second.alpha, selected ? 1.f : 0.f, g.IO.DeltaTime * 6.f);
        it_anim->second.size_cap = ImLerp(it_anim->second.size_cap, selected ? 0.f : size_arg.x, g.IO.DeltaTime * 6.f);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max - ImVec2(it_anim->second.size_cap, 0), GetColorU32(c::page::background, it_anim->second.alpha), c::page::rounding);

        PushFont(font::icomoon[17]);
        GetWindowDrawList()->AddText(bb.Min + ImVec2((size_arg.y - CalcTextSize(icon_font).y) / 2, (size_arg.y - CalcTextSize(icon_font).y - 2) / 2 + 2), GetColorU32(it_anim->second.icon), icon_font);
        PopFont();

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(bb.Min + ImVec2((size_arg.y - CalcTextSize(label).y) / 2 + 35, (size_arg.y - CalcTextSize(label).y) / 2), GetColorU32(it_anim->second.text), label);
        PopFont();

        return pressed;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PAGE TWO
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct subpage_state
    {
        ImVec4 background, text, icon;
        float alpha, alpha_line;
    };

    bool SubPage(bool selected, const char* label, const char* information, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        const ImRect bb(pos, pos + size);

        ItemSize(size, 0.f);
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        static std::map<ImGuiID, subpage_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, subpage_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.icon = ImLerp(it_anim->second.icon, selected ? c::accent : c::text::text, g.IO.DeltaTime * 6.f);
        it_anim->second.text = ImLerp(it_anim->second.text, selected ? c::text::text_active : c::text::text, g.IO.DeltaTime * 6.f);
        it_anim->second.alpha = ImLerp(it_anim->second.alpha, selected ? 1.f : 0.5f, g.IO.DeltaTime * 6.f);
        it_anim->second.alpha_line = ImLerp(it_anim->second.alpha_line, selected ? 1.f : 0.0f, g.IO.DeltaTime * 6.f);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::page::background, it_anim->second.alpha), c::page::rounding);
        GetWindowDrawList()->AddRectFilled(bb.Max - ImVec2(size_arg.x, 0), bb.Max, GetColorU32(c::accent, it_anim->second.alpha_line), c::page::rounding);

        GetWindowDrawList()->AddText(bb.Min + ImVec2(10, 10), GetColorU32(it_anim->second.text), label);

        PushFont(font::inter_semibold_widget);

        GetWindowDrawList()->AddText(bb.Min + ImVec2(10, 35), GetColorU32(c::text::text), information);

        PopFont();

        return pressed;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// COLOR PICKER
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
    {
        ImGuiContext& g = *GImGui;

        if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
            *H = g.ColorEditSavedHue;

        if (*V == 0.0f) *S = g.ColorEditSavedSat;
    }

    struct edit_state
    {
        float alpha = 0.f;
        bool hovered, active;
        ImVec4 text;
    };

    bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const float square_sz = GetFrameHeight();
        const float w_full = CalcItemWidth();
        const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
        const float w_inputs = w_full - w_button;
        const char* label_display_end = FindRenderedTextEnd(label);
        g.NextItemData.ClearFlags();

        BeginGroup();
        PushID(label);
        const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
        if (set_current_color_edit_id)
            g.ColorEditCurrentID = window->IDStack.back();

        // If we're not showing any slider there's no point in doing any HSV conversions
        const ImGuiColorEditFlags flags_untouched = flags;
        if (flags & ImGuiColorEditFlags_NoInputs)
            flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

        // Context menu: display and modify options (before defaults are applied)
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            ColorEditOptionsPopup(col, flags);

        // Read stored options
        if (!(flags & ImGuiColorEditFlags_DisplayMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
        if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
        if (!(flags & ImGuiColorEditFlags_PickerMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
        if (!(flags & ImGuiColorEditFlags_InputMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
        flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

        const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
        const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
        const int components = alpha ? 4 : 3;

        // Convert to the formats we need
        float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
        {
            // Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
        }
        int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

        bool value_changed = false;
        bool value_changed_as_float = false;

        const ImVec2 pos = window->DC.CursorPos;
        const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
        window->DC.CursorPos.x = pos.x + inputs_offset_x;

        if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {
            const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
            const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

            const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
            static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
            static const char* fmt_table_int[3][4] =
            {
                {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
                { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
                { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
            };
            static const char* fmt_table_float[3][4] =
            {
                {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
                { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
                { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
            };
            const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

            for (int n = 0; n < components; n++)
            {
                if (n > 0)
                    SameLine(0, style.ItemInnerSpacing.x);
                SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

                // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
                if (flags & ImGuiColorEditFlags_Float)
                {
                    value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                    value_changed_as_float |= value_changed;
                }
                else
                {
                    value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
                }
                if (!(flags & ImGuiColorEditFlags_NoOptions))
                    OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
            }
        }
        else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {
            char buf[64];
            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
            SetNextItemWidth(w_inputs);
            if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
            {
                value_changed = true;
                char* p = buf;
                while (*p == '#' || ImCharIsBlankA(*p))
                    p++;
                i[0] = i[1] = i[2] = 0;
                i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
                int r;
                if (alpha)
                    r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
                else
                    r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
                IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        }

        ImGuiWindow* picker_active_window = NULL;
        if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
        {
            const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

            const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
            if (ColorButton("##ColorButton", col_v4, flags))
            {
                if (!(flags & ImGuiColorEditFlags_NoPicker))
                {
                    // Store current color and open a picker
                    g.ColorPickerRef = col_v4;
                    OpenPopup("picker");
                    SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
                }
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

            static std::map<ImGuiID, edit_state> anim;
            auto it_anim = anim.find(ImGui::GetID(label));

            if (it_anim == anim.end())
            {
                anim.insert({ ImGui::GetID(label), edit_state() });
                it_anim = anim.find(ImGui::GetID(label));
            }

            if (ItemHoverable(g.LastItemData.Rect, g.LastItemData.ID, NULL) && g.IO.MouseClicked[0] || it_anim->second.active && g.IO.MouseClicked[0] && !it_anim->second.hovered)
                it_anim->second.active = !it_anim->second.active;

            it_anim->second.alpha = ImClamp(it_anim->second.alpha + (8.f * g.IO.DeltaTime * (it_anim->second.active ? 1.f : -1.f)), 0.f, 1.f);

            PushStyleColor(ImGuiCol_WindowBg, GetColorU32(c::picker::background));
            PushStyleVar(ImGuiStyleVar_WindowRounding, c::picker::rounding);
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
            PushStyleVar(ImGuiStyleVar_Alpha, it_anim->second.alpha);

            if (it_anim->second.alpha >= 0.01f)
            {
                SetNextWindowPos(g.LastItemData.Rect.GetTR() - ImVec2(39, 12));

                Begin("picker", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
                {
                    it_anim->second.hovered = IsWindowHovered();

                    char buf[64];
                    ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));

                    picker_active_window = g.CurrentWindow;
                    ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                    ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;

                    value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x, true);
                }
                End();
            }
        }
        PopStyleColor(1);
        PopStyleVar(3);

        if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
        {
            char buf[64];

            SameLine(0.0f, style.ItemInnerSpacing.x);
            window->DC.CursorPos.x = pos.x - w_button + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full);

            GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(0, 1), GetColorU32(c::text::text), label);

        }

        // Convert back
        if (value_changed && picker_active_window == NULL)
        {
            if (!value_changed_as_float)
                for (int n = 0; n < 4; n++)
                    f[n] = i[n] / 255.0f;
            if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
            {
                g.ColorEditSavedHue = f[0];
                g.ColorEditSavedSat = f[1];
                ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
                g.ColorEditSavedID = g.ColorEditCurrentID;
                g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
            }
            if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

            col[0] = f[0];
            col[1] = f[1];
            col[2] = f[2];
            if (alpha)
                col[3] = f[3];
        }

        if (set_current_color_edit_id)
            g.ColorEditCurrentID = 0;
        PopID();
        EndGroup();

        // Drag and Drop Target
        // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
        if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
        {
            bool accepted_drag_drop = false;
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
                value_changed = accepted_drag_drop = true;
            }
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * components);
                value_changed = accepted_drag_drop = true;
            }

            // Drag-drop payloads are always RGB
            if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
            EndDragDropTarget();
        }

        // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
        if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
            g.LastItemData.ID = g.ActiveId;

        if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    // Helper for ColorPicker4()
    static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
    {
        ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32(0, 0, 0, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32(255, 255, 255, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32(0, 0, 0, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32(255, 255, 255, alpha8));
    }



    struct picker_state
    {
        float hue_bar;
        float alpha_bar;
        float circle;
        ImVec2 circle_move;
        float bar0_line_x;
    };

    bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col, bool popup)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;
   
        ImDrawList* draw_list = window->DrawList;
        ImGuiStyle& style = g.Style;
        ImGuiIO& io = g.IO;
   
        PushID(label);
        BeginGroup();
   
        g.NextItemData.ClearFlags();
   
        const float width = ((GetContentRegionMax().x - ((style.WindowPadding.x * 5) + style.ItemSpacing.x)) / c::columns);
   
        if (!(flags & ImGuiColorEditFlags_NoSidePreview)) flags |= ImGuiColorEditFlags_NoSmallPreview;
   
        if (!(flags & ImGuiColorEditFlags_NoOptions)) ColorPickerOptionsPopup(col, flags);
   
        if (!(flags & ImGuiColorEditFlags_PickerMask_))  flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
        if (!(flags & ImGuiColorEditFlags_InputMask_)) flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
   
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_)); 
   
        if (!(flags & ImGuiColorEditFlags_NoOptions)) flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);
   
        int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
        bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
        ImVec2 picker_pos = window->DC.CursorPos + ImVec2(10, 60);
        ImVec2 bar_pos = window->DC.CursorPos + ImVec2(10, 135);
   
        float sv_picker_size = ImMax(280.f, width - (alpha_bar ? 2 : 1) * width) + 0;
        float sv_bar_size = 13;
        float bar0_pos_x = picker_pos.x;
      
        float backup_initial_col[4];
        memcpy(backup_initial_col, col, components * sizeof(float));
        GetWindowDrawList()->AddRectFilled(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width + 20, (flags & ImGuiColorEditFlags_AlphaBar) ? 200 : 180), GetColorU32(c::background_widgets), c::picker::rounding);
   
        GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(10, 10), GetColorU32(c::text::text_active), label);
   
        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(10, 30), GetColorU32(c::text::text), "Drag mouse for choice color.");
        PopFont();
   
        float H = col[0], S = col[1], V = col[2];
        float R = col[0], G = col[1], B = col[2];
        if (flags & ImGuiColorEditFlags_InputRGB)
        {
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            ColorEditRestoreHS(col, &H, &S, &V);
        }
        else if (flags & ImGuiColorEditFlags_InputHSV)
        {
            ColorConvertHSVtoRGB(H, S, V, R, G, B);
        }
   
        bool value_changed = false, value_changed_h = false, value_changed_sv = false;
   
        PushItemFlag(ImGuiItemFlags_NoNav, true);
   
        InvisibleButton("sv", ImVec2(width, 120));
        if (IsItemActive())
        {
            S = ImSaturate((io.MousePos.x - picker_pos.x) / (width - 1));
            V = 1.001f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 220));
   
            if (g.ColorEditSavedColor == ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], col[3]))) H = g.ColorEditSavedHue;
   
            value_changed = value_changed_sv = true;
        }
   
        SetCursorScreenPos(ImVec2(bar0_pos_x, bar_pos.y - 5));
        InvisibleButton("hue", ImVec2(width, sv_bar_size));
        if (IsItemActive())
        {
            H = 1.f - ImSaturate((io.MousePos.x - bar_pos.x) / (width - 1));
            value_changed = value_changed_h = true;
        }
   
        if (alpha_bar)
        {
            SetCursorScreenPos(ImVec2(bar0_pos_x, bar_pos.y + 16));
            InvisibleButton("alpha", ImVec2(width, sv_bar_size));
            if (IsItemActive())
            {
                col[3] = ImSaturate((io.MousePos.x - bar_pos.x) / (width - 1));
                value_changed = true;
            }
        }
        PopItemFlag();
   
        if (value_changed_h || value_changed_sv)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
                g.ColorEditSavedHue = H;
                g.ColorEditSavedSat = S;
                g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
            }
            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                col[0] = H;
                col[1] = S;
                col[2] = V;
            }
        }
   
        if (value_changed)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                R = col[0];
                G = col[1];
                B = col[2];
                ColorConvertRGBtoHSV(R, G, B, H, S, V);
                ColorEditRestoreHS(col, &H, &S, &V);   // Fix local Hue as display below will use it immediately.
            }
            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                H = col[0];
                S = col[1];
                V = col[2];
                ColorConvertHSVtoRGB(H, S, V, R, G, B);
            }
        }
        ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha)); // Important: this is still including the main rendering/style alpha!!
   
        const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
        const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
        const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
        const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
        const ImU32 col_hues[7] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(0,0,255,style_alpha8),IM_COL32(0,255,255,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(255,0,0,style_alpha8) };
   
        ImVec4 hue_color_f(1, 1, 1, style.Alpha); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
        ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
   
        ImVec2 sv_cursor_pos;
   
        const int vtx_idx_0 = draw_list->VtxBuffer.Size;
        GetWindowDrawList()->AddRectFilled(picker_pos, picker_pos + ImVec2(width, sv_picker_size - 220), col_white, c::picker::rounding + 1);
        const int vtx_idx_1 = draw_list->VtxBuffer.Size;
        ShadeVertsLinearColorGradientKeepAlpha(GetWindowDrawList(), vtx_idx_0, vtx_idx_1, picker_pos, picker_pos + ImVec2(width - 2, 0.0f), col_white, hue_color32);
   
        GetWindowDrawList()->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(width + 0.5f, sv_picker_size - 220), 0, 0, col_black, col_black, c::picker::rounding);
   
        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * width), picker_pos.x, picker_pos.x + width - 2);
        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * (sv_picker_size - 220)), picker_pos.y + 2, picker_pos.y + sv_picker_size - 220);
   
        static std::map<ImGuiID, picker_state> anim;
        auto it_anim = anim.find(ImGui::GetID(label));
   
        if (it_anim == anim.end())
        {
            anim.insert({ ImGui::GetID(label), picker_state() });
            it_anim = anim.find(ImGui::GetID(label));
        }
   
        for (int i = 0; i < 6; ++i) GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(picker_pos.x + i * (width / 6) - (i == 5 ? 1 : 0), picker_pos.y + 70), ImVec2(picker_pos.x + (i + 1) * (width / 6) + (i == 0 ? 1 : 0), picker_pos.y + 70 + sv_bar_size - 7), col_hues[i], col_hues[i + 1], col_hues[i + 1], col_hues[i], c::picker::rounding, i == 0 ? ImDrawFlags_RoundCornersLeft : i == 5 ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);
   
        float bar0_line_x = IM_ROUND(bar_pos.x + (1.f - H) * width);
   
        bar0_line_x = ImClamp(bar0_line_x, picker_pos.x + 7.f, picker_pos.x + width);
   
        it_anim->second.hue_bar = ImLerp(it_anim->second.hue_bar, bar0_line_x - (bar_pos.x + 7), g.IO.DeltaTime * 26.f);
   
        GetWindowDrawList()->AddRectFilled(ImVec2(it_anim->second.hue_bar + (bar_pos.x - 2), bar_pos.y - 8), ImVec2(it_anim->second.hue_bar + (bar_pos.x + 8), (bar_pos.y + 4)), GetColorU32(c::text::text_active), c::picker::rounding);
   
        it_anim->second.circle_move = ImLerp(it_anim->second.circle_move, sv_cursor_pos - bar_pos, g.IO.DeltaTime * 26.f);
        it_anim->second.circle = ImLerp(it_anim->second.circle, value_changed_sv ? 3.f : 4.5f, g.IO.DeltaTime * 26.f);
   
        GetWindowDrawList()->AddCircle(it_anim->second.circle_move + bar_pos + ImVec2(0, 1), it_anim->second.circle, GetColorU32(c::text::text_active), 0, 1.f);
   
        float alpha = ImSaturate(col[3]);
   
        if (alpha_bar)
        {
            ImRect bar1_bb(bar0_pos_x, bar_pos.y + 25, bar0_pos_x + width, bar_pos.y + 25 + sv_bar_size);
            GetWindowDrawList()->AddRectFilledMultiColor(picker_pos + ImVec2(0, 91), picker_pos + ImVec2(width, 97), col_black, user_col32_striped_of_alpha, user_col32_striped_of_alpha, col_black, c::picker::rounding);
   
            float bar1_line_x = IM_ROUND(bar_pos.x + 7 + alpha * width);
            bar1_line_x = ImClamp(bar1_line_x, bar_pos.x, picker_pos.x + width);
            it_anim->second.alpha_bar = ImLerp(it_anim->second.alpha_bar, bar1_line_x - (bar_pos.x + 7), g.IO.DeltaTime * 26.f);
   
            GetWindowDrawList()->AddRectFilled(ImVec2(it_anim->second.alpha_bar + (bar_pos.x - 2), bar1_bb.Min.y - 12), ImVec2(it_anim->second.alpha_bar + (bar_pos.x + 8), (bar1_bb.Max.y - 13)), GetColorU32(c::text::text_active), c::picker::rounding);
   
        }
   
        EndGroup();
        PopID();
   
        float f[4] = { col[0], col[1], col[2], 1.f };
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
        {
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
        }
   
        ImVec4 col_v4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
   
        int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };
   
        char buf[64];
        ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
   
        SetCursorPos(GetCursorPos() + ImVec2(width - 10, -13));
        ColorButton("##current", col_v4, NULL, ImVec2(20, 20));
   
        GetWindowDrawList()->AddText(font::inter_semibold_widget, 16, GetCursorScreenPos() - ImVec2(-10, 50), GetColorU32(c::text::text_active), buf);
   
        return value_changed;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// HUE BAR
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool HueSlider(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;
  
        ImDrawList* draw_list = window->DrawList;
        ImGuiStyle& style = g.Style;
        ImGuiIO& io = g.IO;
  
        const float width = ((GetContentRegionMax().x - ((style.WindowPadding.x * 5) + style.ItemSpacing.x)) / c::columns);

        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width, -50));

        PushID(label);
        BeginGroup();

        g.NextItemData.ClearFlags();
    
        if (!(flags & ImGuiColorEditFlags_NoSidePreview)) flags |= ImGuiColorEditFlags_NoSmallPreview;
    
        if (!(flags & ImGuiColorEditFlags_PickerMask_))  flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
        if (!(flags & ImGuiColorEditFlags_InputMask_)) flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
  
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));
  
        if (!(flags & ImGuiColorEditFlags_NoOptions)) flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);
  
        ImVec2 picker_pos = window->DC.CursorPos + ImVec2(10, -10);
        ImVec2 bar_pos = window->DC.CursorPos + ImVec2(10, 65);
  
        float sv_picker_size = ImMax(280.f, width - 1 * width);
        float sv_bar_size = 13;
        float bar0_pos_x = picker_pos.x;
  
        GetWindowDrawList()->AddRectFilled(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width + 20, 80), GetColorU32(c::background_widgets), c::picker::rounding);
  
        GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(10, 10), GetColorU32(c::text::text_active), label);
  
        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(10, 30), GetColorU32(c::text::text), "Drag mouse for choice color.");
        PopFont();
  
        float H = col[0], S = col[1], V = col[2];
        float R = col[0], G = col[1], B = col[2];
        if (flags & ImGuiColorEditFlags_InputRGB)
        {
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            ColorEditRestoreHS(col, &H, &S, &V);
        }
  
        bool value_changed = true, value_changed_h = true, value_changed_sv = true;
  
        SetCursorScreenPos(ImVec2(bar0_pos_x, bar_pos.y - 5));
        InvisibleButton("hue", ImVec2(width, sv_bar_size));
        if (IsItemActive())
        {
            H = 1.f - ImSaturate((io.MousePos.x - bar_pos.x) / (width - 1));
            value_changed = true;
        }
  
        if (value_changed_h || value_changed_sv)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
                g.ColorEditSavedHue = H;
                g.ColorEditSavedSat = S;
                g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 1.0));
            }
        }
  
        ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha));

        const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
        const ImU32 col_hues[7] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(0,0,255,style_alpha8),IM_COL32(0,255,255,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

        static std::map<ImGuiID, picker_state> anim;
        auto it_anim = anim.find(ImGui::GetID(label));
  
        if (it_anim == anim.end())
        {
            anim.insert({ ImGui::GetID(label), picker_state() });
            it_anim = anim.find(ImGui::GetID(label));
        }
  
        for (int i = 0; i < 6; ++i) GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(picker_pos.x + i * (width / 6) - (i == 5 ? 1 : 0), picker_pos.y + 70), ImVec2(picker_pos.x + (i + 1) * (width / 6) + (i == 0 ? 1 : 0), picker_pos.y + 70 + sv_bar_size - 7), col_hues[i], col_hues[i + 1], col_hues[i + 1], col_hues[i], c::picker::rounding, i == 0 ? ImDrawFlags_RoundCornersLeft : i == 5 ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);

        it_anim->second.bar0_line_x = IM_ROUND(bar_pos.x + (1.02f - H) * width);
  
        it_anim->second.bar0_line_x = ImClamp(it_anim->second.bar0_line_x, picker_pos.x + 7.f, picker_pos.x + width);
  
        it_anim->second.hue_bar = ImLerp(it_anim->second.hue_bar, it_anim->second.bar0_line_x - (bar_pos.x + 7), g.IO.DeltaTime * 26.f);
  
        GetWindowDrawList()->AddRectFilled(ImVec2(it_anim->second.hue_bar + (bar_pos.x - 2), bar_pos.y - 8), ImVec2(it_anim->second.hue_bar + (bar_pos.x + 8), (bar_pos.y + 4)), GetColorU32(c::text::text_active), c::picker::rounding);
  
        EndGroup();
        PopID();

        SetCursorPosY(GetCursorPosY() + 5);

        return value_changed;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// COLOR BUTTON   
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        const ImGuiID id = window->GetID(desc_id);
        const float default_size = GetFrameHeight();

        const ImVec2 size(size_arg.x == 0.0f ? default_size : size_arg.x, size_arg.y == 0.0f ? default_size : size_arg.y);
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

        ItemSize(ImRect(bb.Min, bb.Max + ImVec2(0, 10)));
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held);

        ImVec4 col_rgb_without_alpha(col.x, col.y, col.z, 1.0f);

        float grid_step = ImMin(size.x, size.y) / 2.99f;
        float rounding = ImMin(c::picker::rounding, grid_step * 0.5f);
        
        RenderColorRectWithAlphaCheckerboard(GetWindowDrawList(), bb.Min, bb.Max, GetColorU32(col), grid_step, ImVec2(-grid_step, 0.f), c::picker::rounding);

        return pressed;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// COMBO   
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext& g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }

    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

        const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
    }

    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }

    struct begin_state
    {
        ImVec4 background, text, outline;
        float open, alpha, combo_size = 0.f, shadow_opticaly;
        bool opened_combo = false, hovered = false;
        float arrow_roll;
    };

    bool BeginCombo(const char* label, const char* preview_value, int val, bool multi, ImGuiComboFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();

        g.NextWindowData.ClearFlags();
        if (window->SkipItems) return false;

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const float w = ((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns);

        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, 90));
        const ImRect total_bb(bb.Min, bb.Max);
        ItemSize(bb, 0.f);

        if (!ItemAdd(total_bb, id, &bb)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held);

        static std::map<ImGuiID, begin_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, begin_state() });
            it_anim = anim.find(id);
        }

        if (hovered && g.IO.MouseClicked[0] || it_anim->second.opened_combo && g.IO.MouseClicked[0] && !it_anim->second.hovered) it_anim->second.opened_combo = !it_anim->second.opened_combo;

        it_anim->second.arrow_roll = ImLerp(it_anim->second.arrow_roll, it_anim->second.opened_combo ? -1.f : 1.f, g.IO.DeltaTime * 6.f);
        it_anim->second.text = ImLerp(it_anim->second.text, it_anim->second.opened_combo ? c::text::text_active : c::text::text, g.IO.DeltaTime * 6.f);
        it_anim->second.combo_size = ImLerp(it_anim->second.combo_size, it_anim->second.opened_combo ? (val * 33) + 17 : 0.f, g.IO.DeltaTime * 12.f);

        const float value_x2 = ImMax(bb.Min.x, bb.Max.x - GetFrameHeight());

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::background_widgets), c::combo::rounding);

        GetWindowDrawList()->AddRectFilled(bb.Min + ImVec2(5, 55), bb.Max - ImVec2(5, 5), GetColorU32(c::combo::background), c::slider::rounding);

        PushFont(font::icomoon[18]);
        ImRotateStart();
        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (w - (12 + CalcTextSize("a").x)), bb.Min.y + 61), GetColorU32(c::accent), "a");
        ImRotateEnd(1.57f * it_anim->second.arrow_roll);
        PopFont();

        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 10, bb.Min.y + 10), GetColorU32(c::text::text_active), label);

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 10, bb.Min.y + 30), GetColorU32(c::text::text), "ComboBox description");

        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 12, bb.Min.y + 62), GetColorU32(c::text::text_active), preview_value);
        PopFont();

        if (!IsRectVisible(bb.Min, bb.Max + ImVec2(0, 2)))
        {
            it_anim->second.opened_combo = false;
            it_anim->second.combo_size = 0.f;
        }

        if (!it_anim->second.opened_combo && it_anim->second.combo_size < 2.f) return false;

        ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + 5));
        ImGui::SetNextWindowSize(ImVec2(bb.GetWidth(), it_anim->second.combo_size));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse;

        PushStyleColor(ImGuiCol_WindowBg, c::combo::list);
        PushStyleVar(ImGuiStyleVar_WindowRounding, c::combo::rounding);
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
        PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

        bool ret = Begin(label, NULL, window_flags);

        PopStyleVar(3);
        PopStyleColor(1);

        it_anim->second.hovered = IsWindowHovered();

        if (multi && it_anim->second.hovered && g.IO.MouseClicked[0]) it_anim->second.opened_combo = false;

        return true;
    }

    void EndCombo()
    {
        End();
    }

    void MultiCombo(const char* label, bool variable[], const char* labels[], int count)
    {
        ImGuiContext& g = *GImGui;

        std::string preview = "None";

        for (auto i = 0, j = 0; i < count; i++)
        {
            if (variable[i])
            {
                if (j)
                    preview += (", ") + (std::string)labels[i];
                else
                    preview = labels[i];

                j++;
            }
        }

        if (BeginCombo(label, preview.c_str(), count))
        {
            for (auto i = 0; i < count; i++)
            {
                PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
                PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
                custom::Selectable(labels[i], &variable[i], ImGuiSelectableFlags_DontClosePopups);
                PopStyleVar(2);
            }
            End();
        }

        preview = ("None");
    }

    bool BeginComboPreview()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

        if (window->SkipItems || !(g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible))
            return false;
        IM_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y);
        if (!window->ClipRect.Overlaps(preview_data->PreviewRect))
            return false;

        preview_data->BackupCursorPos = window->DC.CursorPos;
        preview_data->BackupCursorMaxPos = window->DC.CursorMaxPos;
        preview_data->BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
        preview_data->BackupPrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
        preview_data->BackupLayout = window->DC.LayoutType;
        window->DC.CursorPos = preview_data->PreviewRect.Min + g.Style.FramePadding;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.LayoutType = ImGuiLayoutType_Horizontal;
        window->DC.IsSameLine = false;
        PushClipRect(preview_data->PreviewRect.Min, preview_data->PreviewRect.Max, true);

        return true;
    }

    void EndComboPreview()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

        ImDrawList* draw_list = window->DrawList;
        if (window->DC.CursorMaxPos.x < preview_data->PreviewRect.Max.x && window->DC.CursorMaxPos.y < preview_data->PreviewRect.Max.y)
            if (draw_list->CmdBuffer.Size > 1)
            {
                draw_list->_CmdHeader.ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 2].ClipRect;
                draw_list->_TryMergeDrawCmds();
            }
        PopClipRect();
        window->DC.CursorPos = preview_data->BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, preview_data->BackupCursorMaxPos);
        window->DC.CursorPosPrevLine = preview_data->BackupCursorPosPrevLine;
        window->DC.PrevLineTextBaseOffset = preview_data->BackupPrevLineTextBaseOffset;
        window->DC.LayoutType = preview_data->BackupLayout;
        window->DC.IsSameLine = false;
        preview_data->PreviewRect = ImRect();
    }

    static const char* Items_ArrayGetter(void* data, int idx)
    {
        const char* const* items = (const char* const*)data;
        return items[idx];
    }

    static const char* Items_SingleStringGetter(void* data, int idx)
    {
        const char* items_separated_by_zeros = (const char*)data;
        int items_count = 0;
        const char* p = items_separated_by_zeros;
        while (*p)
        {
            if (idx == items_count)
                break;
            p += strlen(p) + 1;
            items_count++;
        }
        return *p ? p : NULL;
    }

    bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items)
    {
        ImGuiContext& g = *GImGui;

        const char* preview_value = NULL;
        if (*current_item >= 0 && *current_item < items_count)
            preview_value = getter(user_data, *current_item);

        if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
            SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

        if (!BeginCombo(label, preview_value, items_count, ImGuiComboFlags_None)) return false;

        bool value_changed = false;
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
        for (int i = 0; i < items_count; i++)
        {
            const char* item_text = getter(user_data, i);
            if (item_text == NULL)
                item_text = "*Unknown item*";

            PushID(i);
            const bool item_selected = (i == *current_item);
            if (custom::Selectable(item_text, item_selected) && *current_item != i)
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }
        PopStyleVar();

        EndCombo();

        if (value_changed)
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
    {
        const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
        return value_changed;
    }

    bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
    {
        int items_count = 0;
        const char* p = items_separated_by_zeros;
        while (*p)
        {
            p += strlen(p) + 1;
            items_count++;
        }
        bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
        return value_changed;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SELECTABLE 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct select_state
    {
        ImVec4 text;
    };

    bool Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        ImGuiID id = window->GetID(label);
        ImVec2 label_size = CalcTextSize(label, NULL, true);
        ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
        ImVec2 pos = window->DC.CursorPos;
        pos.y += window->DC.CurrLineTextBaseOffset;
        ItemSize(size, 0.0f);

        const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth)) size.x = ImMax(label_size.x, max_x - min_x);

        const ImVec2 text_min = pos;
        const ImVec2 text_max(min_x + size.x, pos.y + size.y);

        ImRect bb(min_x, pos.y, text_max.x, text_max.y);
        if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
            const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += (spacing_x - spacing_L);
            bb.Max.y += (spacing_y - spacing_U);
        }

        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if (span_all_columns)
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
        const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
        if (span_all_columns)
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if (!item_add) return false;

        const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
        if (disabled_item && !disabled_global) BeginDisabled();

        if (span_all_columns && window->DC.CurrentColumns) PushColumnsBackground();
        else if (span_all_columns && g.CurrentTable) TablePushBackgroundChannel();

        ImGuiButtonFlags button_flags = 0;
        if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
        if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
        if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
        if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
        if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
        if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

        const bool was_selected = selected;
        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

        if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
            if (g.NavJustMovedToId == id)  selected = pressed = true;

        // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
        if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
        {
            if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
            {
                SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
                g.NavDisableHighlight = true;
            }
        }
        if (pressed) MarkItemEdited(id);

        if (selected != was_selected)  g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;


        if (g.NavId == id) RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

        if (span_all_columns && window->DC.CurrentColumns) PopColumnsBackground();
        else if (span_all_columns && g.CurrentTable) TablePopBackgroundChannel();

        static std::map<ImGuiID, select_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, select_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.text = ImLerp(it_anim->second.text, selected ? c::accent : c::text::text_active, g.IO.DeltaTime * 6.f);

        PushFont(font::inter_semibold_widget);
        PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text));
        RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
        PopStyleColor();
        PopFont();

        if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup)) CloseCurrentPopup();

        if (disabled_item && !disabled_global) EndDisabled();

        return pressed;
    }

    bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
    {
        if (Selectable(label, *p_selected, flags, size_arg))
        {
            *p_selected = !*p_selected;
            return true;
        }
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// QUANDSCALAR
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct quand_state
    {
        float slow0, slow1, tickness;
        int move_y, move_x;
        float move_yy, move_xx;
    };

    bool QuandScalar(const char* label, ImGuiDataType data_type, void* p_data0, void* p_data1, const void* p_min0, const void* p_max0, const void* p_min1, const void* p_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
    {

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const float w = ((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns);

        ImVec2 pos = window->DC.CursorPos;
        const ImRect frame_bb(pos + ImVec2(10, 55), pos + ImVec2(w - 10, 130));
        const ImRect rect(pos, pos + ImVec2(w, 170));
        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;

        ItemSize(rect, 0.f);
        if (!ItemAdd(rect, id)) return false;

        if (format0 == NULL) format0 = DataTypeGetInfo(data_type)->PrintFmt;
        if (format1 == NULL) format1 = DataTypeGetInfo(data_type)->PrintFmt;

        bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

        bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);

        ImRect grab_bb0, grab_bb1;

        static std::map<ImGuiID, quand_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, quand_state() });
            it_anim = anim.find(id);
        }

        if ((flags & ImGuiDataType_S32) == 0 && IsMouseHoveringRect(pos, pos + ImVec2(w - 10, 130))) {
      
            it_anim->second.move_y = ImLerp(it_anim->second.move_y, *(int*)p_data1 < *(int*)p_max1 && GetAsyncKeyState(VK_DOWN) & 0x01 ? *(int*)p_data1 += 1 : *(int*)p_data1 > *(int*)p_min1 && GetAsyncKeyState(VK_UP) & 0x01 ? *(int*)p_data1 -= 1 : 0, g.IO.DeltaTime * 6.f);
            it_anim->second.move_x = ImLerp(it_anim->second.move_x, *(int*)p_data0 > *(int*)p_min0 && GetAsyncKeyState(VK_LEFT) & 0x01 ? *(int*)p_data0 -= 1 : *(int*)p_data0 < *(int*)p_max0 && GetAsyncKeyState(VK_RIGHT) & 0x01 ? *(int*)p_data0 += 1 : 0, g.IO.DeltaTime * 6.f);
      
        }
        else if ((flags & ImGuiDataType_Float) == 0 && IsMouseHoveringRect(pos, pos + ImVec2(w - 10, 130)))
        {
            it_anim->second.move_yy = ImLerp(it_anim->second.move_yy, *(float*)p_data1 < *(float*)p_max1 && GetAsyncKeyState(VK_DOWN) & 0x01 ? *(float*)p_data1 += 0.1f : *(float*)p_data1 > *(float*)p_min1 && GetAsyncKeyState(VK_UP) & 0x01 ? *(float*)p_data1 -= 0.1f : 0, g.IO.DeltaTime * 6.f);
            it_anim->second.move_xx = ImLerp(it_anim->second.move_xx, *(float*)p_data0 > *(float*)p_min0 && GetAsyncKeyState(VK_LEFT) & 0x01 ? *(float*)p_data0 -= 0.1f : *(float*)p_data0 < *(float*)p_max0 && GetAsyncKeyState(VK_RIGHT) & 0x01 ? *(float*)p_data0 += 0.1f : 0, g.IO.DeltaTime * 6.f);
        }
      
        const bool value_changed0 = SliderBehavior(ImRect(frame_bb.Min - ImVec2(7, 0), frame_bb.Max + ImVec2(5, 0)), id, data_type, p_data0, p_min0, p_max0, format0, flags, &grab_bb0);
        const bool value_changed1 = SliderBehavior(ImRect(frame_bb.Min - ImVec2(0, 7), frame_bb.Max + ImVec2(0, 4)), id, data_type, p_data1, p_max1, p_min1, format1, flags | ImGuiSliderFlags_Vertical, &grab_bb1);

        if (value_changed0) MarkItemEdited(id);

        char value_buf0[64];
        char value_buf1[64];
        const char* value_buf_end0 = value_buf0 + DataTypeFormatString(value_buf0, IM_ARRAYSIZE(value_buf0), data_type, p_data0, format0);
        const char* value_buf_end1 = value_buf1 + DataTypeFormatString(value_buf1, IM_ARRAYSIZE(value_buf1), data_type, p_data1, format1);

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(c::position_bar::background), c::position_bar::rounding);

        it_anim->second.slow0 = ImLerp(it_anim->second.slow0, grab_bb0.Min.x - (frame_bb.Min.x) + 7, g.IO.DeltaTime * 25.f);
        it_anim->second.slow1 = ImLerp(it_anim->second.slow1, grab_bb1.Min.y - (frame_bb.Min.y) + 7, g.IO.DeltaTime * 25.f);

        GetWindowDrawList()->AddLine(ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Min.y), ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Max.y), GetColorU32(c::accent), 0.5f);
        GetWindowDrawList()->AddShadowRect(ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Min.y), ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Max.y), GetColorU32(c::accent, 0.5f), 25.f, ImVec2(0, 0), 100.f);

        GetWindowDrawList()->AddLine(ImVec2(grab_bb1.Min.x, it_anim->second.slow1 + frame_bb.Min.y), ImVec2(grab_bb1.Max.x, it_anim->second.slow1 + frame_bb.Min.y), GetColorU32(c::accent), 0.5f);
        GetWindowDrawList()->AddShadowRect(ImVec2(grab_bb1.Min.x, it_anim->second.slow1 + frame_bb.Min.y), ImVec2(grab_bb1.Max.x, it_anim->second.slow1 + frame_bb.Min.y), GetColorU32(c::accent, 0.5f), 25.f, ImVec2(0, 0), 100.f);

        GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 10), GetColorU32(c::text::text_active), label);

        std::string blah0(value_buf0), blah1(value_buf1);

        std::string label_x = "X - " + blah0 + ", Y - " + blah1;

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 30), GetColorU32(c::text::text), "Select point on the screen.");

        GetWindowDrawList()->AddText(ImVec2(rect.Min.x + 10, rect.Min.y + 140), GetColorU32(c::text::text_active), label_x.c_str());
        PopFont();

    }

    bool QuandFloat(const char* label, float* v0, float* v1, float v_min0, float v_max0, float v_min1, float v_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
    {
        return QuandScalar(label, ImGuiDataType_Float, v0, v1, &v_min0, &v_max0, &v_min1, &v_max1, format0, format1, flags);
    }

    bool QuandInt(const char* label, int* v0, int* v1, int v_min0, int v_max0, int v_min1, int v_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
    {
        return QuandScalar(label, ImGuiDataType_S32, v0, v1, &v_min0, &v_max0, &v_min1, &v_max1, format0, format1, flags);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CIRCLE SLIDER
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct cslider_state {
        float plus_float;
        int plus_int;
        ImVec4 background, circle, text;
        float slow_anim, circle_anim;
        float position, tickness;
    };


    bool KnobScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = ((GetContentRegionMax().x - ((style.WindowPadding.x * 2) + style.ItemSpacing.x)) / c::columns);

        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect frame_bb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(w, 100));

        const ImRect slider_bb(window->DC.CursorPos + ImVec2(w - 30, 0), window->DC.CursorPos + ImVec2(w, 100));

        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? label_size.x : 0.0f, 0.0f));

        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;

        ItemSize(ImRect(frame_bb.Min, frame_bb.Max - ImVec2(0, 0)));

        if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

        if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

        bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags), held, pressed = ButtonBehavior(frame_bb, id, &hovered, &held, NULL);
        bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);

        ImRect grab_bb;

        static std::map<ImGuiID, cslider_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, cslider_state() });
            it_anim = anim.find(id);
        }

        GetWindowDrawList()->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(c::position_bar::background), c::position_bar::rounding);

        it_anim->second.circle_anim = ImLerp(it_anim->second.circle_anim, IsItemActive() ? 11.f : 10.f, g.IO.DeltaTime * 6.f);
        it_anim->second.tickness = ImLerp(it_anim->second.tickness, IsItemActive() ? 3.f : 5.f, g.IO.DeltaTime * 6.f);

        if ((flags & ImGuiSliderFlags_Integer) == 0) {
            it_anim->second.plus_float = ImLerp(it_anim->second.plus_float, *(float*)p_data <= *(float*)p_max && hovered && GetAsyncKeyState(VK_OEM_PLUS) & 0x01 ? *(float*)p_data += 0.1f : *(float*)p_data >= *(float*)p_min && hovered && GetAsyncKeyState(VK_OEM_MINUS) & 0x01 ? *(float*)p_data -= 0.1f : 0, g.IO.DeltaTime * 6.f);
            if (*(float*)p_data > *(float*)p_max) *(float*)p_data = *(float*)p_max;
            if (*(float*)p_data < *(float*)p_min) *(float*)p_data = *(float*)p_min;
        }
        else
        {
            it_anim->second.plus_int = ImLerp(it_anim->second.plus_int, *(int*)p_data <= *(int*)p_max && hovered && GetAsyncKeyState(VK_OEM_PLUS) & 0x01 ? *(int*)p_data += 1 : *(int*)p_data >= *(int*)p_min && hovered && GetAsyncKeyState(VK_OEM_MINUS) & 0x01 ? *(int*)p_data -= 1 : 0, g.IO.DeltaTime * 6.f);
            if (*(int*)p_data > *(int*)p_max) *(int*)p_data = *(int*)p_max;
            if (*(int*)p_data < *(int*)p_min) *(int*)p_data = *(int*)p_min;
        }

        const bool value_changed = DragBehavior(id, data_type, p_data, 0.f, p_min, p_max, format, NULL);


        if (value_changed) MarkItemEdited(id);

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        static float radius = 30.f;
        static float thickness = 6.0f;

        it_anim->second.position = ImLerp(it_anim->second.position, *static_cast<float*>(p_data) / (*reinterpret_cast<const float*>(p_max)) / 2 * (6.32f * 2), ImGui::GetIO().DeltaTime * 18.f);

        GetWindowDrawList()->PathClear();
        GetWindowDrawList()->PathArcTo(ImVec2(frame_bb.Max.x - (100 / 2), frame_bb.Min.y + (100 / 2)), radius, 0.f, 2.f * IM_PI, 40.f);
        GetWindowDrawList()->PathStroke(GetColorU32(c::slider::background), 0, thickness);

        GetWindowDrawList()->PathClear();
        GetWindowDrawList()->PathArcTo(ImVec2(frame_bb.Max.x - (100 / 2), frame_bb.Min.y + (100 / 2)), radius, IM_PI * 1.5f, IM_PI * 1.5f + it_anim->second.position, 40.f);
        GetWindowDrawList()->PathStroke(GetColorU32(c::accent), 0, it_anim->second.tickness);

        GetWindowDrawList()->AddText(ImVec2(frame_bb.Min.x + 10, frame_bb.Min.y + 10), GetColorU32(c::text::text_active), label);

        PushFont(font::inter_semibold_widget);
        GetWindowDrawList()->AddText(ImVec2(frame_bb.Min.x + 10, frame_bb.Min.y + 30), GetColorU32(c::text::text), "Drag Slider.");

        GetWindowDrawList()->AddText(ImVec2(frame_bb.Min.x + 10, frame_bb.Min.y + 70), GetColorU32(c::text::text_active), value_buf);
        PopFont();

        return value_changed;
    }

    bool KnobFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        return KnobScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
    }

    bool KnobInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        return KnobScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags | ImGuiSliderFlags_Integer);
    }
}