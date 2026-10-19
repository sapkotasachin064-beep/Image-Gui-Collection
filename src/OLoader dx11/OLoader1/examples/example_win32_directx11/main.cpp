#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "main.h"
#include "Image.h"
#include "Font.h"
#include <string>
#include <codecvt>
#include <iomanip>
#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace image
{
    ID3D11ShaderResourceView* heart = nullptr;
    ID3D11ShaderResourceView* update = nullptr;
    ID3D11ShaderResourceView* docs = nullptr;
    ID3D11ShaderResourceView* save = nullptr;
    ID3D11ShaderResourceView* user = nullptr;
    ID3D11ShaderResourceView* password = nullptr;
    ID3D11ShaderResourceView* info = nullptr;
    ID3D11ShaderResourceView* key = nullptr;
    ID3D11ShaderResourceView* game = nullptr;
    ID3D11ShaderResourceView* settings = nullptr;
    ID3D11ShaderResourceView* csgo = nullptr;
    ID3D11ShaderResourceView* csgo2 = nullptr;
    ID3D11ShaderResourceView* minecraft = nullptr;
    ID3D11ShaderResourceView* rust = nullptr;
    ID3D11ShaderResourceView* gta = nullptr;
    ID3D11ShaderResourceView* apex = nullptr;
    ID3D11ShaderResourceView* garrys = nullptr;
    ID3D11ShaderResourceView* tf2 = nullptr;
    ID3D11ShaderResourceView* lethal = nullptr;
    ID3D11ShaderResourceView* back = nullptr;
    ID3D11ShaderResourceView* check = nullptr;
    ID3D11ShaderResourceView* exit = nullptr;
    ID3D11ShaderResourceView* arrows = nullptr;
}

