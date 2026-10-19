#include "../settings/functions.h"

void c_widget::text_colored(ImFont* font, const ImU32 col, std::string text)
{
    gui->push_font(font);
    TextColored(ImColor(col), text.data());
    gui->pop_font();
}

void item_hande_shortcut(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    ImGuiInputFlags flags = g.NextItemData.ShortcutFlags;
    IM_ASSERT((flags & ~ImGuiInputFlags_SupportedBySetNextItemShortcut) == 0);

    if (flags & ImGuiInputFlags_Tooltip)
    {
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasShortcut;
        g.LastItemData.Shortcut = g.NextItemData.Shortcut;
    }
    if (!Shortcut(g.NextItemData.Shortcut, flags & ImGuiInputFlags_SupportedByShortcut, id) || g.NavActivateId != 0) return;

    g.NavActivateId = id;
    g.NavActivateFlags = ImGuiActivateFlags_PreferInput | ImGuiActivateFlags_FromShortcut;

    g.NavActivateDownId = g.NavActivatePressedId = id;
    NavHighlightActivated(id);
}

void c_gui::push_font(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    if (!font) font = GetDefaultFont();

    SetCurrentFont(font);
    g.FontStack.push_back(font);
    g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void c_gui::pop_font()
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow->DrawList->PopTextureID();
    g.FontStack.pop_back();

    SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void c_gui::set_next_window_pos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
    ImGuiContext& g = *GImGui;

    g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasPos;
    g.NextWindowData.PosVal = pos;
    g.NextWindowData.PosPivotVal = ImVec2(ImClamp(pivot.x, 0.0f, 1.0f), ImClamp(pivot.y, 0.0f, 1.0f));
    g.NextWindowData.PosCond = (cond != 0 && ImIsPowerOfTwo(cond)) ? cond : ImGuiCond_Always;
}
void c_gui::set_next_window_size(const ImVec2& size, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(size.x >= 0.0f && size.y >= 0.0f);
    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond));

    g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasSize;
    g.NextWindowData.SizeVal = size;
    g.NextWindowData.SizeCond = cond != 0 ? cond : ImGuiCond_Always;
}

ImVec2 c_gui::get_cursor_pos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos - window->Pos + window->Scroll;
}

float c_gui::get_cursor_pos_x()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float c_gui::get_cursor_pos_y()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}


void c_gui::set_cursor_pos(const ImVec2& local_pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
    window->DC.IsSetPos = true;
}

void c_gui::set_cursor_pos_x(float x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
    window->DC.IsSetPos = true;
}

void c_gui::set_cursor_pos_y(float y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
    window->DC.IsSetPos = true;
}

void c_gui::new_frame()
{
    NewFrame();
}

void c_gui::end_frame()
{
    Render();
}

ImU32 c_gui::get_clr(const ImVec4& col, float alpha)
{
    ImVec4 c = col;
    c.w *= GetStyle().Alpha * alpha;
    return ColorConvertFloat4ToU32(c);
}

void c_gui::push_style_color(ImGuiCol idx, ImU32 col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColorMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorStack.push_back(backup);
    if (g.DebugFlashStyleColorIdx != idx) g.Style.Colors[idx] = ColorConvertU32ToFloat4(col);
}

void c_gui::pop_style_color(int count)
{
    ImGuiContext& g = *GImGui;
    if (g.ColorStack.Size < count)
    {
        count = g.ColorStack.Size;
    }
    while (count > 0)
    {
        ImGuiColorMod& backup = g.ColorStack.back();
        g.Style.Colors[backup.Col] = backup.BackupValue;
        g.ColorStack.pop_back();
        count--;
    }
}

