#include "settings/functions.h"
#include "shader/blur.hpp"

void c_gui::render()
{

	gui->new_frame();
	{

		gui->set_next_window_size(SCALE(set->c_window.window_size));
		gui->set_next_window_pos({ 0, 0 });

		gui->begin({ set->c_window.name }, { 0 }, set->c_window.window_flags);
		{

			clr->c_other_clr.accent_clr = ImLerp(clr->c_other_clr.accent_clr, func->c_sign.invalid_account || func->c_inject.invalid_license ? ImColor(250, 75, 85, 255) : ImColor(71, 76, 175, 255), gui->fixed_speed(4.f));
			var->c_gui.drag_animated_position = ImLerp(var->c_gui.drag_animated_position, gui->get_mouse_move_delta(), gui->fixed_speed(2.f));

			const ImVec2 pos = GetWindowPos();
			const ImVec2 size = GetWindowSize();

			ImDrawList* draw_list = GetWindowDrawList();
			ImGuiStyle* style = &GetStyle();

			{	// styles
				style->WindowBorderSize = SCALE(set->c_window.border_size);
				style->WindowRounding = SCALE(set->c_window.rounding);
				style->WindowPadding = SCALE(set->c_window.window_padding);

				style->ScrollbarSize = SCALE(set->c_window.scrollbar_size);
				style->ItemSpacing = SCALE(set->c_window.item_spacing);
			}

			{	// decoration
				draw->add_rect_filled(draw_list, pos, { pos + size }, gui->get_clr(clr->c_window.layout), set->c_window.rounding);

				draw->add_image(draw_list, set->c_texture.logo, { pos.x + (SCALE(80) - SCALE(50)) / 2, pos.y + (size.y - SCALE(50)) / 2 }, { pos.x + (SCALE(80) + SCALE(50)) / 2, pos.y + (size.y + SCALE(50)) / 2 }, { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.accent_clr));

				draw->add_rect_filled(draw_list, { pos.x + SCALE(80), pos.y }, { pos + size }, gui->get_clr(clr->c_window.general_layout), SCALE(set->c_window.rounding), ImDrawFlags_RoundCornersRight);
				draw->add_image_rounded(draw_list, set->c_texture.decor_background, { pos.x + SCALE(80), pos.y }, { pos + size }, { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.accent_clr), SCALE(set->c_window.rounding - 3), ImDrawFlags_RoundCornersRight);
			}

			var->c_selection.selection_alpha = ImClamp(var->c_selection.selection_alpha + (4.f * ImGui::GetIO().DeltaTime * (var->c_selection.selection == var->c_selection.selection_active ? 1.f : -1.f)), 0.f, 1.f);

			if (var->c_selection.selection_alpha == 0.f && var->c_selection.selection_add == 0.f && var->c_gui.loading_timer < 49.f)
			{
				var->c_gui.loading_timer += 1 * gui->fixed_speed(10.f);

				if (var->c_gui.loading_timer >= 49.f)
				{
					var->c_selection.selection_active = var->c_selection.selection;
					var->c_gui.loading_timer = 0;
				}

			} else
				var->c_gui.reset = true;

			gui->loading(var->c_gui.loading_description, & var->c_gui.reset, pos + SCALE(0, 0), pos + SCALE(set->c_window.window_size), 1.f - var->c_selection.selection_alpha);

			gui->push_style_var(ImGuiStyleVar_Alpha, var->c_selection.selection_alpha * style->Alpha);

			if (var->c_selection.selection_active == 0)
			{
				gui->set_cursor_pos(ImVec2(size.x - SCALE(60), 0));
				{
					if (widget->ico_button(set->c_font.icons[0], "E", { 60, 60 })) PostQuitMessage(0);
				}

				{	// decoration logo & name

					draw->add_image(draw_list, set->c_texture.decor_background_sign, { pos.x + SCALE(80) + var->c_gui.drag_animated_position.x, pos.y + var->c_gui.drag_animated_position.y }, { pos + size + var->c_gui.drag_animated_position }, { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.accent_clr));

					draw->render_text(draw_list, set->c_font.icons[2], pos + SCALE(80, 0), pos + SCALE(340, 160), gui->get_clr(clr->c_other_clr.accent_clr, 0.3f), "D", NULL, NULL, { 0.5, 0.5 });
					widget->colored_text(set->c_font.futura_bold[0], pos + SCALE(80, 0), pos + SCALE(340, 160), { { gui->get_clr(clr->c_other_clr.white_clr), "SAPP" }, {gui->get_clr(clr->c_other_clr.accent_clr), "HIRE"} }, {0.5, 0.5});

					draw->render_text(draw_list, set->c_font.poppins_medium[1], pos + SCALE(80, 0), pos + SCALE(340, 160), gui->get_clr(clr->c_element.text), "We're happy to see you again", NULL, NULL, { 0.5, 0.75 });

				}

				gui->set_cursor_pos(SCALE(80, 160));

				gui->begin_content("sign", SCALE(260, 215), { 45, 35 }, { 10, 10 });
				{

					{
						widget->text_field("Login", "B", func->c_sign.login, IM_ARRAYSIZE(func->c_sign.login), SCALE(170, 33));

						widget->text_field("Password", "A", func->c_sign.password, IM_ARRAYSIZE(func->c_sign.password), SCALE(170, 33), ImGuiInputTextFlags_Password);
					}

					draw->add_rect_filled(draw_list, GetCursorScreenPos() + SCALE(30, 0), GetCursorScreenPos() + SCALE(140, 1), gui->get_clr(clr->c_field.layout_inactive)); /* */ gui->spacing();

					gui->set_cursor_pos_x(gui->get_cursor_pos_x() + func->c_sign.error_animation);
					{
						if (widget->inject_button("LOG IN", SCALE(170, 33)) && (func->c_sign.login == func->c_sign.login_success && func->c_sign.password == func->c_sign.password_success))
						{
							var->c_gui.loading_description = "Please wait, account verification";
							var->c_selection.selection = 1;
						};
					}

					func->c_sign.error_animation = easing->easing_in_out(reinterpret_cast<int>(&func->c_sign.error_animation), IsItemClicked() && (func->c_sign.login != func->c_sign.login_success || func->c_sign.password != func->c_sign.password_success), 0.f, SCALE(-20.f), gui->fixed_speed(16.f), elastic);

				}
				gui->end_content();

			}
			else if (var->c_selection.selection_active == 1)
			{

				gui->set_cursor_pos(ImVec2(size.x - SCALE(60), 0));
				{
					if (widget->ico_button(set->c_font.icons[0], "E", {60, 60})) PostQuitMessage(0);
				}

				{	// decoration logo & name
					widget->colored_text(set->c_font.poppins_bold[0], pos + SCALE(95, 0), { pos.x + size.x, pos.y + SCALE(80) }, { { gui->get_clr(clr->c_element.text_active), "Available "}, {gui->get_clr(clr->c_other_clr.white_clr), "products"}}, {0.0, 0.25});
					widget->colored_text(set->c_font.poppins_medium[1], pos + SCALE(95, 0), { pos.x + size.x, pos.y + SCALE(80) }, { { gui->get_clr(clr->c_element.text), "Welcome back, "}, {gui->get_clr(clr->c_other_clr.accent_clr), func->c_sign.login_success.data() } }, { 0.0, 0.75 });
				}

				var->c_selection.selection_offset = easing->easing_in_out(reinterpret_cast<int>(&var->c_selection.selection_offset), var->c_selection.selection_alpha == 0.f, 0.f, 100.f, gui->fixed_speed(16.f), back);

				gui->set_cursor_pos(SCALE(80, 80 + var->c_selection.selection_offset));

				gui->begin_content("product", size - SCALE(80, 80), { 15, 0 }, { 10, 10 });
				{
					
					gui->begin_group();
					{

						if (widget->product(set->c_texture.call_of_duty, "Call of duty", "01.01.2025", safe_for_use))
						{
							var->c_gui.loading_description = "Loading the product and its components.";
							var->c_selection.selection = 2;
						}

						widget->product(set->c_texture.apex_legend, "Apex legend", "01.01.2025", on_update);

						widget->product(set->c_texture.warcraft, "Warcraft", "01.01.2025", totally_unsafe);

					}
					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{

						widget->product(set->c_texture.valorant, "Valorant", "01.01.2025", at_own_risk);

						widget->product(set->c_texture.dota_2, "Dota 2", "01.01.2025", totally_unsafe);

						widget->product(set->c_texture.rust, "Rust", "01.01.2025", safe_for_use);

					}
					gui->end_group();

					gui->set_cursor_pos_y(gui->get_cursor_pos_y() + SCALE(5));

				}
				gui->end_content();

			}
			else if (var->c_selection.selection_active == 2)
			{

				gui->set_cursor_pos(SCALE(95, 23));
				{

					if (widget->selection_details("PRODUCT", "VALORANT", { 210, 20 }))
					{
						var->c_gui.loading_description = "Returning to the product tab, please wait";
						var->c_selection.selection = 1;
					};

				}

				draw->render_text(draw_list, set->c_font.poppins_medium[1], pos + SCALE(80, 0), pos + ImVec2(size.x - SCALE(15), SCALE(65)), gui->get_clr(clr->c_other_clr.accent_clr), "1 . 4 . 6", NULL, NULL, {1.0, 0.5});


				gui->set_cursor_pos(SCALE(80, 50));

				gui->begin_group();
				{

					gui->begin_content("product & video", SCALE(320, 192), { 15, 15 }, { 10, 10 }, false);
					{

						widget->video_preview("https://youtu.be/dQw4w9WgXcQ?si=n7fMXGGY9eGtAUe8", set->c_texture.valorant_preview_0, SCALE(290, 110), {0, 0}, {1, 1}, gui->get_clr(clr->c_other_clr.white_clr), SCALE(4));

						{
							widget->image(set->c_texture.valorant_preview_1, SCALE(90, 42), { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.white_clr), SCALE(4));

							gui->sameline();

							widget->image(set->c_texture.valorant_preview_2, SCALE(90, 42), { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.white_clr), SCALE(4));

							gui->sameline();

							widget->video_preview("https://youtu.be/dQw4w9WgXcQ?si=n7fMXGGY9eGtAUe8", set->c_texture.valorant_preview_3, SCALE(90, 42), { 0, 0 }, { 1, 1 }, gui->get_clr(clr->c_other_clr.white_clr), SCALE(4));
						}

					}
					gui->end_content();

					gui->sameline();

					gui->begin_content("key & info", SCALE(200, 192), { 1, 15 }, { 0, 0 }, false);
					{

						gui->begin_child("container", SCALE(185, 162));
						{

							widget->product_info("01.01.2025", "safe for use");

							gui->set_cursor_pos_y(get_cursor_pos_y() + SCALE(20));
							{

								widget->text_field("License key", "O", func->c_inject.license, IM_ARRAYSIZE(func->c_inject.license), ImVec2(GetContentRegionAvail().x, SCALE(35)), ImGuiInputTextFlags_Password);

								if (widget->license_button("INJECT", ImVec2(GetContentRegionAvail().x, SCALE(35))) && func->c_inject.license == func->c_inject.license_success)
								{
									var->c_gui.loading_description = "Wait for the injection to complete and enter the game.";
									var->c_selection.selection = 1;
								}

							}

						}
						gui->end_child();

					}
					gui->end_content();

					gui->begin_content("include", ImVec2(size.x - SCALE(80), SCALE(20)), { 15, 2 }, { 15, 0 }, false);
					{

						{
							widget->checkbox("Spoof before the injection", &func->c_inject.spoof_before_injection);

							gui->sameline();

							widget->checkbox("External injection", &func->c_inject.external_injection);
						}

					}
					gui->end_content();

					gui->begin_content("info about project", ImVec2(size.x - SCALE(80), SCALE(115)), { 15, 15 }, { 0, 0 }, false);
					{

						gui->begin_child("My PC", GetContentRegionAvail(), {12, 10}, {26, 13});
						{

							{
								widget->part_characteristic("CPU", "Intel(R) Core I5-9400k", { 220, 13 });

								gui->sameline();

								widget->part_characteristic("Graphics Card", "GeForce GTX 1050", { 220, 13 });
							}

							{
								widget->part_characteristic("RAM", "8GB", { 220, 13 });

								gui->sameline();

								widget->part_characteristic("DirectX", "10 / 11", { 220, 13 });
							}

							{
								widget->part_characteristic("Operating System", "Windows 11", { 220, 13 });

								gui->sameline();

								widget->part_characteristic("Disk Space", "512 GB", { 220, 13 });
							}

						}
						gui->end_child();

					}
					gui->end_content();

				}
				gui->end_group();

			}

			gui->pop_style_var(1);

			gui->move_window(var->winapi.hwnd, var->winapi.rc);
		}
		gui->end();

		{	// smooth scale menu

			if (GetAsyncKeyState(VK_OEM_PLUS) & 0x1 && var->c_dpi.stored_dpi < 2.f)
			{
				var->c_dpi.stored_dpi += 0.5f;
				var->c_dpi.dpi_changed = true;
			}

			if (GetAsyncKeyState(VK_OEM_MINUS) & 0x1 && var->c_dpi.stored_dpi > 1.f)
			{
				var->c_dpi.stored_dpi -= 0.5f;
				var->c_dpi.dpi_changed = true;
			}

			var->c_dpi.dpi = ImLerp(var->c_dpi.dpi, var->c_dpi.stored_dpi, gui->fixed_speed(25.f));

		}

	}
	gui->end_frame();
}