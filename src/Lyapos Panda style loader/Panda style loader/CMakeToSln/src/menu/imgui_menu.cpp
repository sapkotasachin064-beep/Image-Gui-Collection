//
// Created by n08i40k on 18.01.2024.
//

#include "imgui_menu.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ranges>

#include "animation/imgui_animation_manager.h"
#include "animation/linear/linear_color_animation.h"
#include "animation/linear/linear_animation.h"
#include "dynamic_size/DynImVec2.h"
#include "dynamic_size/imgui_window_overlay.h"
#include "animation/animated_bg.h"
#include "render/AnimatedBackground.h"

namespace plugin_systems {
    imgui_menu::imgui_menu(const imgui_menu_flags flags): flags_(flags) {}

    void
    imgui_menu::render_tab_list_button(const size_t tab_index,
                                       const ImVec2& tab_size,
                                       const float invisible_border_size) {
        const auto& [tab_name, tab_description, render_callback] = tabs_.at(tab_index);

        const auto& style = ImGui::GetStyle();
        auto* window = ImGui::GetCurrentWindow();

        const auto start_point = ImGui::GetCursorScreenPos();
        ImRect bb{start_point, ImVec2{start_point.x + tab_size.x, start_point.y + tab_size.y}};

        const ImGuiID id = window->GetID(tab_name.c_str());
        if (!ImGui::ItemAdd(bb, id))
            return;

        bool hovered;
        bool held;

        if (ImGui::ButtonBehavior(bb, id, &hovered, &held))
            selected_tab_id_ = tab_index;

        if (hovered)
            hovered_tab_id_ = tab_index;

        auto text_color = style.Colors[ImGuiCol_Text];

        if (tab_index != selected_tab_id_) {
            if (held) {
                text_color.x *= 0.9F;
                text_color.y *= 0.9F;
                text_color.z *= 0.9F;
            }
            else if (hovered) {
                text_color.x *= 0.75F;
                text_color.y *= 0.75F;
                text_color.z *= 0.75F;
            }
            else
                text_color = style.Colors[ImGuiCol_TextDisabled];
        }

        auto* background_animation = imgui_animation_manager::get_instance()->get_animation<linear_color_animation>(
                window, id, "button_background"_sh, 3.F, ImGui::GetStyleColorVec4(ImGuiCol_Button), ImGui::GetStyleColorVec4(ImGuiCol_Button));

        background_animation->set_speed(3.F);

        if (held || tab_index == selected_tab_id_) {
            background_animation->set_target_color(ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            background_animation->set_speed(6.F);
        }
        else if (hovered)
            background_animation->set_target_color(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
        else
            background_animation->set_target_color(ImGui::GetStyleColorVec4(ImGuiCol_Button));

        ImGui::RenderFrame(bb.Min, bb.Max, background_animation->get_current_u32_color(), true, style.FrameRounding);

        ImGui::PushStyleColor(ImGuiCol_Text, text_color);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + invisible_border_size);
        ImGui::Indent(invisible_border_size);

        ImGui::Text("%s", tab_name.c_str());
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::Text("%s", tab_description.c_str());
        ImGui::PopFont();

        ImGui::Unindent(invisible_border_size);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + invisible_border_size);
        ImGui::PopStyleColor();
    }

