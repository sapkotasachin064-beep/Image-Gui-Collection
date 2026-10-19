#include "gui/pages/pages.hpp"

#include "config/config.hpp"
#include "gui/framework/badge/badge.hpp"
#include "gui/framework/bind/bind.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/colorswatch/colorswatch.hpp"
#include "gui/framework/groupbox/groupbox.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/slider/slider.hpp"

namespace vars
{
    bool visRadarEnabled = true;
    int visRadarZoom = 50;
    bool visRadarBorder = false;
    bool visRadarCompass = true;
    bool visRangeMarkers = true;
    int visRangeMarkerDistance = 50;
    bool visDrawLocalCamera = true;
    float visLocalCameraColor[4] = { 1.f, 0.80f, 0.25f, 1.f };
    bool visDrawLocalPlayer = true;
    float visLocalPlayerColor[4] = { 0.30f, 0.55f, 0.90f, 1.f };
    bool visDrawPlayers = true;
    float visPlayersColor[4] = { 0.85f, 0.45f, 0.18f, 1.f };
    bool visDrawPlayerDistance = true;
    bool visDrawPlayersDirection = true;
    int visPlayerDirectionDistance = 20;

    bool visNodeDrawWorld = false;
    bool visNodeDrawMap = false;
    float visNodeColor[4] = { 0.40f, 0.80f, 0.30f, 1.f };
    int visNodeDepth = 6;
    bool visIncludeStone = true;
    bool visIncludeMetal = true;
    bool visIncludeSulfur = true;

    bool visBoldFont = false;
    bool visWatermark = true;
    bool visActiveFeatures = true;
    bool visFullBright = true;
    int visFullBrightKey = ImGuiKey_K;
    int visFullBrightMode = 0;
    bool visDisableScreenShake = true;
    bool visDrawServerTime = true;
    bool visRaidsWorld = true;
    float visRaidsWorldColor[4] = { 0.85f, 0.45f, 0.18f, 1.f };
    bool visRaidsMap = true;
    float visRaidsMapColor[4] = { 0.85f, 0.45f, 0.18f, 1.f };
    bool visEventCrates = true;
    bool visPatrolMap = true;
    bool visXray = true;
    int visXrayKey = ImGuiKey_X;
    int visXrayMode = 0;
    bool visCombatMode = false;
    int visCombatKey = ImGuiKey_P;
    int visCombatMode2 = 0;

    bool crossEnabled = true;
    float crossColor[4] = { 1.f, 1.f, 1.f, 1.f };
    int crossGap = 2;
    int crossLength = 4;
    int crossHeight = 2;
    bool crossOutline = true;
    int crossWeight = 1;
}

