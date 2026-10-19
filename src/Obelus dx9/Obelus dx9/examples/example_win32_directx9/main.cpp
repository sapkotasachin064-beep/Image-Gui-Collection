// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "astriumwep.h"
#include "MuseoSansCyrl-300_0.h"
#include "MuseoSansCyrl-500_0.h"
#include "logo-menu.h"
#include <vector>
#include "avatar.h"
#include <map>
#include <imgui_internal.h>

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* LexendRegular;
ImFont* LexendLight;
ImFont* LexendRegularBold;

ImFont* TabsFont;
ImFont* WeaponFont;
ImFont* MainFont;
IDirect3DTexture9* Logo;
IDirect3DTexture9* Avatar;

namespace menu
{
    static float switch_alpha[3], open_alpha = 0;
    static bool toggled = true;
    static int m_tabs;

    class c_globals {
    public:
        bool active = true;
        bool show_login = true;
        bool exit = false;
        bool swow_accept_login = false;


        char user_name[255] = "Login";
        char pass_word[255] = "Password";

        int button_opacity = 255;

        bool hide = false;

    } globals;

    void Auth()
    {
        if (m_tabs == 0)
        {
            auto p = ImGui::GetWindowPos();

            static float fill;
            fill = ImClamp(fill + (2.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
            fill *= ImGui::GetStyle().Alpha;

            if (globals.swow_accept_login)
            {
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 25, ImVec2(p.x + 90, p.y + 17), ImColor(118, 118, 118, int(255 * fill)), "Counter-Strike: Global Offensive");
                ImGui::GetWindowDrawList()->AddText(LexendLight, 16, ImVec2(p.x + 90, p.y + 42), ImColor(200, 200, 200, int(255 * fill)), "Status: ");
                ImGui::GetWindowDrawList()->AddText(LexendLight, 16, ImVec2(p.x + 135, p.y + 42), ImColor(116, 255, 94, int(255 * fill)), "Undetect");
            }

            if (globals.swow_accept_login)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[1]);
                {
                    ImGui::SetCursorPos(ImVec2{ 85, 70 });
                    ImGui::MenuChildTextex(" Software info ", ImVec2(335, 120), true);
                    {
                        ImGui::PushFont(LexendRegularBold);
                        {
                            ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.50f, 0.50f, 0.50f, 1.00f));
                            {
                                ImGui::Spacing();

                                ImGui::SetCursorPosX(10);
                                ImGui::Text("Version: 5.3");

                                ImGui::SetCursorPosX(10);
                                ImGui::Text("Build Type : Beta");

                                ImGui::SetCursorPosX(10);
                                ImGui::Text("Subscription till: 30.12.1337");
                            }
                            ImGui::PopStyleColor();
                        }
                        ImGui::PopFont();
                    }
                    ImGui::EndChild();

                    ImGui::Spacing();

                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                    {
                        ImGui::SetCursorPosX(85);
                        if (ImGui::Button("Exit", ImVec2(155, 31)))
                        {
                            globals.show_login = true;
                            globals.swow_accept_login = false;
                        }
                        ImGui::SameLine();

                        ImGui::SetCursorPosX(265);
                        if (ImGui::Button("Inject", ImVec2(155, 31)))
                        {
                            globals.swow_accept_login = false;
                            globals.hide = true;
                        }
                    }
                    ImGui::PopStyleVar();
                }
                ImGui::PopStyleVar();
            }
        }
    }

    void Decoration()
    {
        auto p = ImGui::GetWindowPos();

        static float fill;
        fill = ImClamp(fill + (2.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
        fill *= ImGui::GetStyle().Alpha;

        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 457, p.y + 250), ImColor(15, 15, 15, int(255 * fill)), 10.f); //bg

        if (!globals.hide)
        {
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 70, p.y + 250), ImColor(10, 10, 10, int(255 * fill)), 10.f); // left bg = tbs

            ImGui::SetCursorPos(ImVec2(-11, 5));
            ImGui::Image(Logo, ImVec2(92.52f, 77.66f));//logo

            Auth();
        }
        else
        {
            ImGui::GetWindowDrawList()->AddText(LexendRegular, 25, ImVec2(p.x + 70, p.y + 17), ImColor(118, 118, 118, int(255 * fill)), "Counter-Strike: Global Offensive");

            ImGui::GetWindowDrawList()->AddText(LexendRegularBold, 16, ImVec2(p.x + 198, p.y + 75), ImColor(169, 169, 169, int(255 * fill)), "Injecting...");

            ImGui::SetCursorPos(ImVec2(155, 200));
            if (ImGui::Button("Exit", ImVec2(155, 31)))
            {
                globals.hide = false;
                globals.swow_accept_login = true;
            }
            ImGui::SameLine();


            auto front = ImGui::GetForegroundDrawList();
            ImVec2 center = ImVec2(p.x + 230, p.y + 140);
            static ImColor fore_color(213, 9, 2);
            static ImColor back_color(54, 53, 58);
            static float arc_size = 0.45f;
            static float radius = 35.f;
            static float thickness = 4.f;

            static float position = 0.f;
            position = ImLerp(position, IM_PI * 2.f, ImGui::GetIO().DeltaTime * 2.3f);

            front->PathClear();
            front->PathArcTo(center, radius, 0.f, 2.f * IM_PI, 40.f);
            front->PathStroke(ImGui::GetColorU32(back_color.Value), 0, thickness);

            front->PathClear();
            front->PathArcTo(center, radius, IM_PI * 1.5f + position, IM_PI * (1.5f + arc_size) + position, 40.f);
            front->PathStroke(ImGui::GetColorU32(fore_color.Value), 0, thickness);

            if (position >= IM_PI * 1.90f)
                position = 0.f;

        }
    }

    void RenderTab()
    {
        ImGui::PushFont(TabsFont);
        {
            ImGui::SetCursorPos(ImVec2{ 18, 103 });
            if (ImGui::Renderingtab("A", !m_tabs)) m_tabs = 0;

            ImGui::SetCursorPos(ImVec2{ 18, 160.8f });
            if (ImGui::Renderingtab("E", m_tabs == 1)) m_tabs = 1;
        }
        ImGui::PopFont();
    }

    void Login()
    {
        if (m_tabs == 0)
        {
            auto p = ImGui::GetWindowPos();

            static float fill;
            fill = ImClamp(fill + (2.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
            fill *= ImGui::GetStyle().Alpha;

            if (globals.show_login)
            {
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 25, ImVec2(p.x + 90, p.y + 17), ImColor(118, 118, 118, int(255 * fill)), "Influense > Authorization");
                ImGui::GetWindowDrawList()->AddText(LexendLight, 16, ImVec2(p.x + 90, p.y + 42), ImColor(200, 200, 200, int(255 * fill)), "Enter your login and password");
            }

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[1]);
            {
                ImGui::SetCursorPos(ImVec2{ 80, 55 });
                ImGui::MenuChild(" Login ", ImVec2(350, 150), true);
                {
                    if (globals.show_login)
                    {
                        ImGui::SetCursorPosX(10);
                        ImGui::InputText("##Username", globals.user_name, IM_ARRAYSIZE(globals.user_name));

                        ImGui::SetCursorPosX(10);
                        ImGui::InputText("##Passowrd", globals.pass_word, IM_ARRAYSIZE(globals.pass_word));

                        ImGui::Spacing();

                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                        {
                            if (ImGui::Button("Exit", ImVec2(155, 31)))
                            {

                            }
                            ImGui::SameLine();

                            ImGui::SetCursorPosX(185);
                            if (ImGui::Button("Auth", ImVec2(155, 31)))
                            {
                                globals.show_login = false;
                                globals.swow_accept_login = true;
                            }
                        }
                        ImGui::PopStyleVar();
                    }
                }
                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
        }
    }

    void Render()
    {
        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.5f : -1.5f)), 0.f, 1.f);
        if (open_alpha > 0.01f)
        {
            if (toggled && open_alpha > 0.8f || !toggled)
                switch_alpha[0] = ImClamp(switch_alpha[0] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);

            ImGui::SetNextWindowSize(ImVec2(700, 450));
            ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Unknown name cheat", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {

                Decoration();

                if (!globals.hide)
                {
                    RenderTab();
                }
          
                if (switch_alpha[0] > 0.01f)
                {
                    if (toggled && switch_alpha[0] > 0.8f || !toggled)
                        switch_alpha[1] = ImClamp(switch_alpha[1] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                    if (switch_alpha[1] > 0.01f)
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[1]);
                        ImGui::SetCursorPos(ImVec2(130, 60));
                        ImGui::PushFont(MainFont);
                        ImGui::BeginGroup();
                        {

                            switch (m_tabs)
                            {
                            case 0: Login(); break;
                            case 1: break;
                            case 2: break;
                            case 3: break;
                            case 4: break;
                            }
                        }
                        ImGui::EndGroup();
                        ImGui::PopFont();
                        ImGui::PopStyleVar();
                    }
                    ImGui::PopStyleVar(2);
                }
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
    }

    void CreateStyle()
    {
        auto& style = ImGui::GetStyle();
        auto& color = style.Colors;

        style.FramePadding = ImVec2(1, 0);
        style.FrameRounding = 3;
        style.WindowBorderSize = 0;
        style.WindowPadding = ImVec2(5, 5);
        style.ScrollbarRounding = 3;
        style.ScrollbarSize = 5;
    }
}

// Main code
int main(int, char**)
{
    // Create application window
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
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    menu::CreateStyle();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logomenu, sizeof(logomenu), 92.52f, 77.66f, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Logo);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, avatar, sizeof(avatar), 35, 35, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Avatar);

    // Load Fonts
    MainFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl3000, sizeof(MuseoSansCyrl3000), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
    TabsFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl5000, sizeof(MuseoSansCyrl5000), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    WeaponFont = io.Fonts->AddFontFromMemoryTTF(astriumwep, sizeof(astriumwep), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

    LexendRegularBold = io.Fonts->AddFontFromMemoryTTF(LexendRegularfont, sizeof(LexendRegularfont), 14, NULL, io.Fonts->GetGlyphRangesCyrillic());

    LexendRegular = io.Fonts->AddFontFromMemoryTTF(LexendRegularfont, sizeof(LexendRegularfont), 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
    LexendLight = io.Fonts->AddFontFromMemoryTTF(LexendLightfont, sizeof(LexendLightfont), 14, NULL, io.Fonts->GetGlyphRangesCyrillic());

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        menu::Render();  //Renderin

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
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
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
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

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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
