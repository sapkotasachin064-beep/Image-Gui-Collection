#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/bBlur.hpp"
#include <thread>

std::string key_input = "";

bool IsInternetConnected() {
    // its a ghetto method but idc tbh, also needs admin rights
    int result = system("ping -c 1 google.com");
    return result == 0;
}
void RenderCustomTransparentMenuBar() {
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    // Calculate the position and size of the menu bar

    // Add more menu items or buttons as needed...
}
void TextCentered(std::string text, ImColor startColor, ImColor endColor, float add_x = 0, float add_y = 0, bool should_anim = false) {
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    float horizontalIndentation = (windowSize.x - textSize.x) * 0.5f;
    float verticalIndentation = (windowSize.y - textSize.y) * 0.5f;

    float minHorizontalIndentation = 20.0f;
    if (horizontalIndentation <= minHorizontalIndentation) {
        horizontalIndentation = minHorizontalIndentation;
    }

    float minVerticalIndentation = 20.0f;
    if (verticalIndentation <= minVerticalIndentation) {
        verticalIndentation = minVerticalIndentation;
    }

    ImGui::SetCursorPosY(verticalIndentation + add_y);
    ImGui::SetCursorPosX(horizontalIndentation + add_x);
    ImGui::PushTextWrapPos(windowSize.x - horizontalIndentation);
    if (should_anim) {
        // Calculate the gradient animation based on time
        float time = ImGui::GetTime();
        float frequency = -2.0f; // Adjust the frequency of the animation

        for (int i = 0; i < text.size(); i++) {
            // Interpolate the color between startColor and endColor
            ImVec4 interpolatedColor;
            interpolatedColor.x = ImLerp(startColor.Value.x, endColor.Value.x, 0.5f + 0.5f * sinf(time * frequency + i * 0.3f)); // Red component
            interpolatedColor.y = ImLerp(startColor.Value.y, endColor.Value.y, 0.5f + 0.5f * sinf(time * frequency + i * 0.3f)); // Green component
            interpolatedColor.z = ImLerp(startColor.Value.z, endColor.Value.z, 0.5f + 0.5f * sinf(time * frequency + i * 0.3f)); // Blue component
            interpolatedColor.w = 1.0f; // Alpha is set to 1 (fully opaque)

            // Set the text color for the current letter
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(interpolatedColor.x, interpolatedColor.y, interpolatedColor.z, interpolatedColor.w));
            ImGui::TextUnformatted(text.substr(i, 1).c_str());
            ImGui::SameLine(0, 0);
            ImGui::PopStyleColor();
        }
    }
    else {
        ImGui::SetCursorPosX(horizontalIndentation + add_x);
        ImGui::TextWrapped(text.c_str());
    }

    ImGui::PopTextWrapPos();
}

static float rotation = 0.0f;
static ImVec2 spinnerCenter(0.0f, 0.0f);
static float spinnerRadius = 60.0f;
static ImU32 backgroundColor = IM_COL32(0, 0, 0, 128);

void DrawCircularLoadingSpinner(float x_add = 0.f, float y_add = 0.f, float current_ticks = 0.f, float max_ticks = 0.f)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window == nullptr)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Calculate the spinner position (centered)
    spinnerCenter = ImVec2(window->Pos.x + x_add + window->Size.x * 0.5f, window->Pos.y + y_add + window->Size.y * 0.5f);

    static int def_alpha = 255;

    if (current_ticks > max_ticks)
        def_alpha--;

    // Draw a semi-transparent background circle
    //drawList->AddCircleFilled(spinnerCenter, spinnerRadius + 2.0f, backgroundColor, 32);

    // Draw the rotating circular loading spinner as dots
    const int numDots = 12; // Number of dots in the spinner
    for (int i = 0; i < numDots; ++i)
    {
        float angle = rotation + i * (30.0f * 3.14159265359f / 180.0f);
        ImVec2 dotPos(spinnerCenter.x + cosf(angle) * spinnerRadius, spinnerCenter.y + sinf(angle) * spinnerRadius);
        float dotSize = 4.0f; // Adjust the size of the dots
        drawList->AddCircleFilled(dotPos, dotSize, IM_COL32(171, 92, 255, def_alpha), 12);
    }

    // Adjust the rotation speed by changing the value below
    rotation += 3.05f * io.DeltaTime; // Adjust the multiplier (0.05f) for speed
    if (rotation > 360.0f)
        rotation -= 360.0f;
}

int hello_world() {
    const char* command = "cmd /c @echo off && title saturn && color b && echo Hello World! && pause >NUL";
    int result = system(command);
    return true;
}