void c_gui::push_style_var(ImGuiStyleVar idx, float val)
{
    ImGuiContext& g = *GImGui;
    const ImGuiDataVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
    {
        float* pvar = (float*)var_info->GetVarPtr(&g.Style);
        g.StyleVarStack.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
}

void c_gui::push_style_var(ImGuiStyleVar idx, const ImVec2& val)
{
    ImGuiContext& g = *GImGui;
    const ImGuiDataVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
    {
        ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&g.Style);
        g.StyleVarStack.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
}

void c_gui::pop_style_var(int count)
{
    ImGuiContext& g = *GImGui;
    if (g.StyleVarStack.Size < count)
    {
        count = g.StyleVarStack.Size;
    }
    while (count > 0)
    {
        ImGuiStyleMod& backup = g.StyleVarStack.back();
        const ImGuiDataVarInfo* info = GetStyleVarInfo(backup.VarIdx);
        void* data = info->GetVarPtr(&g.Style);
        if (info->Type == ImGuiDataType_Float && info->Count == 1) { ((float*)data)[0] = backup.BackupFloat[0]; }
        else if (info->Type == ImGuiDataType_Float && info->Count == 2) { ((float*)data)[0] = backup.BackupFloat[0]; ((float*)data)[1] = backup.BackupFloat[1]; }
        g.StyleVarStack.pop_back();
        count--;
    }
}

void c_gui::spacing()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;
    ImGui::ItemSize(ImVec2(0, 0));
}

void c_gui::sameline(float offset_from_start_x, float spacing_w)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems) return;

    if (offset_from_start_x != 0.0f)
    {
        if (spacing_w < 0.0f) spacing_w = 0.0f;
        window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + offset_from_start_x + spacing_w + window->DC.GroupOffset.x + window->DC.ColumnsOffset.x;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    else
    {
        if (spacing_w < 0.0f) spacing_w = GetStyle().ItemSpacing.x;

        window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    window->DC.CurrLineSize = window->DC.PrevLineSize;
    window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
    window->DC.IsSameLine = true;
}

void c_gui::begin_group()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    g.GroupStack.resize(g.GroupStack.Size + 1);
    ImGuiGroupData& group_data = g.GroupStack.back();
    group_data.WindowID = window->ID;
    group_data.BackupCursorPos = window->DC.CursorPos;
    group_data.BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
    group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
    group_data.BackupIndent = window->DC.Indent;
    group_data.BackupGroupOffset = window->DC.GroupOffset;
    group_data.BackupCurrLineSize = window->DC.CurrLineSize;
    group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
    group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
    group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
    group_data.BackupIsSameLine = window->DC.IsSameLine;
    group_data.BackupActiveIdPreviousFrameIsAlive = g.ActiveIdPreviousFrameIsAlive;
    group_data.EmitItem = true;

    window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
    window->DC.Indent = window->DC.GroupOffset;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
    if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;
}

void c_gui::end_group()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT(g.GroupStack.Size > 0);

    ImGuiGroupData& group_data = g.GroupStack.back();
    IM_ASSERT(group_data.WindowID == window->ID);

    if (window->DC.IsSetPos) ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

    ImRect group_bb(group_data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, group_data.BackupCursorPos));

    window->DC.CursorPos = group_data.BackupCursorPos;
    window->DC.CursorPosPrevLine = group_data.BackupCursorPosPrevLine;
    window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
    window->DC.Indent = group_data.BackupIndent;
    window->DC.GroupOffset = group_data.BackupGroupOffset;
    window->DC.CurrLineSize = group_data.BackupCurrLineSize;
    window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
    window->DC.IsSameLine = group_data.BackupIsSameLine;
    if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;

    if (!group_data.EmitItem)
    {
        g.GroupStack.pop_back();
        return;
    }

    window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrLineTextBaseOffset);
    ImGui::ItemSize(group_bb.GetSize());
    ItemAdd(group_bb, 0, NULL, ImGuiItemFlags_NoTabStop);

    const bool group_contains_curr_active_id = (group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
    const bool group_contains_prev_active_id = (group_data.BackupActiveIdPreviousFrameIsAlive == false) && (g.ActiveIdPreviousFrameIsAlive == true);
    if (group_contains_curr_active_id) g.LastItemData.ID = g.ActiveId;
    else if (group_contains_prev_active_id) g.LastItemData.ID = g.ActiveIdPreviousFrame;
    g.LastItemData.Rect = group_bb;

    const bool group_contains_curr_hovered_id = (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
    if (group_contains_curr_hovered_id) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;

    if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame)  g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;

    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;
    if (group_contains_prev_active_id && g.ActiveId != g.ActiveIdPreviousFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;

    g.GroupStack.pop_back();
    if (g.DebugShowGroupRects) window->DrawList->AddRect(group_bb.Min, group_bb.Max, IM_COL32(255, 0, 255, 255));
}

std::string c_gui::get_current_date()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm* now_tm = std::localtime(&now_time);

    std::ostringstream date_stream;
    date_stream << std::setfill('0') << std::setw(2) << now_tm->tm_mday << '/'
        << std::setfill('0') << std::setw(2) << (now_tm->tm_mon + 1) << '/'
        << std::setfill('0') << std::setw(2) << (now_tm->tm_year % 100);

    return date_stream.str();
}

