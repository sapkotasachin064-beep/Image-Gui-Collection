#include "../pages.hpp"
#include "misc.hpp"

#include "../../../project/config.hpp"

namespace gui::pages
{
	static ImVec4 Lighten(const ImVec4& color, const float amount) noexcept
	{
		return ImVec4(
			color.x + (1.0f - color.x) * amount,
			color.y + (1.0f - color.y) * amount,
			color.z + (1.0f - color.z) * amount,
			color.w);
	}

	static ImVec4 Darken(const ImVec4& color, const float amount) noexcept
	{
		return ImVec4(
			color.x * (1.0f - amount),
			color.y * (1.0f - amount),
			color.z * (1.0f - amount),
			color.w);
	}

	static ImVec4 WithAlpha(const ImVec4& color, const float alpha) noexcept
	{
		return ImVec4(color.x, color.y, color.z, alpha);
	}

	static void ApplyAccent() noexcept
	{
		const ImVec4 accent = ImVec4(
			vars::colorAccent[0],
			vars::colorAccent[1],
			vars::colorAccent[2],
			vars::colorAccent[3]);

		const ImVec4 hover = Lighten(accent, 0.15f);
		const ImVec4 active = Darken(accent, 0.20f);

		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = WithAlpha(colors[ImGuiCol_WindowBg], 1.0f);
		colors[ImGuiCol_PopupBg] = WithAlpha(colors[ImGuiCol_PopupBg], 1.0f);
		colors[ImGuiCol_MenuBarBg] = WithAlpha(colors[ImGuiCol_MenuBarBg], 1.0f);
		colors[ImGuiCol_ScrollbarBg] = WithAlpha(colors[ImGuiCol_ScrollbarBg], 1.0f);
		colors[ImGuiCol_FrameBg] = WithAlpha(colors[ImGuiCol_FrameBg], 1.0f);
		colors[ImGuiCol_FrameBgHovered] = WithAlpha(colors[ImGuiCol_FrameBgHovered], 1.0f);
		colors[ImGuiCol_FrameBgActive] = WithAlpha(colors[ImGuiCol_FrameBgActive], 1.0f);
		colors[ImGuiCol_TitleBg] = WithAlpha(colors[ImGuiCol_TitleBg], 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = WithAlpha(colors[ImGuiCol_TitleBgCollapsed], 1.0f);

		colors[ImGuiCol_CheckMark] = Lighten(accent, 0.30f);
		colors[ImGuiCol_CheckboxSelectedBg] = accent;

		colors[ImGuiCol_SliderGrab] = accent;
		colors[ImGuiCol_SliderGrabActive] = hover;

		colors[ImGuiCol_Button] = accent;
		colors[ImGuiCol_ButtonHovered] = hover;
		colors[ImGuiCol_ButtonActive] = active;

		colors[ImGuiCol_Header] = accent;
		colors[ImGuiCol_HeaderHovered] = hover;
		colors[ImGuiCol_HeaderActive] = active;

		colors[ImGuiCol_ScrollbarGrab] = WithAlpha(Darken(accent, 0.35f), 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = WithAlpha(Darken(accent, 0.15f), 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = WithAlpha(accent, 1.0f);

		colors[ImGuiCol_Tab] = WithAlpha(Darken(accent, 0.55f), 1.0f);
		colors[ImGuiCol_TabHovered] = hover;
		colors[ImGuiCol_TabSelected] = accent;
		colors[ImGuiCol_TabSelectedOverline] = Lighten(accent, 0.35f);
		colors[ImGuiCol_TabDimmed] = WithAlpha(Darken(accent, 0.75f), 1.0f);
		colors[ImGuiCol_TabDimmedSelected] = WithAlpha(Darken(accent, 0.45f), 1.0f);

		colors[ImGuiCol_TitleBgActive] = WithAlpha(Darken(accent, 0.50f), 1.0f);

		colors[ImGuiCol_SeparatorHovered] = hover;
		colors[ImGuiCol_SeparatorActive] = accent;

		colors[ImGuiCol_ResizeGrip] = WithAlpha(accent, 0.30f);
		colors[ImGuiCol_ResizeGripHovered] = WithAlpha(accent, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = accent;

		colors[ImGuiCol_TextSelectedBg] = WithAlpha(accent, 0.45f);
		colors[ImGuiCol_TextLink] = Lighten(accent, 0.30f);
		colors[ImGuiCol_InputTextCursor] = Lighten(accent, 0.35f);
		colors[ImGuiCol_NavCursor] = accent;
		colors[ImGuiCol_DragDropTarget] = Lighten(accent, 0.35f);
		colors[ImGuiCol_PlotHistogram] = accent;
		colors[ImGuiCol_PlotHistogramHovered] = hover;
	}

	void ApplyTheme(const int theme) noexcept
	{
		switch (theme)
		{
		case 1:
			ImGui::StyleColorsClassic();
			break;
		case 2:
			ImGui::StyleColorsLight();
			break;
		default:
			ImGui::StyleColorsDark();
			break;
		}

		ApplyAccent();
	}

	void RenderMisc() noexcept
	{
		const char* themes[] = { "Dark", "Classic", "Light" };
		if (ImGui::Combo("Theme", &vars::iTheme, themes, IM_ARRAYSIZE(themes)))
		{
			ApplyTheme(vars::iTheme);
		}

		if (ImGui::ColorEdit4("Accent Color", vars::colorAccent))
		{
			ApplyAccent();
		}

		ImGui::TextWrapped("Config file: %s", config::GetConfigPath().c_str());
		ImGui::Spacing();

		static const char* statusMessage = "";
		static bool statusSuccess = true;

		if (ImGui::Button("Save", ImVec2(120.0f, 0.0f)))
		{
			if (config::Save())
			{
				statusMessage = "Config saved.";
				statusSuccess = true;
			}
			else
			{
				statusMessage = "Failed to save config.";
				statusSuccess = false;
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Load", ImVec2(120.0f, 0.0f)))
		{
			if (config::Load())
			{
				ApplyTheme(vars::iTheme);
				statusMessage = "Config loaded.";
				statusSuccess = true;
			}
			else
			{
				statusMessage = "Failed to load config.";
				statusSuccess = false;
			}
		}

		if (statusMessage[0] != '\0')
		{
			ImGui::Spacing();
			if (statusSuccess)
			{
				ImGui::TextUnformatted(statusMessage);
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.35f, 1.0f), "%s", statusMessage);
			}
		}
	}
}