D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

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

    Lexend_Medium_1 = io.Fonts->AddFontFromMemoryTTF(&Lexend_M, sizeof Lexend_M, 17.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Medium = io.Fonts->AddFontFromMemoryTTF(&Lexend_M, sizeof Lexend_M, 22.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Bold = io.Fonts->AddFontFromMemoryTTF(&Lexend_B, sizeof Lexend_B, 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Bold_1 = io.Fonts->AddFontFromMemoryTTF(&Lexend_B, sizeof Lexend_B, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Regular = io.Fonts->AddFontFromMemoryTTF(&Lexend_R, sizeof Lexend_R, 15.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Regular_2 = io.Fonts->AddFontFromMemoryTTF(&Lexend_R, sizeof Lexend_R, 13.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Regular_1 = io.Fonts->AddFontFromMemoryTTF(&Lexend_R, sizeof Lexend_R, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Semibold = io.Fonts->AddFontFromMemoryTTF(&Lexend_S, sizeof Lexend_S, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Semibold_1 = io.Fonts->AddFontFromMemoryTTF(&Lexend_S, sizeof Lexend_S, 15.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Semibold_2 = io.Fonts->AddFontFromMemoryTTF(&Lexend_S, sizeof Lexend_S, 12.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Lexend_Medium_2 = io.Fonts->AddFontFromMemoryTTF(&Lexend_M, sizeof Lexend_M, 13.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    if (image::heart == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Heart, sizeof(I_Heart), &info, pump, &image::heart, 0);

    if (image::update == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Update, sizeof(I_Update), &info, pump, &image::update, 0);

    if (image::docs == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Docs, sizeof(I_Docs), &info, pump, &image::docs, 0);

    if (image::save == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Save, sizeof(I_Save), &info, pump, &image::save, 0);

    if (image::user == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_User, sizeof(I_User), &info, pump, &image::user, 0);

    if (image::password == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Password, sizeof(I_Password), &info, pump, &image::password, 0);

    if (image::info == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Info, sizeof(I_Info), &info, pump, &image::info, 0);

    if (image::key == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Key, sizeof(I_Key), &info, pump, &image::key, 0);

    if (image::game == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Game, sizeof(I_Game), &info, pump, &image::game, 0);

    if (image::settings == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_Settings, sizeof(I_Settings), &info, pump, &image::settings, 0);

    if (image::csgo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, CSGO, sizeof(CSGO), &info, pump, &image::csgo, 0);

    if (image::csgo2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, CSGO2, sizeof(CSGO2), &info, pump, &image::csgo2, 0);

    if (image::minecraft == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, MINECRAFT, sizeof(MINECRAFT), &info, pump, &image::minecraft, 0);

    if (image::rust == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, RUST, sizeof(RUST), &info, pump, &image::rust, 0);

    if (image::gta == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, GTA, sizeof(GTA), &info, pump, &image::gta, 0);

    if (image::apex == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, APEX, sizeof(APEX), &info, pump, &image::apex, 0);

    if (image::garrys == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, GARRYS, sizeof(GARRYS), &info, pump, &image::garrys, 0);

    if (image::tf2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, TF2, sizeof(TF2), &info, pump, &image::tf2, 0);

    if (image::lethal == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, LETHAL, sizeof(LETHAL), &info, pump, &image::lethal, 0);

    if (image::back == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, BACK, sizeof(BACK), &info, pump, &image::back, 0);

    if (image::check == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, check_icon, sizeof(check_icon), &info, pump, &image::check, 0);

    if (image::exit == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, exit_icon, sizeof(exit_icon), &info, pump, &image::exit, 0);

    if (image::arrows == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, arrows_icon, sizeof(arrows_icon), &info, pump, &image::arrows, 0);

    if (logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, I_logo, sizeof(I_logo), &info, pump, &logo, 0);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
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

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;
            CustomStyleColor();

            ImGui::GetStyle().ItemSpacing = ImVec2(0, 10);

            if (bg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, BackGround, sizeof(BackGround), &info, pump, &bg, 0);
            ImGui::GetBackgroundDrawList()->AddImage(bg, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

            ImGui::SetNextWindowSize(ImVec2(600, 412));
            ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {               
                static float alpha = 0.f;
                static float alpha1 = 0.f;
                auto draw = ImGui::GetWindowDrawList();
                const auto& p = ImGui::GetWindowPos();
                const ImVec2& region = ImGui::GetContentRegionMax();

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + region.x, p.y + 50), c::background, 12.f, ImDrawFlags_RoundCornersTop);

                if (Logotip == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Logo, sizeof(Logo), &info, pump, &Logotip, 0); ImGui::GetWindowDrawList()->AddImageRounded(Logotip, ImVec2(p.x + 14, p.y + 14), ImVec2(p.x + 36, p.y + 36), ImVec2(0, 0), ImVec2(1, 1), c::main_color, 0);

                ImGui::GetWindowDrawList()->AddText(Lexend_Medium, 22.f, ImVec2(p.x + 45, p.y + 14), c::text_activite, "OLoader");  ImGui::GetWindowDrawList()->AddText(Lexend_Regular, 15.f, ImVec2(p.x + 128, p.y + 19), c::text_version, "[v1.0]");

                ImGui::SetCursorPos(ImVec2(568, 18));
                if (ImGui::ButtonIco("exit", image::exit, ImVec2(14, 14)));

                static int page = 0;

                static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                tab_alpha = ImClamp(tab_alpha + (5.f * ImGui::GetIO().DeltaTime * (page == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (page == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                if (tab_alpha == 0.f && tab_add == 0.f) active_tab = page;

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style.Alpha);
                if (active_tab == 0)
                {
                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 300, p.y + 75), ImVec2(p.x + 301, p.y + 387), c::background);

                    ImGui::SetCursorPos(ImVec2(10, 60));
                    if (ImGui::Switch("Sing In", 0 == page_switch, ImVec2(140, 40))) page_switch = 0;
                    ImGui::SetCursorPos(ImVec2(145, 60));
                    if (ImGui::Switch("Sing Up", 1 == page_switch, ImVec2(140, 40))) page_switch = 1;

                    if (page_switch == 0)
                    {
                        ImGui::GetWindowDrawList()->AddText(Lexend_Medium_1, 17.f, ImVec2(p.x + 105, p.y + 116), c::text_autch, "Authorization");
                       
                        ImGui::SetCursorPos(ImVec2(10, 148));
                        ImGui::BeginGroup();
                        {
                            ImGui::InputTextWithHint("Login:", image::user, "Login", input, 64);

                            ImGui::InputTextWithHint("Password:", image::password, "Password", input1, 64);

                            ImGui::InputTextWithHint("Secret:", image::info, "Secret Phrase", input2, 64);

                        }ImGui::EndGroup();

                        ImGui::SetCursorPos(ImVec2(10, 309));
                        if (ImGui::Button("LAUNCH", ImVec2(276, 45))) page = 1;

                        ImGui::GetWindowDrawList()->AddText(Lexend_Medium_1, 17.f, ImVec2(p.x + 55, p.y + 370), c::text_autch, "Forgot Password?");
                        ImGui::SetCursorPos(ImVec2(155, 371));
                        if (ImGui::ButtonText("Restore", ImVec2(100, 15)));

                    }
                    if (page_switch == 1)
                    {
                        ImGui::GetWindowDrawList()->AddText(Lexend_Medium_1, 17.f, ImVec2(p.x + 109, p.y + 116), c::text_autch, "Registration");

                        ImGui::SetCursorPos(ImVec2(10, 148));
                        ImGui::BeginGroup();
                        {
                            ImGui::InputTextWithHint("Login1:", image::user, "Login", input3, 64);

                            ImGui::InputTextWithHint("Password1:", image::password, "Password", input4, 64);

                            ImGui::InputTextWithHint("Invite:", image::key, "Invite Key", input5, 64);

                        }ImGui::EndGroup();

                        ImGui::SetCursorPos(ImVec2(10, 309));
                        if (ImGui::Button("LAUNCH", ImVec2(276, 45))) page_switch = 0;
                        ImGui::GetWindowDrawList()->AddText(Lexend_Medium_1, 17.f, ImVec2(p.x + 55, p.y + 370), c::text_autch, "Don't have a key?");
                        ImGui::SetCursorPos(ImVec2(155, 371));
                        if (ImGui::ButtonText("Buy one", ImVec2(100, 15)));
                    }
                    ImGui::SetCursorPos(ImVec2(310, 60));
                    ImGui::BeginGroup();
                    {
                        ImGui::News("High quality", "Are you looking for the best quality?", "Then you have come to the right place!", image::heart, c::Image_heart);

                        ImGui::News("Frequent updates", "Our product updates occur every week", "on Saturday 00:00", image::update, c::Image_update);

                        ImGui::News("Number of games", "Here you will find cheats for even the", "most unpopular games", image::docs, c::Image_docs);

                        ImGui::News("Best defense", "The product has the best protection", "among competitors", image::save, c::Image_save);

                    }ImGui::EndGroup();

                }
                if (active_tab == 1)
                {
                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 200, p.y + 75), ImVec2(p.x + 201, p.y + 387), c::background);

                    static int page1 = 0;

                    ImGui::SetCursorPos(ImVec2(10, 60));
                    ImGui::BeginGroup();
                    {
                        if (ImGui::Tab("Games", image::game, 0 == page1, ImVec2(180, 40))) page1 = 0;

                        if (ImGui::Tab("Settings", image::settings, 1 == page1, ImVec2(180, 40))) page1 = 1;
                    }

                    ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 10, p.y + 361), ImVec2(p.x + 191, p.y + 403), c::background, 8.f);
                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 11, p.y + 362), ImVec2(p.x + 190, p.y + 402), c::input, 8.f);

                    if (Username == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Uset_Logo, sizeof(Uset_Logo), &info, pump, &Username, 0); ImGui::GetWindowDrawList()->AddImageRounded(Username, ImVec2(p.x + 14, p.y + 366), ImVec2(p.x + 46, p.y + 398), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255), 100.f);

                    ImGui::GetWindowDrawList()->AddText(Lexend_Medium_1, 17.f, ImVec2(p.x + 50, p.y + 367), c::text_activite, "Ostin");

                    ImGui::GetWindowDrawList()->AddText(Lexend_Regular, 15.f, ImVec2(p.x + 50, p.y + 383), c::text_grey, "Til::"); ImGui::GetWindowDrawList()->AddText(Lexend_Regular, 15.f, ImVec2(p.x + 70, p.y + 383), c::text_red, "Lifetime");

                    ImGui::EndGroup();

                    if (page1 == 0)
                    {
                        static int game = 0;
                        if (game == 0)
                        {
                            ImGui::SetCursorPos(ImVec2(215, 60));
                            ImGui::BeginChild("Generals", ImVec2(380, 345));
                            {
                                ImGui::SetCursorPos(ImVec2(2, 0));
                                ImGui::BeginGroup();
                                {
                                    if (ImGui::Game_Select("CS2", image::csgo, ImVec2(119, 119))) game = 1;
                                    ImGui::SameLine(0, 10);
                                    if (ImGui::Game_Select("CS:GO", image::csgo2, ImVec2(119, 119))) game = 2;
                                    ImGui::SameLine(0, 10);
                                    if (ImGui::Game_Select("Minecraft", image::minecraft, ImVec2(119, 119))) game = 3;

                                    //----\\

                                    if (ImGui::Game_Select("RUST", image::rust, ImVec2(119, 119))) game = 4;
                                    ImGui::SameLine(0, 10);
                                    if (ImGui::Game_Select("GTA V", image::gta, ImVec2(119, 119))) game = 5;
                                    ImGui::SameLine(0, 10);
                                    if (ImGui::Game_Select("Apex Legends", image::apex, ImVec2(119, 119))) game = 6;

                                    //----\\

                                    if (ImGui::Game_Select("Garry's Mod", image::garrys, ImVec2(119, 119))) game = 7;
                                    ImGui::SameLine(0, 10);
                                    if (ImGui::Game_Select("TF2", image::tf2, ImVec2(119, 119))) game = 8;
                                    ImGui::SameLine(0, 10);
                                    if (ImGui::Game_Select("Lethal Company", image::lethal, ImVec2(119, 119))) game = 9;

                                }ImGui::EndGroup();

                            }ImGui::EndChild();
                        }
                        if (game == 1)
                        {
                            ImGui::SetCursorPos(ImVec2(219, 68));
                            if (ImGui::ButtonIco("back", image::back, ImVec2(24, 24))) game = 0;
                            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 247, p.y + 68), ImVec2(p.x + 248, p.y + 92), c::background);
                            ImGui::GetWindowDrawList()->AddText(Lexend_Medium, 22.f, ImVec2(p.x + 253, p.y + 69), c::text_activite, "Counter Strike 2");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold, 18.f, ImVec2(p.x + 211, p.y + 114), c::text_activite, "Description:");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Medium_2, 13.f, ImVec2(p.x + 211, p.y + 133), c::text_grey1, "The award-winning free-to-play hero shooter from Entertainment.\nMaster the intricacies of playing as legendary characters, whose numbers\ncontinue to grow, and use their powerful skills to strategize your squad.");

                            if (screen6 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN6, sizeof(SCREEN6), &info, pump, &screen6, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen6, ImVec2(p.x + 211, p.y + 187), ImVec2(p.x + 331, p.y + 255), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN1, sizeof(SCREEN1), &info, pump, &screen1, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen1, ImVec2(p.x + 341, p.y + 187), ImVec2(p.x + 461, p.y + 255), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN2, sizeof(SCREEN2), &info, pump, &screen2, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen2, ImVec2(p.x + 471, p.y + 187), ImVec2(p.x + 591, p.y + 255), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN3, sizeof(SCREEN3), &info, pump, &screen3, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen3, ImVec2(p.x + 211, p.y + 265), ImVec2(p.x + 331, p.y + 333), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen4 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN4, sizeof(SCREEN4), &info, pump, &screen4, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen4, ImVec2(p.x + 341, p.y + 265), ImVec2(p.x + 461, p.y + 333), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen5 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN5, sizeof(SCREEN5), &info, pump, &screen5, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen5, ImVec2(p.x + 471, p.y + 265), ImVec2(p.x + 591, p.y + 333), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 211, p.y + 348), ImVec2(p.x + 395, p.y + 400), c::rect_main, 4.f, 0, 2.f);

                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 223, p.y + 363), c::red_text, "The modification is fully finalized");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 230, p.y + 375), c::red_text, "and no updates are expected");

                            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 405, p.y + 348), ImVec2(p.x + 590, p.y + 400), c::rect_main, 4.f, 0, 2.f);

                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 415, p.y + 363), c::green_text, "Bans for proper use modification");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 440, p.y + 375), c::green_text, "have not been issued");

                            ImGui::SetCursorPos(ImVec2(430, 60));
                            if (ImGui::Button("INJECT", ImVec2(160, 40))) page_inject = 1;

                            if (page_inject == 1)
                            {

                                alpha = ImLerp(alpha, page_inject == 1 ? 0.95f : 0.f, ImGui::GetIO().DeltaTime * 8);

                                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 201, p.y + 114), ImVec2(p.x + 600, p.y + 412), ImColor(21 / 255.f, 22 / 255.f, 29 / 255.f, alpha), 12.f, ImDrawFlags_RoundCornersBottomRight);

                                static float loading_test1 = 0.00f;
                                loading_test1 += 0.0015f;
                                if (loading_test1 < 1.f)
                                {
                                    ImGui::SetCursorPos(ImVec2(430, 96));
                                    ImGui::ProgressBar(loading_test1, ImVec2(160, 4), "%%%");
                                                           
                                    float circleRadius = 50.0f;
                                    float circleThickness = 3.0f;
                                    static DWORD dwTickStart = GetTickCount();
                                    if (GetTickCount() - dwTickStart > 370)
                                    {
                                        if (i < 12)
                                        {

                                            i++;

                                        }

                                        dwTickStart = GetTickCount();
                                    }                                
                                    ImGui::PushFont(Lexend_Bold);
                                    ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + 380 + ImGui::CalcTextSize(std::to_string(i).c_str()).x, ImGui::GetWindowPos().y + 254), ImColor(92, 138, 255), "/12");
                                    ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + 380, ImGui::GetWindowPos().y + 254), ImColor(92, 138, 255), std::to_string(i).c_str());
                                    ImGui::PopFont();

                                    progress += 0.01f;

                                    ImGui::GetWindowDrawList()->AddCircle(p + ImVec2(circleRadius + 350, circleRadius + 213), circleRadius, ImColor(0, 0, 0, 63),0,5);

                                    ImGui::GetWindowDrawList()->PathClear();
                                  
                                    ImGui::GetWindowDrawList()->PathArcTo(p + ImVec2(circleRadius + 350, circleRadius + 213), circleRadius, IM_PI * 0.5f, IM_PI * 0.5f + (progress * 0.3f * IM_PI), 32);
                                    ImGui::GetWindowDrawList()->PathStroke(ImColor(92, 138, 255), false, circleThickness);

                                    if (i >= 12)
                                    {
                                        timer = 1;
                                    }
                                }
                                else
                                {
                                    static DWORD dwTickStart = GetTickCount();
                                    if (GetTickCount() - dwTickStart > 2000)
                                    {
                                        timer = 0;

                                        dwTickStart = GetTickCount();
                                    }
                                    if (timer == 0)
                                    {
                                        static DWORD dwTickStart = GetTickCount();
                                        if (GetTickCount() - dwTickStart > 1000)
                                        {
                                            page_inject = 0;

                                            dwTickStart = GetTickCount();
                                        }
                                    }

                                    alpha1 = ImLerp(alpha1, timer == 1 ? 0.95f : 0.f, ImGui::GetIO().DeltaTime * 8);

                                    if (succes == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Succes_icon, sizeof(Succes_icon), &info, pump, &succes, 0); ImGui::GetWindowDrawList()->AddImageRounded(succes, ImVec2(p.x + 350, p.y + 213), ImVec2(p.x + 450, p.y + 313), ImVec2(0, 0), ImVec2(1, 1), ImColor(92 / 255.f, 138 / 255.f, 255 / 255.f, alpha1), 100);
                                }
                             
                            }
                        }
                        if (game == 6)
                        {
                            ImGui::SetCursorPos(ImVec2(219, 68));
                            if (ImGui::ButtonIco("back", image::back, ImVec2(24, 24))) game = 0;
                            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 247, p.y + 68), ImVec2(p.x + 248, p.y + 92), c::background);
                            ImGui::GetWindowDrawList()->AddText(Lexend_Medium, 22.f, ImVec2(p.x + 253, p.y + 69), c::text_activite, "Apex Legends");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold, 18.f, ImVec2(p.x + 211, p.y + 114), c::text_activite, "Description:");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Medium_2, 13.f, ImVec2(p.x + 211, p.y + 133), c::text_grey1, "The award-winning free-to-play hero shooter from Entertainment.\nMaster the intricacies of playing as legendary characters, whose numbers\ncontinue to grow, and use their powerful skills to strategize your squad.");

                            if (screen == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN, sizeof(SCREEN), &info, pump, &screen, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen, ImVec2(p.x + 211, p.y + 187), ImVec2(p.x + 331, p.y + 255), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN1, sizeof(SCREEN1), &info, pump, &screen1, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen1, ImVec2(p.x + 341, p.y + 187), ImVec2(p.x + 461, p.y + 255), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN2, sizeof(SCREEN2), &info, pump, &screen2, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen2, ImVec2(p.x + 471, p.y + 187), ImVec2(p.x + 591, p.y + 255), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN3, sizeof(SCREEN3), &info, pump, &screen3, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen3, ImVec2(p.x + 211, p.y + 265), ImVec2(p.x + 331, p.y + 333), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen4 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN4, sizeof(SCREEN4), &info, pump, &screen4, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen4, ImVec2(p.x + 341, p.y + 265), ImVec2(p.x + 461, p.y + 333), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            if (screen5 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SCREEN5, sizeof(SCREEN5), &info, pump, &screen5, 0); ImGui::GetWindowDrawList()->AddImageRounded(screen5, ImVec2(p.x + 471, p.y + 265), ImVec2(p.x + 591, p.y + 333), ImVec2(0, 0), ImVec2(1, 1), c::text_activite, 4);

                            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 211, p.y + 348), ImVec2(p.x + 395, p.y + 400), c::rect_main, 4.f, 0, 2.f);

                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 223, p.y + 363), c::red_text, "The modification is fully finalized");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 230, p.y + 375), c::red_text, "and no updates are expected");

                            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 405, p.y + 348), ImVec2(p.x + 590, p.y + 400), c::rect_main, 4.f, 0, 2.f);

                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 415, p.y + 363), c::green_text, "Bans for proper use modification");
                            ImGui::GetWindowDrawList()->AddText(Lexend_Semibold_2, 12.f, ImVec2(p.x + 440, p.y + 375), c::green_text, "have not been issued");

                            ImGui::SetCursorPos(ImVec2(430, 60));
                            if (ImGui::Button("INJECT", ImVec2(160, 40))) page_inject = 6;

                            if (page_inject == 6)
                            {

                                alpha = ImLerp(alpha, page_inject == 6 ? 0.95f : 0.f, ImGui::GetIO().DeltaTime * 8);

                                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 201, p.y + 114), ImVec2(p.x + 600, p.y + 412), ImColor(21 / 255.f, 22 / 255.f, 29 / 255.f, alpha), 12.f, ImDrawFlags_RoundCornersBottomRight);

                                static float loading_test1 = 0.00f;
                                loading_test1 += 0.0015f;
                                if (loading_test1 < 1.f)
                                {
                                    ImGui::SetCursorPos(ImVec2(430, 96));
                                    ImGui::ProgressBar(loading_test1, ImVec2(160, 4), "%%%");
                                    
                                    float circleRadius1 = 50.0f;
                                    float circleThickness1 = 3.0f;
                                    static DWORD dwTickStart = GetTickCount();
                                    if (GetTickCount() - dwTickStart > 370)
                                    {
                                        if (i6 < 12)
                                        {

                                            i6++;

                                        }
                                      

                                        dwTickStart = GetTickCount();
                                    }
                                    ImGui::PushFont(Lexend_Bold);
                                    ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + 380 + ImGui::CalcTextSize(std::to_string(i6).c_str()).x, ImGui::GetWindowPos().y + 254), ImColor(92, 138, 255), "/12");
                                    ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + 380, ImGui::GetWindowPos().y + 254), ImColor(92, 138, 255), std::to_string(i6).c_str());
                                    ImGui::PopFont();

                                    progress6 += 0.01f;

                                    ImGui::GetWindowDrawList()->AddCircle(p + ImVec2(circleRadius1 + 350, circleRadius1 + 213), circleRadius1, ImColor(0, 0, 0, 63), 0, 5);

                                    ImGui::GetWindowDrawList()->PathClear();

                                    ImGui::GetWindowDrawList()->PathArcTo(p + ImVec2(circleRadius1 + 350, circleRadius1 + 213), circleRadius1, IM_PI * 0.5f, IM_PI * 0.5f + (progress6 * 0.3f * IM_PI), 32);
                                    ImGui::GetWindowDrawList()->PathStroke(ImColor(92, 138, 255), false, circleThickness1);

                                    if (i6 >= 12)
                                    {                                      
                                            timer = 1;                    
                                    }
                                }
                                else
                                {
                                    static DWORD dwTickStart = GetTickCount();
                                    if (GetTickCount() - dwTickStart > 2000)
                                    {                                    
                                        timer = 0;                                       

                                        dwTickStart = GetTickCount();
                                    }
                                    if (timer == 0)
                                    {
                                        static DWORD dwTickStart = GetTickCount();
                                        if (GetTickCount() - dwTickStart > 1000)
                                        {
                                            page_inject = 0;

                                            dwTickStart = GetTickCount();
                                        }
                                    }
                                  
                                    alpha1 = ImLerp(alpha1, timer == 1 ? 0.95f : 0.f, ImGui::GetIO().DeltaTime * 8);
                                
                                    if (succes == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Succes_icon, sizeof(Succes_icon), &info, pump, &succes, 0); ImGui::GetWindowDrawList()->AddImageRounded(succes, ImVec2(p.x + 350, p.y + 213), ImVec2(p.x + 450, p.y + 313), ImVec2(0, 0), ImVec2(1, 1), ImColor(92 / 255.f, 138 / 255.f, 255 / 255.f, alpha1), 100);

                                }
                            }
                        }
                    }
                    if (page1 == 1)
                    {
                        ImGui::SetCursorPos(ImVec2(215, 70));
                        ImGui::BeginGroup();
                        {
                            const char* items[4]{ "Regedit", "Gun M4A1", "Gun Deagle","Gun Glock" };
                            static int iTheme = 0;
                            ImGui::Combo("Clean data", &iTheme, items, IM_ARRAYSIZE(items), 2);
                            static bool Hotkey = true;
                            ImGui::Checkboxx("Hotkey", image::check, &Hotkey);
                            const char* items1[4]{ "Legit", "Train", "Bomb","Glock" };
                            static int iTheme1 = 0;
                            ImGui::Combo("Secret", &iTheme1, items1, IM_ARRAYSIZE(items1), 2);
                            static bool Hotkey1 = false;
                            ImGui::Checkboxx("Use v2", image::check, &Hotkey1);
                        }ImGui::EndGroup();

                        ImGui::SetCursorPos(ImVec2(410, 70));
                        ImGui::BeginGroup();
                        {
                            const char* items2[2]{ "Old", "New" };
                            static int iTheme2 = 0;
                            ImGui::Combo("New Login", &iTheme2, items2, IM_ARRAYSIZE(items2), 2);
                            static bool Hotkey2 = false;
                            ImGui::Checkboxx("2FA", image::check, &Hotkey2);

                        }ImGui::EndGroup();
                    }
                }
                ImGui::PopStyleVar();

            }ImGui::End();
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);

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
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
