// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <dinput.h>
#include <tchar.h>
#include "bgs.h"

#include "misc/fonts/fonts.h"

#include <vector>
#include "options.hpp"

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ID3D11ShaderResourceView* bgs = nullptr;

auto p = ImVec2{};
static int tab;
static float m_anim = 0.f;

float rounding = 5.f;
ImVec2 menu_size{650, 450};

void aimbot()
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_anim);

    ImGui::PushFont(opt.InterRegular);

    ImGui::SetCursorPos(ImVec2(25, 110 - 20 + 10 - (10 * m_anim)));
    ImGui::MenuChild("General", ImVec2(290, 320));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(25 + 290 + 30, 110 - 20 + 10 - (10 * m_anim)));
    ImGui::MenuChild("Main", ImVec2(290, 150));
    {

    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(25 + 290 + 30, 110 - 20 + 150 + 20 + 10 - (10 * m_anim)));
    ImGui::MenuChild("Other", ImVec2(290, 150));
    {

    }
    ImGui::EndChild();

    ImGui::PopFont();

    ImGui::PopStyleVar();
}

void render_tab()
{
    ImGui::SetCursorPos(ImVec2(50, 45));
    ImGui::BeginGroup();

    if (ImGui::render_tab("AimBot", 0, tab)) {
        if (tab != 0)
        {
            m_anim = 0.f;
        }
        tab = 0;
    }
    ImGui::SameLine(0, 115);
    if (ImGui::render_tab("Visuals", 1, tab)) {
        if (tab != 1)
        {
            m_anim = 0.f;
        }
        tab = 1;
    }
    ImGui::SameLine(0, 115);
    if (ImGui::render_tab("Misc", 2, tab)) {
        if (tab != 2)
        {
            m_anim = 0.f;
        }
        tab = 2;
    }
    ImGui::SameLine(0, 115);
    if (ImGui::render_tab("Setting", 3, tab)) {
        if (tab != 3)
        {
            m_anim = 0.f;
        }
        tab = 3;
    }

    ImGui::EndGroup();

    switch (tab)
    {
        case 0: aimbot(); break;
    }
};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUPWINDOW, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, wc.hInstance, NULL);

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

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    opt.InterRegular = io.Fonts->AddFontFromMemoryTTF(Interregular, sizeof(Interregular), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
    opt.InterRegular_small = io.Fonts->AddFontFromMemoryTTF(Interregular, sizeof(Interregular), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

    if (bgs == nullptr)
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, bg, sizeof(bg), nullptr, nullptr, &bgs, 0);

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

        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(-1, -1));
        ImGui::SetNextWindowSizeConstraints(ImVec2(w, h), ImVec2(w, h));
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::Begin("bg", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
        {
            auto Render = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetWindowPos();
            Render->AddImage(bgs, p, ImVec2(p.x + w, p.y + h));
        }
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::SetNextWindowSize(ImVec2(menu_size.x + rounding * 2.f, menu_size.y + rounding * 2.f));
        if (ImGui::Begin(("###"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
        {
            auto draw_list = ImGui::GetWindowDrawList();
            p = ImVec2(ImGui::GetWindowPos().x + rounding, ImGui::GetWindowPos().y + rounding);

            m_anim = ImLerp(m_anim, 1.f, 0.045f + ImGui::GetIO().DeltaTime * 2.f);

            draw_list->AddRectFilled(p, ImVec2(p.x + 650, p.y + 450), ImColor(17, 17, 17), rounding);

            draw_list->AddRectFilled(ImVec2(p.x + 0, p.y + 35), ImVec2(p.x + 650, p.y + 35 + 35), ImColor(21, 21, 21));
            draw_list->AddLine(ImVec2(p.x + 0, p.y + 35), ImVec2(p.x + 650, p.y + 35), ImColor(31, 31, 31), 1.5f);
            draw_list->AddLine(ImVec2(p.x + 0, p.y + 35 + 35), ImVec2(p.x + 650, p.y + 35 + 35), ImColor(31, 31, 31), 1.5f);

            draw_list->AddRectFilled(ImVec2(p.x + 0, p.y + 450 - 30), ImVec2(p.x + 650, p.y + 450), ImColor(21, 21, 21), rounding, ImDrawCornerFlags_Bot);
            draw_list->AddLine(ImVec2(p.x + 0, p.y + 450 - 30), ImVec2(p.x + 650, p.y + 450 - 30), ImColor(31, 31, 31), 1.5f);

            draw_list->AddRect(p, ImVec2(p.x + 650, p.y + 450), ImColor(31, 31, 31), rounding, ImDrawCornerFlags_All, 1.5f);

            ImGui::PushFont(opt.InterRegular);
            draw_list->AddText(ImVec2(p.x + 270, p.y + 7), ImColor(255, 255, 255), "Exodus.codes");
            ImGui::PopFont();

            ImGui::PushFont(opt.InterRegular_small);
            draw_list->AddText(ImVec2(p.x + 15, p.y + 450 - 24), ImColor(195, 195, 195), "Welcome back, byman");
            ImGui::PopFont();

            ImGui::PushFont(opt.InterRegular_small);
            draw_list->AddText(ImVec2(p.x + 590 - 2, p.y + 450 - 24), ImColor(195, 195, 195), "Lifetime");
            ImGui::PopFont();

            {
                render_tab();
            }
        }
        ImGui::End();   

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