    void
    imgui_menu::render_tab_list() {
        ImGui::BeginChild("tab_selector", DynImVec2(150.F, -1.F));

        hovered_tab_id_ = -1;

        const auto& io = ImGui::GetIO();
        const auto& style = ImGui::GetStyle();

        const float invisible_border_size = 10 * imgui_dynamic_size::get_current_scale_factor();
        const float text_height = io.Fonts->Fonts[0]->FontSize + io.Fonts->Fonts[1]->FontSize + style.ItemSpacing.y;
        const ImVec2 tab_size{
                ImGui::GetContentRegionAvail().x,
                invisible_border_size * 2 + style.ChildBorderSize * 2 + text_height
        };

        for (const auto& tab_index : tabs_ | std::views::keys)
            render_tab_list_button(tab_index, tab_size, invisible_border_size);

        bool background_hovered;
        bool background_held;
        ImGui::ButtonBehavior(ImGui::GetCurrentWindow()->Rect(),
                              ImGui::GetID("tab_selector"),
                              &background_hovered,
                              &background_held);

        size_t saved_view_tab_id = view_tab_id_;

        if (hovered_tab_id_ == -1) {
            view_tab_id_ = background_hovered
                           ? view_tab_id_
                           : selected_tab_id_;
        }
        else
            view_tab_id_ = hovered_tab_id_;

        if (view_tab_id_ != saved_view_tab_id) {
            previous_view_tab_id_ = saved_view_tab_id;
            view_tab_id_changed_ = true;
        }

        ImGui::EndChild();

        ImGui::SameLine();
    }

    void
    imgui_menu::render(ImTextureID logotype, ImTextureID bg) {
        if (!show_)
            return;

        if (tabs_.empty())
            return;

        ImGui::SetNextWindowSize(DynImVec2{690, 450});
        ImGui::SetNextWindowPos(ImVec2{0, 0});
        ImGui::Begin("##PROJECT_NAME-menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        auto* window = ImGui::GetCurrentWindow();

        animatedBackground animated_background;

        ImColor darl_col = ImColor(13, 14, 17, 255);
        ImColor main_col = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

        window->DrawList->AddRectFilled(window->Rect().Min, window->Rect().Max, ImGui::GetColorU32(ImGuiCol_WindowBg), ImGui::GetStyle().WindowRounding,ImDrawFlags_RoundCornersRight);

        if(bg != nullptr)
        {
            ImGui::GetWindowDrawList()->AddImage(bg, ImGui::GetWindowPos() + DynImVec2(345, 0), ImGui::GetWindowPos() + ImGui::GetWindowSize());
        }
        // animated_background.Render(window, main_col);


//        if (view_tab_id_ == 2) {
//            ImGui::PushClipRect(ImVec2(window->Rect().Min.x + 60.f, window->Rect().Min.y),
//                                ImVec2(window->Rect().Max.x, window->Rect().Max.y), true);
//            window->DrawList->AddShadowCircle(window->Rect().GetBL() - ImVec2(100.f, -100.f), 200.f,
//                                              main_col.SetAlpha(0.8F),
//                                              2565.f, ImVec2(0.f, 0.f));
//            ImGui::PopClipRect();
//
//            window->DrawList->AddRectFilledMultiColor(window->Rect().Min + ImVec2(60.f, 0.f),
//                                                      ImVec2(window->Rect().Min +
//                                                             ImVec2(260.f, window->Rect().GetHeight())),
//                                                      darl_col.SetAlpha(0.45f), darl_col.SetAlpha(0.f),
//                                                      darl_col.SetAlpha(0.f), darl_col.SetAlpha(0.45f));
//
//
//            window->DrawList->AddRectFilledMultiColor(window->Rect().Min + ImVec2(60.f, 200.f),
//                                                      window->Rect().Max,
//                                                      darl_col.SetAlpha(0.f), darl_col.SetAlpha(0.f),
//                                                      darl_col.SetAlpha(0.45f), darl_col.SetAlpha(0.45f));
//        }

        /*if(logotype != nullptr)
        {
            window->DrawList->AddImage(logotype, ImGui::GetWindowPos() + DynImVec2(12, 182), ImGui::GetWindowPos() + DynImVec2(12, 182) + DynImVec2(36, 36));
        }*/

        // CHILD COLOR
        const auto window_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
        ImGui::PushStyleColor(ImGuiCol_ChildBg, window_color);

        const bool render_list = (flags_ & imgui_menu_flags_no_list) == 0;
        // Module selector
        if (render_list)
            render_tab_list();

        auto* swap_animation = imgui_animation_manager::get_instance()->get_animation<linear_animation>(window, ImGui::GetID("swap_animation"), 0, 3.F, vector2f{1, 0});

        if (view_tab_id_changed_) {
            swap_animation->reset_current();
            view_tab_id_changed_ = false;
        }

        if (!render_list && selected_tab_id_ != view_tab_id_ && swap_animation->is_finished())
            selected_tab_id_ = view_tab_id_;

        if (const float animation_value = swap_animation->get_current();
                previous_view_tab_id_ != -1 and view_tab_id_ != previous_view_tab_id_ and animation_value > 0) {
            auto cursor = ImGui::GetCursorPos();

            ImGui::BeginDisabled();
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, swap_animation->get_current());
            tabs_[previous_view_tab_id_].render_callback();
            ImGui::PopStyleVar();
            ImGui::EndDisabled();

            ImGui::SetCursorPos(cursor);

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.F - swap_animation->get_current());
            tabs_[view_tab_id_].render_callback();
            ImGui::PopStyleVar();
        }
        else
            tabs_[view_tab_id_].render_callback();

