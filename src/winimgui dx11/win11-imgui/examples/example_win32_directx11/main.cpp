// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <imgui_internal.h>



// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;



// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);




// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
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
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            static float windowWidth = 340;
            static float windowAlpha = 1.f;
            static int sliderIntValue = 0;
            static float sliderFloatValue = 0;
            static bool toggle1 = false;
            static bool toggle2 = false;
            static bool toggle3 = false;
            static bool isInsertPressed = false;

            static float tabAlpha = 0.f;
            static int currentTab = 0;

            if (ImGui::IsKeyPressed(ImGuiKey_Insert))
            {
                isInsertPressed = !isInsertPressed; // Toggle the state of the flag
            }

            if (isInsertPressed)
            {
                if (windowWidth > 0) // Decrease the width if it's greater than 0
                {
                    windowWidth -= 10;
                    windowAlpha -= 0.05f;
                }
            }
            else
            {
                if (windowWidth < 806) // Increase the width if it's less than 806
                {
                    windowWidth += 10;
                    windowAlpha += 0.05f;
                }
            }

            static int previousTab = -1;
            if (currentTab != previousTab)
            {
                if (tabAlpha < 1.f) // Increase alpha if it's less than 1.f
                {
                    tabAlpha += 0.05f;
                }
            }
            else
            {
                if (tabAlpha > 0.f) // Decrease alpha if it's greater than 0.f
                {
                    tabAlpha -= 0.05f;
                }
            }

            static int currentCategory = 0;

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, windowAlpha);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, 450));
            ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            {
                ImGui::Columns(2, nullptr, false);
                ImGui::SetColumnOffset(1, 120.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.f);
                if (ImGui::subtab("", "tab one", currentTab == 0)) currentTab = 0, tabAlpha = 0.f;
                if (ImGui::subtab("", "tab two", currentTab == 1)) currentTab = 1, tabAlpha = 0.f;
                if (ImGui::subtab("", "tab three", currentTab == 2)) currentTab = 2, tabAlpha = 0.f;
                ImGui::PopStyleVar();
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tabAlpha);

                ImGui::NextColumn();
                ImGui::BeginGroup();
                switch (currentTab)
                {
                case 0:
                {
                    static bool boolArray[9] = { false, false, false, false, false, false, false, false, false };
                    static bool boolArray2[9] = { false, false, false, false, false, false, false, false, false };
                    for (int i = 0; i < 9; i++) {
                        std::string label = "Checkbox " + std::to_string(i + 1);
                        ImGui::Checkbox(label.c_str(), &boolArray[i], "This is a description");
                    }

                    ImGui::SetCursorPos(ImVec2(460, 8));
                    ImGui::BeginGroup();
                    for (int i = 0; i < 9; i++) {
                        std::string label = "No Desc. " + std::to_string(i + 1);
                        ImGui::Checkbox(label.c_str(), &boolArray2[i], "");
                    }
                    ImGui::EndGroup();
                    break;
                }
                case 1:
                    ImGui::SliderInt("SliderInt", &sliderIntValue, 0, 100);
                    ImGui::SliderFloat("SliderFloat", &sliderFloatValue, 0.00f, 5.00f, "%.2f");
                    ImGui::ToggleButton("Toggle", &toggle1);
                    ImGui::ToggleButton("Toggle 2", &toggle2, "Description");

                    ImGui::PushItemWidth(324);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 5));
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 12.f));
                    ImGui::Combo("Combo", &currentCategory, "%100\0%125\0%150\0%200\0%250\0");
                    ImGui::PopStyleVar(2);
                    ImGui::PopItemWidth();
                    ImGui::ToggleButton("Toggle 3", &toggle3);
                    break;
                case 2:
                    ImGui::Button("Launch Phone", ImVec2(130, 32), "Phone Link", "Remote Access Phone");
                    ImGui::Button("Button2", ImVec2(130, 32), "No Desc.");
                    const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
                    static int listbox_item_current = 1;
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 5));
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 12.f));
                    ImGui::PushItemWidth(324);
                    ImGui::ListBox("##", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
                    ImGui::PopStyleVar(2);
                    ImGui::PopItemWidth();
                    break;
                }
                ImGui::EndGroup();
                ImGui::PopStyleVar();
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
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
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
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
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
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
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