void c_gui::render_blur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary("user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 50 };
            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}

void c_gui::move_window(HWND hwnd, RECT rc)
{
    bool hovered = IsMouseHoveringRect(GetWindowPos(), GetWindowPos() + ImVec2(SCALE(80), GetWindowHeight()));
    static bool clicked = false;

    if (hovered && GetIO().MouseClicked[0])
        clicked = true;

    if (!GetIO().MouseDown[0])
        clicked = false;

    if (clicked)
    {
        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + GetMouseDragDelta().x, rc.top + GetMouseDragDelta().y, GetWindowWidth(), GetWindowHeight(), FALSE);
    }

    while (rc.right - rc.left != GetWindowWidth() || rc.bottom - rc.top != GetWindowHeight())
    {
        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left, rc.top, GetWindowWidth(), GetWindowHeight(), FALSE);
    }
}

int rotation_start_index;
void c_gui::rotate_start()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 rotate_center()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

void c_gui::rotate_end(float rad, ImVec2 center)
{
    if (center.x == 0 && center.y == 0) center = rotate_center();

    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

float c_gui::deg_to_rad(float deg)
{
    return deg / 57.3f;
}

void c_widget::colored_text(ImFont* font, ImVec2 pos1, ImVec2 pos2, const std::vector<rich_text>& texts, ImVec2 align)
{
    gui->push_font(font);

    ImVec2 total_text_size(0, 0);
    ImVec2 cursor;

    // Calculate total text size
    for (const auto& entry : texts)
    {
        if (entry.text == nullptr)
            continue;

        ImVec2 text_size = ImGui::CalcTextSize(entry.text);
        total_text_size.x += text_size.x + ImGui::GetStyle().ItemSpacing.x;
        total_text_size.y = ImMax(total_text_size.y, text_size.y);
    }

    total_text_size.x -= ImGui::GetStyle().ItemSpacing.x;

    // Calculate cursor position based on alignment
    cursor.x = pos1.x + (pos2.x - pos1.x) * align.x - total_text_size.x * align.x;
    cursor.y = pos1.y + (pos2.y - pos1.y) * align.y - total_text_size.y * align.y;

    // Draw colored text
    for (const auto& entry : texts)
    {
        if (entry.text == nullptr)
            continue;

        ImGui::SetCursorScreenPos(cursor);
        ImGui::TextColored(ImColor(entry.color), "%s", entry.text);

        ImVec2 text_size = ImGui::CalcTextSize(entry.text);
        cursor.x += text_size.x + ImGui::GetStyle().ItemSpacing.x;
    }

    gui->pop_font();
}

void c_gui::loading(std::string_view label, bool* reset, ImVec2 pos_min, ImVec2 pos_max, float transparent)
{
    struct c_loading
    {
        float offset_line;
    };

    ImGuiWindow* window = GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label.data());

    c_loading* state{ gui->anim_container(&state, id) };

    state->offset_line = easing->easing_in_out(id, transparent >= 0.1f, 70.f, 30.f, gui->fixed_speed(16.f), back);

    const ImVec2 pos = ImVec2((pos_min.x + pos_max.x) / 2.f - SCALE(state->offset_line), (pos_min.y + pos_max.y) / 2.f);
    ImDrawList* drawlist = GetForegroundDrawList();
    const ImVec2 size = SCALE(15, state->offset_line);

    static float offset[3] = { 0, 0, 0 };
    static float alpha[3] = { 0, 0, 0 };
    static float timer = 0.f;
    float spacing = 0;
    int bar_count = 3;

    timer += gui->fixed_speed(12.f);
    if (timer >= 5.f * bar_count)
        timer = 0.f;

    push_style_var(ImGuiStyleVar_Alpha, transparent);

    draw->add_rect_filled(drawlist, pos_min, pos_max, gui->get_clr(clr->c_other_clr.loading_active), SCALE(set->c_window.rounding));

    for (int i = 0; i < bar_count; i++)
    {
        bool active = timer >= 5.f * i && timer <= 5.f * (i + 1);
        offset[i] = easing->easing_in_out(reinterpret_cast<int>(&offset[i]), active, SCALE(0.f), SCALE(8.f), gui->fixed_speed(14.f), back);
        alpha[i] = ImClamp(alpha[i] + (gui->fixed_speed(8.f) * (!active ? 1.f : -1.f)), 0.f, 1.f);

        draw->add_rect_filled(drawlist, pos + ImVec2(spacing, -offset[i]) - size / 2.f, pos + size / 2.f + ImVec2(spacing, offset[i]), gui->get_clr(clr->c_other_clr.accent_clr), SCALE(100.f));
        draw->add_rect_filled(drawlist, pos + ImVec2(spacing, -offset[i]) - size / 2.f, pos + size / 2.f + ImVec2(spacing, offset[i]), gui->get_clr({ 0.f, 0.f, 0.f, 0.4f * alpha[i] }), SCALE(100.f));

        spacing += SCALE(state->offset_line);
    }

    draw->render_text(drawlist, set->c_font.poppins_medium[0], pos_min, pos_max, gui->get_clr(clr->c_other_clr.white_clr), label.data(), NULL, NULL, { 0.5, 0.78 });

    pop_style_var(1);

    if (*reset)
    {
        timer = 0.f;
        *reset = false;
    }
}

