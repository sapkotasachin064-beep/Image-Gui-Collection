#include "main.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Create application window
    RECT rc;
    GetWindowRect(GetDesktopWindow(), &rc);
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXMAXTRACK);    //If you have more than one monitor, you will get the total width*
    rc.bottom = GetSystemMetrics(SM_CYMAXTRACK);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, L"ImGui Example", NULL, WS_POPUP, 0, 0, rc.right + 760, rc.bottom, NULL, NULL, wc.hInstance, NULL);
    SetLayeredWindowAttributes(main_hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(main_hwnd);
    SetWindowPos(main_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(main_hwnd, &margins);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromMemoryTTF(&ubuntu_1, sizeof ubuntu_1, 15, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico_0 = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 21, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ico_1 = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 28, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ubu_0 = io.Fonts->AddFontFromMemoryTTF(&ubuntu_0, sizeof ubuntu_0, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ubu_1 = io.Fonts->AddFontFromMemoryTTF(&ubuntu_0, sizeof ubuntu_0, 28, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ubu_2 = io.Fonts->AddFontFromMemoryTTF(&ubuntu_2, sizeof ubuntu_2, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    cruin_0 = io.Fonts->AddFontFromMemoryTTF(&cruin, sizeof cruin, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ubu_child = io.Fonts->AddFontFromMemoryTTF(&ubuntu_1, sizeof ubuntu_1, 15, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ubu_preview = io.Fonts->AddFontFromMemoryTTF(&ubuntu_1, sizeof ubuntu_1, 12, NULL, io.Fonts->GetGlyphRangesCyrillic());
    weapon_font = io.Fonts->AddFontFromMemoryTTF(&weapon, sizeof weapon, 14, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::SetNextWindowPos(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2));

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

            ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));

            CustomStyleColor();

            if (bg == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &background, sizeof(background), 1920, 1080, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bg);
            if (lg == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &logo, sizeof(logo), 35, 35, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &lg);
            if (settings_img == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &settings, sizeof(settings), 16, 16, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &settings_img);
            if (keyboard_img == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &keyboard, sizeof(keyboard), 16, 16, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &keyboard_img);
            if (person_img == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &person, sizeof(person), 16, 16, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &person_img);
            if (visual_img == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &visual, sizeof(visual), 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &visual_img);
            if (rifle_img == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &rifle, sizeof(rifle), 32, 32, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &rifle_img);

            //ImGui::GetBackgroundDrawList()->AddImage(bg, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(150, 150, 150, 255));

            static bool menu = true;
            ImGui::Begin("Hello, world!", &menu, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {

                const auto& p = ImGui::GetWindowPos();
                ImGuiStyle* s = &ImGui::GetStyle();

                DrawBackgroundBlur(ImGui::GetWindowDrawList(), g_pd3dDevice);


                const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(192 + p.x, 0 + p.y), ImVec2(WIDTH + p.x, HEIGHT + p.y), ImColor(21, 21, 26, 255), s->WindowRounding, ImDrawFlags_RoundCornersRight);
                
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 0 + p.y), ImVec2(192 + p.x, HEIGHT + p.y), ImColor(35, 32, 45, 200), s->WindowRounding, ImDrawFlags_RoundCornersLeft);
                ImGui::GetWindowDrawList()->AddRect(ImVec2(0 + p.x, 0 + p.y), ImVec2(WIDTH + p.x, HEIGHT + p.y), ImColor(39, 38, 45, 255), s->WindowRounding, ImDrawFlags_None, 1.5f);
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(192 + p.x, 0 + p.y), ImVec2(193.5f + p.x, HEIGHT + p.y), ImColor(39, 38, 45, 255), 0);              
                
                ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 192, 45 + p.y), ImVec2(WIDTH + p.x, 45 + p.y), ImColor(38, 37, 43, 255), 0.5f);

                ///////////////////////////////////////////// LOGO + NAME AND DATA
          
                //ImGui::GetWindowDrawList()->AddImage(lg, ImVec2(735 + p.x, 15 + p.y), ImVec2(770 + p.x, 50 + p.y), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
               
                ///////////////////////////////////////////// CHEAT NAME

                ImGui::GetWindowDrawList()->AddText(cruin_0, 25, ImVec2(45 + p.x, 20 + p.y), ImColor(255, 255, 255, 255), "EVICTED");

                ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x + 47, p.y + 64), ImVec2(p.x + 91, p.y + 65), ImGui::GetColorU32(c::accent_color, alpha_line), ImGui::GetColorU32(c::accent_color), ImGui::GetColorU32(c::accent_color), ImGui::GetColorU32(c::accent_color, alpha_line));
                ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x + 91, p.y + 64), ImVec2(p.x + 141, p.y + 65), ImGui::GetColorU32(c::accent_color), ImGui::GetColorU32(c::accent_color, alpha_line), ImGui::GetColorU32(c::accent_color, alpha_line), ImGui::GetColorU32(c::accent_color));

                ImGui::SetCursorPosY(80);

                ///////////////////////////////////////////// TABS

                ImGui::BeginGroup();
                {
                    ImGui::Spacing();

                    ImGui::TextColored(ImColor(77, 80, 97, 255), "\tAIMBOT");

                    if (ImGui::Tab("A", "General", tabs == 0, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 0;

                    if (ImGui::Tab("B", "Anti-Aim", tabs == 1, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 1;

                    ImGui::Spacing();

                    ImGui::TextColored(ImColor(77, 80, 97, 255), "\tLEGITBOT");

                    if (ImGui::Tab("D", "Triggerbot", tabs == 3, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 3;

                    if (ImGui::Tab("C", "General ", tabs == 4, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 4;

                    if (ImGui::Tab("E", "More ", tabs == 5, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 5;

                    if (ImGui::Tab("F", "Players", tabs == 6, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 6;

                    ImGui::Spacing();

                    ImGui::TextColored(ImColor(77, 80, 97, 255), "\tVISUALS");                 

                    if (ImGui::Tab("C", "World", tabs == 8, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 8;

                    if (ImGui::Tab("E", "More", tabs == 9, ImVec2(200, 37), true, ImGuiButtonFlags_None)) tabs = 9;

                

                }
                ImGui::EndGroup();

                tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;             

                subtab_alpha = ImClamp(subtab_alpha + (4.f * ImGui::GetIO().DeltaTime * (sub_tabs == active_subtab ? 1.f : -1.f)), 0.f, 1.f);
                if (subtab_alpha == 0.f && subtab_add == 0.f) active_subtab = sub_tabs;

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s->Alpha);

                ImGui::SetCursorPos(ImVec2(205, 62));

                ImGui::BeginChild("General-CHILD", ImVec2(566, 495), true, ImGuiWindowFlags_NoBackground);
                {


                    if (active_tab == 0) {

                        ImGui::BeginGroup(); // START GROUP
                        {
                            ImGui::BeginChildPos(settings_img, "General", ImVec2(272, 340));
                            {

                                ImGui::Checkbox("Checkbox", &check_0);
                                
                                ImGui::SliderInt("Slider", &slider_one, 1, 100, "%d%%");

                                ImGui::InputTextWithHint("Textfield", "indoor millionaire", input, 64);

                                ImGui::Combo("Combobox", &selectedItem, items, IM_ARRAYSIZE(items), 5);

                            }
                            ImGui::EndChildPos();


                            ImGui::BeginChildPos(settings_img, "Misc", ImVec2(272, 132));
                            {

                                if (ImGui::Button("Button", ImVec2(227, 37)));

                            }
                            ImGui::EndChildPos();


                            ImGui::BeginChildPos(settings_img, "Other", ImVec2(272, 260));
                            {

                                ImGui::Checkbox("Checkbox one", &check_1);

                                ImGui::Checkbox("Checkbox two", &check_0);

                                ImGui::Checkbox("Esp preview", &esp_preview);

                                

                            }
                            ImGui::EndChildPos();

  
                            ImGui::BeginChildPos(settings_img, "Exploit", ImVec2(272, 207));
                            {

                                ImGui::Combo("Combobox ", &selectedItem1, items1, 4);

                            }
                            ImGui::EndChildPos();


                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(0, 10); // 1, 2

                        ImGui::BeginGroup();
                        {

                            ImGui::BeginChildPos(settings_img, "Main", ImVec2(272, 242));
                            {

                                ImGui::Checkbox("Checkbox ", &check_1);

                                ImGui::ColorEdit4("Colorpicker##0", (float*)&c::accent_color, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                                ImGui::Keybind("Binderbox", &key0, true);

                                if (ImGui::Button("Button", ImVec2(227, 37)));

                            }
                            ImGui::EndChildPos();


                            ImGui::BeginChildPos(settings_img, "Settings", ImVec2(272, 230));
                            {

                                ImGui::SliderInt("Slider", &slider_two, 1, 100, "%du/s");

                                ImGui::MultiCombo("Multi Combo", multi_items_count, multi_items, 5);

                            }
                            ImGui::EndChildPos();


                            ImGui::BeginChildPos(settings_img, "Header", ImVec2(272, 260));
                            {

                                ImGui::SliderInt("Slider", &slider_one, 1, 100, "%d%%");

                                ImGui::SliderInt("Slider", &slider_one, 1, 100, "%d%%");

                                ImGui::SliderInt("Slider", &slider_one, 1, 100, "%d%%");

                            }
                            ImGui::EndChildPos();


                            ImGui::BeginChildPos(settings_img, "World", ImVec2(272, 205));
                            {

                            }
                            ImGui::EndChildPos();


                        }

                        ImGui::EndGroup(); // END GROUP
                    }
                    else if (active_tab == 0) {

                        ImGui::BeginGroup();
                        {
                            ImGui::BeginChildPos(settings_img, "Child One", ImVec2(267, 210));
                            {

                            }
                            ImGui::EndChildPos();

                            ImGui::BeginChildPos(settings_img, "Child Three", ImVec2(267, 300));
                            {

                            }
                            ImGui::EndChildPos();

                            ImGui::BeginChildPos(settings_img, "Child Five", ImVec2(267, 110));
                            {

                            }
                            ImGui::EndChildPos();

                            ImGui::BeginChildPos(settings_img, "Child Seven", ImVec2(267, 180));
                            {

                            }
                            ImGui::EndChildPos();
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(0, 20); // 1, 2

                        ImGui::BeginGroup();
                        {
                            ImGui::BeginChildPos(settings_img, "Child Two", ImVec2(267, 135));
                            {

                            }
                            ImGui::EndChildPos();

                            ImGui::BeginChildPos(settings_img, "Child Four", ImVec2(267, 260));
                            {

                            }
                            ImGui::EndChildPos();

                            ImGui::BeginChildPos(settings_img, "Child Six", ImVec2(267, 215));
                            {

                            }
                            ImGui::EndChildPos();

                            ImGui::BeginChildPos(settings_img, "Child Eight", ImVec2(267, 230));
                            {

                            }
                            ImGui::EndChildPos();
                        }

                        ImGui::EndGroup();

                    }
                }
                ImGui::EndChild();

                ImGui::PopStyleVar();

                

              

            }
            ImGui::End();

            keybind_alpha = ImClamp(keybind_alpha + (4.f * ImGui::GetIO().DeltaTime * (check_1 ? 1.f : -1.f)), 0.f, 1.f);
            ImGuiStyle* s = &ImGui::GetStyle();

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, keybind_alpha * s->Alpha);

            if (check_1) {

                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ImColor(21, 21, 26, 255)));
                ImGui::SetNextWindowPos(ImVec2(1200, 50), ImGuiCond_Once);
                ImGui::SetNextWindowSize(ImVec2(160, 135));
                ImGui::Begin("Keybinds", &check_1, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
                ImGui::PopStyleColor();
                
                const auto& p = ImGui::GetWindowPos();
                const auto& draw_list = ImGui::GetWindowDrawList();

                draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 160, p.y + 35), ImColor(23, 23, 29, 255), 8, ImDrawCornerFlags_Top);
                draw_list->AddImage(keyboard_img, ImVec2(p.x + 10, p.y + 10), ImVec2(p.x + 26, p.y + 26), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color));
                draw_list->AddText(ubu_child, 15.f, ImVec2(p.x + 37, p.y + 9), ImColor(255, 255, 255), "Spectators");

                ImGui::SetCursorPos(ImVec2(10, 45));
                ImGui::BeginGroup(); {

                    

                }ImGui::EndGroup();

                ImGui::End();
                
            }
            ImGui::PopStyleVar();

            preview_alpha = ImClamp(preview_alpha + (4.f * ImGui::GetIO().DeltaTime * (esp_preview ? 1.f : -1.f)), 0.f, 1.f);

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, preview_alpha * s->Alpha);

            if (esp_preview) {

                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ImColor(21, 21, 26, 255)));
                ImGui::SetNextWindowPos(ImVec2(1200, 300), ImGuiCond_Once);
                ImGui::SetNextWindowSize(ImVec2(280, 420));
                ImGui::Begin("Esp Preview", &esp_preview, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
                ImGui::PopStyleColor();

                const auto& p = ImGui::GetWindowPos();
                const auto& draw_list = ImGui::GetWindowDrawList();              

                draw_list->AddText(ubu_child, 15.f, ImVec2(p.x + 10, p.y + 10), ImColor(80, 80, 80), "Esp Preview");
                draw_list->AddImage(visual_img, ImVec2(p.x + 245, p.y + 6), ImVec2(p.x + 269, p.y + 30), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color));
                draw_list->AddText(ubu_child, 15.f, ImVec2(p.x + 83, p.y + 40), ImColor(255, 255, 255), "Miserio View");

                draw_list->AddRect(ImVec2(p.x + 40, p.y + 65), ImVec2(p.x + 210, p.y + 375), ImColor(30,30,30), 2, 2);
                draw_list->AddRectFilledMultiColor(ImVec2(p.x + 215, p.y + 65), ImVec2(p.x + 218, p.y + 375), ImGui::GetColorU32(c::accent_color), ImGui::GetColorU32(c::accent_color), ImGui::GetColorU32(c::accent_color, 40), ImGui::GetColorU32(c::accent_color, 40));
                draw_list->AddRectFilledMultiColor(ImVec2(p.x + 40, p.y + 380), ImVec2(p.x + 210, p.y + 383), ImGui::GetColorU32(c::accent_color), ImGui::GetColorU32(c::accent_color, 40), ImGui::GetColorU32(c::accent_color, 40), ImGui::GetColorU32(c::accent_color));
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 68), ImColor(255, 255, 255), "DEFUSE");
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 85), ImColor(255, 255, 255), "SCOPE");
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 102), ImColor(255, 255, 255), "ZOOM");
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 119), ImColor(255, 255, 255), "BOMB");
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 136), ImColor(255, 255, 255), "BLIND");
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 153), ImColor(255, 255, 255), "BONE");
                draw_list->AddText(ubu_preview, 12.f, ImVec2(p.x + 228, p.y + 170), ImColor(255, 255, 255), "HIT");

                draw_list->AddText(weapon_font, 14.f, ImVec2(p.x + 113, p.y + 390), ImColor(255, 255, 255), "W");
                
                ImGui::End();

            }
            ImGui::PopStyleVar();
        }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(main_hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
