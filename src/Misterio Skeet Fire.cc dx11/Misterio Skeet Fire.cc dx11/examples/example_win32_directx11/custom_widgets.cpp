#define IMGUI_DEFINE_MATH_OPERATORS
#define _USE_MATH_DEFINES
#define IMGUI_DEFINE_PLACEMENT_NEW
#undef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS

#include "custom_widgets.hpp"

#include <vector>

using namespace ImGui;

namespace fonts {

    extern ImFont* sans_font;
    extern ImFont* sans_font_small;

}

namespace custom {

    void custom::GradientRect(ImDrawList* draw_list, const ImVec2 min, ImVec2 max, ImColor col_top_left, ImColor col_top_right, ImColor col_bot_right, ImColor col_bot_left, float thickness, float rounding) {

        draw_list->AddRectFilledMultiColor({ min.x + rounding, min.y }, { max.x - rounding, min.y + thickness }, col_top_left, col_top_right, col_top_right, col_top_left);
        draw_list->AddRectFilledMultiColor({ max.x - thickness, min.y + rounding }, { max.x, max.y - rounding }, col_top_right, col_top_right, col_bot_right, col_bot_right);
        draw_list->AddRectFilledMultiColor({ min.x + rounding, max.y - thickness }, { max.x - rounding, max.y }, col_bot_left, col_bot_right, col_bot_right, col_bot_left);
        draw_list->AddRectFilledMultiColor({ min.x, min.y + rounding }, { min.x + thickness, max.y - rounding }, col_top_left, col_top_left, col_bot_left, col_bot_left);

        draw_list->PathArcToFast(min + ImVec2(rounding, rounding) + ImVec2(thickness, thickness) / 2, rounding, 6, 9);
        draw_list->PathStroke(col_top_right, 0, thickness);
        draw_list->PathArcToFast({ max.x - rounding - thickness / 2, min.y + rounding + thickness / 2 }, rounding, 9, 12);
        draw_list->PathStroke(col_top_right, 0, thickness);
        draw_list->PathArcToFast(max - ImVec2(rounding, rounding) - ImVec2(thickness, thickness) / 2, rounding, 0, 3);
        draw_list->PathStroke(col_bot_right, 0, thickness);
        draw_list->PathArcToFast({ min.x + rounding + thickness / 2, max.y - rounding }, rounding, 3, 6);
        draw_list->PathStroke(col_bot_left, 0, thickness);
    }