void c_widget::image(ImTextureID user_texture_id, const ImVec2& image_size, const ImVec2& uv0, const ImVec2& uv1, ImU32 color, float rounding)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;

    ImVec2 size{ image_size.x, image_size.y };
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + image_size);

    ItemSize(size, 0);
    if (!ItemAdd(bb, 0)) return;

    draw->add_image_rounded(window->DrawList, user_texture_id, bb.Min, bb.Max, uv0, uv1, color, rounding);
}

bool c_widget::video_preview(std::string_view video_link, ImTextureID user_texture_id, const ImVec2& image_size, const ImVec2& uv0, const ImVec2& uv1, ImU32 color, float rounding)
{
    ImGuiWindow* window = GetCurrentWindow();

    struct c_preview
    {
        float offset = 0;
    };

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + image_size);

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ItemSize(rect, 0);
    if (!ItemAdd(rect, window->GetID(user_texture_id))) return false;

    bool hovered, held, pressed = ButtonBehavior(rect, window->GetID(user_texture_id), &hovered, &held);

    if (pressed)
        ShellExecute(0, 0, video_link.data(), 0, 0, SW_SHOW);

    c_preview* state{ gui->anim_container(&state, window->GetID(user_texture_id)) };

    state->offset = ImLerp(state->offset, IsItemClicked() || hovered ? SCALE(16.f) : SCALE(13.f), gui->fixed_speed(set->c_product.speed_animation));

    {
        draw->add_image_rounded(window->DrawList, user_texture_id, rect.Min, rect.Max, uv0, uv1, color, rounding);
        draw->add_circle_filled(window->DrawList, rect.Min + image_size / 2, state->offset, gui->get_clr(clr->c_other_clr.black_clr, 0.7f), SCALE(100.f));
    }

    draw->render_text(window->DrawList, set->c_font.icons[3], rect.Min, rect.Max, gui->get_clr(clr->c_other_clr.white_clr), "G", NULL, NULL, { 0.5, 0.5 });

    return pressed;
}

