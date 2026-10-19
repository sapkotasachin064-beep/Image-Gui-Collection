#include "imgui.h"
#include <vector>
#include <string>
#include <wtypes.h>

class c_variable
{
public:

	struct
	{
		HWND hwnd;
		RECT rc;

	} winapi;

	struct
	{
		int selection = 0, selection_active = 0;
		float selection_alpha = 0, selection_add;

		float selection_offset = 0;

	} c_selection;

	struct
	{
		float dpi = 1.0f;
		float stored_dpi = 1.0f;

		bool dpi_changed = true;

	} c_dpi;

	struct
	{

		float loading_timer = 0;
		float injecting_timer = 0;
		float loading_alpha = 0;
		bool reset = true;

		ImVec2 drag_animated_position;

		std::string loading_description = "";

	} c_gui;

};

inline c_variable* var = new c_variable();

class c_functional
{
public:

	struct
	{

		std::string login_success = "PastOwl";
		std::string password_success = "123456";

		char login[16] = { "" };
		char password[32] = { "" };

		float error_animation = 0;
		bool invalid_account = false;

	} c_sign;

	struct
	{
		std::string license_success = "123456";
		char license[16] = { "" };

		float license_animation = 0;
		bool invalid_license = false;

		bool external_injection = true;
		bool spoof_before_injection = true;

	} c_inject;

};

inline c_functional* func = new c_functional();