    void ShadowText(ImFont* font, const char* text, ImVec2 pos, ImVec2 rel, ImColor color, bool outline, float size) {
        float dpiscale = 1.50f;
        float relx;
        float rely;
        switch ((int)rel.x) {
        case 0:
            relx = (ImGui::CalcTextSize(text).x / (22 * dpiscale) * (size * dpiscale)) / 2;
            break;
        case 1:
            relx = 0;
            break;
        case -1:
            relx = (ImGui::CalcTextSize(text).x / (22 * dpiscale) * (size * dpiscale));
            break;
        }
        switch ((int)rel.y) {
        case 0:
            rely = (ImGui::CalcTextSize(text).y / (22 * dpiscale) * (size * dpiscale)) / 2;
            break;
        case 1:
            rely = 0;
            break;
        case -1:
            rely = (ImGui::CalcTextSize(text).y / (22 * dpiscale) * (size * dpiscale));
            break;
        }
        if (outline) {
            ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx - 1, pos.y - rely - 1), ImColor(0, 0, 0), text);
            ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx + 1, pos.y - rely - 1), ImColor(0, 0, 0), text);
            ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx - 1, pos.y - rely + 1), ImColor(0, 0, 0), text);
            ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx + 1, pos.y - rely + 1), ImColor(0, 0, 0), text);
            ImGui::GetWindowDrawList()->AddText(font, size * dpiscale, ImVec2(pos.x - relx, pos.y - rely), color, text);
        }
    }

    void Spacing(float h)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems)
            return;
        ItemSize(ImVec2(0, h));
    }

    void RenderTextColor(ImFont* font, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, const char* text, const ImVec2& align)
    {
        PushFont(font);
        PushStyleColor(ImGuiCol_Text, col);
        RenderTextClipped(p_min, p_max, text, NULL, NULL, align, NULL);
        PopStyleColor();
        PopFont();
    }

    struct tab_state
    {
        ImVec4 icon_color, shadow_color;
        float inflating;
    };

    bool Tab(const char* label, ImTextureID icon, bool active)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect rect(pos, pos + ImVec2(62, 62));
        ItemSize(ImRect(rect.Min, rect.Max + ImVec2(0, 15)));
        if (!ItemAdd(rect, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(rect, id, &hovered, &held);
        if (pressed)
            MarkItemEdited(id);

        static std::map<ImGuiID, tab_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, tab_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.icon_color = ImLerp(it_anim->second.icon_color, active ? colors::accent_color : hovered ? colors::tabs::icon_hovered : colors::tabs::icon_inactive, g.IO.DeltaTime * 5.f);
        it_anim->second.shadow_color = ImLerp(it_anim->second.shadow_color, active ? colors::accent_color : hovered ? colors::tabs::shadow_inactive : colors::tabs::shadow_inactive, g.IO.DeltaTime * 5.f);
        it_anim->second.inflating = ImLerp(it_anim->second.inflating, active ? 0.f : 10.f, g.IO.DeltaTime * 6.f);

        window->DrawList->AddRectFilled(rect.Min, rect.Max, ImGui::GetColorU32(colors::tabs::fill));
        window->DrawList->AddRect(rect.Min, rect.Max, ImGui::GetColorU32(colors::tabs::fill_outline), 0, 0, 0.2f);
        window->DrawList->AddImage(icon, rect.Min + ImVec2(18, 18), rect.Min + ImVec2(42, 42), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(it_anim->second.icon_color));
        window->DrawList->AddShadowCircle(rect.Min + ImVec2(30, 30), 12.f, GetColorU32(it_anim->second.shadow_color), 30.f, ImVec2(0, 0));

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed;
    }

    bool BeginChildEx(ImTextureID icon, const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;

        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoBackground;
        flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

        // Size
        const ImVec2 content_avail = GetContentRegionAvail();
        ImVec2 size = ImFloor(size_arg);
        const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
        if (size.x <= 0.0f)
            size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too many issues)
        if (size.y <= 0.0f)
            size.y = ImMax(content_avail.y + size.y, 4.0f);
        SetNextWindowSize(size);

        // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
        const char* temp_window_name;
        if (name)
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
        else
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

        if (!border) {

            GetWindowDrawList()->AddRectFilled(parent_window->DC.CursorPos + ImVec2(0.0f, 0.0f), parent_window->DC.CursorPos + ImVec2(size_arg.x + 0.2f, size_arg.y), GetColorU32(colors::child::child_background));

            GradientRect(GetWindowDrawList(), parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size_arg.x + 0.2f, size_arg.y), GetColorU32(colors::child::col_top_left), GetColorU32(colors::child::col_top_right), GetColorU32(colors::child::col_bot_right), GetColorU32(colors::child::col_bot_right), 1, 0);

            //ShadowText(fonts::sans_font, name, parent_window->DC.CursorPos + ImVec2(50, 19), ImVec2(0, -1), ImGui::GetColorU32(colors::child::child_text), true, 12);

            GetWindowDrawList()->AddText(fonts::sans_font, 18.f, parent_window->DC.CursorPos + ImVec2(35, 9), ImGui::GetColorU32(colors::child::child_text), name);

            GetWindowDrawList()->AddImage(icon, parent_window->DC.CursorPos + ImVec2(10, 10), parent_window->DC.CursorPos + ImVec2(26, 26), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(colors::accent_color));


        }
        bool ret = Begin(temp_window_name, NULL, flags);

        ImGuiWindow* child_window = g.CurrentWindow;
        child_window->ChildId = id;
        child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

        if (child_window->BeginCount == 1)
            parent_window->DC.CursorPos = child_window->Pos;

        // Process navigation-in immediately so NavInit can run on first frame
        if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
        {
            FocusWindow(child_window);
            NavInitWindow(child_window, false);
            SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
            g.ActiveIdSource = ImGuiInputSource_Nav;
        }
        return ret;
    }

    bool BeginChild(ImTextureID icon, const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 50));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 28));
        return BeginChildEx(icon, str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
    }

    bool BeginChild(ImTextureID icon, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
    {
        IM_ASSERT(id != 0);
        return BeginChildEx(icon, NULL, id, size_arg, border, extra_flags);
    }

    void EndChild()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        PopStyleVar(2);
        IM_ASSERT(g.WithinEndChild == false);
        IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

        g.WithinEndChild = true;
        if (window->BeginCount > 1)
        {
            End();
        }
        else
        {
            ImVec2 sz = window->Size;
            if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
                sz.x = ImMax(4.0f, sz.x);
            if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
                sz.y = ImMax(4.0f, sz.y);
            End();

            ImGuiWindow* parent_window = g.CurrentWindow;
            ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
            ItemSize(sz);
            if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
            {
                ItemAdd(bb, window->ChildId);
                RenderNavHighlight(bb, window->ChildId);

                // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
                if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
                    RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
            }
            else
            {
                // Not navigable into
                ItemAdd(bb, 0);
            }
            if (g.HoveredWindow == window)
                g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
        }
        g.WithinEndChild = false;
        g.LogLinePosY = -FLT_MAX; // To enforce a carriage return

    }

    struct checkbox_state
    {
        ImVec4 text, filled, shadow_opacity;
    };

    bool Checkbox(const char* label, bool* v)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const float w = GetWindowWidth();
        const ImVec2 pos = window->DC.CursorPos;

        const ImRect rect(pos - ImVec2(5, 0), pos + ImVec2(8, 12));
        const ImRect total_bb(pos, pos + ImVec2(w, label_size.y));

        ItemSize(ImRect(total_bb.Min, total_bb.Max - ImVec2(0, 13)));
        if (!ItemAdd(total_bb, id))
        {
            IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
            return false;
        }

        bool hovered, held;
        bool pressed = ButtonBehavior(rect, id, &hovered, &held);
        if (pressed)
        {
            *v = !(*v);
            MarkItemEdited(id);
        }

        static std::map<ImGuiID, checkbox_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, checkbox_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.filled = ImLerp(it_anim->second.filled, *v ? colors::accent_color : hovered ? colors::checkbox::checkbox_bg_inactive : colors::checkbox::checkbox_bg_inactive, g.IO.DeltaTime * 6.f);
        it_anim->second.shadow_opacity = ImLerp(it_anim->second.shadow_opacity, *v ? colors::accent_color : hovered ? colors::checkbox::shadow_inactive : colors::checkbox::shadow_inactive, g.IO.DeltaTime * 6.f);

        window->DrawList->AddRectFilled(rect.Min, rect.Max, GetColorU32(it_anim->second.filled));
        window->DrawList->AddRect(rect.Min, rect.Max, GetColorU32(colors::checkbox::rect));
        window->DrawList->AddShadowRect(rect.Min, rect.Max, GetColorU32(it_anim->second.shadow_opacity), 8.f, ImVec2(0, 0));
        window->DrawList->AddText(fonts::sans_font_small, 16.f, rect.Min + ImVec2(26, -3), ImGui::GetColorU32(colors::checkbox::text_active), label);

        return pressed;
    }

    struct slider_state
    {
        ImVec4 text;
        float slow, blick;
    };

    bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags) {

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = GetWindowWidth() - 20;

        const ImVec2 label_size = CalcTextSize(label, 0, 1);
        const ImRect frame_bb(window->DC.CursorPos + ImVec2(23, 13), window->DC.CursorPos + ImVec2(w - 35, 20));
        const ImRect frame_bb_hovered(window->DC.CursorPos + ImVec2(0, 13), window->DC.CursorPos + ImVec2(w - 45, 20));
        const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

        ItemAdd(total_bb, id, &frame_bb);
        ItemSize(ImRect(total_bb.Min, total_bb.Max - ImVec2(0, 2)));

        if (format == NULL)
            format = DataTypeGetInfo(data_type)->PrintFmt;

        const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        if (clicked)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }

        ImRect grab_bb;
        const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
        if (value_changed)
            MarkItemEdited(id);

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        static std::unordered_map< ImGuiID, float > values;
        auto value = values.find(id);

        if (value == values.end()) {

            values.insert({ id, 0.f });
            value = values.find(id);
        }

        float percent = (grab_bb.Max.x - window->DC.CursorPos.x - frame_bb.Min.x + window->Pos.x + 11) / frame_bb.GetWidth();

        value->second = ImLerp(value->second, percent * frame_bb.GetWidth(), 0.35f);

        window->DrawList->AddRectFilled(frame_bb_hovered.Min + ImVec2(23, 0), frame_bb_hovered.Max, GetColorU32(colors::slider::slider_inactive));

        window->DrawList->AddRect(frame_bb_hovered.Min + ImVec2(23, 0), frame_bb_hovered.Max, GetColorU32(colors::slider::rect));

        window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(frame_bb.Min.x + value->second, frame_bb.Max.y), GetColorU32(colors::accent_color, style.Alpha));

        window->DrawList->AddShadowRect(frame_bb.Min, ImVec2(frame_bb.Min.x + value->second, frame_bb.Max.y), GetColorU32(colors::accent_color, style.Alpha), 10.f, ImVec2(0, 0));

        PushFont(fonts::sans_font_small);

        RenderText(total_bb.Min + ImVec2(21, -8), label);

        PopFont();

        ShadowText(fonts::sans_font, value_buf, ImVec2(frame_bb.Min.x + 6 + value->second - CalcTextSize(value_buf, 0, 1).x / 2, frame_bb.Min.y + 25 - CalcTextSize(value_buf, 0, 1).y), ImVec2(0, -1), ImGui::GetColorU32(colors::child::child_text), true, 12);

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

    struct begin_state
    {
        ImVec4 background, text, outline;
        float open, alpha, combo_size = 0.f, shadow_opticaly;
        bool opened_combo = false, hovered = false;
        float arrow_roll;
    };

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext& g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    static const char* Items_ArrayGetter(void* data, int idx)
    {
        const char* const* items = (const char* const*)data;
        return items[idx];
    }

    bool BeginCombo(const char* label, const char* preview_value, int val, bool multi, ImGuiComboFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();

        g.NextWindowData.ClearFlags();
        if (window->SkipItems) return false;

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const float w = ((GetContentRegionMax().x - style.WindowPadding.x));
        const float y_size = 50;
        const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();

        const ImRect bb(window->DC.CursorPos + ImVec2(0, 12), window->DC.CursorPos + ImVec2(w, 50));
        const ImRect rect(window->DC.CursorPos + ImVec2(20, 13), window->DC.CursorPos + ImVec2(w - 21, 35));

        const ImRect total_bb(bb.Min, bb.Max);
        ItemSize(ImRect(total_bb.Min, total_bb.Max - ImVec2(0, 10)));

        if (!ItemAdd(bb, id, &bb)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held);

        static std::map<ImGuiID, begin_state> anim;
        begin_state& state = anim[id];

        if (hovered && g.IO.MouseClicked[0] || state.opened_combo && g.IO.MouseClicked[0] && !state.hovered) state.opened_combo = !state.opened_combo;

        const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);

        state.arrow_roll = ImLerp(state.arrow_roll, state.opened_combo ? -1.f : 1.f, g.IO.DeltaTime * 6.f);
        state.text = ImLerp(state.text, state.opened_combo ? colors::combo::text_active : hovered ? colors::combo::text_hovered : colors::combo::text_inactive, g.IO.DeltaTime * 6.f);
        state.combo_size = ImLerp(state.combo_size, state.opened_combo ? (val * 27) : 0.f, g.IO.DeltaTime * 12.f);

        GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, GetColorU32(colors::combo::combo_bg));

        GetWindowDrawList()->AddRect(rect.Min, rect.Max, GetColorU32(colors::combo::rect));

        GetWindowDrawList()->AddText(fonts::sans_font_small, 16.f, bb.Min + ImVec2(21, -20), GetColorU32(colors::combo::text_active), label);

        PushFont(fonts::sans_font_small);

        PushStyleColor(ImGuiCol_Text, GetColorU32(colors::combo::text_hovered));

        RenderTextClipped(bb.Min + ImVec2(28, 4), ImVec2(value_x2, bb.Max.y + 5), preview_value, NULL, NULL);

        PopStyleColor();

        PopFont();

        if (!IsRectVisible(rect.Min, rect.Max + ImVec2(0, 2)))
        {
            state.opened_combo = false;
            state.combo_size = 0.f;
        }

        if (!state.opened_combo && state.combo_size < 2.f) return false;

        ImGui::SetNextWindowPos(ImVec2(rect.Min.x, rect.Max.y + 5));
        ImGui::SetNextWindowSize(ImVec2(rect.GetWidth(), state.combo_size));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse;

        PushStyleColor(ImGuiCol_WindowBg, colors::combo::preview_bg);
        PushStyleColor(ImGuiCol_Border, colors::combo::rect);
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

        bool ret = Begin(label, NULL, window_flags);

        PopStyleVar(2);
        PopStyleColor(2);

        state.hovered = IsWindowHovered();

        if (multi && state.hovered && g.IO.MouseClicked[0]) state.opened_combo = false;

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

        if (BeginCombo(label, preview.c_str(), count, NULL, NULL))
        {
            for (auto i = 0; i < count; i++)
            {
                PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
                PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
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

        if (window->SkipItems || !(g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible)) return false;

        IM_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y);

        if (!window->ClipRect.Overlaps(preview_data->PreviewRect)) return false;

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

        if (!BeginCombo(label, preview_value, items_count, ImGuiComboFlags_None, NULL)) return false;

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

    struct select_state
    {
        ImVec4 text;
        float opticaly;
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
        select_state& state = anim[id];

        state.text = ImLerp(state.text, selected ? colors::accent_color : colors::combo::text_inactive, g.IO.DeltaTime * 6.f);
        state.opticaly = ImLerp(state.opticaly, selected ? 1.f : 0.f, g.IO.DeltaTime * 6.f);

        RenderTextColor(fonts::sans_font_small, text_min + ImVec2(1, 1), text_max, GetColorU32(state.text), label, ImVec2(0.f, 0.5f));

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


    const char* keys[] = {
    "None",
    "Mouse 1",
    "Mouse 2",
    "CN",
    "Mouse 3",
    "Mouse 4",
    "Mouse 5",
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
    "ESC",
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
    "Numpad 0",
    "Numpad 1",
    "Numpad 2",
    "Numpad 3",
    "Numpad 4",
    "Numpad 5",
    "Numpad 6",
    "Numpad 7",
    "Numpad 8",
    "Numpad 9",
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

    struct keybind_state
    {
        ImVec4 mode_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 key_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 text_color = ImVec4(1.f, 1.f, 1.f, 1.f);
        float alpha = 0.f;
        bool active = false;
        bool hovered = false;
    };

    bool Keybind(const char* label, int* key, int* variable, bool widget_label) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        ImGuiIO& io = g.IO;
        const ImGuiStyle& style = g.Style;

        ImVec2 mode_size;
        ImVec2 key_size;

        SetCursorPos(GetCursorPos() - ImVec2(0, 10));

        const ImGuiID id = window->GetID(label);
        const float size = GetWindowWidth() - 25;
        const ImRect rect(window->DC.CursorPos + ImVec2(size - 50, -35), window->DC.CursorPos + ImVec2(size, -30));
        const ImRect rect_text(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(size, 30));
        ImRect mode_rect(window->DC.CursorPos + ImVec2(size - 20, 0), window->DC.CursorPos + ImVec2(size, 20));
        ImRect key_rect;

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos + ImVec2(200, 0), pos + ImVec2(270, 35));

        ImGui::ItemSize(ImRect(rect.Min, rect.Max + ImVec2(0, 33)));
        if (!ImGui::ItemAdd(rect, id))
            return false;

        bool popup_open = ImGui::IsPopupOpen(id, ImGuiPopupFlags_None);
        bool hovered_rect = ItemHoverable(rect, id, 0);

        bool value_changed = false;
        int k = *key;

        mode_rect.Min.x -= mode_size.x;
        key_rect = ImRect(mode_rect.Min, mode_rect.Max);

        bool hovered_mode = ItemHoverable(mode_rect, id, 0);

        char buf_display1[64] = "[-]";

        std::string active_key = "[";
        std::string active_key2 = "]";
        active_key += keys[*key] + active_key2;

        if (*key != 0 && g.ActiveId != id) {
            strcpy_s(buf_display1, active_key.c_str());
        }
        else if (g.ActiveId == id) {
            strcpy_s(buf_display1, "[-]");
        }
        key_size = CalcTextSize(buf_display1, NULL, true);
        key_rect.Min.x -= key_size.x;

        bool hovered_key = ItemHoverable(key_rect, id, 0);

        std::string nametwo = "keybind_state";
        nametwo += label;


        if (hovered_key && io.MouseClicked[0])
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

            if (IsKeyPressedMap(ImGuiKey_Escape)) {
                *key = 0;
                ImGui::ClearActiveID();
            }
            else {
                *key = k;
            }
        }

        static std::map<ImGuiID, keybind_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, keybind_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.text_color = ImLerp(it_anim->second.text_color, IsItemActive() ? colors::binder::text_active : hovered_rect ? colors::binder::text_hovered : colors::binder::text_inactive, g.IO.DeltaTime * 6.f);

        static std::map<ImGuiID, float> alpha_animation;
        auto it_alpha = alpha_animation.find(id);
        if (it_alpha == alpha_animation.end())
        {
            alpha_animation.insert({ id, 0.f });
            it_alpha = alpha_animation.find(id);
        }

        it_alpha->second = ImLerp(it_alpha->second, hovered_rect ? 0.2f : 0.f, g.IO.DeltaTime * 6.f);

        it_alpha->second *= style.Alpha;

        if (widget_label) window->DrawList->AddText(rect_text.Min + ImVec2(-3, 4), GetColorU32(it_anim->second.text_color), label);

        PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text_color));

        RenderTextClipped(key_rect.Min + ImVec2(0, 3), key_rect.Max, buf_display1, NULL, NULL, ImVec2(0.5f, 0.5f));

        PopStyleColor();



        if (hovered_key && g.IO.MouseClicked[1] || it_anim->second.active && (g.IO.MouseClicked[0] || g.IO.MouseClicked[1]) && !it_anim->second.hovered)
            it_anim->second.active = !it_anim->second.active;

        it_anim->second.alpha = ImClamp(it_anim->second.alpha + (8.f * g.IO.DeltaTime * (it_anim->second.active ? 1.f : -1.f)), 0.f, 1.f);

        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
        PushStyleVar(ImGuiStyleVar_Alpha, it_anim->second.alpha);
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
        PushStyleVar(ImGuiStyleVar_WindowRounding, 4);
        PushStyleColor(ImGuiCol_WindowBg, GetColorU32(colors::binder::bind_settings));
        PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        if (it_anim->second.alpha >= 0.01f)
        {
            SetNextWindowPos(ImVec2(key_rect.GetCenter().x - (34 + 17 / 2), key_rect.Max.y + 5));
            SetNextWindowSize(ImVec2(69 + 20, 90));

            Begin(label, NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
            {
                it_anim->second.hovered = IsWindowHovered();

                ImGui::SetCursorPos(GetCursorPos() + ImVec2((86 - CalcTextSize("Hold").x) / 2, 0));
                if (Selectable("Hold", *variable == 0))
                {
                    *variable = 0;
                    it_anim->second.active = false;
                }

                ImGui::SetCursorPos(GetCursorPos() + ImVec2((86 - CalcTextSize("Toggle").x) / 2, 0));
                if (Selectable("Toggle", *variable == 1))
                {
                    *variable = 1;
                    it_anim->second.active = false;
                }

                ImGui::SetCursorPos(GetCursorPos() + ImVec2((86 - CalcTextSize("Always").x) / 2, 0));
                if (Selectable("Always", *variable == 2))
                {
                    *variable = 2;
                    it_anim->second.active = false;
                }
            }
            End();
        }
        PopStyleColor();
        PopStyleVar(5);

        return value_changed;
    }

    void KeybindBox(const char* label, bool* v, int* key, int* variable)
    {
        std::string label_key = "bind", label_bind = label_key + label;

        SetCursorPos(GetCursorPos() - ImVec2(5, 0));
        custom::Keybind(label_bind.c_str(), key, variable, false);

        SetCursorPos(GetCursorPos() - ImVec2(0, GetStyle().ItemSpacing.y * 2 + 2));
        Checkbox(label, v);
    }

    struct button_state
    {
        float text_alpha = 0.3f;
        float rect_alpha = 0.f;
        bool active = false;
        ImVec4 text, bg;
    };

    bool ButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
            pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = size_arg;
        if (size.x <= 0.0f)
            size.x = GetWindowWidth() - (g.Style.WindowPadding.x * 2);
        if (size.y <= 0.0f)
            size.y = 27;

        const ImRect rect(pos + ImVec2(-5, 0), pos + size);
        ItemSize(ImRect(rect.Min, rect.Max - ImVec2(0, 13)));
        if (!ItemAdd(rect, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(rect, id, &hovered, &held, flags);

        // Render
        static std::map<ImGuiID, button_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, button_state() });
            it_anim = anim.find(id);
        }

        if (pressed)
            it_anim->second.active = true;


        if (it_anim->second.rect_alpha >= 0.9f)
            it_anim->second.active = false;

        it_anim->second.text = ImLerp(it_anim->second.text, IsItemActive() ? colors::button::text_active : colors::button::text_inactive, g.IO.DeltaTime * 2.f);

        const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;

        window->DrawList->AddRectFilled(rect.Min, rect.Max, GetColorU32(colors::button::button_bg));
        window->DrawList->AddRectFilledMultiColor(rect.Min, rect.Max, GetColorU32(colors::button::button_bg), GetColorU32(colors::button::button_bg), GetColorU32(colors::button::button_gradient), GetColorU32(colors::button::button_gradient));
        window->DrawList->AddRect(rect.Min, rect.Max, GetColorU32(colors::button::rect));

        PushFont(fonts::sans_font_small);
        PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text));
        RenderTextClipped(rect.Min - ImVec2(0, 2), rect.Max - ImVec2(0, 2), label, NULL, NULL, ImVec2(0.5f, 0.5f));
        PopStyleColor();
        PopFont();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed;
    }

    bool Button(const char* label, const ImVec2& size_arg)
    {
        return ButtonEx(label, size_arg, ImGuiButtonFlags_None);
    }

    static void ColorEditRestoreH(const float* col, float* H)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(g.ColorEditCurrentID != 0);
        if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
            return;
        *H = g.ColorEditSavedHue;
    }

    static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(g.ColorEditCurrentID != 0);
        if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0))) return;

        if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
            *H = g.ColorEditSavedHue;

        if (*V == 0.0f) *S = g.ColorEditSavedSat;
    }


    struct edit_state
    {
        ImVec4 text;
        float alpha = 0.f, alpha_search;
        bool hovered, active;
    };

    bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const float square_sz = 18.f;
        const float w_full = CalcItemWidth();
        const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : square_sz;
        const float w_inputs = w_full - w_button;
        const char* label_display_end = FindRenderedTextEnd(label);
        g.NextItemData.ClearFlags();

        char buf[64];
        static bool search_col = false;

        BeginGroup();
        PushID(label);
        const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
        if (set_current_color_edit_id) g.ColorEditCurrentID = window->IDStack.back();

        const ImGuiColorEditFlags flags_untouched = flags;

        if (flags & ImGuiColorEditFlags_NoInputs) flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

        if (!(flags & ImGuiColorEditFlags_NoOptions)) ColorEditOptionsPopup(col, flags);

        if (!(flags & ImGuiColorEditFlags_DisplayMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
        if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
        if (!(flags & ImGuiColorEditFlags_PickerMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
        if (!(flags & ImGuiColorEditFlags_InputMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
        flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));

        const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
        const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
        const int components = alpha ? 4 : 3;

        float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
        {
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
                {   "%3d",   "%3d",   "%3d",   "%3d" },
                { "R:%3d", "G:%3d", "B:%3d", "A:%3d" },
                { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }
            };

            static const char* fmt_table_float[3][4] =
            {
                {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" },
                { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" },
                { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }
            };

            const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

            for (int n = 0; n < components; n++)
            {
                if (n > 0) SameLine(0, style.ItemInnerSpacing.x);
                SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

                if (flags & ImGuiColorEditFlags_Float)
                {
                    value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                    value_changed_as_float |= value_changed;
                }
                else
                {
                    value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
                }
                if (!(flags & ImGuiColorEditFlags_NoOptions)) OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
            }
        }

        static std::map<ImGuiID, edit_state> anim;
        edit_state& state = anim[ImGui::GetID(label)];

        ImGuiWindow* picker_active_window = NULL;
        if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
        {
            const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

            const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
            if (ColorButton("##ColorButton", col_v4, flags, ImVec2(20, 20)))
            {
                if (!(flags & ImGuiColorEditFlags_NoPicker))
                {
                    g.ColorPickerRef = col_v4;
                    OpenPopup("picker");
                    SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
                }
            }

            if (!(flags & ImGuiColorEditFlags_NoOptions)) OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

            if (ItemHoverable(g.LastItemData.Rect, g.LastItemData.ID, NULL) && g.IO.MouseClicked[0] || state.active && !search_col && g.IO.MouseClicked[0] && !state.hovered)
                state.active = !state.active;

            state.alpha_search = ImLerp(state.alpha_search, search_col ? 1.f : 0.f, g.IO.DeltaTime * 6.f);
            state.alpha = ImClamp(state.alpha + (8.f * g.IO.DeltaTime * (state.active ? 1.f : -1.f)), 0.f, 1.f);

            PushStyleColor(ImGuiCol_WindowBg, GetColorU32(colors::color_picker::picker_bg));
            PushStyleColor(ImGuiCol_Border, GetColorU32(colors::color_picker::picker_rect));

            PushStyleVar(ImGuiStyleVar_Alpha, state.alpha);
            PushStyleVar(ImGuiStyleVar_WindowRounding, 1);
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
            PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);

            POINT cursorPos;
            COLORREF color;

            if (state.active)
            {
                SetNextWindowPos(g.LastItemData.Rect.GetTR() + ImVec2(-45, -5));

                Begin("picker", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
                {
                    state.hovered = IsWindowHovered();

                    ImVec4 col_v4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);

                    GetCursorPos(&cursorPos);
                    HDC hdc = GetDC(NULL);
                    color = GetPixel(hdc, cursorPos.x, cursorPos.y);

                    if (search_col)
                    {

                        static DWORD dwTickStart = GetTickCount();
                        if (GetTickCount() - dwTickStart > 150)
                        {
                            col[0] = GetRValue(color) / 255.f;
                            col[1] = GetGValue(color) / 255.f;
                            col[2] = GetBValue(color) / 255.f;
                            dwTickStart = GetTickCount();

                            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) search_col = false;

                        }
                    }

                    if (alpha)
                        ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
                    else
                        ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));

                    GetWindowDrawList()->AddText(fonts::sans_font_small, 16.f, GetCursorScreenPos() - ImVec2(0, 2), GetColorU32(colors::color_picker::text_active), (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel)) ? label : "Color picker");


                    ImGui::SetCursorPosY(GetCursorPosY() + 30);

                    picker_active_window = g.CurrentWindow;
                    ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                    ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;

                    SetNextItemWidth(square_sz * 11.5f);
                    value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);


                }
                End();
            }
        }
        PopStyleColor(2);
        PopStyleVar(4);

        state.text = ImLerp(state.text, state.active ? colors::color_picker::text_active : colors::color_picker::text_inactive, g.IO.DeltaTime * 6.f);

        if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
        {
            SameLine(0.0f, style.ItemInnerSpacing.x);
            window->DC.CursorPos.x = pos.x - w_button + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full);

            RenderTextColor(fonts::sans_font_small, pos + ImVec2(22, -6), pos + ImVec2(GetContentRegionMax().x, 30), GetColorU32(colors::color_picker::text_active), label, ImVec2(0.0, 0.2));

        }

        PopID();
        EndGroup();

        return value_changed;
    }

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
    };

    bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImDrawList* draw_list = window->DrawList;
        ImGuiStyle& style = g.Style;
        ImGuiIO& io = g.IO;

        const float width = CalcItemWidth();
        g.NextItemData.ClearFlags();

        PushID(label);
        const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
        if (set_current_color_edit_id)
            g.ColorEditCurrentID = window->IDStack.back();
        BeginGroup();

        if (!(flags & ImGuiColorEditFlags_NoSidePreview)) flags |= ImGuiColorEditFlags_NoSmallPreview;

        if (!(flags & ImGuiColorEditFlags_NoOptions)) ColorPickerOptionsPopup(col, flags);

        if (!(flags & ImGuiColorEditFlags_PickerMask_)) flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
        if (!(flags & ImGuiColorEditFlags_InputMask_)) flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_));
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));
        if (!(flags & ImGuiColorEditFlags_NoOptions)) flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

        int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
        bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
        ImVec2 picker_pos = window->DC.CursorPos;
        float square_sz = GetFrameHeight();
        float bars_width = 15.f;
        float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width));
        float bar0_pos_x = picker_pos.x + sv_picker_size + 10;
        float bar1_pos_x = bar0_pos_x + bars_width + 10;
        float bars_triangles_half_sz = IM_TRUNC(bars_width * 0.20f);

        float backup_initial_col[4];
        memcpy(backup_initial_col, col, components * sizeof(float));

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

        InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
        if (IsItemActive())
        {
            S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
            V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
            ColorEditRestoreH(col, &H);
            value_changed = value_changed_sv = true;
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions)) OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

        SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
        InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
            value_changed = value_changed_h = true;
        }

        if (alpha_bar)
        {
            SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
            InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
            if (IsItemActive())
            {
                col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
                value_changed = true;
            }
        }

        if (!(flags & ImGuiColorEditFlags_NoLabel))
        {
            const char* label_display_end = FindRenderedTextEnd(label);
            if (label != label_display_end)
            {
                if ((flags & ImGuiColorEditFlags_NoSidePreview))
                    SameLine(0, style.ItemInnerSpacing.x);
                TextEx(label, label_display_end);
            }
        }

        if (value_changed_h || value_changed_sv)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
                g.ColorEditSavedHue = H;
                g.ColorEditSavedSat = S;
                g.ColorEditSavedID = g.ColorEditCurrentID;
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
                ColorEditRestoreHS(col, &H, &S, &V);
            }
            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                H = col[0];
                S = col[1];
                V = col[2];
                ColorConvertHSVtoRGB(H, S, V, R, G, B);
            }
        }

        const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
        const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
        const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
        const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
        const ImU32 col_hues[6 + 1] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(0,255,255,style_alpha8), IM_COL32(0,0,255,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

        ImVec4 hue_color_f(1, 1, 1, style.Alpha); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
        ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
        ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha));

        static std::map<ImGuiID, picker_state> anim;
        picker_state& state = anim[ImGui::GetID(label)];

        ImVec2 sv_cursor_pos;

        draw_list->AddRectFilledMultiColorRounded(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), col_white, hue_color32, hue_color32, col_white, 5.f);
        draw_list->AddRectFilledMultiColorRounded(picker_pos - ImVec2(1, 1), picker_pos + ImVec2(sv_picker_size + 1, sv_picker_size + 1), 0, 0, col_black, col_black, 5.f);

        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2);
        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * sv_picker_size), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

        for (int i = 0; i < 6; ++i) draw_list->AddRectFilledMultiColorRounded(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6) - (i == 5 ? 1 : 0)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6) + (i == 0 ? 1 : 0)), col_hues[i], col_hues[i], col_hues[i + 1], col_hues[i + 1], 10.f, i == 0 ? ImDrawFlags_RoundCornersTop : i == 5 ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersNone);

        float bar0_line_y = IM_ROUND(picker_pos.y + H * sv_picker_size);
        bar0_line_y = ImClamp(bar0_line_y, picker_pos.y + 3.f, picker_pos.y + (sv_picker_size - 13));

        state.hue_bar = ImLerp(state.hue_bar, bar0_line_y + 5, g.IO.DeltaTime * 24.f);
        draw_list->AddCircleFilled(ImVec2(bar0_pos_x + 7.5f, state.hue_bar), 4.5f, GetColorU32(colors::color_picker::picker_circle), 100.f);

        draw_list->AddShadowCircle(ImVec2(bar0_pos_x + 7.5f, state.hue_bar), 4.5f, GetColorU32(colors::color_picker::picker_circle_shadow), 35.f, ImVec2(0, 0));

        float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
        int sv_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(sv_cursor_rad);

        state.circle_move = ImLerp(state.circle_move, sv_cursor_pos, g.IO.DeltaTime * 10.f);
        state.circle = ImLerp(state.circle, value_changed_sv ? 6.0f : 4.0f, g.IO.DeltaTime * 24.f);

        draw_list->AddCircle(state.circle_move, state.circle, col_white, sv_cursor_segments, 2.f);

        draw_list->AddShadowCircle(state.circle_move, state.circle, GetColorU32(colors::color_picker::picker_circle_shadow), 30.f, ImVec2(0, 0));

        if (alpha_bar)
        {
            float alpha = ImSaturate(col[3]);
            ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);

            draw_list->AddRectFilledMultiColorRounded(bar1_bb.Min, bar1_bb.Max, user_col32_striped_of_alpha, user_col32_striped_of_alpha, user_col32_striped_of_alpha & ~IM_COL32_A_MASK, user_col32_striped_of_alpha & ~IM_COL32_A_MASK, 100.f);

            float bar1_line_y = IM_ROUND(picker_pos.y + (1.0f - alpha) * sv_picker_size);
            bar1_line_y = ImClamp(bar1_line_y, picker_pos.y + 3.f, picker_pos.y + (sv_picker_size - 13));

            state.alpha_bar = ImLerp(state.alpha_bar, bar1_line_y + 5, g.IO.DeltaTime * 24.f);
            draw_list->AddCircleFilled(ImVec2(bar1_pos_x + 7.5f, state.alpha_bar), 4.5f, GetColorU32(colors::color_picker::picker_circle), 100.f);

            draw_list->AddShadowCircle(ImVec2(bar1_pos_x + 7.5f, state.alpha_bar), 4.5f, GetColorU32(colors::color_picker::picker_circle_shadow), 35.f, ImVec2(0, 0));
        }

        EndGroup();

        if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0) value_changed = false;
        if (value_changed) MarkItemEdited(g.LastItemData.ID);

        PopID();

        return value_changed;
    }


    bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->GetID(desc_id);
        const float default_size = GetFrameHeight();
        const ImVec2 pos = window->DC.CursorPos;
        const float width = GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x;
        const ImRect rect(pos, pos + ImVec2(width, 50));

        const ImRect clickable(rect.Min + ImVec2(width - 40, 0), rect.Max - ImVec2(2, 35));

        ItemSize(ImRect(rect.Min, rect.Max - ImVec2(0, 0)));
        if (!ItemAdd((flags & ImGuiColorEditFlags_NoLabel) ? clickable : rect, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(clickable, id, &hovered, &held);

        if (flags & ImGuiColorEditFlags_NoAlpha) flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

        ImVec4 col_rgb = col;
        if (flags & ImGuiColorEditFlags_InputHSV) ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

        GetWindowDrawList()->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(col_rgb));

        RenderColorRectWithAlphaCheckerboard(window->DrawList, clickable.Min, clickable.Max, GetColorU32(col_rgb), ImMin(20, 20) / 2.99f, ImVec2(0.f, 0.f));

        GetWindowDrawList()->AddRect(clickable.Min, clickable.Max, GetColorU32(colors::color_picker::picker_rect));

        return pressed;
    }

}


