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
ImFont* LexendRegularTabs;
ImFont* Tabsicon;

ImFont* TabsFont;
ImFont* WeaponFont;
ImFont* MainFont;
IDirect3DTexture9* Logo;
IDirect3DTexture9* Avatar;
IDirect3DTexture9* Logo3;
IDirect3DTexture9* bg;
IDirect3DTexture9* info;

bool time;
 
namespace menu
{
    static float switch_alpha[3], open_alpha = 0;
    static bool toggled = true;
    static int m_tabs;
    static int  m_tabs1;
    static int enemy_tab;
    static int weapon_tab;

    class c_globals {
    public:
        bool active = true;
        bool show_login = true;
        bool iject = true;
        bool swow_accept_login = false;


        char user_name[255] = "casperskiyi";
        char pass_word[255] = "Enter your password";

        char invite_key[255] = "Enter your invite key";

        int button_opacity = 255;

    } globals;

    void Injectgame()
    {
        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        static float fill;
        fill = ImClamp(fill + (9.5f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
        fill *= style.Alpha;
        if (m_tabs == 0)
        {
            auto p = ImGui::GetWindowPos();
            ImGui::GetWindowDrawList()->AddText(LexendRegular, 21, ImVec2(p.x + 420, p.y + 60), ImColor(240, 240, 240, int(255 * fill)), "Welcome to, ");
            ImGui::GetWindowDrawList()->AddText(LexendRegular, 21, ImVec2(p.x + 525, p.y + 60), ImColor(145, 222, 52, int(255 * fill)), "casperskiyi");
            ImGui::GetWindowDrawList()->AddText(LexendRegular, 17, ImVec2(p.x + 405, p.y + 240), ImColor(170, 170, 170, int(255 * fill)), "info");

            ImGui::SetCursorPos(ImVec2(405, 100));
            ImGui::Image(info, ImVec2(230, 130));//logo

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 405, p.y + 270), ImVec2(p.x + 635, p.y + 300), ImColor(24, 25, 27), 4); //bg
            {
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 17, ImVec2(p.x + 415, p.y + 275), ImColor(170, 170, 170, int(255 * fill)), "sub utill ");
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 17, ImVec2(p.x + 595, p.y + 275), ImColor(145, 222, 52, int(255 * fill)), "11.11");
            }
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 405, p.y + 310), ImVec2(p.x + 635, p.y + 340), ImColor(24, 25, 27), 4); //bg1
            {
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 17, ImVec2(p.x + 415, p.y + 315), ImColor(170, 170, 170, int(255 * fill)), "status");
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 17, ImVec2(p.x + 585, p.y + 315), ImColor(145, 222, 52, int(255 * fill)), "ready");
            }


            ImGui::PushFont(LexendRegular);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
            ImGui::SetCursorPos(ImVec2(405, 355));
            if (ImGui::Button("Load", ImVec2(230.f, 30.f)))
            {
                globals.show_login = false;
                globals.iject = true;
            }
            ImGui::PopStyleVar();
            ImGui::PopFont();
        }
    }

    void Login_game()
    {
        ImGui::PushFont(LexendRegularTabs);
        {
            ImGui::SetCursorPos(ImVec2{ 20, 103 });
            if (ImGui::Renderingtab( "A", "CSGO", "last upd: 18.01", !m_tabs)) m_tabs = 0;

            ImGui::SetCursorPos(ImVec2{ 20, 160.8f });
            if (ImGui::Renderingtab("B", "CSGO BETA", "last upd: 08.12", m_tabs == 1)) m_tabs = 1;

            ImGui::SetCursorPos(ImVec2{ 20, 218.5f });
            if (ImGui::Renderingtab("C", "CSGO ALPHA", "last upd: 23.03", m_tabs == 2)) m_tabs = 2;
        }
        ImGui::PopFont();

        Injectgame();
    }

    void Decoration()
    {
        /// <summary>
        /// 
        /// </summary>
        const char* label;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        const ImGuiID id = window->GetID(label);

        bool hovered;
        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        static std::map<ImGuiID, float> hovered_animation;
        auto it_hovered = hovered_animation.find(id);
        if (globals.swow_accept_login)
        {
            if (it_hovered == hovered_animation.end())
            {
                hovered_animation.insert({ id, 0.f });
                it_hovered = hovered_animation.find(id);
            }
            it_hovered->second = ImClamp(it_hovered->second + (1.f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.5f)), 0.0f, 1.0f);
            it_hovered->second *= style.Alpha;
        }

        static std::map<ImGuiID, float> open_animation;
        auto it_open = open_animation.find(id);
        if (ImGui::GetTime() >= 2.5 || time)
        {
            if (it_open == open_animation.end())
            {
                open_animation.insert({ id, 0.f });
                it_open = open_animation.find(id);
            }
            it_open->second = ImClamp(it_open->second + (3.0f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.0f)), 0.0f, 5.0f);
            it_open->second *= style.Alpha;
        }

        static std::map<ImGuiID, float> active_animation;
        auto it_active = active_animation.find(id);

        if (ImGui::GetTime() >= 1.5 || time)
        {
            if (it_active == active_animation.end())
            {
                active_animation.insert({ id, 0.f });
                it_active = active_animation.find(id);
            }
            it_active->second = ImClamp(it_active->second + (1.5f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.f)), 0.0f, 1.0f);
            it_active->second *= style.Alpha;
        }

        static float fill;
        fill = ImClamp(fill + (9.5f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
        fill *= style.Alpha;

        /// <summary>
        /// 
        /// </summary>

        if (globals.show_login)
        {
            auto p = ImGui::GetWindowPos();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 700, p.y + 400), ImColor(19, 20, 22)); //bg

            static double end_time = ImGui::GetTime() + 2.5;
            if (ImGui::GetTime() <= end_time) //server time
            {
                ImGui::GetWindowDrawList()->AddText(LexendRegular, 19, ImVec2(p.x + 310, p.y + 185), ImColor(240, 240, 240, int(255 * fill)), "predator.systems");

                ImGui::SetCursorPos(ImVec2(230, 160));
                ImGui::Image(Avatar, ImVec2(80, 80));//logo

                if (ImGui::GetTime() >= 1.5 || time)
                {
                    ImGui::SetCursorPos(ImVec2(230 + it_active->second * 5, 160 - it_active->second * 70));
                    ImGui::Image(Avatar, ImVec2(80 + it_active->second * 150, 80 + it_active->second * 146));//logo
                }
            }

            if (ImGui::GetTime() >= 2.5 || time)
            {
                ImGui::SetCursorPos(ImVec2(235 + it_open->second * 15 , 90 - it_open->second * 57));
                ImGui::Image(Logo, ImVec2(230 + it_open->second * 115, 226 + it_open->second * 115));//logo
            }
        }
        if (globals.swow_accept_login)
        {
            auto p = ImGui::GetWindowPos();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 700, p.y + 400), ImColor(19, 20, 22)); //bg

            //можно было 1 картинку ну ладно)
            ImGui::SetCursorPos(ImVec2(200, 90));
            ImGui::Image(Logo3, ImVec2(120, 120));//logo

            ImGui::SetCursorPos(ImVec2(75, 222));
            ImGui::Image(Logo3, ImVec2(120, 120));//logo

            ImGui::SetCursorPos(ImVec2(15, 15));
            ImGui::Image(Logo3, ImVec2(120, 120));//logo

            ImGui::SetCursorPos(ImVec2(245, 25));
            ImGui::Image(Logo3, ImVec2(120, 120));//logo

            ImGui::SetCursorPos(ImVec2(235, 300));
            ImGui::Image(Logo3, ImVec2(120, 120));//logo

            ImGui::SetCursorPos(ImVec2(15, 300));
            ImGui::Image(Logo3, ImVec2(120, 120));//logo

            ImGui::GetWindowDrawList()->AddText(LexendRegular, 21, ImVec2(p.x + 100, p.y + 60), ImColor(240, 240, 240, int(255 * fill)), "Chose your");
            ImGui::GetWindowDrawList()->AddText(LexendRegular, 21, ImVec2(p.x + 195, p.y + 60), ImColor(145, 222, 52, int(255 * fill)), "game");

            Login_game();

            static double end_time = ImGui::GetTime() + 2.5;
            if (ImGui::GetTime() <= end_time) //server time
            {
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 700, p.y + 400), ImColor(19, 20, 22)); //bg

                ImGui::SetCursorPos(ImVec2(105, 200));
                ImGui::BufferingBar("##buffer_bar", 5.7f, ImVec2(200, 5), ImColor(145, 222, 52), ImColor(145, 222, 52, int(255 * fill)));
            }
        }
    }

    void Login()
    {
        if (ImGui::GetTime() >= 2.5 || time)
        {
            if (switch_alpha[1] > 0.01f)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[1]); 
                ImGui::SetCursorPos(ImVec2(40, 100));
                ImGui::MenuChild("Login", ImVec2(240, 220), true);
                {
                    if (globals.show_login)
                    {
                        static bool remember = false;

                        ImGui::SetCursorPos(ImVec2(5, 0));
                        ImGui::TextDisabled("username");

                        ImGui::SetCursorPos(ImVec2(20, 20));
                        ImGui::InputText("##Username", globals.user_name, IM_ARRAYSIZE(globals.user_name));

                        ImGui::SetCursorPos(ImVec2(5, 80));
                        ImGui::TextDisabled("password");

                        ImGui::SetCursorPos(ImVec2(20, 100));
                        ImGui::InputText("##Passowrd", globals.pass_word, IM_ARRAYSIZE(globals.pass_word));

                        ImGui::Checkbox("remember me", &remember);

                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                        if (ImGui::Button("Login", ImVec2(200.f, 35.f)))
                        {
                            globals.show_login = false;
                            globals.swow_accept_login = true;
                        }
                        ImGui::PopStyleVar();
                    }

                }
                ImGui::EndChild();
                ImGui::PopStyleVar();
            }
        }
    }

    void Render()
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
            toggled = !toggled;

        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.5f : -1.5f)), 0.f, 1.f);
        if (open_alpha > 0.01f)
        {
            if (toggled && open_alpha > 0.8f || !toggled)
                switch_alpha[0] = ImClamp(switch_alpha[0] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);

            ImGui::SetNextWindowSize(ImVec2(700, 400));
            ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Unknown name cheat", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Decoration();

                if (switch_alpha[0] > 0.01f)
                {
                    if (toggled && switch_alpha[0] > 0.8f || !toggled)
                        switch_alpha[1] = ImClamp(switch_alpha[1] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                    if (switch_alpha[1] > 0.01f)
                    {
                        if (globals.show_login)
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[1]);
                            ImGui::SetCursorPos(ImVec2(130, 60));
                            ImGui::PushFont(LexendRegular);
                            ImGui::BeginGroup();
                            {
                                switch (m_tabs)
                                {
                                case 0: Login();
                                    break;
                                }
                            }
                            ImGui::EndGroup();
                            ImGui::PopFont();
                            ImGui::PopStyleVar();
                        }
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

int main(int, char**)
{
    //Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

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

    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bgs, sizeof(bgs), 500, 500, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bg);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logomenu, sizeof(logomenu), 500, 500, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Logo);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, avatar, sizeof(avatar), 500, 500, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Avatar);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logomenu2, sizeof(logomenu2), 500, 500, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Logo3);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, info1, sizeof(info1), 500, 500, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &info);

    // Load Fonts
    MainFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl3000, sizeof(MuseoSansCyrl3000), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
    TabsFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl5000, sizeof(MuseoSansCyrl5000), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    WeaponFont = io.Fonts->AddFontFromMemoryTTF(astriumwep, sizeof(astriumwep), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Tabsicon = io.Fonts->AddFontFromMemoryTTF(Tabsiconfont, sizeof(Tabsiconfont), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    LexendRegularTabs = io.Fonts->AddFontFromMemoryTTF(LexendRegularfont, sizeof(LexendRegularfont), 15, NULL, io.Fonts->GetGlyphRangesCyrillic());
    LexendRegular = io.Fonts->AddFontFromMemoryTTF(LexendRegularfont, sizeof(LexendRegularfont), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());
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

    {
        time = true;
        Sleep(2000);
        time = false;
        Sleep(2000);
    }

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
