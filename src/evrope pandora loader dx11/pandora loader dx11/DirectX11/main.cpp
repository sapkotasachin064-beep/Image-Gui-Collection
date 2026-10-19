#include "main.hpp"
c_main gui;

int main( int, char** ) {
    WNDCLASSEXW wc = { sizeof( wc ), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, L"DirectX11", NULL };
    ::RegisterClassExW( &wc );
    HWND hwnd = ::CreateWindowW( wc.lpszClassName, L"DirectX11 Window", WS_POPUP, GetSystemMetrics( SM_CXSCREEN ) / 2 - ui::size.x / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 - ui::size.y / 2, ui::size.x, ui::size.y, NULL, NULL, wc.hInstance, NULL );

    IMGUI_CHECKVERSION( );

    CreateDeviceD3D( hwnd );
    {
        ::c_context context( hwnd );

        ImGuiIO& io = ImGui::GetIO( ); io.IniFilename = nullptr;
        gui.initialize( io );

        ::ShowWindow( hwnd, SW_SHOWDEFAULT );
        ::UpdateWindow( hwnd );

        bool done = false;
        while ( !done ) {
            MSG msg;
            while ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
                ::TranslateMessage( &msg );
                ::DispatchMessage( &msg );
                if ( msg.message == WM_QUIT )
                    done = true;
            }
            if ( done )
                break;

            if ( g_SwapChainOccluded && g_pSwapChain->Present( 0, DXGI_PRESENT_TEST ) == DXGI_STATUS_OCCLUDED ) {
                ::Sleep( 10 );
                continue;
            }
            g_SwapChainOccluded = false;

            if ( g_ResizeWidth != 0 && g_ResizeHeight != 0 ) {
                CleanupRenderTarget( );
                g_pSwapChain->ResizeBuffers( 0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0 );
                g_ResizeWidth = g_ResizeHeight = 0;
                CreateRenderTarget( );
            }

            {
                ::c_frame frame{};

                RECT rect;
                GetWindowRect( hwnd, &rect );

                if ( GImGui->MovingWindow != NULL ) {
                    MoveWindow( hwnd, rect.left + ImGui::GetMouseDragDelta( ).x, rect.top + ImGui::GetMouseDragDelta( ).y, ui::size.x, ui::size.y, TRUE );
                }

                gui.render( );
            }
        }
    }
    CleanupDeviceD3D( );

    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
}