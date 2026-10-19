// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "config.h"

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);


    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    self::load_image();

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    fonts::zona = io.Fonts->AddFontFromMemoryTTF(zona, sizeof(zona), 18.0f);
    fonts::bigzona = io.Fonts->AddFontFromMemoryTTF(zona, sizeof(zona), 24.0f);
    fonts::smallzona = io.Fonts->AddFontFromMemoryTTF(zona, sizeof(zona), 12.0f);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
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


        if (g_DeviceLost)
        {
            HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(1920, 1080));
        if (ImGui::Begin("background", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetWindowPos();
            draw_list->AddImage(img::background, pos, ImVec2(pos.x + 1920, pos.y + 1080));
            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(600, 400));
        ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoDecoration);
        ImGui::GetStyle().WindowRounding = 5;
        ImGui::GetStyle().PopupRounding = 5;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 window_pos = ImGui::GetWindowPos();

        {
            ImColor rect_col(12, 12, 12, 255);
            ImVec2 min = ImVec2(window_pos.x + 10, window_pos.y + 10);
            ImVec2 max = ImVec2(window_pos.x + 160, window_pos.y + 390);
            draw_list->AddRectFilled(min, max, rect_col, 5);
        }

        {
            ImGui::PushFont(fonts::bigzona);
            const char* label = "ImGui Menu";
            ImVec2 label_size = ImGui::CalcTextSize(label);
            ImColor label_col(150, 200, 255, 255);
            ImVec2 min = ImVec2(window_pos.x + 85 - label_size.x / 2, window_pos.y + 35 - label_size.y / 2);
            draw_list->AddText(min, label_col, label);
            ImGui::PopFont();
        }
        {
            ImVec2 size = ImVec2(130, 50);
            ImGui::SetCursorPos(ImVec2(20, 70));
            if (ImGui::Button("Aimbot", size, true, img::aimbot)) config::tab = 0;
            ImGui::SetCursorPosX(20);
            if (ImGui::Button("Visuals", size, true, img::visuals)) config::tab = 1;
            ImGui::SetCursorPosX(20);
            if (ImGui::Button("Misc", size, true, img::misc)) config::tab = 2;
            ImGui::SetCursorPosX(20);
            if (ImGui::Button("Configs", size, true, img::configs)) config::tab = 3;
            ImGui::SetCursorPosX(20);
            if (ImGui::Button("Our TG", size, true, img::telegram))
            ImGui::SetCursorPosX(20);
        }

        char* tab_name[4] = {
            "Current: Aimbot", 
            "Current: Visuals", 
            "Current: Misc", 
            "Current: Configs"
        };

        ImGui::SetCursorPos(ImVec2(170, 10));
        ImGui::Button(tab_name[config::tab], ImVec2(420, 50));

        if (config::tab == 0) {

            ImGui::SetCursorPosX(170);
            ImGui::BeginChild("Aimbot", ImVec2(430, 320));
            ImGui::Checkbox("Enable Aimbot", &config::aimbot::enable, true, img::settings);
            {
                ImGui::SetNextWindowSize(ImVec2(440, 250));
                if (ImGui::BeginPopup("Enable Aimbot")) {

                    ImGui::SetCursorPos(ImVec2(10, 10));
                    ImGui::Checkbox("Auto Fire", &config::aimbot::auto_fire);
                    ImGui::SetCursorPosX(10);
                    ImGui::Checkbox("Friendly Fire", &config::aimbot::friendly_fire);
                    ImGui::SetCursorPosX(10);
                    ImGui::SliderFloat("Radius", &config::aimbot::fov, 0, 180, "%.0f");
                    ImGui::SetCursorPosX(10);
                    ImGui::SliderFloat("Smooth", &config::aimbot::smooth, 0, 50, "%.0f");

                    ImGui::EndPopup();
                }
            }
            ImGui::Checkbox("Enable Silent Aimbot", &config::silent::enable);
            ImGui::Checkbox("Enable Backtrack", &config::backtrack::enable, true, img::settings);
            {
                ImGui::SetNextWindowSize(ImVec2(440, 70));
                if (ImGui::BeginPopup("Enable Backtrack")) {

                    ImGui::SetCursorPos(ImVec2(10, 10));
                    ImGui::SliderFloat("Smooth", &config::backtrack::max_backtrack, 0, 50, "%.0f ms");

                    ImGui::EndPopup();
                }
            }
            ImGui::Checkbox("Ignores", &config::ignores::enable, true, img::settings);
            {
                ImGui::SetNextWindowSize(ImVec2(440, 250));
                if (ImGui::BeginPopup("Ignores")) {

                    ImGui::SetCursorPos(ImVec2(10, 10));
                    ImGui::Checkbox("Smoke", &config::ignores::smoke);
                    ImGui::SetCursorPosX(10);
                    ImGui::Checkbox("Flash", &config::ignores::flash);
                    ImGui::SetCursorPosX(10);
                    ImGui::Checkbox("Jump", &config::ignores::jump);
                    ImGui::SetCursorPosX(10);
                    ImGui::Checkbox("Walls", &config::ignores::walls);

                    ImGui::EndPopup();
                }
            }

            if (ImGui::BeginCombo("Aimbot Mode", config::current_mode))
            {
                for (int n = 0; n < IM_ARRAYSIZE(config::mode); n++)
                {
                    bool is_selected = (config::current_mode == config::mode[n]);
                    if (ImGui::Selectable(config::mode[n], is_selected))
                        config::current_mode = config::mode[n];
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::EndChild();
        }
        else if (config::tab == 3) {
            ImGui::SetCursorPosX(170);
            ImGui::Button("Create Config", ImVec2(420, 50));
        }

        ImGui::End();
        
        ImGui::EndFrame();
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(50, 50, 50, 255);
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void self::load_image() {

    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, aimbot, sizeof(aimbot), &img::aimbot);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, visuals, sizeof(visuals), &img::visuals);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, misc, sizeof(misc), &img::misc);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, configs, sizeof(configs), &img::configs);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, settings, sizeof(settings), &img::settings);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, background, sizeof(background), &img::background);
    D3DXCreateTextureFromFileInMemory(g_pd3dDevice, telegram, sizeof(telegram), &img::telegram);
    
}
