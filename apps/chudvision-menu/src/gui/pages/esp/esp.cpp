#include "gui/pages/pages.hpp"

#include "config/config.hpp"
#include "gui/framework/badge/badge.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/colorswatch/colorswatch.hpp"
#include "gui/framework/groupbox/groupbox.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/list/list.hpp"
#include "gui/framework/slider/slider.hpp"
#include "gui/framework/style/style.hpp"
#include "gui/framework/tabbar/tabbar.hpp"

#include <cstdio>

namespace vars
{
    bool espPlayersEnabled = true;
    int espPlayersDistance = 104;
    bool espNpcsEnabled = true;
    int espNpcsDistance = 62;
    bool espAiEnabled = true;
    int espAiDistance = 146;
    bool espTrapEnabled = true;
    int espTrapDistance = 300;
    bool espVehiclesEnabled = false;
    int espVehiclesDistance = 300;
    bool espResourceEnabled = true;
    int espResourceDistance = 153;
    bool espCratesEnabled = true;
    int espCratesDistance = 300;
    bool espWorldEnabled = true;
    int espWorldDistance = 300;

    bool espShowPlayer = true;
    bool espShowFriendly = true;
    bool espShowEnemy = true;

    bool espPlayerName = true;
    float espPlayerNameColor[4] = { 1.f, 1.f, 1.f, 1.f };
    float espPlayerNameColorAlt[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espPlayerDistance = true;
    float espPlayerDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    float espPlayerDistanceColorAlt[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espPlayerBox = true;
    float espPlayerBoxColor[4] = { 1.f, 1.f, 1.f, 1.f };
    float espPlayerBoxColorAlt[4] = { 0.56f, 0.12f, 0.12f, 1.f };
    bool espPlayerSkeleton = false;
    float espPlayerSkeletonColor[4] = { 1.f, 1.f, 1.f, 1.f };
    float espPlayerSkeletonColorAlt[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espPlayerActiveItem = true;
    float espPlayerActiveItemColor[4] = { 1.f, 1.f, 1.f, 1.f };
    float espPlayerActiveItemColorAlt[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espPlayerTeam = true;
    float espPlayerTeamColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espPlayerRandomizeTeamColors = true;
    bool espPlayerFlags = true;
    bool espPlayerExplosives = true;
    bool espPlayerDrawOnMap = false;
    bool espPlayerOutOfFovArrows = false;
    float espPlayerOutOfFovArrowsColor[4] = { 0.84f, 0.42f, 0.12f, 1.f };
    int espPlayerArrowRadius = 71;
    int espPlayerArrowSize = 5;
    int espPlayerMaxDistance = 216;
    bool espPlayerIgnoreSleepers = true;
    bool espPlayerDrawBelt = true;
    int espPlayerBeltFov = 7;

    bool espShowScientist = true;
    bool espShowHeavyScientist = true;
    bool espShowPeacekeeper = false;
    bool espShowShopkeeper = false;

    bool espNpcName = false;
    float espNpcNameColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espNpcDistance = false;
    float espNpcDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espNpcBox = false;
    float espNpcBoxColor[4] = { 0.35f, 0.55f, 0.85f, 1.f };
    bool espNpcSkeleton = true;
    float espNpcSkeletonColor[4] = { 0.30f, 0.75f, 0.72f, 1.f };
    bool espNpcOverrideDistance = false;
    int espNpcSubDistance = 100;

    bool espShowBradley = true;
    bool espShowPatrol = true;
    bool espShowCargo = false;
    bool espShowBear = true;
    bool espShowHog = true;
    bool espShowChicken = false;
    bool espShowDeer = false;
    bool espShowWolf = true;
    bool espShowShark = false;
    bool espShowPanther = true;
    bool espShowTiger = true;
    bool espShowCrocodile = true;
    bool espShowSnake = false;

    bool espAiName = true;
    float espAiNameColor[4] = { 0.35f, 0.78f, 0.42f, 1.f };
    bool espAiDistanceRow = true;
    float espAiDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espAiHealth = true;
    bool espAiOverrideDistance = false;
    int espAiSubDistance = 100;

    bool espShowAutoTurret = true;
    bool espShowBearTrap = true;
    bool espShowSamSite = true;
    bool espShowShotgunTrap = true;
    bool espShowLandmine = true;
    bool espShowFlameTurret = true;
    bool espShowSpikes = false;

    bool espTrapName = true;
    float espTrapNameColor[4] = { 0.85f, 0.30f, 0.30f, 1.f };
    bool espTrapDistanceRow = true;
    float espTrapDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espTrapHealth = false;
    bool espTrapDrawOffline = false;
    bool espTrapOverrideDistance = true;
    int espTrapSubDistance = 100;

    bool espShowCar = true;
    bool espShowTrain = true;
    bool espShowBoat = false;
    bool espShowTugboat = false;
    bool espShowRhib = false;
    bool espShowHorse = false;
    bool espShowMinicopter = true;
    bool espShowScrapHelicopter = false;
    bool espShowAttackHelicopter = false;
    bool espShowSnowmobile = true;
    bool espShowBike = true;
    bool espShowMotorcycle = true;
    bool espShowKayak = true;
    bool espShowBoogieBoard = true;
    bool espShowDpv = true;
    bool espShowSoloSubmarine = true;
    bool espShowDuoSubmarine = true;

    bool espVehicleName = true;
    float espVehicleNameColor[4] = { 0.72f, 0.70f, 0.95f, 1.f };
    bool espVehicleDistanceRow = true;
    float espVehicleDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espVehicleHealth = false;
    bool espVehicleOverrideDistance = false;
    int espVehicleSubDistance = 100;

    bool espShowStoneOre = false;
    bool espShowMetalOre = false;
    bool espShowSulfurOre = false;
    bool espShowStone = false;
    bool espShowMetal = false;
    bool espShowSulfur = false;
    bool espShowWood = false;
    bool espShowDiesel = false;
    bool espShowHemp = true;
    bool espShowCorn = false;
    bool espShowPotato = true;
    bool espShowPumpkin = false;
    bool espShowMushroom = false;
    bool espShowYellowBerry = false;
    bool espShowBlueBerry = false;
    bool espShowWhiteBerry = false;
    bool espShowGreenBerry = false;
    bool espShowRedBerry = false;
    bool espShowBlackBerry = false;
    bool espShowWoodPile = false;

    bool espResourceName = true;
    float espResourceNameColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espResourceDistanceRow = true;
    float espResourceDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espResourceOverrideDistance = false;
    int espResourceSubDistance = 100;

    bool espShowHackableCrate = true;
    bool espShowBradleyCrate = false;
    bool espShowHeliCrate = false;
    bool espShowEliteCrate = false;
    bool espShowMilitaryCrate = false;
    bool espShowSupplyDrop = true;
    bool espShowBasicCrate = false;
    bool espShowMedicalCrate = false;
    bool espShowYellowCrate = false;
    bool espShowWoodCrate = false;
    bool espShowSmallCrate = false;
    bool espShowTechCrate = false;
    bool espShowAmmoBox = false;
    bool espShowFoodBox = false;
    bool espShowToolbox = false;
    bool espShowCarParts = false;
    bool espShowBarrel = false;
    bool espShowBlueBarrel = false;
    bool espShowFuelBarrel = false;
    bool espShowSunkenLargeBox = false;
    bool espShowSunkenSmallBox = false;
    bool espShowFoodCache = false;

    bool espCrateName = true;
    float espCrateNameColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espCrateDistanceRow = true;
    float espCrateDistanceColor[4] = { 1.f, 1.f, 1.f, 1.f };
    bool espCrateInfo = true;
    bool espCrateOverrideDistance = false;
    int espCrateSubDistance = 100;

    bool espShowItem = true;
    bool espShowPlayerCorpse = true;
    bool espShowScientistCorpse = false;
    bool espShowAnimalCorpse = false;
    bool espShowRecycler = false;
    bool espShowRepairBench = false;
    bool espShowResearchTable = false;
    bool espShowVendingMachine = false;
    bool espShowWorkbench1 = false;
    bool espShowWorkbench2 = false;
    bool espShowWorkbench3 = true;
    bool espShowSmallBox = false;
    bool espShowLargeBox = false;
    bool espShowSleepingBag = false;
    bool espShowSeismicSensor = false;
    bool espShowHbhfSensor = false;
    bool espShowLootBag = false;
    bool espShowRefinery = false;
    bool espShowTc = false;

    bool espWorldWeapon = true;
    float espWorldWeaponColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldConstruction = true;
    float espWorldConstructionColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldItems = true;
    float espWorldItemsColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldResources = true;
    float espWorldResourcesColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldAttire = true;
    float espWorldAttireColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldTool = true;
    float espWorldToolColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldMedical = true;
    float espWorldMedicalColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldFood = true;
    float espWorldFoodColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldAmmunition = true;
    float espWorldAmmunitionColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldTraps = true;
    float espWorldTrapsColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldMisc = true;
    float espWorldMiscColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldComponent = true;
    float espWorldComponentColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldElectrical = true;
    float espWorldElectricalColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldFun = true;
    float espWorldFunColor[4] = { 0.45f, 0.66f, 0.85f, 1.f };
    bool espWorldDistanceRow = true;
    float espWorldDistanceColor[4] = { 0.62f, 0.62f, 0.62f, 1.f };
    bool espWorldOverrideDistance = false;
    int espWorldSubDistance = 100;
    bool espWorldIgnoreArrowsNails = true;
}

namespace pages
{
    static const float kPlayerModifiersHeight = 104.f;
    static const float kNpcModifiersHeight = 64.f;

    static float espListWidth = 0.f;
    static float espPanelWidth = 0.f;

    static ksd::Selection selectedPlayer = { "Player" };
    static ksd::Selection selectedNpc = { "Scientist" };
    static ksd::Selection selectedAi = { "Bradley" };
    static ksd::Selection selectedTrap = { "Auto Turret" };
    static ksd::Selection selectedVehicle = { "Car" };
    static ksd::Selection selectedResource = { "Stone Ore" };
    static ksd::Selection selectedCrate = { "Hackable Crate" };
    static ksd::Selection selectedWorld = { "Item" };

    static ImVec2 EspListSize()
    {
        return ImVec2(espListWidth, ImGui::GetContentRegionAvail().y);
    }

    static ImVec2 EspPanelSize()
    {
        return ImVec2(espPanelWidth, ImGui::GetContentRegionAvail().y);
    }

    static ImVec2 EspPanelSize(float modifiersHeight)
    {
        const float spacing = ImGui::GetStyle().ItemSpacing.y;
        return ImVec2(espPanelWidth, ImGui::GetContentRegionAvail().y - style::Scaled(modifiersHeight) - spacing);
    }

    static ImVec2 EspModifiersSize(float height)
    {
        return ImVec2(espPanelWidth, style::Scaled(height));
    }

    static void EspOverrideRow(bool* isOverriding, int* distance, const char* id)
    {
        ksd::Checkbox("Override max distance", isOverriding);

        if (*isOverriding)
        {
            char idBuffer[64];
            snprintf(idBuffer, sizeof(idBuffer), "Max distance##%s", id);
            ksd::SliderDistance(idBuffer, distance, 300);
        }
    }

    void RenderEsp() noexcept
    {
        const ImVec2 available = ImGui::GetContentRegionAvail();
        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        const float categoriesWidth = available.x * 0.27f;

        espListWidth = available.x * 0.36f;
        espPanelWidth = available.x - categoriesWidth - espListWidth - spacing * 2.f;

        if (ksd::BeginGroupBox("Categories", ImVec2(categoriesWidth, available.y)))
        {
            ksd::Checkbox("Players", &vars::espPlayersEnabled);
            if (vars::espPlayersEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatPlayers", &vars::espPlayersDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("NPCs", &vars::espNpcsEnabled);
            ksd::StatusBadge(vars::espNpcsEnabled);
            if (vars::espNpcsEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatNpcs", &vars::espNpcsDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("AI", &vars::espAiEnabled);
            ksd::StatusBadge(vars::espAiEnabled);
            if (vars::espAiEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatAi", &vars::espAiDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("Trap", &vars::espTrapEnabled);
            ksd::StatusBadge(vars::espTrapEnabled);
            if (vars::espTrapEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatTrap", &vars::espTrapDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("Vehicles", &vars::espVehiclesEnabled);
            ksd::StatusBadge(vars::espVehiclesEnabled);
            if (vars::espVehiclesEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatVehicles", &vars::espVehiclesDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("Resource", &vars::espResourceEnabled);
            ksd::StatusBadge(vars::espResourceEnabled);
            if (vars::espResourceEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatResource", &vars::espResourceDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("Crates", &vars::espCratesEnabled);
            ksd::StatusBadge(vars::espCratesEnabled);
            if (vars::espCratesEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatCrates", &vars::espCratesDistance, 300);
                ksd::Unindent();
            }

            ksd::Checkbox("World", &vars::espWorldEnabled);
            ksd::StatusBadge(vars::espWorldEnabled);
            if (vars::espWorldEnabled)
            {
                ksd::Indent();
                ksd::SliderDistance("Max distance##espCatWorld", &vars::espWorldDistance, 300);
                ksd::Unindent();
            }
        }
        ksd::EndGroupBox();

        ImGui::SameLine();
        ImGui::BeginGroup();

        if (ksd::BeginTabBar("##espCategories"))
        {
            if (ksd::BeginTabItem("Players"))
            {
                if (ksd::BeginGroupBox("Players", EspListSize()))
                {
                    if (ksd::BeginList("##espPlayerItems"))
                    {
                        ksd::ListRow("Player", &vars::espShowPlayer, selectedPlayer);
                        ksd::ListRow("Friendly", &vars::espShowFriendly, selectedPlayer);
                        ksd::ListRow("Enemy", &vars::espShowEnemy, selectedPlayer);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();
                ImGui::BeginGroup();

                if (ksd::BeginGroupBox(selectedPlayer.current, EspPanelSize(kPlayerModifiersHeight)))
                {
                    ksd::Checkbox("Name", &vars::espPlayerName);
                    ksd::ColorSwatch("##espPlayerNameColor", vars::espPlayerNameColor);
                    ksd::ColorSwatch("##espPlayerNameColorAlt", vars::espPlayerNameColorAlt);

                    ksd::Checkbox("Distance", &vars::espPlayerDistance);
                    ksd::ColorSwatch("##espPlayerDistanceColor", vars::espPlayerDistanceColor);
                    ksd::ColorSwatch("##espPlayerDistanceColorAlt", vars::espPlayerDistanceColorAlt);

                    ksd::Checkbox("Box", &vars::espPlayerBox);
                    ksd::ColorSwatch("##espPlayerBoxColor", vars::espPlayerBoxColor);
                    ksd::ColorSwatch("##espPlayerBoxColorAlt", vars::espPlayerBoxColorAlt);

                    ksd::Checkbox("Skeleton", &vars::espPlayerSkeleton);
                    ksd::ColorSwatch("##espPlayerSkeletonColor", vars::espPlayerSkeletonColor);
                    ksd::ColorSwatch("##espPlayerSkeletonColorAlt", vars::espPlayerSkeletonColorAlt);

                    ksd::Checkbox("Active Item", &vars::espPlayerActiveItem);
                    ksd::ColorSwatch("##espPlayerActiveItemColor", vars::espPlayerActiveItemColor);
                    ksd::ColorSwatch("##espPlayerActiveItemColorAlt", vars::espPlayerActiveItemColorAlt);

                    ksd::Checkbox("Team", &vars::espPlayerTeam);
                    ksd::ColorSwatch("##espPlayerTeamColor", vars::espPlayerTeamColor);

                    ksd::Checkbox("Randomize team colors", &vars::espPlayerRandomizeTeamColors);
                    ksd::Checkbox("Flags", &vars::espPlayerFlags);
                    ksd::Checkbox("Explosives", &vars::espPlayerExplosives);
                    ksd::Checkbox("Draw on map", &vars::espPlayerDrawOnMap);

                    ksd::Checkbox("Out of fov arrows", &vars::espPlayerOutOfFovArrows);
                    ksd::ColorSwatch("##espPlayerArrowsColor", vars::espPlayerOutOfFovArrowsColor);

                    ksd::Indent();
                    ksd::SliderInt("Arrow radius", &vars::espPlayerArrowRadius, 0, 100, "%d%%");
                    ksd::SliderInt("Arrow size", &vars::espPlayerArrowSize, 0, 100, "%d%%");
                    ksd::Unindent();
                }
                ksd::EndGroupBox();

                if (ksd::BeginGroupBox("Player modifiers", EspModifiersSize(kPlayerModifiersHeight)))
                {
                    ksd::SliderDistance("Max distance", &vars::espPlayerMaxDistance, 300);

                    ksd::Checkbox("Ignore sleepers", &vars::espPlayerIgnoreSleepers);
                    ksd::StatusBadge(vars::espPlayerIgnoreSleepers);

                    ksd::Checkbox("Draw belt", &vars::espPlayerDrawBelt);
                    ksd::SliderInt("Belt field of view", &vars::espPlayerBeltFov, 0, 180, "%d\xC2\xB0");
                }
                ksd::EndGroupBox();

                ImGui::EndGroup();
                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("NPCs"))
            {
                if (ksd::BeginGroupBox("NPCs", EspListSize()))
                {
                    if (ksd::BeginList("##espNpcItems"))
                    {
                        ksd::ListRow("Scientist", &vars::espShowScientist, selectedNpc);
                        ksd::ListRow("Heavy Scientist", &vars::espShowHeavyScientist, selectedNpc);
                        ksd::ListRow("Peacekeeper", &vars::espShowPeacekeeper, selectedNpc);
                        ksd::ListRow("Shopkeeper", &vars::espShowShopkeeper, selectedNpc);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();
                ImGui::BeginGroup();

                if (ksd::BeginGroupBox(selectedNpc.current, EspPanelSize(kNpcModifiersHeight)))
                {
                    ksd::Checkbox("Name", &vars::espNpcName);
                    ksd::ColorSwatch("##espNpcNameColor", vars::espNpcNameColor);

                    ksd::Checkbox("Distance", &vars::espNpcDistance);
                    ksd::ColorSwatch("##espNpcDistanceColor", vars::espNpcDistanceColor);

                    ksd::Checkbox("Box", &vars::espNpcBox);
                    ksd::ColorSwatch("##espNpcBoxColor", vars::espNpcBoxColor);

                    ksd::Checkbox("Skeleton", &vars::espNpcSkeleton);
                    ksd::ColorSwatch("##espNpcSkeletonColor", vars::espNpcSkeletonColor);
                }
                ksd::EndGroupBox();

                char modifiersTitle[80];
                snprintf(modifiersTitle, sizeof(modifiersTitle), "%s modifiers", selectedNpc.current);

                if (ksd::BeginGroupBox(modifiersTitle, EspModifiersSize(kNpcModifiersHeight)))
                {
                    EspOverrideRow(&vars::espNpcOverrideDistance, &vars::espNpcSubDistance, "espNpcSub");
                }
                ksd::EndGroupBox();

                ImGui::EndGroup();
                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("AI"))
            {
                if (ksd::BeginGroupBox("AI", EspListSize()))
                {
                    if (ksd::BeginList("##espAiItems"))
                    {
                        ksd::ListRow("Bradley", &vars::espShowBradley, selectedAi);
                        ksd::ListRow("Patrol", &vars::espShowPatrol, selectedAi);
                        ksd::ListRow("Cargo", &vars::espShowCargo, selectedAi);
                        ksd::ListRow("Bear", &vars::espShowBear, selectedAi);
                        ksd::ListRow("Hog", &vars::espShowHog, selectedAi);
                        ksd::ListRow("Chicken", &vars::espShowChicken, selectedAi);
                        ksd::ListRow("Deer", &vars::espShowDeer, selectedAi);
                        ksd::ListRow("Wolf", &vars::espShowWolf, selectedAi);
                        ksd::ListRow("Shark", &vars::espShowShark, selectedAi);
                        ksd::ListRow("Panther", &vars::espShowPanther, selectedAi);
                        ksd::ListRow("Tiger", &vars::espShowTiger, selectedAi);
                        ksd::ListRow("Crocodile", &vars::espShowCrocodile, selectedAi);
                        ksd::ListRow("Snake", &vars::espShowSnake, selectedAi);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();

                if (ksd::BeginGroupBox(selectedAi.current, EspPanelSize()))
                {
                    ksd::Checkbox("Name", &vars::espAiName);
                    ksd::ColorSwatch("##espAiNameColor", vars::espAiNameColor);

                    ksd::Checkbox("Distance", &vars::espAiDistanceRow);
                    ksd::ColorSwatch("##espAiDistanceColor", vars::espAiDistanceColor);

                    ksd::Checkbox("Health", &vars::espAiHealth);

                    EspOverrideRow(&vars::espAiOverrideDistance, &vars::espAiSubDistance, "espAiSub");
                }
                ksd::EndGroupBox();

                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("Trap"))
            {
                if (ksd::BeginGroupBox("Trap", EspListSize()))
                {
                    if (ksd::BeginList("##espTrapItems"))
                    {
                        ksd::ListRow("Auto Turret", &vars::espShowAutoTurret, selectedTrap);
                        ksd::ListRow("Bear Trap", &vars::espShowBearTrap, selectedTrap);
                        ksd::ListRow("Sam Site", &vars::espShowSamSite, selectedTrap);
                        ksd::ListRow("Shotgun Trap", &vars::espShowShotgunTrap, selectedTrap);
                        ksd::ListRow("Landmine", &vars::espShowLandmine, selectedTrap);
                        ksd::ListRow("Flame Turret", &vars::espShowFlameTurret, selectedTrap);
                        ksd::ListRow("Spikes", &vars::espShowSpikes, selectedTrap);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();

                if (ksd::BeginGroupBox(selectedTrap.current, EspPanelSize()))
                {
                    ksd::Checkbox("Name", &vars::espTrapName);
                    ksd::ColorSwatch("##espTrapNameColor", vars::espTrapNameColor);

                    ksd::Checkbox("Distance", &vars::espTrapDistanceRow);
                    ksd::ColorSwatch("##espTrapDistanceColor", vars::espTrapDistanceColor);

                    ksd::Checkbox("Health", &vars::espTrapHealth);
                    ksd::Checkbox("Draw offline", &vars::espTrapDrawOffline);

                    EspOverrideRow(&vars::espTrapOverrideDistance, &vars::espTrapSubDistance, "espTrapSub");
                }
                ksd::EndGroupBox();

                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("Vehicles"))
            {
                if (ksd::BeginGroupBox("Vehicles", EspListSize()))
                {
                    if (ksd::BeginList("##espVehicleItems"))
                    {
                        ksd::ListRow("Car", &vars::espShowCar, selectedVehicle);
                        ksd::ListRow("Train", &vars::espShowTrain, selectedVehicle);
                        ksd::ListRow("Boat", &vars::espShowBoat, selectedVehicle);
                        ksd::ListRow("Tugboat", &vars::espShowTugboat, selectedVehicle);
                        ksd::ListRow("RHIB", &vars::espShowRhib, selectedVehicle);
                        ksd::ListRow("Horse", &vars::espShowHorse, selectedVehicle);
                        ksd::ListRow("Minicopter", &vars::espShowMinicopter, selectedVehicle);
                        ksd::ListRow("Scrap Helicopter", &vars::espShowScrapHelicopter, selectedVehicle);
                        ksd::ListRow("Attack Helicopter", &vars::espShowAttackHelicopter, selectedVehicle);
                        ksd::ListRow("Snowmobile", &vars::espShowSnowmobile, selectedVehicle);
                        ksd::ListRow("Bike", &vars::espShowBike, selectedVehicle);
                        ksd::ListRow("Motorcycle", &vars::espShowMotorcycle, selectedVehicle);
                        ksd::ListRow("Kayak", &vars::espShowKayak, selectedVehicle);
                        ksd::ListRow("Boogie Board", &vars::espShowBoogieBoard, selectedVehicle);
                        ksd::ListRow("Dpv", &vars::espShowDpv, selectedVehicle);
                        ksd::ListRow("Solo Submarine", &vars::espShowSoloSubmarine, selectedVehicle);
                        ksd::ListRow("Duo Submarine", &vars::espShowDuoSubmarine, selectedVehicle);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();

                if (ksd::BeginGroupBox(selectedVehicle.current, EspPanelSize()))
                {
                    ksd::Checkbox("Name", &vars::espVehicleName);
                    ksd::ColorSwatch("##espVehicleNameColor", vars::espVehicleNameColor);

                    ksd::Checkbox("Distance", &vars::espVehicleDistanceRow);
                    ksd::ColorSwatch("##espVehicleDistanceColor", vars::espVehicleDistanceColor);

                    ksd::Checkbox("Health", &vars::espVehicleHealth);

                    EspOverrideRow(&vars::espVehicleOverrideDistance, &vars::espVehicleSubDistance, "espVehicleSub");
                }
                ksd::EndGroupBox();

                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("Resource"))
            {
                if (ksd::BeginGroupBox("Resource", EspListSize()))
                {
                    if (ksd::BeginList("##espResourceItems"))
                    {
                        ksd::ListRow("Stone Ore", &vars::espShowStoneOre, selectedResource);
                        ksd::ListRow("Metal Ore", &vars::espShowMetalOre, selectedResource);
                        ksd::ListRow("Sulfur Ore", &vars::espShowSulfurOre, selectedResource);
                        ksd::ListRow("Stone", &vars::espShowStone, selectedResource);
                        ksd::ListRow("Metal", &vars::espShowMetal, selectedResource);
                        ksd::ListRow("Sulfur", &vars::espShowSulfur, selectedResource);
                        ksd::ListRow("Wood", &vars::espShowWood, selectedResource);
                        ksd::ListRow("Diesel", &vars::espShowDiesel, selectedResource);
                        ksd::ListRow("Hemp", &vars::espShowHemp, selectedResource);
                        ksd::ListRow("Corn", &vars::espShowCorn, selectedResource);
                        ksd::ListRow("Potato", &vars::espShowPotato, selectedResource);
                        ksd::ListRow("Pumpkin", &vars::espShowPumpkin, selectedResource);
                        ksd::ListRow("Mushroom", &vars::espShowMushroom, selectedResource);
                        ksd::ListRow("Yellow Berry", &vars::espShowYellowBerry, selectedResource);
                        ksd::ListRow("Blue Berry", &vars::espShowBlueBerry, selectedResource);
                        ksd::ListRow("White Berry", &vars::espShowWhiteBerry, selectedResource);
                        ksd::ListRow("Green Berry", &vars::espShowGreenBerry, selectedResource);
                        ksd::ListRow("Red Berry", &vars::espShowRedBerry, selectedResource);
                        ksd::ListRow("Black Berry", &vars::espShowBlackBerry, selectedResource);
                        ksd::ListRow("Wood Pile", &vars::espShowWoodPile, selectedResource);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();

                if (ksd::BeginGroupBox(selectedResource.current, EspPanelSize()))
                {
                    ksd::Checkbox("Name", &vars::espResourceName);
                    ksd::ColorSwatch("##espResourceNameColor", vars::espResourceNameColor);

                    ksd::Checkbox("Distance", &vars::espResourceDistanceRow);
                    ksd::ColorSwatch("##espResourceDistanceColor", vars::espResourceDistanceColor);

                    EspOverrideRow(&vars::espResourceOverrideDistance, &vars::espResourceSubDistance, "espResourceSub");
                }
                ksd::EndGroupBox();

                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("Crates"))
            {
                if (ksd::BeginGroupBox("Crates", EspListSize()))
                {
                    if (ksd::BeginList("##espCrateItems"))
                    {
                        ksd::ListRow("Hackable Crate", &vars::espShowHackableCrate, selectedCrate);
                        ksd::ListRow("Bradley Crate", &vars::espShowBradleyCrate, selectedCrate);
                        ksd::ListRow("Heli Crate", &vars::espShowHeliCrate, selectedCrate);
                        ksd::ListRow("Elite Crate", &vars::espShowEliteCrate, selectedCrate);
                        ksd::ListRow("Military Crate", &vars::espShowMilitaryCrate, selectedCrate);
                        ksd::ListRow("Supply Drop", &vars::espShowSupplyDrop, selectedCrate);
                        ksd::ListRow("Basic Crate", &vars::espShowBasicCrate, selectedCrate);
                        ksd::ListRow("Medical Crate", &vars::espShowMedicalCrate, selectedCrate);
                        ksd::ListRow("Yellow Crate", &vars::espShowYellowCrate, selectedCrate);
                        ksd::ListRow("Wood Crate", &vars::espShowWoodCrate, selectedCrate);
                        ksd::ListRow("Small Crate", &vars::espShowSmallCrate, selectedCrate);
                        ksd::ListRow("Tech Crate", &vars::espShowTechCrate, selectedCrate);
                        ksd::ListRow("Ammo Box", &vars::espShowAmmoBox, selectedCrate);
                        ksd::ListRow("Food Box", &vars::espShowFoodBox, selectedCrate);
                        ksd::ListRow("Toolbox", &vars::espShowToolbox, selectedCrate);
                        ksd::ListRow("Car Parts", &vars::espShowCarParts, selectedCrate);
                        ksd::ListRow("Barrel", &vars::espShowBarrel, selectedCrate);
                        ksd::ListRow("Blue Barrel", &vars::espShowBlueBarrel, selectedCrate);
                        ksd::ListRow("Fuel Barrel", &vars::espShowFuelBarrel, selectedCrate);
                        ksd::ListRow("Sunken Large Box", &vars::espShowSunkenLargeBox, selectedCrate);
                        ksd::ListRow("Sunken Small Box", &vars::espShowSunkenSmallBox, selectedCrate);
                        ksd::ListRow("Food Cache", &vars::espShowFoodCache, selectedCrate);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();

                if (ksd::BeginGroupBox(selectedCrate.current, EspPanelSize()))
                {
                    ksd::Checkbox("Name", &vars::espCrateName);
                    ksd::ColorSwatch("##espCrateNameColor", vars::espCrateNameColor);

                    ksd::Checkbox("Distance", &vars::espCrateDistanceRow);
                    ksd::ColorSwatch("##espCrateDistanceColor", vars::espCrateDistanceColor);

                    ksd::Checkbox("Info", &vars::espCrateInfo);

                    EspOverrideRow(&vars::espCrateOverrideDistance, &vars::espCrateSubDistance, "espCrateSub");
                }
                ksd::EndGroupBox();

                ksd::EndTabItem();
            }

            if (ksd::BeginTabItem("World"))
            {
                if (ksd::BeginGroupBox("World", EspListSize()))
                {
                    if (ksd::BeginList("##espWorldItems"))
                    {
                        ksd::ListRow("Item", &vars::espShowItem, selectedWorld);
                        ksd::ListRow("Player Corpse", &vars::espShowPlayerCorpse, selectedWorld);
                        ksd::ListRow("Scientist Corpse", &vars::espShowScientistCorpse, selectedWorld);
                        ksd::ListRow("Animal Corpse", &vars::espShowAnimalCorpse, selectedWorld);
                        ksd::ListRow("Recycler", &vars::espShowRecycler, selectedWorld);
                        ksd::ListRow("Repair Bench", &vars::espShowRepairBench, selectedWorld);
                        ksd::ListRow("Research Table", &vars::espShowResearchTable, selectedWorld);
                        ksd::ListRow("Vending Machine", &vars::espShowVendingMachine, selectedWorld);
                        ksd::ListRow("Workbench 1", &vars::espShowWorkbench1, selectedWorld);
                        ksd::ListRow("Workbench 2", &vars::espShowWorkbench2, selectedWorld);
                        ksd::ListRow("Workbench 3", &vars::espShowWorkbench3, selectedWorld);
                        ksd::ListRow("Small Box", &vars::espShowSmallBox, selectedWorld);
                        ksd::ListRow("Large Box", &vars::espShowLargeBox, selectedWorld);
                        ksd::ListRow("Sleeping Bag", &vars::espShowSleepingBag, selectedWorld);
                        ksd::ListRow("Seismic Sensor", &vars::espShowSeismicSensor, selectedWorld);
                        ksd::ListRow("HBHF Sensor", &vars::espShowHbhfSensor, selectedWorld);
                        ksd::ListRow("Loot Bag", &vars::espShowLootBag, selectedWorld);
                        ksd::ListRow("Refinery", &vars::espShowRefinery, selectedWorld);
                        ksd::ListRow("Tc", &vars::espShowTc, selectedWorld);
                    }
                    ksd::EndList();
                }
                ksd::EndGroupBox();

                ImGui::SameLine();

                if (ksd::BeginGroupBox(selectedWorld.current, EspPanelSize()))
                {
                    ksd::Checkbox("Weapon", &vars::espWorldWeapon);
                    ksd::ColorSwatch("##espWorldWeaponColor", vars::espWorldWeaponColor);

                    ksd::Checkbox("Construction", &vars::espWorldConstruction);
                    ksd::ColorSwatch("##espWorldConstructionColor", vars::espWorldConstructionColor);

                    ksd::Checkbox("Items", &vars::espWorldItems);
                    ksd::ColorSwatch("##espWorldItemsColor", vars::espWorldItemsColor);

                    ksd::Checkbox("Resources", &vars::espWorldResources);
                    ksd::ColorSwatch("##espWorldResourcesColor", vars::espWorldResourcesColor);

                    ksd::Checkbox("Attire", &vars::espWorldAttire);
                    ksd::ColorSwatch("##espWorldAttireColor", vars::espWorldAttireColor);

                    ksd::Checkbox("Tool", &vars::espWorldTool);
                    ksd::ColorSwatch("##espWorldToolColor", vars::espWorldToolColor);

                    ksd::Checkbox("Medical", &vars::espWorldMedical);
                    ksd::ColorSwatch("##espWorldMedicalColor", vars::espWorldMedicalColor);

                    ksd::Checkbox("Food", &vars::espWorldFood);
                    ksd::ColorSwatch("##espWorldFoodColor", vars::espWorldFoodColor);

                    ksd::Checkbox("Ammunition", &vars::espWorldAmmunition);
                    ksd::ColorSwatch("##espWorldAmmunitionColor", vars::espWorldAmmunitionColor);

                    ksd::Checkbox("Traps", &vars::espWorldTraps);
                    ksd::ColorSwatch("##espWorldTrapsColor", vars::espWorldTrapsColor);

                    ksd::Checkbox("Misc", &vars::espWorldMisc);
                    ksd::ColorSwatch("##espWorldMiscColor", vars::espWorldMiscColor);

                    ksd::Checkbox("Component", &vars::espWorldComponent);
                    ksd::ColorSwatch("##espWorldComponentColor", vars::espWorldComponentColor);

                    ksd::Checkbox("Electrical", &vars::espWorldElectrical);
                    ksd::ColorSwatch("##espWorldElectricalColor", vars::espWorldElectricalColor);

                    ksd::Checkbox("Fun", &vars::espWorldFun);
                    ksd::ColorSwatch("##espWorldFunColor", vars::espWorldFunColor);

                    ksd::Checkbox("Distance", &vars::espWorldDistanceRow);
                    ksd::ColorSwatch("##espWorldDistanceColor", vars::espWorldDistanceColor);

                    EspOverrideRow(&vars::espWorldOverrideDistance, &vars::espWorldSubDistance, "espWorldSub");

                    ksd::Checkbox("Ignore single arrows & nails", &vars::espWorldIgnoreArrowsNails);
                }
                ksd::EndGroupBox();

                ksd::EndTabItem();
            }

            ksd::EndTabBar();
        }

        ImGui::EndGroup();
    }

    void RegisterEspVars(config::VarRegistry& registry)
    {
        registry.AddBool("espPlayersEnabled", &vars::espPlayersEnabled);
        registry.AddInt("espPlayersDistance", &vars::espPlayersDistance);
        registry.AddBool("espNpcsEnabled", &vars::espNpcsEnabled);
        registry.AddInt("espNpcsDistance", &vars::espNpcsDistance);
        registry.AddBool("espAiEnabled", &vars::espAiEnabled);
        registry.AddInt("espAiDistance", &vars::espAiDistance);
        registry.AddBool("espTrapEnabled", &vars::espTrapEnabled);
        registry.AddInt("espTrapDistance", &vars::espTrapDistance);
        registry.AddBool("espVehiclesEnabled", &vars::espVehiclesEnabled);
        registry.AddInt("espVehiclesDistance", &vars::espVehiclesDistance);
        registry.AddBool("espResourceEnabled", &vars::espResourceEnabled);
        registry.AddInt("espResourceDistance", &vars::espResourceDistance);
        registry.AddBool("espCratesEnabled", &vars::espCratesEnabled);
        registry.AddInt("espCratesDistance", &vars::espCratesDistance);
        registry.AddBool("espWorldEnabled", &vars::espWorldEnabled);
        registry.AddInt("espWorldDistance", &vars::espWorldDistance);

        registry.AddBool("espShowPlayer", &vars::espShowPlayer);
        registry.AddBool("espShowFriendly", &vars::espShowFriendly);
        registry.AddBool("espShowEnemy", &vars::espShowEnemy);

        registry.AddBool("espPlayerName", &vars::espPlayerName);
        registry.AddColor("espPlayerNameColor", vars::espPlayerNameColor);
        registry.AddColor("espPlayerNameColorAlt", vars::espPlayerNameColorAlt);
        registry.AddBool("espPlayerDistance", &vars::espPlayerDistance);
        registry.AddColor("espPlayerDistanceColor", vars::espPlayerDistanceColor);
        registry.AddColor("espPlayerDistanceColorAlt", vars::espPlayerDistanceColorAlt);
        registry.AddBool("espPlayerBox", &vars::espPlayerBox);
        registry.AddColor("espPlayerBoxColor", vars::espPlayerBoxColor);
        registry.AddColor("espPlayerBoxColorAlt", vars::espPlayerBoxColorAlt);
        registry.AddBool("espPlayerSkeleton", &vars::espPlayerSkeleton);
        registry.AddColor("espPlayerSkeletonColor", vars::espPlayerSkeletonColor);
        registry.AddColor("espPlayerSkeletonColorAlt", vars::espPlayerSkeletonColorAlt);
        registry.AddBool("espPlayerActiveItem", &vars::espPlayerActiveItem);
        registry.AddColor("espPlayerActiveItemColor", vars::espPlayerActiveItemColor);
        registry.AddColor("espPlayerActiveItemColorAlt", vars::espPlayerActiveItemColorAlt);
        registry.AddBool("espPlayerTeam", &vars::espPlayerTeam);
        registry.AddColor("espPlayerTeamColor", vars::espPlayerTeamColor);
        registry.AddBool("espPlayerRandomizeTeamColors", &vars::espPlayerRandomizeTeamColors);
        registry.AddBool("espPlayerFlags", &vars::espPlayerFlags);
        registry.AddBool("espPlayerExplosives", &vars::espPlayerExplosives);
        registry.AddBool("espPlayerDrawOnMap", &vars::espPlayerDrawOnMap);
        registry.AddBool("espPlayerOutOfFovArrows", &vars::espPlayerOutOfFovArrows);
        registry.AddColor("espPlayerOutOfFovArrowsColor", vars::espPlayerOutOfFovArrowsColor);
        registry.AddInt("espPlayerArrowRadius", &vars::espPlayerArrowRadius);
        registry.AddInt("espPlayerArrowSize", &vars::espPlayerArrowSize);
        registry.AddInt("espPlayerMaxDistance", &vars::espPlayerMaxDistance);
        registry.AddBool("espPlayerIgnoreSleepers", &vars::espPlayerIgnoreSleepers);
        registry.AddBool("espPlayerDrawBelt", &vars::espPlayerDrawBelt);
        registry.AddInt("espPlayerBeltFov", &vars::espPlayerBeltFov);

        registry.AddBool("espShowScientist", &vars::espShowScientist);
        registry.AddBool("espShowHeavyScientist", &vars::espShowHeavyScientist);
        registry.AddBool("espShowPeacekeeper", &vars::espShowPeacekeeper);
        registry.AddBool("espShowShopkeeper", &vars::espShowShopkeeper);

        registry.AddBool("espNpcName", &vars::espNpcName);
        registry.AddColor("espNpcNameColor", vars::espNpcNameColor);
        registry.AddBool("espNpcDistance", &vars::espNpcDistance);
        registry.AddColor("espNpcDistanceColor", vars::espNpcDistanceColor);
        registry.AddBool("espNpcBox", &vars::espNpcBox);
        registry.AddColor("espNpcBoxColor", vars::espNpcBoxColor);
        registry.AddBool("espNpcSkeleton", &vars::espNpcSkeleton);
        registry.AddColor("espNpcSkeletonColor", vars::espNpcSkeletonColor);
        registry.AddBool("espNpcOverrideDistance", &vars::espNpcOverrideDistance);
        registry.AddInt("espNpcSubDistance", &vars::espNpcSubDistance);

        registry.AddBool("espShowBradley", &vars::espShowBradley);
        registry.AddBool("espShowPatrol", &vars::espShowPatrol);
        registry.AddBool("espShowCargo", &vars::espShowCargo);
        registry.AddBool("espShowBear", &vars::espShowBear);
        registry.AddBool("espShowHog", &vars::espShowHog);
        registry.AddBool("espShowChicken", &vars::espShowChicken);
        registry.AddBool("espShowDeer", &vars::espShowDeer);
        registry.AddBool("espShowWolf", &vars::espShowWolf);
        registry.AddBool("espShowShark", &vars::espShowShark);
        registry.AddBool("espShowPanther", &vars::espShowPanther);
        registry.AddBool("espShowTiger", &vars::espShowTiger);
        registry.AddBool("espShowCrocodile", &vars::espShowCrocodile);
        registry.AddBool("espShowSnake", &vars::espShowSnake);

        registry.AddBool("espAiName", &vars::espAiName);
        registry.AddColor("espAiNameColor", vars::espAiNameColor);
        registry.AddBool("espAiDistanceRow", &vars::espAiDistanceRow);
        registry.AddColor("espAiDistanceColor", vars::espAiDistanceColor);
        registry.AddBool("espAiHealth", &vars::espAiHealth);
        registry.AddBool("espAiOverrideDistance", &vars::espAiOverrideDistance);
        registry.AddInt("espAiSubDistance", &vars::espAiSubDistance);

        registry.AddBool("espShowAutoTurret", &vars::espShowAutoTurret);
        registry.AddBool("espShowBearTrap", &vars::espShowBearTrap);
        registry.AddBool("espShowSamSite", &vars::espShowSamSite);
        registry.AddBool("espShowShotgunTrap", &vars::espShowShotgunTrap);
        registry.AddBool("espShowLandmine", &vars::espShowLandmine);
        registry.AddBool("espShowFlameTurret", &vars::espShowFlameTurret);
        registry.AddBool("espShowSpikes", &vars::espShowSpikes);

        registry.AddBool("espTrapName", &vars::espTrapName);
        registry.AddColor("espTrapNameColor", vars::espTrapNameColor);
        registry.AddBool("espTrapDistanceRow", &vars::espTrapDistanceRow);
        registry.AddColor("espTrapDistanceColor", vars::espTrapDistanceColor);
        registry.AddBool("espTrapHealth", &vars::espTrapHealth);
        registry.AddBool("espTrapDrawOffline", &vars::espTrapDrawOffline);
        registry.AddBool("espTrapOverrideDistance", &vars::espTrapOverrideDistance);
        registry.AddInt("espTrapSubDistance", &vars::espTrapSubDistance);

        registry.AddBool("espShowCar", &vars::espShowCar);
        registry.AddBool("espShowTrain", &vars::espShowTrain);
        registry.AddBool("espShowBoat", &vars::espShowBoat);
        registry.AddBool("espShowTugboat", &vars::espShowTugboat);
        registry.AddBool("espShowRhib", &vars::espShowRhib);
        registry.AddBool("espShowHorse", &vars::espShowHorse);
        registry.AddBool("espShowMinicopter", &vars::espShowMinicopter);
        registry.AddBool("espShowScrapHelicopter", &vars::espShowScrapHelicopter);
        registry.AddBool("espShowAttackHelicopter", &vars::espShowAttackHelicopter);
        registry.AddBool("espShowSnowmobile", &vars::espShowSnowmobile);
        registry.AddBool("espShowBike", &vars::espShowBike);
        registry.AddBool("espShowMotorcycle", &vars::espShowMotorcycle);
        registry.AddBool("espShowKayak", &vars::espShowKayak);
        registry.AddBool("espShowBoogieBoard", &vars::espShowBoogieBoard);
        registry.AddBool("espShowDpv", &vars::espShowDpv);
        registry.AddBool("espShowSoloSubmarine", &vars::espShowSoloSubmarine);
        registry.AddBool("espShowDuoSubmarine", &vars::espShowDuoSubmarine);

        registry.AddBool("espVehicleName", &vars::espVehicleName);
        registry.AddColor("espVehicleNameColor", vars::espVehicleNameColor);
        registry.AddBool("espVehicleDistanceRow", &vars::espVehicleDistanceRow);
        registry.AddColor("espVehicleDistanceColor", vars::espVehicleDistanceColor);
        registry.AddBool("espVehicleHealth", &vars::espVehicleHealth);
        registry.AddBool("espVehicleOverrideDistance", &vars::espVehicleOverrideDistance);
        registry.AddInt("espVehicleSubDistance", &vars::espVehicleSubDistance);

        registry.AddBool("espShowStoneOre", &vars::espShowStoneOre);
        registry.AddBool("espShowMetalOre", &vars::espShowMetalOre);
        registry.AddBool("espShowSulfurOre", &vars::espShowSulfurOre);
        registry.AddBool("espShowStone", &vars::espShowStone);
        registry.AddBool("espShowMetal", &vars::espShowMetal);
        registry.AddBool("espShowSulfur", &vars::espShowSulfur);
        registry.AddBool("espShowWood", &vars::espShowWood);
        registry.AddBool("espShowDiesel", &vars::espShowDiesel);
        registry.AddBool("espShowHemp", &vars::espShowHemp);
        registry.AddBool("espShowCorn", &vars::espShowCorn);
        registry.AddBool("espShowPotato", &vars::espShowPotato);
        registry.AddBool("espShowPumpkin", &vars::espShowPumpkin);
        registry.AddBool("espShowMushroom", &vars::espShowMushroom);
        registry.AddBool("espShowYellowBerry", &vars::espShowYellowBerry);
        registry.AddBool("espShowBlueBerry", &vars::espShowBlueBerry);
        registry.AddBool("espShowWhiteBerry", &vars::espShowWhiteBerry);
        registry.AddBool("espShowGreenBerry", &vars::espShowGreenBerry);
        registry.AddBool("espShowRedBerry", &vars::espShowRedBerry);
        registry.AddBool("espShowBlackBerry", &vars::espShowBlackBerry);
        registry.AddBool("espShowWoodPile", &vars::espShowWoodPile);

        registry.AddBool("espResourceName", &vars::espResourceName);
        registry.AddColor("espResourceNameColor", vars::espResourceNameColor);
        registry.AddBool("espResourceDistanceRow", &vars::espResourceDistanceRow);
        registry.AddColor("espResourceDistanceColor", vars::espResourceDistanceColor);
        registry.AddBool("espResourceOverrideDistance", &vars::espResourceOverrideDistance);
        registry.AddInt("espResourceSubDistance", &vars::espResourceSubDistance);

        registry.AddBool("espShowHackableCrate", &vars::espShowHackableCrate);
        registry.AddBool("espShowBradleyCrate", &vars::espShowBradleyCrate);
        registry.AddBool("espShowHeliCrate", &vars::espShowHeliCrate);
        registry.AddBool("espShowEliteCrate", &vars::espShowEliteCrate);
        registry.AddBool("espShowMilitaryCrate", &vars::espShowMilitaryCrate);
        registry.AddBool("espShowSupplyDrop", &vars::espShowSupplyDrop);
        registry.AddBool("espShowBasicCrate", &vars::espShowBasicCrate);
        registry.AddBool("espShowMedicalCrate", &vars::espShowMedicalCrate);
        registry.AddBool("espShowYellowCrate", &vars::espShowYellowCrate);
        registry.AddBool("espShowWoodCrate", &vars::espShowWoodCrate);
        registry.AddBool("espShowSmallCrate", &vars::espShowSmallCrate);
        registry.AddBool("espShowTechCrate", &vars::espShowTechCrate);
        registry.AddBool("espShowAmmoBox", &vars::espShowAmmoBox);
        registry.AddBool("espShowFoodBox", &vars::espShowFoodBox);
        registry.AddBool("espShowToolbox", &vars::espShowToolbox);
        registry.AddBool("espShowCarParts", &vars::espShowCarParts);
        registry.AddBool("espShowBarrel", &vars::espShowBarrel);
        registry.AddBool("espShowBlueBarrel", &vars::espShowBlueBarrel);
        registry.AddBool("espShowFuelBarrel", &vars::espShowFuelBarrel);
        registry.AddBool("espShowSunkenLargeBox", &vars::espShowSunkenLargeBox);
        registry.AddBool("espShowSunkenSmallBox", &vars::espShowSunkenSmallBox);
        registry.AddBool("espShowFoodCache", &vars::espShowFoodCache);

        registry.AddBool("espCrateName", &vars::espCrateName);
        registry.AddColor("espCrateNameColor", vars::espCrateNameColor);
        registry.AddBool("espCrateDistanceRow", &vars::espCrateDistanceRow);
        registry.AddColor("espCrateDistanceColor", vars::espCrateDistanceColor);
        registry.AddBool("espCrateInfo", &vars::espCrateInfo);
        registry.AddBool("espCrateOverrideDistance", &vars::espCrateOverrideDistance);
        registry.AddInt("espCrateSubDistance", &vars::espCrateSubDistance);

        registry.AddBool("espShowItem", &vars::espShowItem);
        registry.AddBool("espShowPlayerCorpse", &vars::espShowPlayerCorpse);
        registry.AddBool("espShowScientistCorpse", &vars::espShowScientistCorpse);
        registry.AddBool("espShowAnimalCorpse", &vars::espShowAnimalCorpse);
        registry.AddBool("espShowRecycler", &vars::espShowRecycler);
        registry.AddBool("espShowRepairBench", &vars::espShowRepairBench);
        registry.AddBool("espShowResearchTable", &vars::espShowResearchTable);
        registry.AddBool("espShowVendingMachine", &vars::espShowVendingMachine);
        registry.AddBool("espShowWorkbench1", &vars::espShowWorkbench1);
        registry.AddBool("espShowWorkbench2", &vars::espShowWorkbench2);
        registry.AddBool("espShowWorkbench3", &vars::espShowWorkbench3);
        registry.AddBool("espShowSmallBox", &vars::espShowSmallBox);
        registry.AddBool("espShowLargeBox", &vars::espShowLargeBox);
        registry.AddBool("espShowSleepingBag", &vars::espShowSleepingBag);
        registry.AddBool("espShowSeismicSensor", &vars::espShowSeismicSensor);
        registry.AddBool("espShowHbhfSensor", &vars::espShowHbhfSensor);
        registry.AddBool("espShowLootBag", &vars::espShowLootBag);
        registry.AddBool("espShowRefinery", &vars::espShowRefinery);
        registry.AddBool("espShowTc", &vars::espShowTc);

        registry.AddBool("espWorldWeapon", &vars::espWorldWeapon);
        registry.AddColor("espWorldWeaponColor", vars::espWorldWeaponColor);
        registry.AddBool("espWorldConstruction", &vars::espWorldConstruction);
        registry.AddColor("espWorldConstructionColor", vars::espWorldConstructionColor);
        registry.AddBool("espWorldItems", &vars::espWorldItems);
        registry.AddColor("espWorldItemsColor", vars::espWorldItemsColor);
        registry.AddBool("espWorldResources", &vars::espWorldResources);
        registry.AddColor("espWorldResourcesColor", vars::espWorldResourcesColor);
        registry.AddBool("espWorldAttire", &vars::espWorldAttire);
        registry.AddColor("espWorldAttireColor", vars::espWorldAttireColor);
        registry.AddBool("espWorldTool", &vars::espWorldTool);
        registry.AddColor("espWorldToolColor", vars::espWorldToolColor);
        registry.AddBool("espWorldMedical", &vars::espWorldMedical);
        registry.AddColor("espWorldMedicalColor", vars::espWorldMedicalColor);
        registry.AddBool("espWorldFood", &vars::espWorldFood);
        registry.AddColor("espWorldFoodColor", vars::espWorldFoodColor);
        registry.AddBool("espWorldAmmunition", &vars::espWorldAmmunition);
        registry.AddColor("espWorldAmmunitionColor", vars::espWorldAmmunitionColor);
        registry.AddBool("espWorldTraps", &vars::espWorldTraps);
        registry.AddColor("espWorldTrapsColor", vars::espWorldTrapsColor);
        registry.AddBool("espWorldMisc", &vars::espWorldMisc);
        registry.AddColor("espWorldMiscColor", vars::espWorldMiscColor);
        registry.AddBool("espWorldComponent", &vars::espWorldComponent);
        registry.AddColor("espWorldComponentColor", vars::espWorldComponentColor);
        registry.AddBool("espWorldElectrical", &vars::espWorldElectrical);
        registry.AddColor("espWorldElectricalColor", vars::espWorldElectricalColor);
        registry.AddBool("espWorldFun", &vars::espWorldFun);
        registry.AddColor("espWorldFunColor", vars::espWorldFunColor);
        registry.AddBool("espWorldDistanceRow", &vars::espWorldDistanceRow);
        registry.AddColor("espWorldDistanceColor", vars::espWorldDistanceColor);
        registry.AddBool("espWorldOverrideDistance", &vars::espWorldOverrideDistance);
        registry.AddInt("espWorldSubDistance", &vars::espWorldSubDistance);
        registry.AddBool("espWorldIgnoreArrowsNails", &vars::espWorldIgnoreArrowsNails);
    }
}
