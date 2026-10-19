#include "menu.h"
#include "var.h"
#include "custom_settings.h"
#include "custom_imgui.h"
#include "ksdDynamicTitle.h"
#include "imgui.h"
#include "iconsfontawesome/IconsFontAwesome6.h"

namespace
{
// kingsleydotdev
using namespace MenuVars;

enum class MenuPage
{
    Aimbot,
    Visuals,
    Other,
    Exploits,
    Config,
    About,
    Count
};

constexpr const char* kEntryText[(int)MenuPage::Count] =
{
    "Aimbot",
    "Visuals",
    "Other",
    "Exploits",
    "Config",
    "About"
};

constexpr const char* kEntryIcon[(int)MenuPage::Count] =
{
    ICON_FA_CROSSHAIRS,
    ICON_FA_EYE,
    ICON_FA_SLIDERS,
    ICON_FA_BUG,
    ICON_FA_GEAR,
    ICON_FA_CIRCLE_INFO
};

constexpr float fPageFadeHalfSeconds = 0.15f;
constexpr float fPageSlideMaxPx = 48.f;
constexpr float fCollapsibleHeaderHeight = 26.f;
constexpr float fSectionWidthPx = 220.f;

void UpdatePageFade()
{
    const float fDt = ImGui::GetIO().DeltaTime;
    if (ePageFade == PageFadeState::Idle)
    {
        if (giCurrentPage != giShownPage)
            ePageFade = PageFadeState::FadeOut;
        return;
    }
    if (ePageFade == PageFadeState::FadeOut)
    {
        gfPageAlpha -= fDt / fPageFadeHalfSeconds;
        if (gfPageAlpha <= 0.f)
        {
            gfPageAlpha = 0.f;
            giShownPage = giCurrentPage;
            ePageFade = PageFadeState::FadeIn;
        }
        return;
    }
    if (ePageFade == PageFadeState::FadeIn)
    {
        gfPageAlpha += fDt / fPageFadeHalfSeconds;
        if (gfPageAlpha >= 1.f)
        {
            gfPageAlpha = 1.f;
            ePageFade = PageFadeState::Idle;
        }
    }
}

void RenderPageContent()
{
    switch ((MenuPage)giShownPage)
    {
    case MenuPage::Aimbot:
    {
        const float fGroupBoxHeightPx = 190.f;
        const float fGridPadLeft = 0.f;
        const float fGridPadMiddle = 14.f;
        const float fGridPadRight = 14.f;
        const float fGridBaseX = ImGui::GetCursorPosX();
        const float fGridStartY = ImGui::GetCursorPos().y;
        float fGridContentW = ImGui::GetContentRegionAvail().x;
        if (fGridContentW < 1.f)
            fGridContentW = 1.f;
        float fSectionW = (fGridContentW - fGridPadLeft - fGridPadMiddle - fGridPadRight) * 0.5f;
        if (fSectionW < 1.f)
            fSectionW = 1.f;
        const float fLeftX = fGridBaseX + fGridPadLeft;
        const float fRightX = fLeftX + fSectionW + fGridPadMiddle;
        float fLeftY = fGridStartY;
        float fRightY = fGridStartY;
        {
            if (ksd::BeginGroupBox("aimbotgeneral", "General", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW, fGroupBoxHeightPx))
            {
                ksd::Checkbox("Enable Aimbot", &bEnableAimbot);
                ksd::SliderFloat("Aim Fov", &fAimFov, 1.f, 45.f);
                ksd::Checkbox("Aim Lock", &bAimLock);
                ksd::Checkbox("Visible Only", &bVisibleOnly);
                ksd::SliderInt("Target Bone", &iTargetBone, 0, 5);
                ksd::SliderFloat("Max Distance", &fMaxDistance, 10.f, 500.f, "%.00f");
            }
            ksd::EndGroupBox();
            fLeftY = ImGui::GetCursorPos().y;
        }
        {
            if (ksd::BeginGroupBox("aimbotassist", "Assist", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW, fGroupBoxHeightPx))
            {
                ksd::Checkbox("Auto Switch", &bAutoSwitch);
                ksd::Checkbox("Ignore Smoke", &bIgnoreSmoke);
                ksd::Checkbox("Prediction", &bPrediction);
                ksd::Checkbox("Recoil Control", &bRecoilControl);
                ksd::Checkbox("Humanize", &bHumanize);
                ksd::Checkbox("Auto Fire", &bAutoFire);
                ksd::Button("Test Button One", ImVec2(0.f, 24.f));
                ImGui::Dummy(ImVec2(1.f, 2.f));
                ksd::Button("Test Button Two", ImVec2(0.f, 24.f));
                ksd::Combo("Test Combo", &iTestComboMode, ppTestComboItems, kiTestComboItemCount);
            }
            ksd::EndGroupBox();
            fRightY = ImGui::GetCursorPos().y;
        }
        {
            if (ksd::BeginGroupBox("aimbotaim", "Aim settings", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW, fGroupBoxHeightPx))
            {
                ksd::SliderInt("Smoothness", &iSmoothness, 1, 20);
                ksd::SliderInt("Aim Curve", &iAimCurve, 1, 10);
                ksd::SliderFloat("Deadzone", &fDeadzone, 0.f, 0.2f, "%.3f");
                ksd::SliderFloat("Micro Adjust", &fMicroAdjust, 0.f, 1.f, "%.2f");
                ksd::SliderFloat("Snap Strength", &fSnapStrength, 0.f, 1.f, "%.2f");
            }
            ksd::EndGroupBox();
            fLeftY = ImGui::GetCursorPos().y;
        }
        {
            if (ksd::BeginGroupBox("aimbottuning", "Tuning", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW, fGroupBoxHeightPx))
            {
                ksd::SliderFloat("Reaction Delay", &fReactionDelay, 0.f, 0.35f, "%.3f");
                ksd::SliderInt("Burst Shots", &iBurstShots, 1, 8);
                ksd::SliderFloat("Target Weight", &fTargetWeight, 0.f, 1.f, "%.2f");
                ksd::SliderFloat("Friction", &fFriction, 0.f, 1.f, "%.2f");
                ksd::SliderFloat("Lead Factor", &fLeadFactor, 0.f, 1.f, "%.2f");
                ksd::SliderInt("Step Mode", &iStepMode, 0, 4);
                ksd::SliderInt("Jitter", &iJitter, 0, 6);
            }
            ksd::EndGroupBox();
            fRightY = ImGui::GetCursorPos().y;
        }
        const float fNextY = (fLeftY > fRightY) ? fLeftY : fRightY;
        ImGui::SetCursorPosY(fNextY);
        ImGui::Dummy(ImVec2(1.f, 0.f));
        break;
    }
    case MenuPage::Visuals:
    {
        const float fGridPadLeft = 0.f;
        const float fGridPadMiddle = 14.f;
        const float fGridPadRight = 14.f;
        const float fGridBaseX = ImGui::GetCursorPosX();
        const float fGridStartY = ImGui::GetCursorPos().y;
        float fGridContentW = ImGui::GetContentRegionAvail().x;
        if (fGridContentW < 1.f)
            fGridContentW = 1.f;
        float fSectionW = (fGridContentW - fGridPadLeft - fGridPadMiddle - fGridPadRight) * 0.5f;
        if (fSectionW < 1.f)
            fSectionW = 1.f;
        const float fLeftX = fGridBaseX + fGridPadLeft;
        const float fRightX = fLeftX + fSectionW + fGridPadMiddle;
        float fLeftY = fGridStartY;
        float fRightY = fGridStartY;

        if (ksd::BeginGroupBox("visualleft", "Esp", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Combo("Esp Mode", &iEspMode, ppEspModes, kiEspModeCount);
            ksd::Checkbox("Enable Esp", &bEnableEsp);
            ksd::Checkbox("Box Esp", &bBoxEsp);
            ksd::Checkbox("Name Esp", &bNameEsp);
            ksd::Checkbox("Health Esp", &bHealthEsp);
            ksd::Checkbox("Distance Esp", &bDistanceEsp);
            ksd::SliderFloat("Glow Power", &fGlowPower, 0.f, 1.f, "%.2f");
            ksd::CheckboxColorPicker1("Check Color 1", &bVisualTintOne, &fVisualTintOneR, &fVisualTintOneG, &fVisualTintOneB);
            ksd::CheckboxColorPicker2(
                "Check Color 2",
                &bVisualTintTwo,
                &fVisualTintTwoR0, &fVisualTintTwoG0, &fVisualTintTwoB0,
                &fVisualTintTwoR1, &fVisualTintTwoG1, &fVisualTintTwoB1);
            ksd::CheckboxColorPicker3(
                "Check Color 3",
                &bVisualTintThree,
                &fVisualTintThreeR0, &fVisualTintThreeG0, &fVisualTintThreeB0,
                &fVisualTintThreeR1, &fVisualTintThreeG1, &fVisualTintThreeB1,
                &fVisualTintThreeR2, &fVisualTintThreeG2, &fVisualTintThreeB2);
            ksd::ColorPicker("ColorPicker1", &fVisualColorPicker1R, &fVisualColorPicker1G, &fVisualColorPicker1B);
            ksd::ColorPicker2(
                "ColorPicker2",
                &fVisualColorPicker2R, &fVisualColorPicker2G, &fVisualColorPicker2B,
                &fVisualColorPicker2R1, &fVisualColorPicker2G1, &fVisualColorPicker2B1);
            ksd::ColorPicker3(
                "ColorPicker3",
                &fVisualColorPicker3R, &fVisualColorPicker3G, &fVisualColorPicker3B,
                &fVisualColorPicker3R1, &fVisualColorPicker3G1, &fVisualColorPicker3B1,
                &fVisualColorPicker3R2, &fVisualColorPicker3G2, &fVisualColorPicker3B2);
        }
        ksd::EndGroupBox();
        fLeftY = ImGui::GetCursorPos().y;

        if (ksd::BeginGroupBox("visualrightcheck", "Overlay", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Checkbox("Chams", &bChams);
            ksd::Checkbox("Radar", &bRadar);
            ksd::Checkbox("Offscreen Arrows", &bOffscreen);
            ksd::Checkbox("Hitmarker", &bHitmarker);
            ksd::Checkbox("Crosshair", &bCrosshair);
            ksd::Checkbox("World Glow", &bWorldGlow);
            ksd::Checkbox("Viewmodel Edit", &bViewmodelEdit);
            ksd::Checkbox("No Flash", &bNoFlash);
        }
        ksd::EndGroupBox();
        fRightY = ImGui::GetCursorPos().y;

        if (ksd::BeginGroupBox("visualrightprofile", "Profile", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW))
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CustomSettings::fRowControlLabelAlignX);
            ImGui::PushFont(ksd::GetRegularFont());
            ImGui::TextUnformatted("Profile Name");
            ImGui::PopFont();
            ksd::InputText("##visualprofile", szVisualProfile, IM_ARRAYSIZE(szVisualProfile));
            ksd::Button("Apply Profile", ImVec2(0.f, 24.f));
            ImGui::Dummy(ImVec2(1.f, 2.f));
            ksd::Button("Clear Profile", ImVec2(0.f, 24.f));
        }
        ksd::EndGroupBox();
        fRightY = ImGui::GetCursorPos().y;

        const float fNextY = (fLeftY > fRightY) ? fLeftY : fRightY;
        ImGui::SetCursorPosY(fNextY);
        ImGui::Dummy(ImVec2(1.f, 0.f));
        break;
    }
    case MenuPage::Other:
    {
        const float fGridPadLeft = 0.f;
        const float fGridPadMiddle = 14.f;
        const float fGridPadRight = 14.f;
        const float fGridBaseX = ImGui::GetCursorPosX();
        const float fGridStartY = ImGui::GetCursorPos().y;
        float fGridContentW = ImGui::GetContentRegionAvail().x;
        if (fGridContentW < 1.f)
            fGridContentW = 1.f;
        float fSectionW = (fGridContentW - fGridPadLeft - fGridPadMiddle - fGridPadRight) * 0.5f;
        if (fSectionW < 1.f)
            fSectionW = 1.f;
        const float fLeftX = fGridBaseX + fGridPadLeft;
        const float fRightX = fLeftX + fSectionW + fGridPadMiddle;
        float fLeftY = fGridStartY;
        float fRightY = fGridStartY;

        if (ksd::BeginGroupBox("othermove", "Movement", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Checkbox("Auto Hop", &bAutoHop);
            ksd::Checkbox("Air Strafe", &bAirStrafe);
            ksd::Checkbox("Edge Jump", &bEdgeJump);
            ksd::Checkbox("Auto Stop", &bAutoStop);
            ksd::Combo("Movement Mode", &iMovementMode, ppMovementModes, kiMovementModeCount);
            ksd::SliderFloat("Strafe Power", &fStrafePower, 0.f, 1.f, "%.2f");
        }
        ksd::EndGroupBox();
        fLeftY = ImGui::GetCursorPos().y;

        if (ksd::BeginGroupBox("othernetwork", "Network and latency", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Combo("Latency Mode", &iLatencyMode, ppLatencyModes, kiLatencyModeCount);
            ksd::SliderInt("Fake Ping", &iFakePing, 0, 200, "%d");
            ksd::SliderInt("Tick Shift", &iTickShift, 0, 16, "%d");
            ksd::SliderInt("Choke", &iChoke, 0, 14, "%d");
            ksd::Checkbox("Round Timer", &bRoundTimer);
            ksd::Checkbox("Watermark", &bWatermark);
        }
        ksd::EndGroupBox();
        fRightY = ImGui::GetCursorPos().y;

        const float fNextY = (fLeftY > fRightY) ? fLeftY : fRightY;
        ImGui::SetCursorPosY(fNextY);
        ImGui::Dummy(ImVec2(1.f, 0.f));
        break;
    }
    case MenuPage::Exploits:
    {
        const float fGridPadLeft = 0.f;
        const float fGridPadMiddle = 14.f;
        const float fGridPadRight = 14.f;
        const float fGridBaseX = ImGui::GetCursorPosX();
        const float fGridStartY = ImGui::GetCursorPos().y;
        float fGridContentW = ImGui::GetContentRegionAvail().x;
        if (fGridContentW < 1.f)
            fGridContentW = 1.f;
        float fSectionW = (fGridContentW - fGridPadLeft - fGridPadMiddle - fGridPadRight) * 0.5f;
        if (fSectionW < 1.f)
            fSectionW = 1.f;
        const float fLeftX = fGridBaseX + fGridPadLeft;
        const float fRightX = fLeftX + fSectionW + fGridPadMiddle;
        float fLeftY = fGridStartY;
        float fRightY = fGridStartY;

        if (ksd::BeginGroupBox("exmain", "Main", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Checkbox("Double Tap", &bDoubleTap);
            ksd::Checkbox("Hide Shots", &bHideShots);
            ksd::Checkbox("Rapid Fire", &bRapidFire);
            ksd::Checkbox("Tick Freeze", &bTickFreeze);
            ksd::Checkbox("Safe Mode", &bSafeMode);
            ksd::Combo("Double Tap Mode", &iDoubleTapMode, ppDoubleTapModes, kiDoubleTapModeCount);
            ksd::SliderFloat("Charge Speed", &fChargeSpeed, 0.f, 1.f, "%.2f");
        }
        ksd::EndGroupBox();
        fLeftY = ImGui::GetCursorPos().y;

        if (ksd::BeginGroupBox("exantiaim", "Anti aim", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Checkbox("Resolver", &bResolver);
            ksd::Checkbox("Lag Exploit", &bLagExploit);
            ksd::Combo("Resolver Mode", &iResolverMode, ppResolverModes, kiResolverModeCount);
            ksd::SliderFloat("Desync Angle", &fDesyncAngle, 0.f, 60.f, "%.0f");
            ksd::SliderInt("Fakelag Ticks", &iFakelagTicks, 0, 14, "%d");
            ksd::Button("Force Recharge", ImVec2(0.f, 24.f));
            ImGui::Dummy(ImVec2(1.f, 2.f));
            ksd::Button("Sync Buffers", ImVec2(0.f, 24.f));
        }
        ksd::EndGroupBox();
        fRightY = ImGui::GetCursorPos().y;

        const float fNextY = (fLeftY > fRightY) ? fLeftY : fRightY;
        ImGui::SetCursorPosY(fNextY);
        ImGui::Dummy(ImVec2(1.f, 0.f));
        break;
    }
    case MenuPage::Config:
    {
        const float fGridPadLeft = 0.f;
        const float fGridPadMiddle = 14.f;
        const float fGridPadRight = 14.f;
        const float fGridBaseX = ImGui::GetCursorPosX();
        const float fGridStartY = ImGui::GetCursorPos().y;
        float fGridContentW = ImGui::GetContentRegionAvail().x;
        if (fGridContentW < 1.f)
            fGridContentW = 1.f;
        float fSectionW = (fGridContentW - fGridPadLeft - fGridPadMiddle - fGridPadRight) * 0.5f;
        if (fSectionW < 1.f)
            fSectionW = 1.f;
        const float fLeftX = fGridBaseX + fGridPadLeft;
        const float fRightX = fLeftX + fSectionW + fGridPadMiddle;
        float fLeftY = fGridStartY;
        float fRightY = fGridStartY;

        if (ksd::BeginGroupBox("cfginterface", "Interface", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::ColorPicker("Menu color", &fMenuAccentR, &fMenuAccentG, &fMenuAccentB);
            ksd::CheckboxColorPicker1("Tint one", &bDemoCheckboxColor1, &fDemoCheckboxColor1R, &fDemoCheckboxColor1G, &fDemoCheckboxColor1B);
            ksd::CheckboxColorPicker2(
                "Tint two",
                &bDemoCheckboxColor2,
                &fDemoCheckboxColor2R0, &fDemoCheckboxColor2G0, &fDemoCheckboxColor2B0,
                &fDemoCheckboxColor2R1, &fDemoCheckboxColor2G1, &fDemoCheckboxColor2B1);
            ksd::CheckboxColorPicker3(
                "Tint three",
                &bDemoCheckboxColor3,
                &fDemoCheckboxColor3R0, &fDemoCheckboxColor3G0, &fDemoCheckboxColor3B0,
                &fDemoCheckboxColor3R1, &fDemoCheckboxColor3G1, &fDemoCheckboxColor3B1,
                &fDemoCheckboxColor3R2, &fDemoCheckboxColor3G2, &fDemoCheckboxColor3B2);
            ksd::Combo("Theme preset", &iThemePreset, ppThemePresets, kiThemePresetCount);
            ksd::Combo("Language", &iLanguage, ppLanguages, kiLanguageCount);
            ksd::SliderFloat("Font Scale", &fFontScale, 0.8f, 1.3f, "%.2f");
            ksd::KeyBindRow("Menu key", &iMenuHotkey);
        }
        ksd::EndGroupBox();
        fLeftY = ImGui::GetCursorPos().y;

        if (ksd::BeginGroupBox("cfgprofiles", "Profiles", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW))
        {
            ksd::Combo("Slot", &iSelectedConfig, ppConfigSlots, kiConfigSlotCount);
            ksd::InputText("Config name", szConfigName, IM_ARRAYSIZE(szConfigName));
            ksd::Checkbox("Auto Save", &bAutoSave);
            ksd::Checkbox("Cloud Sync", &bCloudSync);
            ksd::Checkbox("Confirm On Load", &bConfirmOnLoad);
            ksd::Checkbox("Show Debug", &bShowDebug);
            ksd::Button("Load Config", ImVec2(0.f, 24.f));
            ImGui::Dummy(ImVec2(1.f, 2.f));
            ksd::Button("Save Config", ImVec2(0.f, 24.f));
            ImGui::Dummy(ImVec2(1.f, 2.f));
            ksd::Button("Create New", ImVec2(0.f, 24.f));
            ImGui::Dummy(ImVec2(1.f, 2.f));
            ksd::Button("Delete Slot", ImVec2(0.f, 24.f));
        }
        ksd::EndGroupBox();
        fRightY = ImGui::GetCursorPos().y;

        const float fNextY = (fLeftY > fRightY) ? fLeftY : fRightY;
        ImGui::SetCursorPosY(fNextY);
        ImGui::Dummy(ImVec2(1.f, 0.f));
        break;
    }
    case MenuPage::About:
    default:
        {
            const float fGridPadLeft = 0.f;
            const float fGridPadMiddle = 14.f;
            const float fGridPadRight = 14.f;
            const float fGridBaseX = ImGui::GetCursorPosX();
            const float fGridStartY = ImGui::GetCursorPos().y;
            float fGridContentW = ImGui::GetContentRegionAvail().x;
            if (fGridContentW < 1.f)
                fGridContentW = 1.f;
            float fSectionW = (fGridContentW - fGridPadLeft - fGridPadMiddle - fGridPadRight) * 0.5f;
            if (fSectionW < 1.f)
                fSectionW = 1.f;
            const float fLeftX = fGridBaseX + fGridPadLeft;
            const float fRightX = fLeftX + fSectionW + fGridPadMiddle;
            float fLeftY = fGridStartY;
            float fRightY = fGridStartY;

            if (ksd::BeginGroupBox("aboutinfo", "Info", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW))
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CustomSettings::fRowControlLabelAlignX);
                ImGui::PushFont(ksd::GetRegularFont());
                ImGui::TextUnformatted("Static ImGui Framework");
                ImGui::TextUnformatted("Version 1.0 Preview");
                ImGui::TextUnformatted("Author: KingsleydotDev");
                ImGui::TextUnformatted("Renderer: Dear ImGui");
                ImGui::PopFont();
                ksd::Checkbox("Show Changelog", &bShowChangelog);
                ksd::Button("Copy Build Info", ImVec2(0.f, 24.f));
            }
            ksd::EndGroupBox();
            fLeftY = ImGui::GetCursorPos().y;

            if (ksd::BeginGroupBox("aboutupdate", "Update", fRightX, fRightY, fCollapsibleHeaderHeight, fSectionW))
            {
                ksd::Combo("Build Track", &iBuildTrack, ppBuildTracks, kiBuildTrackCount);
                ksd::Checkbox("Enable Telemetry", &bEnableTelemetry);
                ksd::Checkbox("Beta Channel", &bBetaChannel);
                ksd::Button("Check Updates", ImVec2(0.f, 24.f));
                ImGui::Dummy(ImVec2(1.f, 2.f));
                ksd::Button("Download Package", ImVec2(0.f, 24.f));
            }
            ksd::EndGroupBox();
            fRightY = ImGui::GetCursorPos().y;

            if (ksd::BeginGroupBox("aboutlinks", "Resources", fLeftX, fLeftY, fCollapsibleHeaderHeight, fSectionW))
            {
                ksd::Button("Open Documentation", ImVec2(0.f, 24.f));
                ImGui::Dummy(ImVec2(1.f, 2.f));
                ksd::Button("Open Issue Tracker", ImVec2(0.f, 24.f));
                ImGui::Dummy(ImVec2(1.f, 2.f));
                ksd::Button("Open Community", ImVec2(0.f, 24.f));
            }
            ksd::EndGroupBox();
            fLeftY = ImGui::GetCursorPos().y;

            const float fNextY = (fLeftY > fRightY) ? fLeftY : fRightY;
            ImGui::SetCursorPosY(fNextY);
            ImGui::Dummy(ImVec2(1.f, 0.f));
        }
        break;
    }
}
}

void KSD::Render()
{
    if (MenuVars::iMenuHotkey != 0)
    {
        if (ImGui::IsKeyPressed((ImGuiKey)MenuVars::iMenuHotkey, false))
            MenuVars::bMenuVisible = !MenuVars::bMenuVisible;
    }
    else
        MenuVars::bMenuVisible = true;
    if (!MenuVars::bMenuVisible)
        return;

    const ImGuiViewport* pViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(pViewport->Pos.x + 20.f, pViewport->Pos.y + 20.f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(CustomSettings::fMenuWidth, CustomSettings::fMenuHeight), ImGuiCond_FirstUseEver);
    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("##ksdmenu", nullptr, windowFlags);
    const ImVec2 vWindowPos = ImGui::GetWindowPos();
    const ImVec2 vWindowSize = ImGui::GetWindowSize();
    ImDrawList* pDraw = ImGui::GetWindowDrawList();
    const ImU32 uMenuColor = ImColor(Colors::fBackgroundR, Colors::fBackgroundG, Colors::fBackgroundB, Colors::fBackgroundA);
    const ImU32 uStripColor = ImColor(Colors::fStripR, Colors::fStripG, Colors::fStripB, Colors::fStripA);
    pDraw->AddRectFilled(vWindowPos, ImVec2(vWindowPos.x + vWindowSize.x, vWindowPos.y + vWindowSize.y), uMenuColor, CustomSettings::fWindowRounding);
    pDraw->AddRectFilled(vWindowPos, ImVec2(vWindowPos.x + vWindowSize.x, vWindowPos.y + CustomSettings::fTopStripHeight), uStripColor, CustomSettings::fWindowRounding, ImDrawFlags_RoundCornersTop);
    pDraw->AddRectFilled(
        ImVec2(vWindowPos.x, vWindowPos.y + CustomSettings::fTopStripHeight),
        ImVec2(vWindowPos.x + CustomSettings::fLeftStripWidth, vWindowPos.y + vWindowSize.y),
        uStripColor,
        CustomSettings::fWindowRounding,
        ImDrawFlags_RoundCornersBottomLeft
    );
    KsdDynamicTitle::Draw(pDraw, vWindowPos);

    {
        const float fDividerY = vWindowPos.y + CustomSettings::fTopStripHeight;
        const float fX0 = vWindowPos.x;
        const float fX1 = vWindowPos.x + vWindowSize.x;
        const ImVec2 vLineStart(fX0, fDividerY);
        const ImVec2 vLineEnd(fX1, fDividerY);
        pDraw->AddLine(vLineStart, vLineEnd, ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, 0.14f), 6.f);
        pDraw->AddLine(vLineStart, vLineEnd, ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, 0.35f), 3.f);
        pDraw->AddLine(vLineStart, vLineEnd, ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, 0.85f), 1.f);
    }

