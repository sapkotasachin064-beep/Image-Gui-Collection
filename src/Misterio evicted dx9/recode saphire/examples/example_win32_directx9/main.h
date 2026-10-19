#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <Windows.h>
#include <dwmapi.h>
#include "blur.hpp"
#include "blur_x.h"
#include "blur_y.h"

#include "background.h"

#include "ubunutu_regular.h"
#include "ubuntu_medium.h"
#include "ubuntu_bold.h"
#include "cruin.h"
#include "icon_font.h"

#include "colors.h"
#include "logo.h"
#include "images.h"

#pragma comment(lib, "Dwmapi.lib")

#define WIDTH 790
#define HEIGHT 578

HWND main_hwnd = nullptr;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool check_0 = true;
 bool check_1 = false;
int slider_one, slider_two = 50;
float slider_three = 0.0f;
char input[64] = { "" };
const char* items[]{ "Value", "Random" };
float color_edit[4] = { 64 / 255.f, 77 / 255.f, 236 / 255.f, 190 / 255.f };
int selectedItem = 0;
float alpha_line = 0.0f;
const char* items1[4]{ "Uno", "Dos", "Tres", "Quatro" };
int selectedItem1 = 0;

bool esp_preview;
float preview_alpha;

const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };
bool multi_items_count[5];

int min0 = 1, max0 = 50;
int ilow0 = 1, ihigh0 = 50;

float min1 = 0.0f, max1 = 5.0f;
float ilow1 = 0.0f, ihigh1 = 5.0f;

float min2 = 0.0f, max2 = 105.0f;
float ilow2 = 0.0f, ihigh2 = 105.0f;

IDirect3DTexture9* bg = nullptr;
IDirect3DTexture9* lg = nullptr;
IDirect3DTexture9* settings_img = nullptr;
IDirect3DTexture9* keyboard_img = nullptr;
IDirect3DTexture9* person_img = nullptr;
IDirect3DTexture9* visual_img = nullptr;
IDirect3DTexture9* rifle_img = nullptr;

ImFont* ico_0;
ImFont* ico_1;
ImFont* cruin_0;
ImFont* ubu_1;
ImFont* ubu_0;
ImFont* ubu_2;
ImFont* ubu_preview;
ImFont* ubu_child;
ImFont* weapon_font;
float tab_alpha = 0.f, subtab_alpha = 0.f;
float keybind_alpha = 0.0f;
float tab_add, subtab_add;
int active_tab = 0, active_subtab = 0;

int tabs = 0, sub_tabs = 0;

float child_sliding = 40.f;
int key0;

void CustomStyleColor() // Отрисовка цветов
{
    ImGuiStyle& s = ImGui::GetStyle();

    s.Colors[ImGuiCol_WindowBg] = ImColor(165, 175, 185, 100);
    s.Colors[ImGuiCol_ChildBg] = ImColor(22, 21, 26, 255);
    s.Colors[ImGuiCol_PopupBg] = ImColor(17, 16, 21, 255);
    s.Colors[ImGuiCol_TextDisabled] = ImColor(66, 65, 70, 255);
    s.Colors[ImGuiCol_Border] = ImColor(14, 13, 19, 0);
    s.Colors[ImGuiCol_TextSelectedBg] = ImColor(35, 32, 43, 160);
    s.WindowBorderSize = 0;
    s.WindowPadding = ImVec2(0, 0);
    s.ChildRounding = 7;
    s.PopupRounding = 5;
    s.PopupBorderSize = 0;
    s.WindowRounding = 8.f;
    s.FrameBorderSize = 0.0f;
    s.FrameRounding = 4.f;
    s.ScrollbarSize = 2.f;
    s.FramePadding = ImVec2(6, 3);
    s.ItemInnerSpacing = ImVec2(10, 0);
    s.ItemSpacing = ImVec2(0, 10);

}

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
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
