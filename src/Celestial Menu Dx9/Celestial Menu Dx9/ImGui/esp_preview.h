#define IMGUI_DEFINE_MATH_OPERATORS
#pragma once
#include "settings.h"
#include <array>
#include "imgui.h"
#include "imgui_internal.h"
#include <algorithm>
#include <string>
#include <misc/addons/imgui_groups.h>

static bool m_bsettings = true;

class c_esp_drag {
public:
    class Box_t {
    public:
        int x, y, w, h;
    };

    struct Position {
        ImVec2 pos;
    };

    class c_drag_item {
    public:
        int pos;
        int type;
        ImColor col;
        std::string text;
        bool small_text = false;
        ImVec2 pos_;
        ImVec2 size;
        bool hovered = false;
        int helding = 0;
        float move_animation = 0;
        float animations[6];
        bool enabled = true;
        int think_pos;

        bool color_edit_open = false;
        std::string color_popup_label;
    };

    std::array<c_drag_item, 13> m_items = {
        //POS 1 RECHTS      //0 - Text
        //POS 2 OBEN        //1 - Rect
        //POS 3 UNTEN       //2 - Circle    
        //POS 4 LINKS


        c_drag_item{4, 1, ImColor(139, 252, 161), "healthbar" },
        c_drag_item{1, 1, ImColor(134, 154, 224), "armorbar" },
        c_drag_item{3, 1, ImColor(255, 171, 89), "reloadbar" },

        c_drag_item{ 2, 0, ImColor(255,255,255), "Nickname",1 },
        c_drag_item{ 3, 0, ImColor(255, 255, 255), "Distance", 1 },
        c_drag_item{2, 0, ImColor(255, 255, 255), "Flash", 1},

        c_drag_item{ 1, 0, ImColor(255,255,255), "Money", 1 },
        c_drag_item{ 1, 0, ImColor(255, 255, 255), "Blind",1 },
        c_drag_item{ 1, 0, ImColor(255,255,255), "Lc",1 },

        c_drag_item{ 4, 0, ImColor(255,255,255), "Scope", 1 },
        c_drag_item{ 4, 0, ImColor(255,255,255), "Ping", 1 },
        c_drag_item{ 4, 0, ImColor(255,255,255), "KD", 1 },

        c_drag_item{ 3, 0, ImColor(255,255,255), ICON_FA_GUN,1 },

    };

    int m_offsets[8];
    Box_t box;

    int find_closest_position(ImVec2 curr, Position positions[]) {
        float closest = FLT_MAX;
        int best = -1;
        for (int i = 0; i < 4; i++) {
            auto pos = positions[i].pos;
            float dist = pos.dist_to(curr);

            if (closest > dist) {
                closest = dist;
                best = i;
            }
        }

        return best;
    }

    void set_positions(bool healtbar, bool armorbar, bool reloadbar, bool nickname, bool dist, bool flash, bool money, bool blind, bool lc, bool scope, bool ping, bool kd, bool weaponicon ) {

        m_items[0].enabled = healtbar;
        m_items[1].enabled = armorbar;
        m_items[2].enabled = reloadbar;
        m_items[3].enabled = nickname;
        m_items[4].enabled = dist;
        m_items[5].enabled = flash;
        m_items[6].enabled = money;
        m_items[7].enabled = blind;
        m_items[8].enabled = lc;
        m_items[9].enabled = scope;
        m_items[10].enabled = ping;
        m_items[11].enabled = kd;
        m_items[12].enabled = weaponicon;

        Position Positions[] = {
            {ImVec2(ImGui::GetWindowPos().x + box.x - 5, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + 2, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y - 5)},
            {ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y + box.h + 2)},
        };

        auto mouse_in_region = [&](ImVec2 pos, ImVec2 size) -> bool {
            auto m_pos = ImGui::GetMousePos();
            if (m_pos.x >= pos.x && m_pos.y >= pos.y &&
                m_pos.x <= pos.x + size.x && m_pos.y <= pos.y + size.y)
                return true;
            return false;
            };