    ImGui::SetCursorPos(ImVec2(0.f, CustomSettings::fTopStripHeight));
    ImGui::BeginChild("leftstrip", ImVec2(CustomSettings::fLeftStripWidth, vWindowSize.y - CustomSettings::fTopStripHeight), false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::SetCursorPos(ImVec2(6.f, 12.f));
        const ImVec2 vButtonSize(CustomSettings::fLeftStripWidth - 12.f, CustomSettings::fButtonHeight);
        ksd::RenderMenuStrip(kEntryText, kEntryIcon, (int)MenuPage::Count, giCurrentPage, vButtonSize, 7.f);
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(CustomSettings::fLeftStripWidth, CustomSettings::fTopStripHeight));
    ImGui::BeginChild("pagecontent", ImVec2(vWindowSize.x - CustomSettings::fLeftStripWidth, vWindowSize.y - CustomSettings::fTopStripHeight), false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    UpdatePageFade();
    if (MenuVars::ePageFade != MenuVars::PageFadeState::Idle)
        ImGui::SetScrollY(0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * MenuVars::gfPageAlpha);
    const float fSlideY = fPageSlideMaxPx * (1.f - MenuVars::gfPageAlpha);
    const ImVec2 vContentStart = ImGui::GetCursorStartPos();
    float fSpacerW = ImGui::GetContentRegionAvail().x;
    if (fSpacerW < 1.f)
        fSpacerW = 1.f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.f));
    ImGui::Dummy(ImVec2(fSpacerW, 18.f + fSlideY));
    ImGui::PopStyleVar();
    ImGui::SetCursorPosX(vContentStart.x + 18.f);
    RenderPageContent();
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
}
