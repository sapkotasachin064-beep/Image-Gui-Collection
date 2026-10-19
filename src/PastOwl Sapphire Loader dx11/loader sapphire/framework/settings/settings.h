#include "imgui.h"

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

class c_settings
{
public:

	struct
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		std::string name = "SAPHIRE";

		ImVec2 window_size = ImVec2(600, 375);
		ImVec2 window_padding = ImVec2(0, 0);
		ImVec2 item_spacing = ImVec2(0, 0);

		float scrollbar_size = 20.f;
		float border_size = 0.f;
		float rounding = 9.f;

	} c_window;

	struct
	{
		ImFont* poppins_medium[3];
		ImFont* poppins_bold[1];

		ImFont* futura_bold[1];
		ImFont* icons[5];

	} c_font;

	struct
	{
		float rounding = 4;
		float speed_animation = 8;

	} c_field;

	struct
	{
		float rounding = 4;
		float speed_animation = 8;

	} c_button;

	struct
	{
		float rounding = 6;
		float button_rounding = 4;

		float speed_animation = 8;

	} c_product;

	struct
	{
		ImVec2 content_item_spacing = ImVec2(0, 0);
		ImVec2 content_window_padding = ImVec2(0, 0);

		ImVec2 child_item_spacing = ImVec2(10, 10);
		ImVec2 child_window_padding = ImVec2(10, 10);

	} c_child;

	struct
	{
		ID3D11ShaderResourceView* logo = nullptr;
		ID3D11ShaderResourceView* background = nullptr;
		ID3D11ShaderResourceView* decor_background = nullptr;
		ID3D11ShaderResourceView* decor_background_sign = nullptr;

		ID3D11ShaderResourceView* call_of_duty = nullptr;
		ID3D11ShaderResourceView* valorant = nullptr;
		ID3D11ShaderResourceView* apex_legend = nullptr;
		ID3D11ShaderResourceView* dota_2 = nullptr;
		ID3D11ShaderResourceView* warcraft = nullptr;
		ID3D11ShaderResourceView* rust = nullptr;

		ID3D11ShaderResourceView* valorant_preview_0 = nullptr;

		ID3D11ShaderResourceView* valorant_preview_1 = nullptr;
		ID3D11ShaderResourceView* valorant_preview_2 = nullptr;
		ID3D11ShaderResourceView* valorant_preview_3 = nullptr;

	} c_texture;
};

inline c_settings* set = new c_settings();

class c_colors
{
public:

	struct
	{
		ImVec4 accent_clr = ImColor(71, 76, 175, 255);
		ImVec4 accent_fade_clr = ImColor(41, 44, 120, 255);

		ImVec4 black_clr = ImColor(0, 0, 0, 255);
		ImVec4 white_clr = ImColor(255, 255, 255, 255);

		ImVec4 col_bg_alpha_0 = ImColor(150, 150, 150, 255);
		ImVec4 col_bg_alpha_1 = ImColor(220, 220, 220, 255);

		ImVec4 loading_active = ImColor(10, 10, 15, 180);

	} c_other_clr;

	struct
	{
		ImVec4 layout = ImColor(12, 12, 15, 220);
		ImVec4 general_layout = ImColor(13, 14, 16, 255);

	} c_window;

	struct
	{
		ImVec4 stroke_corners = ImColor(40, 42, 77, 220);

	} c_child;

	struct
	{
		ImVec4 layout_active = ImColor(17, 17, 24, 255);
		ImVec4 layout_inactive = ImColor(18, 18, 28, 255);

	} c_field;

	struct
	{
		ImVec4 layout_active = ImColor(25, 27, 50, 255);
		ImVec4 layout_inactive = ImColor(30, 31, 42, 255);

		ImVec4 circle_inactive = ImColor(49, 51, 80, 255);

	} c_checkbox;

	struct
	{
		ImVec4 status_bar = ImColor(15, 15, 18, 200);

	} c_product;

	struct
	{
		ImVec4 element_inactive = ImColor(25, 27, 50, 255);

		ImVec4 scrollbar_active = ImColor(41, 42, 55, 255);
		ImVec4 scrollbar_inactive = ImColor(24, 25, 33, 255);

		ImVec4 text_active = ImColor(255, 255, 255, 255);
		ImVec4 text_hov = ImColor(116, 122, 142, 255);
		ImVec4 text = ImColor(58, 62, 87, 255);

		ImVec4 invisible_text = ImColor(0, 0, 0, 0);


	} c_element;

};

inline c_colors* clr = new c_colors();