        ImGui::PopStyleColor();
        //move_window();

        static bool any_item_dragged = false;

        if(ImGui::IsAnyItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            any_item_dragged = true;

        if(!ImGui::IsMouseDown(ImGuiMouseButton_Left))
            any_item_dragged = false;

        if(!any_item_dragged)
            window_move_detector = true;
        else
            window_move_detector = false;

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - DynImVec2(110.f, 0.f).x, DynImVec2(10.f, 0.f).x));

        ImGui::PushFont(imgui_dynamic_size::bold_fonts[2]);

        static bool bThemeChanging = false;
        static ImColor SavedCol;
        static float fCircleRadius = 0;

        ImGui::Text(bTheme ? ICON_SUN_1 : ICON_MOON_STARS_1);
        if (ImGui::IsItemClicked())
        {
            bThemeChanging = true;
            SavedCol =  ImGui::GetColorU32(ImGuiCol_WindowBg);
        }

        fCircleRadius = ImLerp(fCircleRadius, bThemeChanging ? DynImVec2(850.f, 0.f).x : 0.f, ImGui::GetIO().DeltaTime * 6.f);

        ImGui::SameLine();

        ImGui::Text(ICON_MINIMIZE_LINE);
        if (ImGui::IsItemClicked())
        {

        }

        ImGui::SameLine();

        ImGui::Text(ICON_CLOSE_LINE_1);
        if (ImGui::IsItemClicked())
        {




            exit(0);
        }
        ImGui::PopFont();

        ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowSize().x, 0.f), fCircleRadius, SavedCol, 360.f);

        if(fCircleRadius > DynImVec2(830.f, 0.f).x)
        {
            bThemeChanging = false;
            bTheme = !bTheme;
        }

        animated_background.Render(window, main_col);

        ImGui::End();
    }

    size_t
    imgui_menu::emplace(std::string&& title,
                        std::string&& description,
                        const std::function<void()>& render_callback) {
        tabs_.emplace(next_map_id_, menu_tab{std::move(title), std::move(description), render_callback});

        return next_map_id_++;
    }

    void
    imgui_menu::set_current_tab(const size_t tab_id) {
        assert(tabs_.contains(tab_id));

        if (view_tab_id_ == tab_id)
            return;

        previous_view_tab_id_ = view_tab_id_;
        view_tab_id_ = tab_id;
        view_tab_id_changed_ = true;
    }

    bool
    imgui_menu::toggle() {
        show_ ^= true;
        // imgui_renderer::get_instance()->hook_wnd_proc = show_;
        // game_utils::toggle_cursor(show_, true);

        return show_;
    }

    bool
    imgui_menu::is_show() const { return show_; }

    std::weak_ptr<imgui_menu>
    imgui_menu::create_instance(const imgui_menu_flags flags) {
        assert(p_instance_ == nullptr);

        return p_instance_ = std::make_shared<imgui_menu>(flags);
    }

    void
    imgui_menu::delete_instance() {
        assert(p_instance_ != nullptr);

        p_instance_.reset();
    }

    std::shared_ptr<imgui_menu> imgui_menu::p_instance_ = nullptr;
}
