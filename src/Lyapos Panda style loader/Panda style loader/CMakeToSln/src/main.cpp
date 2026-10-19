// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <array>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include <d3d11.h>
#include <format>
#include <ranges>
#include <tchar.h>

#include "animation/imgui_animation_manager.h"
#include "animation/widgets/BubbleCheckbox.h"
#include "widgets/CenteredText.h"
#include "animation/widgets/SmoothColorPicker.h"
#include "animation/widgets/CheatSelection.h"
#include "animation/widgets/SmoothCombo.h"
#include "animation/widgets/SmoothInputText.h"
#include "animation/widgets/SmoothColoredButton.h"
#include "widgets/ColoredText.h"
#include "dynamic_size/imgui_dynamic_size.h"
#include "experimental/notifications/imgui_notification_manager.h"
#include "icomoon_font.h"
#include "icomoon_icons.h"
#include "math/random_value.h"
#include "menu/imgui_menu.h"
#include <dwmapi.h>
#include "./dynamic_size/imgui_window_overlay.h"
#include "fonts/icomoon_font.h"
#include "fonts/Font.h"
#include "render/ImageLoader.h"
#include "experimental/directx_blur.h"
#include "dynamic_size/DynImVec2.h"
#include "dynamic_size/DynImRect.h"
#include "widgets/TextWithDescription.h"

HWND hwnd;
RECT rc;

DynImVec2 menu_size = DynImVec2(690, 450);

void move_window()
{
    if (window_move_detector)
    {
        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd,
                   rc.left + ImGui::GetMouseDragDelta().x,
                   rc.top + ImGui::GetMouseDragDelta().y,
                   menu_size.x,
                   menu_size.y,
                   TRUE);
    }
}

void RenderBlur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary(L"user32.dll");
    if (hm)
    {
        typedef BOOL (WINAPI *pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)
            GetProcAddress(
                hm,
                "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = {3, 0, 0, 0};

            WINCOMPATTRDATA data = {19, &policy, sizeof(ACCENTPOLICY)};
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}


// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);

void CleanupDeviceD3D();

void CreateRenderTarget();

void CleanupRenderTarget();

void setup_imgui_style()
{
    auto& style = ImGui::GetStyle();

    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        const auto ei = static_cast<ImGuiCol_>(i);
        if (auto& col = style.Colors[i];
            ImGuiCol_ModalWindowDimBg != ei &&
            ImGuiCol_NavWindowingDimBg != ei &&
            (col.w < 1.00F || ImGuiCol_FrameBg == ei
                || ImGuiCol_WindowBg == ei
                || ImGuiCol_ChildBg == ei))
        {
            constexpr float alpha = 0.8F;

            col.w *= alpha;
        }
    }

    style.FrameBorderSize = 0.0F;
    style.PopupBorderSize = 0.0F;
    style.WindowBorderSize = 0.0F;

    style.FrameRounding = 12.0F;
    style.ChildRounding = 10.F;
    style.GrabRounding = 3.F;
    style.PopupRounding = 3.F;
    style.ScrollbarRounding = 3.F;
    style.TabRounding = 3.F;
    style.WindowRounding = 10.F;
    style.ItemSpacing = ImVec2(20.F, 14.F);
    style.WindowPadding = ImVec2(15.F, 15.F);
    style.ChildBorderSize = 1.f;
    style.Colors[ImGuiCol_Border].w = 0.0F;
    style.ScrollbarSize = 8.f;

    style.Alpha = 1.0F;
};