namespace pages
{
    void RenderVisuals() noexcept
    {
        ksd::BeginColumn(0.5f);

        if (ksd::BeginColumnBox("Radar", 0.62f))
        {
            ksd::Checkbox("Radar enabled", &vars::visRadarEnabled);

            ksd::Indent();
            ksd::SliderInt("Radar zoom", &vars::visRadarZoom, 0, 100, "%d%%");
            ksd::Unindent();

            ksd::Checkbox("Radar border", &vars::visRadarBorder);
            ksd::Checkbox("Radar compass", &vars::visRadarCompass);
            ksd::Checkbox("Draw range markers", &vars::visRangeMarkers);

            ksd::Indent();
            ksd::SliderInt("Range marker distance", &vars::visRangeMarkerDistance, 0, 200, "%dm");
            ksd::Unindent();

            ksd::Checkbox("Draw local camera", &vars::visDrawLocalCamera);
            ksd::ColorSwatch("##visCamColor", vars::visLocalCameraColor);
            ksd::Checkbox("Draw local player", &vars::visDrawLocalPlayer);
            ksd::ColorSwatch("##visLocalColor", vars::visLocalPlayerColor);
            ksd::Checkbox("Draw players", &vars::visDrawPlayers);
            ksd::ColorSwatch("##visPlayersColor", vars::visPlayersColor);
            ksd::Checkbox("Draw player distance", &vars::visDrawPlayerDistance);
            ksd::Checkbox("Draw players direction", &vars::visDrawPlayersDirection);

            ksd::Indent();
            ksd::SliderInt("Player direction distance", &vars::visPlayerDirectionDistance, 0, 100, "%dm");
            ksd::Unindent();
        }
        ksd::EndGroupBox();

        if (ksd::BeginColumnBox("Node Path"))
        {
            ksd::Checkbox("Draw in world", &vars::visNodeDrawWorld);
            ksd::StatusBadge(vars::visNodeDrawWorld);

            ksd::Checkbox("Draw on map", &vars::visNodeDrawMap);

            ImGui::TextUnformatted("Node path color");
            ksd::ColorSwatch("##visNodeColor", vars::visNodeColor);

            ksd::SliderInt("Depth", &vars::visNodeDepth, 0, 10);

            ksd::Checkbox("Include stone", &vars::visIncludeStone);
            ksd::Checkbox("Include metal", &vars::visIncludeMetal);
            ksd::Checkbox("Include sulfur", &vars::visIncludeSulfur);
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
        ksd::BeginColumn();

        if (ksd::BeginColumnBox("Other", 0.62f))
        {
            ksd::Checkbox("Bold font", &vars::visBoldFont);
            ksd::Checkbox("Watermark", &vars::visWatermark);
            ksd::Checkbox("Active features", &vars::visActiveFeatures);

            ksd::Checkbox("Full bright", &vars::visFullBright);
            ksd::Bind("##visBrightBind", &vars::visFullBright, &vars::visFullBrightKey, &vars::visFullBrightMode);

            ksd::Checkbox("Disable screen shake", &vars::visDisableScreenShake);
            ksd::Checkbox("Draw server time", &vars::visDrawServerTime);

            ksd::Checkbox("Draw raids in world", &vars::visRaidsWorld);
            ksd::ColorSwatch("##visRaidsWorldColor", vars::visRaidsWorldColor);
            ksd::Checkbox("Draw raids on map", &vars::visRaidsMap);
            ksd::ColorSwatch("##visRaidsMapColor", vars::visRaidsMapColor);

            ksd::Checkbox("Draw event crates on map", &vars::visEventCrates);
            ksd::Checkbox("Draw patrol on map", &vars::visPatrolMap);

            ksd::Checkbox("X-Ray", &vars::visXray);
            ksd::Bind("##visXrayBind", &vars::visXray, &vars::visXrayKey, &vars::visXrayMode);

            ImGui::TextUnformatted("Combat mode");
            ksd::Bind("##visCombatBind", &vars::visCombatMode, &vars::visCombatKey, &vars::visCombatMode2);
        }
        ksd::EndGroupBox();

        if (ksd::BeginColumnBox("Crosshair"))
        {
            ksd::Checkbox("Enabled", &vars::crossEnabled);
            ksd::ColorSwatch("##crossColor", vars::crossColor);

            ksd::Indent();
            ksd::SliderInt("Gap", &vars::crossGap, 0, 20, "%dpx");
            ksd::SliderInt("Length", &vars::crossLength, 0, 20, "%dpx");
            ksd::SliderInt("Height", &vars::crossHeight, 0, 20, "%dpx");
            ksd::Unindent();

            ksd::Checkbox("Outline", &vars::crossOutline);

            ksd::Indent();
            ksd::SliderInt("Weight", &vars::crossWeight, 0, 5, "%dpx");
            ksd::Unindent();
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
    }

    void RegisterVisualsVars(config::VarRegistry& registry)
    {
        registry.AddBool("visRadarEnabled", &vars::visRadarEnabled);
        registry.AddInt("visRadarZoom", &vars::visRadarZoom);
        registry.AddBool("visRadarBorder", &vars::visRadarBorder);
        registry.AddBool("visRadarCompass", &vars::visRadarCompass);
        registry.AddBool("visRangeMarkers", &vars::visRangeMarkers);
        registry.AddInt("visRangeMarkerDistance", &vars::visRangeMarkerDistance);
        registry.AddBool("visDrawLocalCamera", &vars::visDrawLocalCamera);
        registry.AddColor("visLocalCameraColor", vars::visLocalCameraColor);
        registry.AddBool("visDrawLocalPlayer", &vars::visDrawLocalPlayer);
        registry.AddColor("visLocalPlayerColor", vars::visLocalPlayerColor);
        registry.AddBool("visDrawPlayers", &vars::visDrawPlayers);
        registry.AddColor("visPlayersColor", vars::visPlayersColor);
        registry.AddBool("visDrawPlayerDistance", &vars::visDrawPlayerDistance);
        registry.AddBool("visDrawPlayersDirection", &vars::visDrawPlayersDirection);
        registry.AddInt("visPlayerDirectionDistance", &vars::visPlayerDirectionDistance);
        registry.AddBool("visNodeDrawWorld", &vars::visNodeDrawWorld);
        registry.AddBool("visNodeDrawMap", &vars::visNodeDrawMap);
        registry.AddColor("visNodeColor", vars::visNodeColor);
        registry.AddInt("visNodeDepth", &vars::visNodeDepth);
        registry.AddBool("visIncludeStone", &vars::visIncludeStone);
        registry.AddBool("visIncludeMetal", &vars::visIncludeMetal);
        registry.AddBool("visIncludeSulfur", &vars::visIncludeSulfur);
        registry.AddBool("visBoldFont", &vars::visBoldFont);
        registry.AddBool("visWatermark", &vars::visWatermark);
        registry.AddBool("visActiveFeatures", &vars::visActiveFeatures);
        registry.AddBool("visFullBright", &vars::visFullBright);
        registry.AddInt("visFullBrightKey", &vars::visFullBrightKey);
        registry.AddInt("visFullBrightMode", &vars::visFullBrightMode);
        registry.AddBool("visDisableScreenShake", &vars::visDisableScreenShake);
        registry.AddBool("visDrawServerTime", &vars::visDrawServerTime);
        registry.AddBool("visRaidsWorld", &vars::visRaidsWorld);
        registry.AddColor("visRaidsWorldColor", vars::visRaidsWorldColor);
        registry.AddBool("visRaidsMap", &vars::visRaidsMap);
        registry.AddColor("visRaidsMapColor", vars::visRaidsMapColor);
        registry.AddBool("visEventCrates", &vars::visEventCrates);
        registry.AddBool("visPatrolMap", &vars::visPatrolMap);
        registry.AddBool("visXray", &vars::visXray);
        registry.AddInt("visXrayKey", &vars::visXrayKey);
        registry.AddInt("visXrayMode", &vars::visXrayMode);
        registry.AddBool("visCombatMode", &vars::visCombatMode);
        registry.AddInt("visCombatKey", &vars::visCombatKey);
        registry.AddInt("visCombatMode2", &vars::visCombatMode2);
        registry.AddBool("crossEnabled", &vars::crossEnabled);
        registry.AddColor("crossColor", vars::crossColor);
        registry.AddInt("crossGap", &vars::crossGap);
        registry.AddInt("crossLength", &vars::crossLength);
        registry.AddInt("crossHeight", &vars::crossHeight);
        registry.AddBool("crossOutline", &vars::crossOutline);
        registry.AddInt("crossWeight", &vars::crossWeight);
    }
}
