#pragma once
#include <unordered_map>
#include <string>

class c_globals {
public:
	IDirect3DDevice9* pDevice;
	LPDIRECT3DDEVICE9        g_pd3dDevice;
	LPDIRECT3DTEXTURE9 saturn_icon = nullptr;
	LPDIRECT3DTEXTURE9 username_icon = nullptr;
	LPDIRECT3DTEXTURE9 password_icon = nullptr;
	LPDIRECT3DTEXTURE9 cs2_icon = nullptr;
	bool active = true;
	char username[255] = "";
	char password[255] = "";
	bool login_form = true;
	bool login_loading = false;
	bool main_form = false;
	bool loading_product = false;
	bool loaded_product = false;
	bool failed = false;
	bool checked_connection = true;
	bool initialized_new_window_name = false;
	bool loaded_successfull = false;
	std::string fail_message = "";
	std::string connection_message = "";
	std::string status = "unknown";
	std::string version = "0.0.1";
	std::string last_uptade = "23.03.2024";
	int final_selected_product = 0;
	int tick_count = 0;
	int tick_count2 = 0;
	ImFont* verdana;
	ImFont* logo_font;
};

inline c_globals globals;