bool c_widget::part_characteristic(std::string_view spare, std::string_view spare_description, const ImVec2& size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    struct c_part
    {
        float offset;
        float alpha;
    };

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(spare.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + SCALE(size));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_part* state{ gui->anim_container(&state, id) };

    state->offset = ImLerp(state->offset, hovered ? SCALE(12.f) : SCALE(0.f), gui->fixed_speed(set->c_product.speed_animation));
    state->alpha = ImLerp(state->alpha, hovered ? 1.f : 0.f, gui->fixed_speed(set->c_product.speed_animation));

    {
        draw->add_circle_filled(window->DrawList, rect.Max - SCALE(0, size.y / 2), SCALE(2.f), gui->get_clr(clr->c_other_clr.accent_clr, state->alpha), SCALE(100.f));

        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min, rect.Max, gui->get_clr(clr->c_element.text_active), spare.data(), NULL, NULL, { 0.0, 0.5 }, NULL);
        draw->render_text(window->DrawList, set->c_font.poppins_medium[2], rect.Min, rect.Max - ImVec2(state->offset, 0), gui->get_clr(clr->c_element.text), spare_description.data(), NULL, NULL, { 1.0, 0.5 }, NULL);
    }
    return pressed;
}

bool c_widget::selection_details(std::string_view selection_list, std::string_view description, const ImVec2& size)
{
    struct select_details
    {
        float offset;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(selection_list.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + SCALE(size));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    select_details* state{ gui->anim_container(&state, id) };
    state->offset = easing->easing_in_out(id, IsItemActive() || hovered, 0.f, SCALE(10.f), gui->fixed_speed(set->c_product.speed_animation * 3), bounce);

    draw->render_text(window->DrawList, set->c_font.icons[4], rect.Min, rect.Max, gui->get_clr(clr->c_other_clr.white_clr), "F", NULL, NULL, {0.0, 0.5}, NULL);

    {
        draw->render_text(window->DrawList, set->c_font.poppins_medium[1], rect.Min + SCALE(25, 0) + ImVec2(state->offset, 0), rect.Max, gui->get_clr(clr->c_element.text_active), selection_list.data(), NULL, NULL, {0.0, 0.5});
        draw->add_circle_filled(window->DrawList, rect.Min + ImVec2(CalcTextSize(selection_list.data()).x + SCALE(50) + state->offset, SCALE(size.y) / 2), SCALE(2.f), gui->get_clr(clr->c_other_clr.accent_clr), SCALE(100.f));
        draw->render_text(window->DrawList, set->c_font.poppins_medium[1], rect.Min + ImVec2(CalcTextSize(selection_list.data()).x + SCALE(65) + state->offset, 0), rect.Max, gui->get_clr(clr->c_other_clr.accent_clr), description.data(), NULL, NULL, { 0.0, 0.5 });
    }

    return pressed;
}

ImVec2 c_gui::get_mouse_move_delta()
{
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();

    ImVec2 window_center = ImVec2(
        window_pos.x + window_size.x / 2.0f,
        window_pos.y + window_size.y / 2.0f
    );

    ImVec2 current_mouse_pos = ImGui::GetMousePos();

    ImVec2 delta = ImVec2(
        current_mouse_pos.x - window_center.x,
        current_mouse_pos.y - window_center.y
    );

    float distance = ImSqrt(delta.x * delta.x + delta.y * delta.y);

    float max_delta = 10.0f;
    float proportional_delta = ImClamp(distance / (window_size.x / 2.0f) * SCALE(max_delta), -SCALE(max_delta), SCALE(max_delta));

    delta.x = (distance > 0) ? ImClamp(delta.x / distance * proportional_delta, -SCALE(max_delta), SCALE(max_delta)) : 0;
    delta.y = (distance > 0) ? ImClamp(delta.y / distance * proportional_delta, -SCALE(max_delta), SCALE(max_delta)) : 0;

    return delta;
}
