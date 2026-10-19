// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include "blur.hpp"

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DrawInfoWindow( bool open, int* scale, int* color, float* speed ) {

    static float alpha = 0.f;
    alpha = ImLerp( alpha, open ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 8.f * *speed );
    if ( alpha < 0.1f )
        return;

    ImGui::PushStyleVar( ImGuiStyleVar_Alpha, alpha );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2(0, 0) );
    ImGui::SetNextWindowSize( ImVec2( 305 * ImGui::GetIO( ).GUIScale, 450 * ImGui::GetIO( ).GUIScale ) );
    ImGui::Begin( "Info window", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration );
    {
        ImGui::SetCursorPosY( 290 * ImGui::GetIO( ).GUIScale );
        ImGui::BeginGroup( );
        {
            static bool bred_ebaniy = false;
            ImGui::Checkbox( "Auto Save",&bred_ebaniy );
            ImGui::Combo( "Dpi Scale", scale, "50%\0" "75%\0" "100%\0" "125%\0" "150%\0" "175%\0" "200%\0" );
            ImGui::SliderFloat( "Animation Speed", speed, 0.1f, 2.f, "%.1f" );
            ImGui::SetCursorPosX( 10 ); ImGui::TextColored( Colors::Text, "Style" );
            ImGui::SameLine( 210 * ImGui::GetIO( ).GUIScale ); if ( ImGui::RadioButton( "Blue", *color == 0 ) ) *color = 0;
            ImGui::SameLine( ); if ( ImGui::RadioButton( "White", *color == 2 ) ) *color = 2;
            ImGui::SameLine( ); if ( ImGui::RadioButton( "Black", *color == 1 ) ) *color = 1;
        }
        ImGui::EndGroup( );
    }
    ImGui::End( );
    ImGui::PopStyleVar( 2 );
}

