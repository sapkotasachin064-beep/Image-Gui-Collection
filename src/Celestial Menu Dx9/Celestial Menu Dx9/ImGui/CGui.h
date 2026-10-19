#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "misc/cpp/imgui_stdlib.h"
#include "misc/freetype/imgui_freetype.h"
#include "misc/addons/imgui_addons.h"
#include "misc/addons/imgui_groups.h"

#include <d3d9.h>

#include "CTextures.h"

class CGui
{
public:
	CGui(Textures* pTextures);
	CGui(const CGui&);
	~CGui();

	bool Initialize(void* hWnd, IDirect3DDevice9* pDevice);
	void Shutdown();
	void PreRender();
	void Render();

	bool MsgProc(HWND, UINT, WPARAM, LPARAM);

private:
	Textures* m_pTextures;

	ImFont* LogoFont;
};

enum MenuPage_
{
	MenuPage_Aimbot,
	MenuPage_Weapons,
	MenuPage_Vehicles,
	MenuPage_Players,
	MenuPage_Objects,
	MenuPage_Executor,
	MenuPage_Configs,
	MenuPage_Settings,
	MenuPage_COUNT,
};

namespace font
{
	inline ImFont* mainfont = nullptr;

}


namespace c{
	inline static ImVec2 size = ImVec2(700, 550);
	inline float ShadowThickness = 20;
	inline ImVec4 accent = ImColor(134, 154, 224);

	namespace background
	{

		inline ImVec4 filling = ImColor(21,21,21);
		inline ImVec4 child = ImColor(25, 25, 25);
		inline ImVec4 stroke = ImColor(24, 26, 36);
		inline ImVec2 size = ImVec2(900, 515);

		inline float rounding = 6;

	}

	namespace roundings
	{
		inline float windowrounding = 3;
		inline float childrounding = 2;
		inline float framerounding = 2;
		inline float popuprounding = 2;
		inline float grabrounding = 2;
		inline float tabrounding = 2;
		inline float scrollbarrounding = 1;
	}

	namespace elements
	{
		inline ImVec4 border = ImColor(31, 31, 31);
		inline ImVec4 seperator = ImColor(31, 31, 31);

		inline ImVec4 scrollbar_background = ImColor(31, 31, 31 , 0);
		inline ImVec4 scrollbar_grabbed = ImColor(25, 25, 25, 0);
		inline ImVec4 scrollbar_grab_hovered = ImColor(25, 25, 25, 0);
		inline ImVec4 scrollbar_active = ImColor(25, 25, 25, 0);


		inline ImVec4 button = ImColor(21, 21, 21);
		inline ImVec4 button_active = ImColor(21, 21, 21, 200);
		inline ImVec4 button_hovered = ImColor(21, 21, 21, 170);


		inline ImVec4 mark = ImColor(0, 0, 0);

		inline ImVec4 stroke = ImColor(28, 26, 37);
		inline ImVec4 background = ImColor(255, 15, 17);
		inline ImVec4 background_widget = ImColor(21, 23, 26);

		inline ImVec4 text_active = ImColor(97, 97, 97);
		inline ImVec4 text_hov = ImColor(255, 255, 255);
		inline ImVec4 text = ImColor(255, 255, 255);
		inline ImVec4 text_disabled = ImColor(97, 97, 97);



		inline float rounding = 4;
	}

	namespace child
	{

	}

	namespace tab
	{
		inline ImVec4 tab_active = ImColor(22, 22, 30);

		inline ImVec4 border = ImColor(14, 14, 15);
	}
}
