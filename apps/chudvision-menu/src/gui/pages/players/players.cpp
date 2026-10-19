#include "gui/pages/pages.hpp"

#include "config/config.hpp"
#include "gui/framework/button/button.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/colorswatch/colorswatch.hpp"
#include "gui/framework/groupbox/groupbox.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/list/list.hpp"
#include "gui/framework/style/style.hpp"

namespace vars
{
    bool playersWatch[6];
    bool playersHighlight = true;
    float playersHighlightColor[4] = { 0.85f, 0.45f, 0.18f, 1.f };
    bool playersIgnore = false;
    bool playersDrawOnMap = true;
    bool playersNotifyOnline = false;
}

namespace pages
{
    static const char* const kSamplePlayers[6] = { "Local Player", "survivor882", "TwitchRust", "kn0cker", "sleeper (roof)", "scientist_np" };

    void RenderPlayers() noexcept
    {
        static ksd::Selection selectedPlayer = { "Local Player" };

        ksd::BeginColumn(0.46f);

        if (ksd::BeginColumnBox("Players"))
        {
            if (ksd::BeginList("##playerList"))
            {
                for (int index = 0; index < 6; index++)
                {
                    ksd::ListRow(kSamplePlayers[index], &vars::playersWatch[index], selectedPlayer);
                }
            }
            ksd::EndList();
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
        ksd::BeginColumn();

        if (ksd::BeginColumnBox(selectedPlayer.current))
        {
            ksd::Checkbox("Highlight player", &vars::playersHighlight);
            ksd::ColorSwatch("##playersHighlightColor", vars::playersHighlightColor);

            ksd::Checkbox("Ignore player", &vars::playersIgnore);
            ksd::Checkbox("Draw on map", &vars::playersDrawOnMap);
            ksd::Checkbox("Notify when online", &vars::playersNotifyOnline);

            ImGui::Spacing();
            ksd::Button("Add to friends", ImVec2(style::Scaled(90.f), style::Scaled(16.f)));
            ksd::Button("Copy name", ImVec2(style::Scaled(90.f), style::Scaled(16.f)));
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
    }

    void RegisterPlayersVars(config::VarRegistry& registry)
    {
        registry.AddBool("playersHighlight", &vars::playersHighlight);
        registry.AddColor("playersHighlightColor", vars::playersHighlightColor);
        registry.AddBool("playersIgnore", &vars::playersIgnore);
        registry.AddBool("playersDrawOnMap", &vars::playersDrawOnMap);
        registry.AddBool("playersNotifyOnline", &vars::playersNotifyOnline);
    }
}