// Main code
int main(int, char**)
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    //::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    ImFontConfig font_config;
    font_config.Density = ImGui_ImplWin32_GetDpiScaleForHwnd(hwnd);
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.FontBuilderFlags = 1;
    //io.Fonts->AddFontDefault(&font_config);
    io.Fonts->AddFontFromFileTTF( "../../misc/fonts/museosanscyrl-500.ttf", 16.0f, &font_config );
    io.Fonts->AddFontFromFileTTF( "../../misc/fonts/museosanscyrl-900.ttf", 30.0f );
    io.Fonts->AddFontFromFileTTF( "../../misc/fonts/museosanscyrl-700.ttf", 16.0f, &font_config );
    io.Fonts->AddFontFromFileTTF( "../../misc/fonts/museosanscyrl-500.ttf", 11.0f, &font_config );
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/neverlose-v2-icons.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF( "../../misc/fonts/neverlose-v2-icons.ttf", 12.0f );
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    io.AnimationSpeed = 1;
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        static float dpi_scale = 1.f; static int dpi_set = 2;
        io.DisplayFramebufferScale = ImVec2(dpi_scale, dpi_scale);
        io.DisplaySize.x /= dpi_scale;
        io.DisplaySize.y /= dpi_scale;

        switch ( dpi_set ) {
        case 0: dpi_scale = 0.70f; break;
        case 1: dpi_scale = 0.85f; break;
        case 2: dpi_scale = 1.f; break;
        case 3: dpi_scale = 1.15f; break;
        case 4: dpi_scale = 1.30f; break;
        case 5: dpi_scale = 1.45f; break;
        case 6: dpi_scale = 1.60f; break;
        }
        io.GUIScale = dpi_scale;
        if (io.MousePos.x != -FLT_MAX && io.MousePos.y != -FLT_MAX)
        {
            io.MousePos.x /= dpi_scale;
            io.MousePos.y /= dpi_scale;
        }

        ImGui::NewFrame();
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        static int theme = 1;
       
        ImGui::Begin( "Modulate", nullptr );
        {
            
        }
        ImGui::End( );

        Colors::LeftBar = ImLerp( Colors::LeftBar, theme == 1 ? ImColor( 8, 8, 8, int( 255 * ImGui::GetStyle( ).Alpha) ).Value : theme == 2 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha * 0.75f ) ).Value : ImColor( 3, 30, 50, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::Background = ImLerp( Colors::Background, theme == 1 ? ImColor( 8, 8, 8, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 9, 8, 13, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::Child = ImLerp( Colors::Child, theme == 1 ? ImColor( 11, 11, 13, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 250, 250, 250, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 1, 11, 21, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::ChildText = ImLerp( Colors::ChildText, theme == 1 ? ImColor( 221, 220, 225, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 0, 0, 0, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 200, 203, 208, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::Borders = ImLerp( Colors::Borders, theme == 1 ? ImColor( 26, 26, 26, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 217, 217, 217, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 12, 33, 52, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::BordersActive = ImLerp( Colors::BordersActive, theme == 1 ? ImColor( 36, 34, 38, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 217, 217, 217, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 4, 25, 40, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::TabFrame = ImLerp( Colors::TabFrame, theme == 1 ? ImColor( 62, 63, 58, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 214, 213, 213, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 1, 72, 104, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::TabText = ImLerp( Colors::TabText, theme == 1 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 38, 38, 38, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::Logo = ImLerp( Colors::Logo, theme == 1 ? ImColor( 253, 254, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 51, 51, 51, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 248, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::Text = ImLerp( Colors::Text, theme == 1 ? ImColor( 162, 176, 185, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 77, 77, 77, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 162, 176, 185, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::TextActive = ImLerp( Colors::TextActive, theme == 1 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 6, 6, 6, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::Accent = ImLerp( Colors::Accent, theme == 2 ? ImColor( 0, 165, 243, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 0, 165, 243, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 6.f );
        Colors::AccentDisabled = ImLerp( Colors::AccentDisabled, theme == 1 ? ImColor( 76, 90, 101, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 76, 90, 101, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );

        Colors::Frame = ImLerp( Colors::Frame, theme == 1 ? ImColor( 3, 6, 15, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 231, 231, 231, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 2, 5, 12, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::FrameDisabled = ImLerp( Colors::FrameDisabled, theme == 1 ? ImColor( 8, 8, 8, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 8, 9, 13, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::FrameHovered = ImLerp( Colors::FrameHovered, theme == 1 ? ImColor( 13, 13, 13, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 255, 255, 255, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 2, 5, 12, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::FrameActive = ImLerp( Colors::FrameActive, theme == 1 ? ImColor( 4, 25, 46, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 0, 119, 188, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 2, 18, 33, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );
        Colors::FrameOpened = ImLerp( Colors::FrameOpened, theme == 1 ? ImColor( 32, 31, 33, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : theme == 2 ? ImColor( 238, 238, 238, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value : ImColor( 2, 18, 33, int( 255 * ImGui::GetStyle( ).Alpha ) ).Value, io.DeltaTime * 12.f * io.AnimationSpeed );

        //  
        BlurData::device = g_pd3dDevice;
        static int ActiveTab = 0;
        ImGui::SetNextWindowSize( ImVec2(800 * io.GUIScale, 620 * io.GUIScale ) );
        ImGui::MainBegin( "Menu", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration );
        {
            ImGui::SetCursorPos( ImVec2(0.f, 62.f * io.GUIScale ) );
            ImGui::BeginGroup( /* Tabs place */ );
            {
                ImGui::SetCursorPosX( 25 ); ImGui::TextColored( Colors::Text, "Aimbot" );
                if ( ImGui::TabButton( "A", "Ragebot", ActiveTab == 0 ) ) ActiveTab = 0;
                if ( ImGui::TabButton( "B", "Anti Aim", ActiveTab == 1 ) ) ActiveTab = 1;
                if ( ImGui::TabButton( "C", "Legitbot", ActiveTab == 2 ) ) ActiveTab = 2;

                ImGui::Spacing( ); ImGui::Spacing( ); ImGui::Spacing( );

                ImGui::SetCursorPosX( 25 ); ImGui::TextColored( Colors::Text, "Visuals" );
                if ( ImGui::TabButton( "D", "Players", ActiveTab == 3 ) ) ActiveTab = 3;
                if ( ImGui::TabButton( "E", "Weapon", ActiveTab == 4 ) ) ActiveTab = 4;
                if ( ImGui::TabButton( "F", "Grenades", ActiveTab == 5 ) ) ActiveTab = 5;
                if ( ImGui::TabButton( "G", "World", ActiveTab == 7 ) ) ActiveTab = 7;
                if ( ImGui::TabButton( "H", "View", ActiveTab == 8 ) ) ActiveTab = 8;

                ImGui::Spacing( ); ImGui::Spacing( ); ImGui::Spacing( );

                ImGui::SetCursorPosX( 25 ); ImGui::TextColored( Colors::Text, "Miscellaneous" );
                if ( ImGui::TabButton( "I", "Main", ActiveTab == 9 ) ) ActiveTab = 9;
                if ( ImGui::TabButton( "J", "Inventory", ActiveTab == 10 ) ) ActiveTab = 10;
                if ( ImGui::TabButton( "K", "Scripts", ActiveTab == 11 ) ) ActiveTab = 11;
                if ( ImGui::TabButton( "L", "Configs", ActiveTab == 12 ) ) ActiveTab = 12;
            }
            ImGui::EndGroup( );

            ImGui::SetCursorPos( ImVec2( 200.f * io.GUIScale, 0 ) );
            ImGui::BeginGroup( /* Info place */ );
            {
                ImGui::SetCursorPosY( 10 * io.GUIScale );
                ImGui::Button( "N", "Save", ImVec2(100 * io.GUIScale, 30 * io.GUIScale ) );

                static bool opened = false;

                ImGui::SetCursorPos( ImVec2(800 - 70 * io.GUIScale, 10 * io.GUIScale) );
                ImGui::PushFont( io.Fonts->Fonts[ 4 ] );
                if ( ImGui::ButtonEx( "L", ImVec2( 25, 30 ), ImGuiButtonFlags_TextOnly ) ) opened = !opened;
                ImGui::SameLine( );
                ImGui::ButtonEx( "M", ImVec2( 25, 30 ), ImGuiButtonFlags_TextOnly );
                ImGui::PopFont( );

                DrawInfoWindow( opened, &dpi_set, &theme, &io.AnimationSpeed );
            }
            ImGui::EndGroup( );

            ImGui::SetCursorPos( ImVec2( 180.f * io.GUIScale, 50.f * io.GUIScale ) );
            ImGui::BeginChild( "content", ImVec2(1110, 1110) );
            {
                int ChildW = 275 * io.GUIScale;
                ImGui::SetCursorPos( ImVec2( 18, 48 ) );
                ImGui::MenuChild( "Movement", ImVec2( ChildW, 270) );
                {
                    static bool Checkboxes[ 8 ];
                    ImGui::Checkbox( "Auto Jump", &Checkboxes[ 0 ] );
                    ImGui::Checkbox( "Auto Strafe", &Checkboxes[ 1 ] );
                    ImGui::Checkbox( "Quick Stop", &Checkboxes[ 2 ] );
                    ImGui::Checkbox( "Strafe Assist", &Checkboxes[ 3 ] );
                    ImGui::Checkbox( "Auto Peek", &Checkboxes[ 4 ] );
                    ImGui::Checkbox( "Edge Jump", &Checkboxes[ 5 ] );
                    ImGui::Checkbox( "Infinity Duck", &Checkboxes[ 6 ] );
                    ImGui::Checkbox( "Blockbot", &Checkboxes[ 7 ] );
                }
                ImGui::EndChild( );

                ImGui::SameLine( 0, 16 );

                ImGui::MenuChild( "Other", ImVec2( ChildW, 300 ) );
                {
                    static bool Checkboxes[ 6 ]; static int Combos[ 2 ]; static int Slider = 0;
                    ImGui::Checkbox( "Anti Untrusted", &Checkboxes[ 0 ] );
                    ImGui::Combo( "Event Log", &Combos[ 0 ], "Damage\0" );
                    ImGui::Combo( "Windows", &Combos[ 1 ], "Binds List, Watermark\0" );
                    ImGui::Checkbox( "Filter server ads", &Checkboxes[ 1 ] );
                    ImGui::Checkbox( "Filter Console", &Checkboxes[ 2 ] );
                    ImGui::Checkbox( "Unlock Cvars", &Checkboxes[ 3 ] );
                    ImGui::Checkbox( "Fast Reload", &Checkboxes[ 4 ] );
                    ImGui::Checkbox( "Fast Weapon Switch", &Checkboxes[ 5 ] );
                    ImGui::SliderInt( "Fake Ping", &Slider, 0, 200 );
                }
                ImGui::EndChild( );
            }
            ImGui::EndChild( );
        }
        ImGui::End( );
       
        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImDrawData* draw_data = ImGui::GetDrawData();
            draw_data->ScaleClipRects(draw_data->FramebufferScale);
            ImGui_ImplDX9_RenderDrawData(draw_data);
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
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
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
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
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