char login[256];
char password[256];

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    WNDCLASSEXW wc = {
        sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
        L"ImGui Example", nullptr
    };
    ::RegisterClassExW(&wc);
    hwnd = ::CreateWindowExW(NULL, wc.lpszClassName, L"Loader", WS_POPUP,
                             (GetSystemMetrics(SM_CXSCREEN) / 2) - (menu_size.x / 2),
                             (GetSystemMetrics(SM_CYSCREEN) / 2) - (menu_size.y / 2), menu_size.x, menu_size.y, NULL,
                             NULL, wc.hInstance, NULL);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = {-1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    POINT mouse;
    rc = {0};
    GetWindowRect(hwnd, &rc);

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
    ImGuiIO& io = ImGui::GetIO();


    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.IniFilename = nullptr;

    // imgui_dynamic_size::game_logo_font = io.Fonts->AddFontFromMemoryCompressedTTF(games_logos, sizeof games_logos, 30.f, NULL);

    setup_imgui_style();
    auto* animation_mgr = imgui_animation_manager::create_instance();

    auto notification_mgr = imgui_notification_manager::create_instance(imgui_notification_manager_options{
        imgui_notif_align_left_top,
        vector2f{ImGui::GetStyle().WindowPadding} / imgui_dynamic_size::get_current_scale_factor()
    }).lock();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    static ImageLoader loading_person[3];
    static ImageLoader game_logos[3];

    static ImageLoader avatar(g_pd3dDevice, "https://i.imgur.com/SWtOgLx.png");

    static ImageLoader gradient_dark(g_pd3dDevice, "https://i.imgur.com/eSJhga8.png");
    static ImageLoader gradient_light(g_pd3dDevice, "https://i.imgur.com/2P5Zz0Q.png");

    static ImageLoader blurr(g_pd3dDevice, "https://i.imgur.com/2P5Zz0Q.png");


    static ImageLoader background_dark(g_pd3dDevice, "https://i.imgur.com/ym4xh18.png");
    static ImageLoader background_light(g_pd3dDevice, "https://i.imgur.com/lMjrOLg.png");

    static ImageLoader background_light1(g_pd3dDevice, "https://i.imgur.com/lMjrOLg.png"); 
    static ImageLoader background_dark1(g_pd3dDevice, "https://i.imgur.com/Xj3CyNA.png"); 

    static ImageLoader person(g_pd3dDevice, "https://i.imgur.com/yc2dq4F.png");
    static ImageLoader logotype(g_pd3dDevice, "https://i.imgur.com/LQG9Pq4.png");


    game_logos[0] = ImageLoader(g_pd3dDevice, "https://i.imgur.com/R13WqZ2.png");
    game_logos[1] = ImageLoader(g_pd3dDevice, "https://i.imgur.com/Ge97m4v.png");
    game_logos[2] = ImageLoader(g_pd3dDevice, "https://i.imgur.com/BzZI1oq.png");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    auto p_menu = plugin_systems::imgui_menu::create_instance(imgui_menu_flags_no_list).lock();

    static float blur_alpha = 0.f;

    enum menu_tabs
    {
        menu_tabs_login = 0,
        menu_tabs_loading,
        menu_tabs_selection,
        menu_tabs_injection
    };

    static std::unordered_map<menu_tabs, size_t> menu_tab_indexes{};
    static int current_game = 0;

    menu_tab_indexes.emplace(
        menu_tabs_login,
        p_menu->emplace(
            "1 Login page",
            "бе",
            [&p_menu]()
            {
                ImGui::GetWindowDrawList()->AddImage(bTheme ? background_light1.image() : background_dark1.image(),
                                                     ImGui::GetWindowPos() + DynImVec2(30, 0),
                                                     ImGui::GetWindowPos() + ImGui::GetWindowSize());

                ImGui::SetCursorPos(DynImVec2(25, 25));
                ImGui::BeginChild("Autorization", ImVec2(295, ImGui::GetContentRegionAvail().y));

                static bool remember_me = false;

                ImGui::PushFont(imgui_dynamic_size::bold_fonts[4]);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - DynImVec2(2.5f, 0).x);
                ImGui::Image(logotype.image(), DynImVec2(35, 35));

                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, DynImVec2(0, 0));
                ImGui::BeginGroup();
                ImGui::Text("Nemesis");
                ImGui::PopFont();

                ImGui::PushFont(imgui_dynamic_size::bold_fonts[3]);

                ImGui::TextColored(ImGui::GetStyle().Colors[ImGuiCol_TextDescription], "Realese");
                ImGui::PopStyleVar();
                ImGui::EndGroup();
                

           
                
                ImGui::Text("" );
                

                ImGui::SmoothInputText("Username", NULL, login, 128, DynImVec2(295, 45), 0);

                ImGui::SmoothInputText("Password", NULL, password, 128, DynImVec2(295, 45),
                                       ImGuiInputTextFlags_Password);

                ImGui::Dummy(DynImVec2(0, 1));

                if (ImGui::Button("Continue" ICON_ARROWS_RIGHT_LINE, DynImVec2(295, 44)))
                {
                    p_menu->set_current_tab(menu_tab_indexes.at(menu_tabs_loading));

                    auto notification_mgr = imgui_notification_manager::get_instance().lock();

                    notification_mgr->add({
                        .title_color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive],
                        .title_text = ICON_APPROVED_1 " Notification",
                        .description_text = std::format("Succesfull autorization " ICON_G19238, login),
                        .alive_millis = random_value::generate<size_t>(1500, 3000)
                    });
                }

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
                ImGui::CenteredText("Forgot Email/Password?");
                ImGui::PopStyleColor();

                ImGui::EndChild();
                ImGui::PopFont();

                ImGui::GetWindowDrawList()->AddImage(person.image(), ImGui::GetWindowPos() + DynImVec2(300, 90),
                                                     ImGui::GetWindowPos() + DynImVec2(380, 25) + DynImVec2(280, 425));
            }));
    menu_tab_indexes.emplace(
        menu_tabs_loading,
        p_menu->emplace(
            "2 Loading page",
            "бе",
            [&p_menu]() {

                ImGui::GetWindowDrawList()->AddImage(bTheme ? background_light1.image() : background_dark1.image(),
                    ImGui::GetWindowPos() + DynImVec2(-250, 0),
                    ImGui::GetWindowPos() + ImGui::GetWindowSize());

                static float timer = 0.0f;  // Timer para controlar a mudança de tema
                static float fCircleRadius = 10.0f;  // Raio inicial do círculo
                static bool changingTheme = false;  // Inicia não mudando de tema
                static bool themeActive = false;  // Para verificar se o tema claro está ativo

                // Atualiza o timer
                timer += ImGui::GetIO().DeltaTime;

                // Inicia a mudança de tema após 1 segundo
                if (timer >= 1.0f) {
                    changingTheme = true;  // Começa a mudar para o tema claro
                }

                // Animação do círculo
                if (changingTheme) {
                    fCircleRadius = ImLerp(fCircleRadius, 850.f, ImGui::GetIO().DeltaTime * 4.f); // Reduzido de 6.f para 3.f

                    // Verifica se o círculo atingiu o tamanho máximo
                    if (fCircleRadius >= 830.f) {
                        bTheme = true;  // Muda para tema claro
                        themeActive = true;  // Indica que o tema claro está ativo

                        // Permite que o tema claro fique ativo por 8 segundos
                        if (timer >= 4.0f + 1.0f) {  // Ajustando para o tempo total
                            bTheme = false;  // Retorna para tema escuro
                            changingTheme = false;  // Reseta a animação
                            fCircleRadius = 0;  // Reseta o raio
                            timer = 0.0f;  // Reseta o timer
                            // Muda para a aba de seleção
                            p_menu->set_current_tab(menu_tab_indexes.at(menu_tabs_selection));
                        }
                    }
                }
                else {
                    fCircleRadius = ImLerp(fCircleRadius, 0.f, ImGui::GetIO().DeltaTime * 3.f); // Reduzido de 6.f para 3.f
                }

                // Desenha o conteúdo da página de loading
                ImGui::Text("Loading...");

                // Desenha o círculo animado na cor desejada (65, 78, 154)
                ImVec2 circlePos = ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowSize().x, 0.f);
                ImGui::GetForegroundDrawList()->AddCircleFilled(
                    circlePos,
                    fCircleRadius,
                    ImGui::GetColorU32(ImVec4(65 / 255.0f, 78 / 255.0f, 154 / 255.0f, 1.0f)), // Círculo na cor desejada
                    360.f
                );

                // Desenha o texto "Bem-vindo" acima do círculo
                ImGui::SetCursorPos(circlePos + ImVec2(-50, -20)); // Ajuste a posição conforme necessário
                ImGui::Text("Bem-vindo");
            } // Fechamento do lambda
        ) // Fechamento do emplace
    ); // Fechamento do emplace







    menu_tab_indexes.emplace(
        menu_tabs_selection,
        p_menu->emplace(
            "3 Сheats page",
            "бе",
            [&p_menu]()
            {
                ImGui::GetWindowDrawList()->AddImage(bTheme ? background_light1.image() : background_dark1.image(),
                    ImGui::GetWindowPos() + DynImVec2(-250, 0),
                    ImGui::GetWindowPos() + ImGui::GetWindowSize());

                ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(),
                                                          ImGui::GetWindowPos() + ImVec2(
                                                              ImGui::GetWindowSize().x, DynImVec2(80, 0.f).x),
                                                          ImGui::GetColorU32(ImGuiCol_TableHeaderBg),
                                                          ImGui::GetStyle().WindowRounding,
                                                          ImDrawFlags_RoundCornersTop); // header

                ImGui::SetCursorPos(DynImVec2(25, 25));
                ImGui::PushFont(imgui_dynamic_size::bold_fonts[4]);
                ImGui::Text(ICON_LOGO); ImGui::SameLine();
                ImGui::PopFont();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + DynImVec2(2.5f, 0).x);
                ImGui::PushFont(imgui_dynamic_size::bold_fonts[2]);
                ImGui::Text(ICON_VECTOR); ImGui::SameLine();
                ImGui::PopFont();
                ImGui::PushFont(imgui_dynamic_size::bold_fonts[3]);
                ImGui::Text(ICON_TG); ImGui::SameLine();
                ImGui::PopFont();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - DynImVec2(2.5f, 0).x);
                ImGui::Image(avatar.image(), DynImVec2(35, 35));

                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, DynImVec2(0, 0));
                ImGui::BeginGroup();
                ImGui::Text("gabriel");
                ImGui::TextColored(ImGui::GetStyle().Colors[ImGuiCol_TextDescription], "We are glad to see you again");
                ImGui::PopStyleVar();
                ImGui::EndGroup();

                static bool injection = false;

                static int current_status = 0;

                const char* succes_status_array[] = {
                    "[+] Starting Injection...",
                    "[+] Stopping Process...\nAttempting to stop Process",
                    "[+] Process stopped",
                    "[+] Killing Process...",
                    "[+] Attempting to kill Process",
                    "[+] Process Killed",
                    "[+] Deleting shims...",
                    "[+] Removing C:\\Users\\SystemX\\AppData\\Local\\Discord\\app-1.0.9152\\modules\\discord_desktop_core-1\\discord_desktop_core\\index.js",                 
                };

                const char* wrong_status_array[] = {
                    "Starting Injection...",
                    "Stopping Process...\nAttempting to stop Process",
                    ICON_G19238 "Process stopped",
                    "Killing Discord...",
                    "Attempting to kill Process",
                    ICON_G19258 "Failed to kill Process",
                    "Deleting shims...",
                    ICON_G19238 "Shims deleted",
                    ICON_G19258 "Failed to delete shims",
                    "Disabling all plugins...",
                    ICON_G19238 "Deleted plugins.json",
                    ICON_G19258 "Failed to disable plugins",
                    ICON_G19238 "Repair completed!",
                    "Reconnecting to server...",
                    ICON_G19258 "Failed to reconnect to server",
                    ICON_G18110 "Updating configuration files...",
                    ICON_G19238 "Configuration files updated",
                    "Cleaning temporary files...",
                    ICON_G19258 "Failed to clean temporary files",
                    "Finalizing setup...",
                    ICON_G19238 "Setup finalized",
                    "Restarting services...",
                    ICON_G19258 "Failed to restart services",
                    ICON_G18110 "Running diagnostics...",
                    ICON_G19258 "Injection failed\n "
                    " "
                };


                ImGui::SetCursorPos(DynImVec2(25, 105));
                ImGui::BeginChild("Games", DynImVec2(644, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_None,
                                  ImGuiWindowFlags_NoBackground);
                {
                    if (ImGui::CheatSelection("Counter Strike 2\nUndetected", "Subscription till:\n29.06.24",
                                              "Last update:\n2 weeks ago", game_logos[0].image(),
                                              current_game == 0)) { current_game = 0; ImGui::OpenPopup("Injection"); }

                    if (ImGui::CheatSelection("Valorant", "Subscription till:\n29.06.24", "Last update:\n2 weeks ago",
                    game_logos[1].image(), current_game == 1)) { current_game = 1; ImGui::OpenPopup("Injection"); }

                    if (ImGui::CheatSelection("Escape From Tarkov", "Subscription till:\n29.06.24",
                                              "Last update:\n2 weeks ago", game_logos[2].image(), current_game == 2)) { current_game = 2; ImGui::OpenPopup("Injection"); }

                    ImGui::GetWindowDrawList()->AddImage(bTheme ? gradient_light.image() : gradient_dark.image(),
                                                         ImGui::GetWindowPos() + ImVec2(0, 355),
                                                         ImGui::GetWindowPos() + ImGui::GetWindowSize());

                    std::cout << current_game << std::endl;

                    ImGui::SetNextWindowSize(DynImVec2(420, 385), ImGuiCond_Always);
                    ImGui::SetNextWindowPos(ImGui::GetCurrentWindow()->Rect().GetCenter() - DynImVec2(410, 480) / 2);
                    if (ImGui::BeginPopupModal("Injection", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
                    {
                        ImGui::Text(ICON_G16878 "Injection...");
                        if(current_game == 2)
                        {
                            bool is_finished = current_status + 1 < IM_ARRAYSIZE(wrong_status_array);

                            ImGui::BeginChild("Inject", DynImVec2(390, 265));
                            {
                                static DWORD dwTickStart = GetTickCount();
                                if (GetTickCount() - dwTickStart > random_value::generate<size_t>(150, 3000))
                                {
                                    if (is_finished)
                                        current_status++;
                                    dwTickStart = GetTickCount();
                                }

                                for (int i = 0; i < IM_ARRAYSIZE(wrong_status_array); i++)
                                {
                                    if (current_status >= i)
                                        ImGui::Text(wrong_status_array[i]);
                                }

                                if(is_finished)
                                    ImGui::SetScrollY(ImGui::GetScrollMaxY());
                                ImGui::EndChild();
                            }

                            if(is_finished) ImGui::BeginDisabled();
                            if(ImGui::Button("Close", DynImVec2(390, 45)))
                            {
                                current_status = 0;
                                is_finished = false;
                                ImGui::CloseCurrentPopup();
                            }
                            if(is_finished) ImGui::EndDisabled();
                        }
                        else
                        {
                            bool is_finished = current_status + 1 < IM_ARRAYSIZE(succes_status_array);

                            ImGui::BeginChild("Inject", DynImVec2(390, 265));
                            {
                                static DWORD dwTickStart = GetTickCount();
                                if (GetTickCount() - dwTickStart > random_value::generate<size_t>(150, 3000))
                                {
                                    if (is_finished)
                                        current_status++;
                                    dwTickStart = GetTickCount();
                                }

                                for (int i = 0; i < IM_ARRAYSIZE(succes_status_array); i++)
                                {
                                    if (current_status >= i)
                                        ImGui::Text(succes_status_array[i]);
                                }

                                if(is_finished)
                                    ImGui::SetScrollY(ImGui::GetScrollMaxY());
                                ImGui::EndChild();
                            }

                            if(is_finished) ImGui::BeginDisabled();
                            if(ImGui::Button("Close", DynImVec2(390, 45)))
                            {
                                current_status = 0;
                                is_finished = false;
                                ImGui::CloseCurrentPopup();
                            }
                            if(is_finished) ImGui::EndDisabled();
                        }
                        ImGui::EndPopup();
                    }
                }
                ImGui::EndChild();
            }));
    menu_tab_indexes.emplace(
        menu_tabs_injection,
        p_menu->emplace(
            "3 Сheats page",
            "бе",
            [&p_menu]()
            {
            }));


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

        imgui_dynamic_size::scale_imgui();
        animation_mgr->pre_render();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (bTheme)
            ImGui::StyleColorsLight();
        else
            ImGui::StyleColorsDark();

        p_menu->render(logotype.image());

        // RenderBlur(hwnd);
        SetWindowRgn(hwnd, CreateRoundRectRgn(0, 0, menu_size.x, menu_size.y, 30, 30), TRUE);
        move_window();

        notification_mgr->render();

        ImGui::Render();

        const float clear_color_with_alpha[4] = {0};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
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

bool
CreateDeviceD3D(HWND hWnd)
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
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0,};
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr,
                                                D3D_DRIVER_TYPE_HARDWARE,
                                                nullptr,
                                                createDeviceFlags,
                                                featureLevelArray,
                                                2,
                                                D3D11_SDK_VERSION,
                                                &sd,
                                                &g_pSwapChain,
                                                &g_pd3dDevice,
                                                &featureLevel,
                                                &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr,
                                            D3D_DRIVER_TYPE_WARP,
                                            nullptr,
                                            createDeviceFlags,
                                            featureLevelArray,
                                            2,
                                            D3D11_SDK_VERSION,
                                            &sd,
                                            &g_pSwapChain,
                                            &g_pd3dDevice,
                                            &featureLevel,
                                            &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void
CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_pd3dDeviceContext)
    {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void
CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void
CleanupRenderTarget()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                               UINT msg,
                               WPARAM wParam,
                               LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd,
                       UINT msg,
                       WPARAM wParam,
                       LPARAM lParam)
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
    case WM_DESTROY: ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