ImVec2 windowSize(400, 300); 
ImVec2 targetSize(200, 150);
float animationDuration = 1.0f; // Duration for the animation (in seconds)
float animationStartTime = -1.0f; // Time at which the animation started (neg
bool showWindow = true;

static void RoundedRect(ImVec2 size, ImU32 color)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    window->DrawList->AddRectFilled(window->DC.CursorPos, window->DC.CursorPos + size, color, 4.0f);
}


void RoundedImage(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& rounding, int border_thickness, const ImU32& border_color, int image_opacity)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 min = pos;
    ImVec2 max = ImVec2(pos.x + size.x, pos.y + size.y);

    ImGui::InvisibleButton("##image", size);

    bool hovered = ImGui::IsItemHovered();
    bool pressed = ImGui::IsItemActive();

    // Draw border
    if (border_thickness > 0) {
        draw_list->AddRect(min, max, border_color, rounding.x, ImDrawCornerFlags_All, border_thickness);
    }
    ImVec2 vector = rounding;
    float x = vector.x;
    // Draw image
    draw_list->AddImageRounded(user_texture_id, min, max, ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, image_opacity), x);

    if (hovered) {
        // Handle hover effect
        // You can add additional logic here, such as changing the border color or displaying a tooltip
    }

    if (pressed) {
        // Handle click event
        // You can add custom logic for when the image is clicked
    }
}

bool check_login_data(std::string username, std::string password)
{
    if (username == "admin" && password == "123")
        return true;
    else
        return false;
}

