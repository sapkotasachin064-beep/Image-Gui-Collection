#include "gui.hpp"

#include <cstdlib>

#include "pages/misc/misc.hpp"

namespace gui
{
	void UpdateMenuTitle()
	{
		static DWORD lastUpdate = 0;
		static DWORD cycleStart = GetTickCount();
		const DWORD currentTime = GetTickCount();

		if (currentTime - lastUpdate > 50)
		{
			lastUpdate = currentTime;
			const DWORD timeInCycle = (currentTime - cycleStart) % 1000;

			if (timeInCycle > 500)
			{
				std::string randomStr;
				const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
				for (size_t i = 0; i < 9; ++i)
				{
					randomStr += charset[rand() % (sizeof(charset) - 1)];
				}
				MenuTitle = randomStr + " by Kingsley";
			}
			else if (MenuTitle != "Corrupted by Kingsley")
			{
				MenuTitle = "Corrupted by Kingsley";
			}
		}
	}

	void Render() noexcept
	{
		const std::string dynamicTitle = MenuTitle + "###MenuID";

		ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT), ImGuiCond_FirstUseEver);

		if (ImGui::Begin(dynamicTitle.c_str(), &open))
		{
			if (ImGui::BeginTabBar("##MainTabs"))
			{
				if (ImGui::BeginTabItem("Player"))
				{
					pages::RenderPlayer();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Visuals"))
				{
					pages::RenderVisuals();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Misc"))
				{
					pages::RenderMisc();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}

		ImGui::End();
	}
}
