#include "main.h"

int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LoadColor | ImGuiFreeTypeBuilderFlags_NoHinting;

    font::inter_black_widget = io.Fonts->AddFontFromMemoryTTF(inter_bold_one, sizeof(inter_bold_one), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_semibold_widget = io.Fonts->AddFontFromMemoryTTF(inter_semibold, sizeof(inter_semibold), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    font::inter_black_name = io.Fonts->AddFontFromMemoryTTF(inter_black, sizeof(inter_black), 22.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    for (int i = 14; i < 25; i++) font::icomoon[i] = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), i, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    if (texture::background == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &texture::background, 0);
    if (texture::background_blurred == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background_blurred, sizeof(background_blurred), &info, pump, &texture::background_blurred, 0);

    if (texture::circle_user == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_user, sizeof(circle_user), &info, pump, &texture::circle_user, 0);
    if (texture::esp_preview == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, esp_preview, sizeof(esp_preview), &info, pump, &texture::esp_preview, 0);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

 
            ImGui::GetBackgroundDrawList()->AddImage(texture::background, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

            ImGui::SetNextWindowSizeConstraints(ImVec2(c::background::size.x, c::background::size.y), ImGui::GetIO().DisplaySize);

            ImGui::Begin("IMGUI", nullptr, win_flags);
            {
                ImGuiStyle* style = &ImGui::GetStyle();

                const ImVec2& pos = ImGui::GetWindowPos();
                const ImVec2& region = ImGui::GetContentRegionMax();
                const ImVec2& spacing = style->ItemSpacing;

                c::accent = { widget_var::color[0], widget_var::color[1], widget_var::color[2], 1.f };
                c::esp_preview::box_color = { preview::box_color[0], preview::box_color[1], preview::box_color[2], preview::box_color[3] };

                c::background::background = { widget_var::bg_color[0], widget_var::bg_color[1], widget_var::bg_color[2], widget_var::bg_color[3] };

                style->WindowPadding = ImVec2(0, 0);
                style->ItemSpacing = ImVec2(20, 20);
                style->WindowBorderSize = 0;
                style->ScrollbarSize = 10.f;

               if (widget_var::bg_blur) ImGui::GetWindowDrawList()->AddImage(texture::background_blurred, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
                ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::background::background), c::background::rounding);

                ImGui::GetWindowDrawList()->AddRectFilled(pos + spacing, pos + ImVec2(180, 60) + spacing, ImGui::GetColorU32(c::child::background_top), c::child::rounding);
                ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(180 + (spacing.x * 2), spacing.y), pos + ImVec2(region.x - spacing.x, 60 + spacing.y), ImGui::GetColorU32(c::child::background_top), c::child::rounding);

                ImGui::SetCursorPos(ImVec2(region.x - (195 + spacing.x), 60 - (40 - spacing.y)) - spacing / 2);
                ImGui::InputTextEx("b", "SEARCH", widget_var::search, ARRAYSIZE(widget_var::search), ImVec2(195, 40), NULL);

                ImGui::GetWindowDrawList()->AddImageRounded(texture::circle_user, pos + ImVec2(190 + spacing.x, 10) + spacing, pos + ImVec2(230 + spacing.x, 50) + spacing, ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), 30.f);

                ImGui::GetWindowDrawList()->AddText(pos + ImVec2(240 + (spacing.x * 2), ((40 + spacing.y * 2) - ImGui::CalcTextSize(widget_var::user).y) / 2), ImGui::GetColorU32(c::text::text_active), widget_var::user);

                ImGui::PushFont(font::inter_semibold_widget);
                ImGui::GetWindowDrawList()->AddText(pos + ImVec2(260 + (spacing.x * 2) + ImGui::CalcTextSize(widget_var::user).x, ((40 + spacing.y * 2) - ImGui::CalcTextSize(widget_var::status).y) / 2), ImGui::GetColorU32(c::accent), widget_var::status);

                ImGui::GetWindowDrawList()->AddText(pos + ImVec2(240 + (spacing.x * 2), ((80 + spacing.y * 2) - ImGui::CalcTextSize(widget_var::UID).y) / 2), ImGui::GetColorU32(c::text::text), widget_var::UID);
                ImGui::PopFont();

                ImGui::PushFont(font::inter_black_name);
                ImGui::GetWindowDrawList()->AddText(pos + (ImVec2(180, 60) + (spacing * 2) - ImGui::CalcTextSize("DEADCODE")) / 2, ImGui::GetColorU32(c::accent), "DEAD");
                ImGui::GetWindowDrawList()->AddText(pos + (ImVec2(180 + ImGui::CalcTextSize("DEADCODE").x, 60) + (spacing * 2) - ImGui::CalcTextSize("DEADCODE")) / 2, ImGui::GetColorU32(c::text::text_active), "CODE");
                ImGui::PopFont();

                ImGui::SetCursorPos(ImVec2((spacing.x * 1), 60 + (spacing.y * 2)));

                custom::BeginGroup();
                {

                    custom::BeginGroup();
                    {
                        custom::BeginChild("PAGE LIST ONE", ImVec2(180, (region.y - (60 + spacing.y * 4)) / 2));
                        {

                            if (custom::Page(0 == widget_var::page, "c", "Aimbot", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) widget_var::page = 0;

                            if (custom::Page(1 == widget_var::page, "w", "Render", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) widget_var::page = 1;

                            if (custom::Page(2 == widget_var::page, "t", "Player", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) widget_var::page = 2;

                        }
                        custom::EndChild();

                        custom::BeginChild("PAGE LIST Two", ImVec2(180, (region.y - (60 + spacing.y * 4)) / 2));
                        {

                            if (custom::Page(3 == widget_var::page, "h", "Misc", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) widget_var::page = 3;

                            if (custom::Page(4 == widget_var::page, "g", "World", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) widget_var::page = 4;

                            if (custom::Page(5 == widget_var::page, "f", "Exploit", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40))) widget_var::page = 5;

                        }
                        custom::EndChild();

                    }
                    custom::EndGroup();

                    ImGui::SameLine();

                    static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                    tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (widget_var::page == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                    if (tab_alpha == 0.f && tab_add == 0.f) active_tab = widget_var::page;

                    custom::BeginChild("CHILD", ImVec2(region.x - (180 + spacing.x * 3), region.y - (60 * 2)));
                    {

                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

                        if (active_tab == 0)
                        {

                            custom::BeginGroup();
                            {

                                custom::Checkbox("Checkbox ON", &widget_var::enable_aim);

                                custom::Checkbox("Checkbox OFF", &widget_var::silent);

                                custom::SliderInt("Slider Integer", &widget_var::fov, -180, 180, "%d [D]");

                                custom::SliderFloat("Slider Float", &widget_var::sl_float, -0.5f, 0.5f, "%.3f [FL]");

                                custom::HueSlider("Hue Slider", widget_var::hue_color, picker_flags);

                            }
                            custom::EndGroup();

                            ImGui::SameLine();

                            custom::BeginGroup();
                            {

                                custom::Combo("ComboBox", &widget_var::select, widget_var::items, IM_ARRAYSIZE(widget_var::items), 2);

                                custom::MultiCombo("MultiCombo", widget_var::body_active, widget_var::body_select, 4);

                                custom::KnobInt("Knob Int", &widget_var::smooth, 0, 15, "%d-0-0");

                                custom::KnobFloat("Knob Float", &widget_var::hit_chance, -100.0f, 100.f, "%.1f-0-0");

                                custom::Keybind("Keybind", &widget_var::key, &widget_var::m);

                            }
                            custom::EndGroup();

                            ImGui::SameLine();

                            custom::BeginGroup();
                            {

                                custom::ColorPicker4("Color picker with alpha", widget_var::shot_color, picker_flags | ImGuiColorEditFlags_AlphaBar);

                                custom::ColorPicker4("Color picker", widget_var::shot_color1, picker_flags);

                                custom::QuandInt("Quand Integer", &widget_var::x_pos0, &widget_var::x_pos1, 0, 1920, 0, 1080);

                                custom::QuandFloat("Quand Float", &widget_var::map_x, &widget_var::map_y, 0.f, 20.f, 0.f, 20.f, "%.1f", "%.1f");

                            }
                            custom::EndGroup();

                        }
                        else if (active_tab == 3)
                        {
                            custom::BeginGroup();
                            {

                                custom::HueSlider("Main color", widget_var::color, picker_flags);

                                custom::ColorPicker4("Background color", widget_var::bg_color, picker_flags | ImGuiColorEditFlags_AlphaBar);

                            }
                            custom::EndGroup();

                            ImGui::SameLine();

                            custom::BeginGroup();
                            {

                                custom::Checkbox("Background blur", &widget_var::bg_blur);

                            }
                            custom::EndGroup();

                            ImGui::SameLine();

                            custom::BeginGroup();
                            {

                            }
                            custom::EndGroup();
                        }
                        else if (active_tab == 1)
                        {

                            custom::BeginGroup();
                            {

                                custom::Checkbox("Show ESP Preview", &preview::show_preview);

                                custom::Combo("HP Bar side", &preview::hp_side, preview::hp_item, IM_ARRAYSIZE(preview::hp_item), 2);

                        //        custom::ColorPicker4("Box color", preview::box_color, picker_flags | ImGuiColorEditFlags_AlphaBar);
                                
                                custom::HueSlider("Box color", preview::box_color, picker_flags);

                                custom::HueSlider("Nick color", preview::nick_color, picker_flags);

                            }
                            custom::EndGroup();

                            ImGui::SameLine();

                            custom::BeginGroup();
                            {

                                custom::Checkbox("Nickname", &preview::show_nick);

                                custom::Checkbox("Scopped", &preview::show_scop);

                                custom::Checkbox("Weapon", &preview::show_weapon);

                                custom::Checkbox("Smoke", &preview::show_smoke);

                                custom::Checkbox("Money", &preview::show_money);

                                custom::Checkbox("Bomb", &preview::show_bomb);

                            }
                            custom::EndGroup();

                            ImGui::SameLine();

                            custom::BeginGroup();
                            {

                                custom::QuandInt("Nickname position", &preview::namepos_x, &preview::namepos_y, 0, 300 - CalcTextSize("Nickname").x, 0, 40);

                                custom::QuandInt("Scopped position", &preview::scoppos_x, &preview::scoppos_y, 0, 300 - CalcTextSize("Scopped").x, 0, 150);

                                custom::QuandInt("Weapon position", &preview::weaponpos_x, &preview::weaponpos_y, 0, 300 - CalcTextSize("Weapon").x, 470, 530);

                                custom::QuandInt("Smoke position", &preview::smokepos_x, &preview::smokepos_y, 0, 300 - CalcTextSize("Smoked").x, 470, 530);

                                custom::QuandInt("Money position", &preview::moneypos_x, &preview::moneypos_y, 0, 300 - CalcTextSize("Money").x, 0, 150);

                                custom::QuandInt("Bomb position", &preview::bombpos_x, &preview::bombpos_y, 0, 300 - CalcTextSize("Bomb").x, 0, 150);

                            }
                            custom::EndGroup();
                        }


                        ImGui::PopStyleVar();

                    }
                    custom::EndChild();
                }
                custom::EndGroup();


            }
            ImGui::End();

            preview::preview_alpha = ImLerp(preview::preview_alpha, preview::show_preview ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            preview::name_alpha = ImLerp(preview::name_alpha, preview::show_nick ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            preview::weapon_alpha = ImLerp(preview::weapon_alpha, preview::show_weapon ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            preview::bomb_alpha = ImLerp(preview::bomb_alpha, preview::show_bomb ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            preview::scop_alpha = ImLerp(preview::scop_alpha, preview::show_scop ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            preview::money_alpha = ImLerp(preview::money_alpha, preview::show_money ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);
            preview::smoke_alpha = ImLerp(preview::smoke_alpha, preview::show_smoke ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f);

            if (preview::preview_alpha > 0.05f) {

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, preview::preview_alpha);

                ImGui::SetNextWindowSize(ImVec2(300, 530));

                ImGui::Begin("ESP PREVIEW", nullptr, ImGuiWindowFlags_NoDecoration);
                {

                    ImGuiStyle* style = &ImGui::GetStyle();

                    const ImVec2& pos = ImGui::GetWindowPos();
                    const ImVec2& region = ImGui::GetContentRegionMax();
                    const ImVec2& spacing = style->ItemSpacing;

                    style->WindowPadding = ImVec2(0, 0);
                    style->ItemSpacing = ImVec2(20, 20);
                    style->WindowBorderSize = 0;
                    style->ScrollbarSize = 10.f;

                   if (widget_var::bg_blur) ImGui::GetWindowDrawList()->AddImage(texture::background_blurred, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

                    ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::background::background), c::background::rounding);

                    ImVec2 text_var[6] = { ImVec2(preview::namepos_x, preview::namepos_y), ImVec2(preview::weaponpos_x, preview::weaponpos_y), ImVec2(preview::bombpos_x, preview::bombpos_y), ImVec2(preview::scoppos_x, preview::scoppos_y), ImVec2(preview::moneypos_x, preview::moneypos_y), ImVec2(preview::smokepos_x, preview::smokepos_y)};
                    float text_alpha[6] = { preview::name_alpha, preview::weapon_alpha, preview::bomb_alpha, preview::scop_alpha, preview::money_alpha, preview::smoke_alpha };
                    const char* push_name[6] = {"Nickname", "Weapon", "Bomb", "Scopped", "16.000$", "Smoked" };

                    for (int i = 0; i < ARRAYSIZE(push_name); i++)
                    ImGui::GetWindowDrawList()->AddText(font::inter_semibold_widget, 15.f, pos + text_var[i], ImGui::GetColorU32(c::text::text_active, text_alpha[i]), push_name[i]);


                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImGui::GetCursorScreenPos() + ImVec2(spacing.x * 2, spacing.y * 3), ImGui::GetCursorScreenPos() + ImVec2(region.x - (style->WindowPadding.x + spacing.x * 2), region.y - (style->WindowPadding.y + spacing.y * 3)), ImGui::GetColorU32(c::esp_preview::box_color, 0.0f), ImGui::GetColorU32(c::esp_preview::box_color, 0.0f), ImGui::GetColorU32(c::esp_preview::box_color, 0.3f), ImGui::GetColorU32(c::esp_preview::box_color, 0.3f));

                    ImGui::GetWindowDrawList()->AddRect(ImGui::GetCursorScreenPos() + ImVec2(spacing.x * 2, spacing.y * 3), ImGui::GetCursorScreenPos() + ImVec2(region.x - (style->WindowPadding.x + spacing.x * 2), region.y - (style->WindowPadding.y + spacing.y * 3)), ImGui::GetColorU32(c::esp_preview::box_color), 0.f);

                    preview::hp_position = ImLerp(preview::hp_position, preview::hp_side == 0 ? (region.x - (style->WindowPadding.x + spacing.x) - 1) : spacing.x, ImGui::GetIO().DeltaTime * 14.f);

                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImGui::GetCursorScreenPos() + ImVec2(preview::hp_position - 1, spacing.y * 3), ImGui::GetCursorScreenPos() + ImVec2(preview::hp_position, region.y - (style->WindowPadding.y + spacing.y * 3)), ImGui::GetColorU32(c::esp_preview::line_green), ImGui::GetColorU32(c::esp_preview::line_green), ImGui::GetColorU32(c::esp_preview::line_red), ImGui::GetColorU32(c::esp_preview::line_red), 0.f);

                    ImGui::GetWindowDrawList()->AddImage(texture::esp_preview, pos + ImVec2(region.x - 204, region.y - 361) / 2, pos + ImVec2(region.x + 204, region.y + 361) / 2, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::text::text_active));

                }
                ImGui::End();

                ImGui::PopStyleVar();

            }

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);

    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) 
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
