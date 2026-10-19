#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>

#include "pshader.hpp"

inline ID3D11PixelShader* pshader;
inline ID3D11ShaderResourceView* tex;
inline ID3D11Device* device;
inline ID3D11DeviceContext* ctx;
inline int next_release = 2;
inline int cur = 0;

struct ImGui_ImplDX11_Data
{
	ID3D11Device*               pd3dDevice;
	ID3D11DeviceContext*        pd3dDeviceContext;
	IDXGIFactory*               pFactory;
	ID3D11Buffer*               pVB;
	ID3D11Buffer*               pIB;
	ID3D11VertexShader*         pVertexShader;
	ID3D11InputLayout*          pInputLayout;
	ID3D11Buffer*               pVertexConstantBuffer;
	ID3D11PixelShader*          pPixelShader;
	ID3D11SamplerState*         pFontSampler;
	ID3D11ShaderResourceView*   pFontTextureView;
	ID3D11RasterizerState*      pRasterizerState;
	ID3D11BlendState*           pBlendState;
	ID3D11DepthStencilState*    pDepthStencilState;
	int                         VertexBufferSize;
	int                         IndexBufferSize;

	ImGui_ImplDX11_Data()       { memset((void*)this, 0, sizeof(*this)); VertexBufferSize = 5000; IndexBufferSize = 10000; }
};

inline void get_back_buffer( IDXGISwapChain* swap_chain ) {
	ID3D11Texture2D* back_buffer;
	swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) );

	D3D11_TEXTURE2D_DESC desc;
	back_buffer->GetDesc( &desc );
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* back_buffer_tex = NULL;
	device->CreateTexture2D( &desc, 0, &back_buffer_tex );
	ctx->CopyResource( back_buffer_tex, back_buffer );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView( back_buffer_tex, &srvDesc, &tex );

	back_buffer->Release( );
	back_buffer_tex->Release( );
}

inline void begin( const ImDrawList* d, const ImDrawCmd* cmd ) {
	const auto swap_chain = reinterpret_cast< IDXGISwapChain* >( cmd->UserCallbackData );
	auto bd = ( ImGui_ImplDX11_Data* )ImGui::GetIO( ).BackendRendererUserData;

	if ( tex && cur == next_release ) {
		tex->Release( );
		tex = nullptr;

		next_release = cur + 2;
	}

	cur++;

	if ( !tex )
		get_back_buffer( swap_chain );

	if ( !pshader ) {
		device->CreatePixelShader( pshaderd, sizeof( pshaderd ), NULL, &pshader );
	}

	ctx->PSSetShader( pshader, nullptr, 0 );
	ctx->PSSetSamplers( 0, 1, &bd->pFontSampler );
}

inline void release( const ImDrawList* d, const ImDrawCmd* cmd ) {
	if ( tex ) {
		tex->Release( );
		tex = nullptr;
	}
}


inline void render_blur( ImDrawList* draw_list, IDXGISwapChain* swap_chain, ID3D11Device* _device, ID3D11DeviceContext* _ctx, ImVec2 start, ImVec2 end, int rounding = 0 ) {
	device = _device;
	ctx = _ctx;

	draw_list->AddCallback( begin, swap_chain );
	draw_list->AddImageRounded( tex, ImVec2(0,0), ImVec2(100, 100), ImVec2(0,0), ImVec2(1,1), ImColor{ 1.f, 1.f, 1.f, GImGui->Style.Alpha }, rounding );
	draw_list->AddCallback( ImDrawCallback_ResetRenderState, 0 );
}