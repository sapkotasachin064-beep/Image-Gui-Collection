#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#define DIRECTINPUT_VERSION 0x0800
#define LOADERW 670
#define LOADERH 500
#include <dinput.h>
#include <tchar.h>
#include <dwmapi.h>

#include "Custom/ImguiElements.h"

#pragma comment(lib, "dwmapi.lib")
static bool test;
static int reaction = 1;
static int curHitbox = 1;
const char* hitboxes[]{ "Head", "Body" };
void RenderLegitBotMain() {
    ImGui::SetCursorPos(ImVec2(19, 130));
    LamantineSDK::Child("General", ImVec2(300, 110));
    ImGui::SetCursorPos(ImVec2(25, 145));
    ImGui::BeginGroup(); {
        ImGui::Checkbox("Enabled", &test);
        ImGui::SetCursorPosY(185);
        ImGui::SliderInt("Reaction Time", &reaction, 1, 10);
        ImGui::SetCursorPosY(225);
        ImGui::Combo("Hitboxes", &curHitbox, hitboxes, IM_ARRAYSIZE(hitboxes)); //fix popups by urself
    }
    ImGui::EndGroup();
}

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - LOADERW / 2; // cetner screen x
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - LOADERH / 2; // center screen y
    HWND hwnd = ::CreateWindowExW(WS_EX_LAYERED, wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_POPUP, x, y, LOADERW, LOADERH, NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

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
    
    LamantineSDK::InitFonts();
    LamantineSDK::InitStyles();
    LamantineSDK::ColorStyles();
    
 

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

   
    static bool opened = true;


    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

       
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(LOADERW, LOADERH));
            ImGui::Begin("LamantineSDK", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar); // Create a window called "Hello, world!" and append into it.
            //Drawing shadow under titlebar
            ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(), ImVec2(670, 20), ImColor(255, 205, 126), ImColor(255, 205, 126), ImColor(255, 205, 126, 0), ImColor(255, 205, 126, 0));
            //Drawing orange titlebar
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(), ImVec2(670, 10), ImColor(255, 205, 126));
            //Drawing subtabs backround
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(19, 75), ImVec2(635 + 19, 45 + 75), ImColor(1, 1, 1, 75), 2);
            //Drawing inner stroke
            ImGui::GetWindowDrawList()->AddRect(ImVec2(19, 75), ImVec2(635 + 19, 45 + 75), ImColor(255, 255, 255, 75), 2);
            ImGui::PushFont(Logo);
            ImGui::SetCursorPos(ImVec2(27, 16));
            ImGui::Text("otc3"); //Draws logo
            //Drawing line left logo
            ImGui::GetWindowDrawList()->AddLine(ImVec2(115, 18), ImVec2(115, 18 + 35), ImColor(255, 255, 255, 50), 1);
            //Drawing line under logo
            ImGui::GetWindowDrawList()->AddLine(ImVec2(19, 60), ImVec2(19 + 635, 60), ImColor(255, 255, 255, 50), 1);
            //Drawing underline
            ImGui::GetWindowDrawList()->AddLine(ImVec2(19, 471), ImVec2(19 + 635, 471), ImColor(255, 255, 255, 50), 1);
            ImGui::PushFont(Elements);
            ImGui::SetCursorPos(ImVec2(20, 475));
            ImGui::Text("xLamantine"); //Draws logo
            ImGui::PushFont(Tabs);
            ImGui::SetCursorPos(ImVec2(134, 25));
            ImGui::BeginGroup(); {
                LamantineSDK::Tab("Legit", 1);
                ImGui::SameLine();
                LamantineSDK::Tab("Rage", 2);
                ImGui::SameLine();
                LamantineSDK::Tab(u8"Anti-Aim", 3);
                ImGui::SameLine();
                LamantineSDK::Tab("Visual", 4);
                ImGui::SameLine();
                LamantineSDK::Tab("Misc", 5);
            }
            switch (active_tab) {
            case 1:
                ImGui::SetCursorPos(ImVec2(71, 100));
                ImGui::BeginGroup(); {
                    LamantineSDK::SubTab("GENERAL", "1", 1); //super shitcode by xLamantine cuz shit font dont want to work correctly
                    ImGui::SetCursorPos(ImVec2(71 + 120, 100));
                    LamantineSDK::SubTab("PISTOL", "i", 2);
                    ImGui::SetCursorPos(ImVec2(71 + 120 * 2, 100));
                    LamantineSDK::SubTab("RIFLE", "q", 3);
                    ImGui::SetCursorPos(ImVec2(71 + 120 * 3, 100));
                    LamantineSDK::SubTab("SNIPER", "e", 4);
                    ImGui::SetCursorPos(ImVec2(71 + 120 * 4, 100));
                    LamantineSDK::SubTab("SMG", "t", 5);
                }
                ImGui::EndGroup();
                break;
            }
            switch (active_subtab) {
            case 1:
                RenderLegitBotMain();
                break;
            }
            ImGui::End();
        }

        // 2. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //ImGui::ShowDemoWindow();

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
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

        if (!opened) msg.message = WM_QUIT;
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

POINTS m_Pos; // store user click pos

// Win32 message handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        m_Pos = MAKEPOINTS(lParam); // set click points
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (wParam == MK_LBUTTON)
        {
            POINTS p = MAKEPOINTS(lParam); // get cur mousemove click points
            RECT rect;
            GetWindowRect(hWnd, &rect);
            rect.left += p.x - m_Pos.x; // get xDelta
            rect.top += p.y - m_Pos.y;  // get yDelta
            if (m_Pos.x >= 0 && m_Pos.x <= LOADERW - 20 /* cuz 20px - close btn */ && m_Pos.y >= 0 && m_Pos.y <= ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f)
                SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;
    }
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