        ImVec2 dragDropPosition = ImGui::GetWindowPos() + ImVec2(585, 20);
        static int draggingItemIndex = -1;
        for (int i = 0; i < m_items.size(); i++) {
            auto& item = m_items[i];
            item.color_popup_label = "Color Edit Popup " + std::to_string(i);
            if (!item.enabled)
                continue;
            ImGui::PushID(i);
            ImGui::SetCursorScreenPos(item.pos_);
            ImGui::PushStyleVar(0, 0.0f);
            ImGui::Button((item.text).c_str(), item.size);
            bool hovered = ImGui::IsItemHovered();
            ImGui::PopStyleVar();

            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
                for (auto& other_item : m_items) {
                    if (&other_item != &item) {
                        other_item.color_edit_open = false;
                    }
                }
                ImGui::OpenPopup(item.color_popup_label.c_str());
                item.color_edit_open = false;
            }




            int beginchild_x = 115;
            ImGui::SetNextWindowSize(ImVec2(300, 300));
            if (ImGui::BeginPopup(item.color_popup_label.c_str())) {
                if (item.type == 0) {
                    beginchild_x = 200;
                }
                if (item.type == 1) {
                    beginchild_x = 150;
                }

                ImGrp::ColorEdit4(item.text.c_str(), &item.col.Value.x);

                if (item.type == 0) {
                    ImGrp::Checkbox("TOUPPER", &text_toupper);
                }

                if (item.type == 1) {
                    ImGrp::Checkbox("MultiColor", &MultiColor);
                    ImGrp::Checkbox("Sytle", &barstyle);
                }

                ImGui::EndPopup();
            }

            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsMousePosValid()) {
                ImVec2 mousePos = ImGui::GetMousePos();

                if (mousePos.x >= dragDropPosition.x && mousePos.y >= dragDropPosition.y &&
                    mousePos.x <= dragDropPosition.x + 30 && mousePos.y <= dragDropPosition.y + 30) {
                    if (draggingItemIndex != -1) {
                        item.color_edit_open = false;
                        if (draggingItemIndex == 0) esp_ff[0] = false;
                        else if (draggingItemIndex == 1) esp_ff[1] = false;
                        else if (draggingItemIndex == 2) esp_ff[2] = false;
                        else if (draggingItemIndex == 3) esp_ff[3] = false;
                        else if (draggingItemIndex == 4) esp_ff[4] = false;
                        else if (draggingItemIndex == 5) esp_ff[5] = false;
                        else if (draggingItemIndex == 6) esp_ff[6] = false;
                        else if (draggingItemIndex == 7) esp_ff[7] = false;
                        else if (draggingItemIndex == 8) esp_ff[8] = false;
                        else if (draggingItemIndex == 9) esp_ff[9] = false;
                        else if (draggingItemIndex == 10) esp_ff[10] = false;
                        else if (draggingItemIndex == 11) esp_ff[11] = false;
                        else if (draggingItemIndex == 12) esp_ff[12] = false;
                    }
                }
                if (ImGui::IsItemActive()) {
                    draggingItemIndex = i;
                }
            }

            int pos = find_closest_position(ImGui::GetMousePos(), Positions);
            item.hovered = false;
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip)) {
                ImGui::SetDragDropPayload("t", &i, sizeof(int), 0);
                for (int t = 0; t < m_items.size(); t++)
                    m_items[t].move_animation = ImGui::GetIO().DeltaTime * 8.f;

                item.pos = 4, item.think_pos = pos;
                item.helding = pos > 1, item.hovered = true;
                ImGui::EndDragDropSource();
            }
            else if (item.pos == 4) {
                item.pos = item.think_pos;
                item.think_pos = -1;
                item.move_animation = 0.f;
            }
            item.animations[0] = ImLerp(item.animations[0], item.hovered || hovered ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            ImGui::GetWindowDrawList()->AddRectFilled(item.pos_ - ImVec2(1, 1), item.pos_ + item.size + ImVec2(1, 1), ImColor(200, 200, 200, int(100 * item.animations[0])));
            // ImGui::GetWindowDrawList()->AddShadowRect(item.pos_ - ImVec2(1, 1), item.pos_ + item.size + ImVec2(1, 1), ImColor(20, 200, 20, int(200 * item.animations[0])),30,ImVec2(1,1));

            ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + item.size + ImVec2(1, 1), ImColor(255, 255, 255, int(255 * item.animations[0])));
            ImGui::PopID();
        }
    }

    void on_draw() {
        box.x = 120, box.y = 95;
        box.w = 130; box.h = 255;

        Position Positions[] = {
            {ImVec2(ImGui::GetWindowPos().x + box.x - 8, ImGui::GetWindowPos().y + box.y)},//左边
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + 6, ImGui::GetWindowPos().y + box.y)},//右边
            {ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y - 8)},//顶部
            {ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y + box.h + 6)},//底部
        };

        ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowPos() + ImVec2(585, 20), ImColor(255, 255, 255), "A");

        ImVec2 Sizes[] = {
            ImVec2(3, box.h),
            ImVec2(3, box.h),
            ImVec2(box.w, 3),
            ImVec2(box.w, 3)
        };


        {

            if (esp_ff[0])
            {

                if (Corneredbox)
                {
                    ImColor color(ImVec4(b0x[0], b0x[1], b0x[2], b0x[3] * ImGui::GetStyle().Alpha));
                    ImVec2 windowPos = ImGui::GetWindowPos();
                    ImDrawList* drawList = ImGui::GetWindowDrawList();

                    ImVec2 topLeft(windowPos.x + box.x, windowPos.y + box.y);
                    ImVec2 topRight(windowPos.x + box.x + box.w, windowPos.y + box.y);
                    ImVec2 bottomLeft(windowPos.x + box.x, windowPos.y + box.y + box.h);
                    ImVec2 bottomRight(windowPos.x + box.x + box.w, windowPos.y + box.y + box.h);

                    drawList->AddLine(topLeft, ImVec2(topLeft.x + lineLength, topLeft.y), color, 1.0f);
                    drawList->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + lineLength), color, 1.0f);

                    drawList->AddLine(topRight, ImVec2(topRight.x - lineLength, topRight.y), color, 1.0f);
                    drawList->AddLine(topRight, ImVec2(topRight.x, topRight.y + lineLength), color, 1.0f);

                    drawList->AddLine(bottomLeft, ImVec2(bottomLeft.x + lineLength, bottomLeft.y), color, 1.0f);
                    drawList->AddLine(bottomLeft, ImVec2(bottomLeft.x, bottomLeft.y - lineLength), color, 1.0f);

                    drawList->AddLine(bottomRight, ImVec2(bottomRight.x - lineLength, bottomRight.y), color, 1.0f);
                    drawList->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y - lineLength), color, 1.0f);

                    if (ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, topLeft.y - 2), ImVec2(bottomRight.x, topLeft.y + 2)) || ImGui::IsMouseHoveringRect(ImVec2(topLeft.x - 2, topLeft.y), ImVec2(topLeft.x + 2, bottomRight.y))
                        || ImGui::IsMouseHoveringRect(ImVec2(bottomRight.x - 2, topLeft.y), ImVec2(bottomRight.x + 2, bottomRight.y)) || ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, bottomRight.y - 2), ImVec2(bottomRight.x, bottomRight.y + 2))) {
                        ImGui::GetWindowDrawList()->AddRect(topLeft, bottomRight, ImColor(255, 0, 0, int(255 * ImGui::GetStyle().Alpha)), boxrounding);
                    }


                    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, topLeft.y - 2), ImVec2(bottomRight.x, topLeft.y + 2)) || ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(topLeft.x - 2, topLeft.y), ImVec2(topLeft.x + 2, bottomRight.y))
                        || ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(bottomRight.x - 2, topLeft.y), ImVec2(bottomRight.x + 2, bottomRight.y)) || ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, bottomRight.y - 2), ImVec2(bottomRight.x, bottomRight.y + 2))) {
                        ImGui::OpenPopup("2dbox");
                    }

                }
                else
                {

                    ImColor color(ImVec4(b0x[0], b0x[1], b0x[2], b0x[3] * ImGui::GetStyle().Alpha));
                    ImGui::GetWindowDrawList()->AddRect(ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y),
                        ImVec2(ImGui::GetWindowPos().x + box.x + box.w, ImGui::GetWindowPos().y + box.y + box.h), color, boxrounding);

                    ImGui::GetWindowDrawList()->AddRect(ImVec2(ImGui::GetWindowPos().x + box.x - 1, ImGui::GetWindowPos().y + box.y - 1),
                        ImVec2(ImGui::GetWindowPos().x + box.x + box.w + 1, ImGui::GetWindowPos().y + box.y + box.h + 1), ImColor(0, 0, 0, int(255 * ImGui::GetStyle().Alpha)), boxrounding);

                    ImGui::GetWindowDrawList()->AddRect(ImVec2(ImGui::GetWindowPos().x + box.x + 1, ImGui::GetWindowPos().y + box.y + 1),
                        ImVec2(ImGui::GetWindowPos().x + box.x + box.w - 1, ImGui::GetWindowPos().y + box.y + box.h - 1), ImColor(0, 0, 0, int(255 * ImGui::GetStyle().Alpha)), boxrounding);

                    ImVec2 topLeft(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y);
                    ImVec2 bottomRight(ImGui::GetWindowPos().x + box.x + box.w, ImGui::GetWindowPos().y + box.y + box.h);

                    if (ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, topLeft.y - 2), ImVec2(bottomRight.x, topLeft.y + 2)) || ImGui::IsMouseHoveringRect(ImVec2(topLeft.x - 2, topLeft.y), ImVec2(topLeft.x + 2, bottomRight.y))
                        || ImGui::IsMouseHoveringRect(ImVec2(bottomRight.x - 2, topLeft.y), ImVec2(bottomRight.x + 2, bottomRight.y)) || ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, bottomRight.y - 2), ImVec2(bottomRight.x, bottomRight.y + 2))) {
                        ImGui::GetWindowDrawList()->AddRect(topLeft, bottomRight, ImColor(255, 0, 0, int(255 * ImGui::GetStyle().Alpha)), boxrounding);
                    }


                    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, topLeft.y - 2), ImVec2(bottomRight.x, topLeft.y + 2)) || ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(topLeft.x - 2, topLeft.y), ImVec2(topLeft.x + 2, bottomRight.y))
                        || ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(bottomRight.x - 2, topLeft.y), ImVec2(bottomRight.x + 2, bottomRight.y)) || ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(ImVec2(topLeft.x, bottomRight.y - 2), ImVec2(bottomRight.x, bottomRight.y + 2))) {
                        ImGui::OpenPopup("2dbox");
                    }
                }


                ImGui::SetNextWindowSize(ImVec2(300, 300));
                if (ImGui::BeginPopup("2dbox")) {
                    ImGrp::Checkbox("Cornered", &Corneredbox);
                    ImGrp::ColorEdit4("2D Box", b0x);
                    ImAdd::SliderFloat("Rounding", &boxrounding, 0.f, 10.f, "%.1f");
                    ImAdd::SliderFloat("Linelength", &lineLength, 10.f, 50.f, "%.1f");
                    ImGui::EndPopup();
                }


            }


        }

        for (auto& item : m_items) {
            item.animations[2] = ImLerp(item.animations[2], item.enabled ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            if (item.animations[2] < 0.1f) {
                for (int i = 0; i < m_items.size(); i++)
                {
                    if (!item.enabled) {
                        continue;
                    }
                    m_items[i].move_animation = ImGui::GetIO().DeltaTime * 8.f;

                }
                continue;
            }
            else if (item.animations[2] > 0.f && item.animations[2] < 0.1f) {
                for (int i = 0; i < m_items.size(); i++)
                {
                    m_items[i].move_animation = ImGui::GetIO().DeltaTime * 8.f;
                }
            }


            item.move_animation += ImGui::GetIO().DeltaTime * 1.2f;
            item.move_animation = ImClamp(item.move_animation, 0.f, 1.f);


            if (item.hovered) {


                auto size = ImGui::CalcTextSize(item.text.c_str());
                item.size = size;
                if (item.small_text)
                    size.y = 12;

                switch (item.think_pos) {
                case 0:
                    item.type == 0 ? m_offsets[4] += 2.f + size.y : m_offsets[0] += 5.f;
                    break;
                case 1:
                    item.type == 0 ? m_offsets[5] += 2.f + size.y : m_offsets[1] += 5.f;
                    break;
                case 2:
                    item.type == 0 ? m_offsets[6] += 2.f + size.y : m_offsets[2] += 5.f;
                    break;
                case 3:
                    item.type == 0 ? m_offsets[7] += 2.f + size.y : m_offsets[3] += 5.f;
                    break;
                }


            }



            if (item.type == 0) {



                if (text_toupper) {
                    std::transform(item.text.begin(), item.text.end(), item.text.begin(), ::toupper);
                }
                else {
                    std::transform(item.text.begin(), item.text.end(), item.text.begin(), ::tolower);
                }


                auto size = ImGui::CalcTextSize(item.text.c_str());
                item.size = size;
                if (item.small_text)
                    size.y = 20;

                switch (item.pos) {
                case 0:
                    item.pos_ = ImLerp(item.pos_, Positions[0].pos + ImVec2(-m_offsets[0] - size.x, m_offsets[4]), item.move_animation);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    m_offsets[4] += 2.f + size.y;
                    break;
                case 1:
                    item.pos_ = ImLerp(item.pos_, Positions[1].pos + ImVec2(m_offsets[1], m_offsets[5]), item.move_animation);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    m_offsets[5] += 2.f + size.y;
                    break;
                case 2:
                    item.pos_ = ImLerp(item.pos_, Positions[2].pos + ImVec2(Sizes[2].x / 2.f - size.x / 2.f, -m_offsets[2] - size.y - m_offsets[6]), item.move_animation);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    m_offsets[6] += 2.f + size.y;
                    break;
                case 3:
                    item.pos_ = ImLerp(item.pos_, Positions[3].pos + ImVec2(Sizes[2].x / 2.f - size.x / 2.f, m_offsets[3] + m_offsets[7]), item.move_animation);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    m_offsets[7] += 2.f + size.y;
                    break;
                case 4:
                    item.pos_ = ImLerp(item.pos_, ImGui::GetMousePos() + ImVec2(-size.x / 2.f, 0), ImGui::GetIO().DeltaTime * 8.f);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(0, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_ + ImVec2(1, 0), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(item.pos_, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.text.c_str());
                    break;


                }


                continue;
            }

            if (item.type == 2) {

                auto size = ImGui::CalcTextSize(item.text.c_str());
                item.size = size;
                if (item.small_text)
                    size.y = 12;

                switch (item.pos) {
                case 0:
                    item.pos_ = ImLerp(item.pos_, Positions[0].pos + ImVec2(-m_offsets[0] - size.x + 2, m_offsets[4] + 5), item.move_animation);
                    ImGui::GetWindowDrawList()->AddCircleFilled(item.pos_, 6, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddCircle(item.pos_, 6, ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(+1.f, +4.f), ImColor(30, 29, 31).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), "9");

                    m_offsets[4] += 2.f + size.y;
                    break;
                case 1:
                    item.pos_ = ImLerp(item.pos_, Positions[1].pos + ImVec2(m_offsets[1] + 4, m_offsets[5] + 5), item.move_animation);
                    ImGui::GetWindowDrawList()->AddCircleFilled(item.pos_, 6, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddCircle(item.pos_, 6, ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(+1.f, +4.f), ImColor(30, 29, 31).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), "9");

                    m_offsets[5] += -4.f + size.y;
                    break;
                case 2:
                    item.pos_ = ImLerp(item.pos_, Positions[2].pos + ImVec2(Sizes[1].x + 3, -m_offsets[2] - size.y + 14), item.move_animation);
                    ImGui::GetWindowDrawList()->AddCircleFilled(item.pos_, 6, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddCircle(item.pos_, 6, ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(+1.f, +4.f), ImColor(30, 29, 31).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), "9");
                    break;
                case 3:
                    item.pos_ = ImLerp(item.pos_, Positions[3].pos + ImVec2(Sizes[1].x + 3, m_offsets[3] + m_offsets[7] + 5), item.move_animation);

                    ImGui::GetWindowDrawList()->AddCircleFilled(item.pos_, 6, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddCircle(item.pos_, 6, ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    ImGui::GetWindowDrawList()->AddText(item.pos_ - ImVec2(+1.f, +4.f), ImColor(30, 29, 31).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), "9");

                    break;
                case 4:
                    item.pos_ = ImLerp(item.pos_, ImGui::GetMousePos() + ImVec2(-size.x / 2.f, 0), ImGui::GetIO().DeltaTime * 8.f);

                    ImGui::GetWindowDrawList()->AddCircleFilled(item.pos_, 6, item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), 36);
                    break;


                }

                continue;
            }

            //Rectangle

            if (item.type == 1)
            {
                item.size = Sizes[item.pos];
                switch (item.pos) {
                case 0:
                    item.pos_ = ImLerp(item.pos_, Positions[0].pos + ImVec2(-m_offsets[0], 0.f), item.move_animation);

                    ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + Sizes[0] + ImVec2(1, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (MultiColor)
                        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(item.pos_, item.pos_ + Sizes[0], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 100), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 10));
                    else
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[0], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (barstyle)
                    {
                        float stepY = Sizes[0].y / 6;
                        for (int i = 1; i < 6; ++i)
                        {
                            float y = item.pos_.y + i * stepY;
                            if (y < item.pos_.y + Sizes[0].y)
                            {
                                ImGui::GetWindowDrawList()->AddLine(ImVec2(item.pos_.x, y), ImVec2(item.pos_.x + Sizes[0].x, y), ImColor(0, 0, 0, 255));
                            }
                        }
                    }

                    m_offsets[0] += 5.f;
                    break;
                case 1:
                    item.pos_ = ImLerp(item.pos_, Positions[1].pos + ImVec2(m_offsets[1], 0.f), item.move_animation);

                    ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + Sizes[1] + ImVec2(1, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (MultiColor)
                        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(item.pos_, item.pos_ + Sizes[1], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 100), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 10));
                    else
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[1], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (barstyle)
                    {
                        float stepY = Sizes[1].y / 6;
                        for (int i = 1; i < 6; ++i)
                        {
                            float y = item.pos_.y + i * stepY;
                            if (y < item.pos_.y + Sizes[1].y)
                            {
                                ImGui::GetWindowDrawList()->AddLine(ImVec2(item.pos_.x, y), ImVec2(item.pos_.x + Sizes[1].x, y), ImColor(0, 0, 0, 255));
                            }
                        }
                    }

                    m_offsets[1] += 5.f;
                    break;
                case 2:
                    item.pos_ = ImLerp(item.pos_, Positions[2].pos + ImVec2(0.f, -m_offsets[2]), item.move_animation);

                    ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + Sizes[2] + ImVec2(1, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (MultiColor)
                        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(item.pos_, item.pos_ + Sizes[2], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 100), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 10), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));
                    else
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[2], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));
                    if (barstyle)
                    {
                        float stepX = Sizes[2].x / 6;
                        for (int i = 1; i < 6; ++i)
                        {
                            float x = item.pos_.x + i * stepX;
                            if (x < item.pos_.x + Sizes[2].x)
                            {
                                ImGui::GetWindowDrawList()->AddLine(ImVec2(x, item.pos_.y), ImVec2(x, item.pos_.y + Sizes[2].y), ImColor(0, 0, 0, 255));
                            }
                        }
                    }
                    m_offsets[2] += 5.f;
                    break;
                case 3:
                    item.pos_ = ImLerp(item.pos_, Positions[3].pos + ImVec2(0.f, m_offsets[3]), item.move_animation);

                    ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + Sizes[3] + ImVec2(1, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (MultiColor)
                        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(item.pos_, item.pos_ + Sizes[3], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 100), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 10), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));
                    else
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[3], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                    if (barstyle)
                    {
                        float stepX = Sizes[3].x / 6;
                        for (int i = 1; i < 6; ++i)
                        {
                            float x = item.pos_.x + i * stepX;
                            if (x < item.pos_.x + Sizes[3].x)
                            {
                                ImGui::GetWindowDrawList()->AddLine(ImVec2(x, item.pos_.y), ImVec2(x, item.pos_.y + Sizes[3].y), ImColor(0, 0, 0, 255));
                            }
                        }
                    }

                    m_offsets[3] += 5.f;
                    break;
                case 4:
                    item.pos_ = ImLerp(item.pos_, ImGui::GetMousePos() + ImVec2(0.f, m_offsets[3]), ImGui::GetIO().DeltaTime * 8.f);
                    if (item.helding == 1) {
                        item.size = Sizes[3];

                        ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + Sizes[3] + ImVec2(1, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                        if (MultiColor)
                            ImGui::GetWindowDrawList()->AddRectFilledMultiColor(item.pos_, item.pos_ + Sizes[3], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 100), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 10));
                        else
                            ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[3], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                        if (barstyle)
                        {
                            float stepY = Sizes[3].y / 6;
                            for (int i = 1; i < 6; ++i)
                            {
                                float y = item.pos_.y + i * stepY;
                                if (y < item.pos_.y + Sizes[3].y)
                                {
                                    ImGui::GetWindowDrawList()->AddLine(ImVec2(item.pos_.x, y), ImVec2(item.pos_.x + Sizes[3].x, y), ImColor(0, 0, 0, 255));
                                }
                            }
                        }


                    }
                    else if (item.helding == 0) {
                        item.size = Sizes[1];

                        ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + Sizes[1] + ImVec2(1, 1), ImColor(0, 0, 0).SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));

                        if (MultiColor)
                            ImGui::GetWindowDrawList()->AddRectFilledMultiColor(item.pos_, item.pos_ + Sizes[1], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 100), item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha - 10));
                        else
                            ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[1], item.col.SetAlpha(item.animations[2] * ImGui::GetStyle().Alpha));
                    }
                    break;
                }
            }

        }

        for (int i = 0; i < 8; i++)
            m_offsets[i] = 0.f;
    }
} m_esp_preview;
