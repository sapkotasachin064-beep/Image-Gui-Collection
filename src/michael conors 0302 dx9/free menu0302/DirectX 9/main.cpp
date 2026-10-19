#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dcompiler.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <filesystem>

#include <gui.h>
#include <font.h>
#pragma comment( lib, "freetype64.lib" )

static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

bool CreateDeviceD3D( HWND hWnd );
void CleanupDeviceD3D( );
void CreateRenderTarget( );
void CleanupRenderTarget( );
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
    WNDCLASSEXW wc = { sizeof( wc ), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, L"DirectX 9", NULL };
    ::RegisterClassExW( &wc );
    HWND hwnd = ::CreateWindowW( wc.lpszClassName, L"DirectX 11", WS_OVERLAPPEDWINDOW, 1444, 444, 1280, 720, NULL, NULL, wc.hInstance, NULL );

    CreateDeviceD3D( hwnd );

    ImGui::CreateContext( );
    ui::styles( );
    ImGuiIO& io = ImGui::GetIO( );
    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX11_Init( g_pd3dDevice, g_pd3dDeviceContext );

    ui::colors( );

    auto config = ImFontConfig( );
    config.FontDataOwnedByAtlas = false;

    /*io.Fonts->AddFontFromMemoryTTF( ( void* )font, sizeof( font ), 14.f, &config );
    io.Fonts->AddFontFromMemoryTTF( ( void* )fontb, sizeof( fontb ), 11.f, &config );
    io.Fonts->AddFontFromMemoryTTF( ( void* )glyphter, sizeof( glyphter ), 16.f, &config );*/

    io.Fonts->AddFontFromFileTTF( std::filesystem::path( std::filesystem::current_path( ) / "GUI" / "resources" / "fontb.ttf" ).string( ).c_str( ), 15.f );
    io.Fonts->AddFontFromFileTTF( std::filesystem::path( std::filesystem::current_path( ) / "GUI" / "resources" / "Glyphter.ttf" ).string( ).c_str( ), 14.f );

    ::ShowWindow( hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow( hwnd );

    ui::add_page( 0, []( ){
        
        ImGui::BeginGroup( );
        {
            ui::begin_child( "GENERAL" );
            {
                static bool bools[10];
                static int ints[10];
                static float col[4];

                ImGui::Checkbox( "Enabled", &bools[0] );
                ImGui::Checkbox( "Diasbled", &bools[1], col );
                ImGui::SliderInt( "Slider Int", &ints[0], 0, 100, "%d%%" );
                ImGui::Combo( "Combobox", &ints[1], "Value 1\0Value 2\0Value 3\0" );
            }
            ui::end_child( );

            ui::begin_child( "GENERAL##2" );
            {
                static bool bools[10];
                static int ints[10];
                static float floats[10];

                ImGui::Checkbox( "Enabled##2", &bools[0] );
                ImGui::SliderInt( "Slider Int##2", &ints[0], 0, 100, "%d%%" );
                ImGui::Combo( "Combobox##2", &ints[1], "Value 1\0Value 2\0Value 3\0" );
            }
            ui::end_child( );
        }
        ImGui::EndGroup( );

        ImGui::SameLine( );

        ImGui::BeginGroup( );
        {
            ui::begin_child( "GENERAL##3" );
            {
                static bool bools[10];
                static int ints[10];
                static float floats[10];

                ImGui::Checkbox( "Enabled##3", &bools[0] );
                ImGui::SliderInt( "Slider Int##3", &ints[0], 0, 100, "%d%%" );
                ImGui::Combo( "Combobox##3", &ints[1], "Value 1\0Value 2\0Value 3\0" );
            }
            ui::end_child( );

            ui::begin_child( "GENERAL##4" );
            {
                static bool bools[10];
                static int ints[10];
                static float floats[10];
                static float col[4];

                ImGui::Checkbox( "Enabled##4", &bools[0] );
                ImGui::Checkbox( "Diasbled##4", &bools[1], col );
                ImGui::SliderInt( "Slider Int##4", &ints[0], 0, 100, "%d%%" );
                ImGui::Combo( "Combobox##4", &ints[1], "Value 1\0Value 2\0Value 3\0" );
            }
            ui::end_child( );
        }
        ImGui::EndGroup( );
        
        } );

    bool done = false;
    while ( !done ) {
        MSG msg;
        while ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            if ( msg.message == WM_QUIT )
                done = true;
        }
        if ( done )
            break;

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 0, 0 } );
        ImGui::Begin( "UI", 0, ImGuiWindowFlags_NoDecoration );
        {
            ImGui::PopStyleVar( );
            ImGui::SetWindowSize( ui::size );
            
            ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, GImGui->Style.WindowRounding );
            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0, 0 } );
            ImGui::BeginChild( "header", { -1, 60 } );
            {
                for ( int i = 0; i < ui::tabs.size( ); ++i ) {
                    ui::tab( i );
                    ImGui::SameLine( 0, 0 );
                }
            }
            ImGui::EndChild( );
            ImGui::PopStyleVar( 2 );

            ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ui::content_anim );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 20, 20 } );
            ImGui::BeginChild( "content", { -1, -1 }, 0, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground );
            {
                ImGui::PopStyleVar( );

                ui::render_page( );
            }
            ImGui::EndChild( );

            ImGui::PopStyleVar( );
        }
        ImGui::End( );

        ui::handle_alpha_anim( );

        ImGui::Render( );

        const float clear_color_with_alpha[4] = { 1.f, 1.f, 1.f, 1.f };
        g_pd3dDeviceContext->OMSetRenderTargets( 1, &g_mainRenderTargetView, NULL );
        g_pd3dDeviceContext->ClearRenderTargetView( g_mainRenderTargetView, clear_color_with_alpha );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        g_pSwapChain->Present( 1, 0 );
    }

    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    CleanupDeviceD3D( );
    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
}

bool CreateDeviceD3D( HWND hWnd )
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
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
    if ( D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext ) != S_OK )
        return false;

    CreateRenderTarget( );
    return true;
}

void CleanupDeviceD3D( )
{
    CleanupRenderTarget( );
    if ( g_pSwapChain ) { g_pSwapChain->Release( ); g_pSwapChain = NULL; }
    if ( g_pd3dDeviceContext ) { g_pd3dDeviceContext->Release( ); g_pd3dDeviceContext = NULL; }
    if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
}

void CreateRenderTarget( )
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
    g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_mainRenderTargetView );
    pBackBuffer->Release( );
}

void CleanupRenderTarget( )
{
    if ( g_mainRenderTargetView ) { g_mainRenderTargetView->Release( ); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
        return true;

    switch ( msg )
    {
   case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget( );
            g_pSwapChain->ResizeBuffers( 0, ( UINT )LOWORD( lParam ), ( UINT )HIWORD( lParam ), DXGI_FORMAT_UNKNOWN, 0 );
            CreateRenderTarget( );
        }
        return 0;
    case WM_SYSCOMMAND:
        if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage( 0 );
        return 0;
    }
    return ::DefWindowProc( hWnd, msg, wParam, lParam );
}