void ui::render() {
    if (!globals.active) return;

#if _DEBUG
    globals.checked_connection = true;
    globals.connection_message = ("Connected");
#endif
    static bool stop_resize = false;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowBgAlpha(0.1f);

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        ImGui::SetWindowSize(ImVec2(350 / 2, 450 / 2));
        auto p = ImGui::GetWindowPos();
        ImGuiStyle& style = ImGui::GetStyle();
        static int wait_for_resize = 0;
        static bool checked_updates = false;
        static bool can_finally_login = false;
        static int alpha = 1;
        static int speed = 0.010f;
        static bool decrease = false;
        static float window_size_x1 = 175;
        static float window_size_y1 = 225;

        static float window_size_x2 = 350;
        static float window_size_y2 = 450;

        static float window_size_x = 175;
        static float window_size_y = 225;

        static float pos_fix_x1 = 0;
        static float pos_fix_y1 = 0;

        static float pos_fix_x = 0;
        static float pos_fix_y = 0;

        static float pos_fix_x2 = 0;
        static float pos_fix_y2 = 0;

        static int logo_pos = -70;
        static int logo_addition = 0;
        static int little_logo_y = -8;
        static bool finished_clean_anim = false;
        static float acceleration_resize = 1.0f;
        static bool little_logo_go_back = false;

        static bool begin_resize = false;

        ImGuiContext& g = *GImGui;
        ImFont* font = g.Font;

        static int login_missmatches = 0;

        if (!little_logo_go_back)
        {
            ImGui::SetCursorPos(ImVec2(!finished_clean_anim ? window_size_x1 / 2 - 6 : window_size_x / 2 - 6, 8 + logo_addition));
            ImGui::Image(globals.saturn_icon, ImVec2(12, 12));
        }

        if (!can_finally_login)
        {
            if (253 > alpha)
            {
                ImColor startColor(171, 92, 255, 255);
                ImColor endColor(46, 0, 94, 255);

                TextCentered("Checking for updates", startColor, endColor, -5, 90, true);
                DrawCircularLoadingSpinner();
            }
                

            if (wait_for_resize > 350)
                checked_updates = true;
            else
                wait_for_resize++;

            if (checked_updates)
            {
                if (decrease)
                {
                    decrease = alpha > 254;
                }
                else
                {
                    alpha += 6;
                    decrease = alpha >= 254;
                }

                if (!can_finally_login)
                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x - 10, p.y + 27), ImVec2(p.x + 350 / 2, p.y + 450 / 2), ImColor(13, 4, 23, int(alpha)), 4);

                if (alpha > 253)
                {
                    if (finished_clean_anim)
                    {
                        if (350 > window_size_x)
                        {
                            window_size_x += 6 * acceleration_resize;
                            pos_fix_x += 6 * acceleration_resize;
                            
                        }

                        if (450 > window_size_y)
                        {
                            window_size_y += 1.2857142857142858 * 6 * acceleration_resize;
                            pos_fix_y += 1.2857142857142858 * 6 * acceleration_resize;
                        }

                        ImGui::SetWindowPos(ImVec2(window_pos.x - pos_fix_x * 0.5, window_pos.y - pos_fix_x * 0.5));
                        ImGui::SetWindowSize(ImVec2(window_size_x, window_size_y));

                        if (window_size_y > 450 && window_size_x > 350)
                            can_finally_login = true;
                    }
                    else
                    {
                        if (window_size_x1 > 135)
                        {
                            window_size_x1 -= 6;
                            pos_fix_x1 -= 6;
                        }

                        if (window_size_y1 > 185)
                        {
                            window_size_y1 -= 1.2857142857142858 * 6;
                            pos_fix_y1 -= 1.2857142857142858 * 6;
                        }

                        ImGui::SetWindowPos(ImVec2(window_pos.x - pos_fix_x1 * 0.5, window_pos.y - pos_fix_x1 * 0.5));
                        ImGui::SetWindowSize(ImVec2(window_size_x1, window_size_y1));

                        if (155 > window_size_x1 && 205 > window_size_y1)
                            finished_clean_anim = true;
                    }
                }
            }
        }
        else
            ImGui::SetWindowSize(ImVec2(window_size_x, window_size_y));
        

        if (!globals.checked_connection)
        {
            if (IsInternetConnected()) {
                globals.connection_message = ("Connected");
            }
            else {
                globals.connection_message = ("Failed to check connection!");
                globals.fail_message = "Failed to check connection!";
                globals.failed = true;
            }
            globals.checked_connection = true;
        }

        if (globals.failed)
        {
            //TextCentered(globals.fail_message.c_str());
            
            globals.tick_count++;

            if (globals.tick_count > 500)
                exit(1337);
        }
        
        if (globals.login_form && can_finally_login && !globals.failed)
        {
            static bool should_fade = false;
            stop_resize = true;

            if (logo_addition > -20)
                logo_addition -= 1;
            else
                if (40 > logo_pos)
                    logo_pos += 6;

            style.Colors[ImGuiCol_Text] = ImColor(171, 92, 255);
            ImGui::SetCursorPos(ImVec2(45, logo_pos));
            ImGui::PushFont(globals.logo_font);
            ImGui::Text("SATURN");
            ImGui::PopFont();
            style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
            ImGui::SetCursorPos(ImVec2(45 + 2, logo_pos + 2));
            ImGui::PushFont(globals.logo_font);
            ImGui::Text("SATURN");
            ImGui::PopFont(); // asnbdi
            

            static int alpha5 = 255;
            static int add_y_for_logo = 0;
            static int speed5 = 0.010f;
            static bool decrease5 = false;

            if (logo_pos > 40)
            {
                if (!should_fade)
                {
                    if (decrease5)
                    {
                        alpha5++ * speed5;
                        decrease5 = alpha5 >= 256; 
                    }
                    else
                    {
                        alpha5 -= 6;
                    }
                }    
            }                
        
            ImGui::SetCursorPos(ImVec2(40, 150));
            ImGui::InputText("", globals.username, IM_ARRAYSIZE(globals.username));

            std::string final_username = globals.username;

            ImGui::SetCursorPos(ImVec2(290, 151));
            ImGui::Image(globals.username_icon, ImVec2(18, 18));

            ImGui::SetCursorPos(ImVec2(40, 210));
            ImGui::InputText(" ", globals.password, IM_ARRAYSIZE(globals.password), ImGuiInputTextFlags_Password);

            std::string final_password = globals.password;

            ImGui::SetCursorPos(ImVec2(290, 211));
            ImGui::Image(globals.password_icon, ImVec2(18, 18));

            static bool checkbox = false;
            ImGui::SetCursorPos(ImVec2(40, 270));

            if (should_fade)
            {
                if (255 > alpha5)
                {
                    alpha5 += 12;
                    add_y_for_logo = 104;
                }
                else
                {
                    globals.login_form = false;
                    globals.login_loading = true;
                }
            }

            if (login_missmatches > 2)
                exit(1337);

            if (ImGui::Button("LOGIN", ImVec2(267, 40))) 
            {
                if (check_login_data(final_username, final_password))
                {
                    should_fade = true;
                }
                else
                {
                    login_missmatches++;
                    MessageBoxA(NULL, "wrong password! :3", "Saturn", MB_ICONERROR);
                }                            
            }

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 105 - add_y_for_logo), ImVec2(p.x + 350, p.y + 450), ImColor(13, 4, 23, int(alpha5)), 4);
/*
*/
        }
        else if (globals.login_loading && !globals.failed)
        {
            static float add_x = 0;
            static float add_x2 = 0;
            static float acceleration = 1.0f;
            static float wait_ticks = 0;
            static bool can_move_logo = false;

            if (add_x < 400) {
                add_x += acceleration / 2;
                acceleration *= 1.01f;
            }
            else
            {
                wait_ticks++;
            }

            if (add_x2 < 300) {
                add_x2 += acceleration / 2;
                acceleration *= 1.01f;
            }

            float time = ImGui::GetTime();
            float frequency = 10.0f; // Adjust the frequency of the animation
            float opacity = 0.5f + 0.5f * sinf(time * frequency);
            
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            static int alpha5 = 1;
            static float add_y_for_logo = 0;
            static int speed5 = 0.010f;
            static bool decrease5 = false;

            if (150 < wait_ticks)
            {
                if (255 > alpha5)
                {
                    alpha5 += 6;
                }   
            }

            if (150 < wait_ticks)
            {
                if (255 > alpha5)
                {

                }
                else
                {

                    opacity = 0;
                    if (add_y_for_logo < 360) {
                        add_y_for_logo += acceleration;
                        acceleration *= 1.01f;
                    }
                    else
                    {

                        if (little_logo_y < 8)
                            little_logo_y++;
                        else
                        {
                            globals.main_form = true;
                            globals.login_loading = false;
                        }
                    }             

                    ImGui::SetCursorPos(ImVec2(window_size_x / 2 - 6, -8 + little_logo_y));
                    ImGui::Image(globals.saturn_icon, ImVec2(12, 12));
                }
            }

            

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 130 - add_y_for_logo), ImVec2(p.x + 350, p.y + 450), ImColor(13, 4, 23, int(alpha5)), 4);

            style.Colors[ImGuiCol_Text] = ImColor(171, 92, 255);
            ImGui::SetCursorPos(ImVec2(45, 170 + add_y_for_logo));
            ImGui::PushFont(globals.logo_font);
            ImGui::Text("SATURN");
            ImGui::PopFont();
            style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
            ImGui::SetCursorPos(ImVec2(45 + 2, 170 + 2 + add_y_for_logo));
            ImGui::PushFont(globals.logo_font);
            ImGui::Text("SATURN");
            ImGui::PopFont();

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + add_x, p.y + 130), ImVec2(p.x + 455, p.y + 460), ImColor(13, 4, 23, 255), 4);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + add_x2, p.y + 170), ImVec2(p.x + add_x2 + 1, p.y + 230), ImColor(1.0f, 1.0f, 1.0f, opacity), 4);
        }
        else if (globals.main_form && !globals.failed)
        {
            static bool can_move_logo = false;
            static int move_logo_y = 170;
            static bool did_update_products = false;
            static int update_ticks = 0;
            static float alpha_updated = 0;

            ImGui::SetCursorPos(ImVec2(window_size_x / 2 - 6, little_logo_y));
            ImGui::Image(globals.saturn_icon, ImVec2(12, 12));

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 15, p.y + 35), ImVec2(p.x + 340, p.y + 280), ImColor(45, 19, 82, int(55)), 4);
            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 15, p.y + 35), ImVec2(p.x + 340, p.y + 280), ImColor(106, 51, 184, int(125)), 4);
            ImColor startColor(171, 92, 255, 255);
            ImColor endColor(46, 0, 94, 255);

            TextCentered("Products", startColor, endColor, 0, -180, true);
            const char* items[] = { "Saturn" };
            static int selected_item = -1;



            if (did_update_products)
            {
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 20, p.y + 65), ImVec2(p.x + 335, p.y + 135), ImColor(45, 19, 82, int(alpha_updated / 4.636363636f)), 4);
                ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 20, p.y + 65), ImVec2(p.x + 335, p.y + 135), ImColor(106, 51, 184, int(alpha_updated / 2.04f)), 4);
                ImGui::SetCursorPos(ImVec2(35, 80));
                RoundedImage(globals.cs2_icon, ImVec2(40, 40), ImVec2(4, 4), 1, ImColor(106, 51, 184, int(alpha_updated)), alpha_updated);

                static std::string GameString = "Counter-Strike 2";
                static std::string LastUpdate = "Last Update: 23.03.2024";
                static std::string ProductStatus = "Undetected";

                ImGui::GetWindowDrawList()->AddText(globals.verdana, 14, ImVec2(p.x + 85, p.y + 75), ImColor(255, 255, 255, int(alpha_updated)), GameString.c_str());
                ImGui::GetWindowDrawList()->AddText(globals.verdana, 13, ImVec2(p.x + 85, p.y + 90), ImColor(255, 255, 255, int(alpha_updated)), LastUpdate.c_str());
                ImGui::GetWindowDrawList()->AddText(globals.verdana, 13, ImVec2(p.x + 85, p.y + 105), ImColor(255, 255, 255, int(alpha_updated)), "Status: ");
                ImGui::GetWindowDrawList()->AddText(globals.verdana, 13, ImVec2(p.x + 85 + ImGui::CalcTextSize("Status:").x, p.y + 105), ImColor(171, 92, 255, int(alpha_updated)), ProductStatus.c_str());
                
                if (selected_item == 0)
                {
                    GameString = items[selected_item];
                    LastUpdate = "Last Update: 23.03.2024";
                    ProductStatus = "Undetected"; 
                }

                ImGui::SetCursorPos(ImVec2(20, 145));
                ImGui::ListBoxHeader("", ImVec2(315, 115));
                for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
                    bool is_selected = (i == selected_item);
                    if (ImGui::Selectable(items[i], is_selected)) {
                        selected_item = i;
                    }
                }
                ImGui::ListBoxFooter();

                if (256 > alpha_updated)
                    alpha_updated += 1;

            }
            else
            {
                DrawCircularLoadingSpinner(0, -80, update_ticks, 195);
                update_ticks++;
   
                if (update_ticks > 450)
                    did_update_products = true;
            }
            

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 15, p.y + 295), ImVec2(p.x + 340, p.y + 380), ImColor(45, 19, 82, int(55)), 4);
            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 15, p.y + 295), ImVec2(p.x + 340, p.y + 380), ImColor(106, 51, 184, int(125)), 4);

            ImColor startColor2(171, 92, 255, 255);
            ImColor endColor2(46, 0, 94, 255);

            TextCentered("Information", startColor2, endColor2, 0, 80, true);

            ImGui::SetCursorPos(ImVec2(15, 395));
            if (ImGui::Button("Execute", ImVec2(325, 40))) {

                globals.loading_product = true;
                globals.main_form = false;
            }

            static int alpha5 = 255;
            static int add_y_for_logo = 0;
            static int speed5 = 0.010f;
            static bool decrease5 = false;

            if (alpha5 > 1)
            {
                alpha5 -= 2;
            }

       
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 35 - add_y_for_logo), ImVec2(p.x + 350, p.y + 450), ImColor(13, 4, 23, int(alpha5)), 4);

        }
        else if (globals.loading_product && !globals.failed)
        {
            static float
                progress = 0.0f,
                progress_dir = 1.0f,
                limitter = 1.0f;
            static float animationTimer = 0.0f;
            static float animationDuration = 2.0f;
            if (progress < limitter)
            {
                float deltaTime = ImGui::GetIO().DeltaTime;
                float animationProgress = fmod(animationTimer, animationDuration) / animationDuration;
                static float minScale = 1.5f; 
                static float maxScale = 2.0f; 
                float scaleFactor = minScale + (maxScale - minScale) * 0.5f * (sinf(animationProgress * 2 * 3.14159f) + 1.0f);
                ImVec2 imageSize(70.0f, 70.0f); 
                ImVec2 imagePosition((windowSize.x - imageSize.x * scaleFactor) * 0.5f - 20, (windowSize.y - imageSize.y * scaleFactor) * 0.5f);

                animationTimer += deltaTime;

                ImGui::SetCursorPos(imagePosition);
                ImGui::Image(globals.cs2_icon, imageSize* scaleFactor);
                float centerX = 100.0f;
                float centerY = 100.0f;
                float radius = 150.0f;
                ImGui::GetWindowDrawList()->AddCircle(
                    ImVec2(centerX, centerY),
                    radius,
                    IM_COL32(255, 255, 255, 255),
                    32,
                    20
                );

                const ImVec2 p[] = {
                    ImVec2(centerX - radius * 0.6f, centerY - radius * 0.1f),
                    ImVec2(centerX - radius * 0.2f, centerY + radius * 0.4f),
                    ImVec2(centerX + radius * 0.7f, centerY - radius * 0.3f)
                };

                ImGui::GetWindowDrawList()->AddPolyline(p, 3, IM_COL32(0, 255, 0, 255), false, 2.0f);

            }      
            else
            {
                if (globals.final_selected_product == 0)
                {

                    if (!globals.loaded_product)
                    {
                        std::thread t1(hello_world);
                        t1.detach();
                    }

                    globals.loaded_product = true;

                    globals.tick_count++;

                    if (globals.tick_count > 500)
                        exit(1337);
                }
            }

        }
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;
	
    // colors
    ImGui::StyleColorsDark();

	if (window_pos.x == 0) {
		RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
		window_pos = (screen_res - window_size) * 0.5f;
	}
}