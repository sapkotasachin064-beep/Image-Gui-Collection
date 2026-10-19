#include "var.h"

namespace MenuVars
{
// kingsleydotdev

int giCurrentPage = 0;
int giShownPage = 0;
float gfPageAlpha = 1.f;
PageFadeState ePageFade = PageFadeState::Idle;

bool bEnableAimbot = true;
float fAimFov = 18.f;
bool bAutoSwitch = false;
bool bIgnoreSmoke = true;
bool bAimLock = false;
bool bVisibleOnly = true;
int iTargetBone = 2;
float fMaxDistance = 240.f;
float fReactionDelay = 0.12f;
int iBurstShots = 3;
float fTargetWeight = 0.65f;
int iSmoothness = 6;
bool bPrediction = true;
bool bRecoilControl = true;
bool bHumanize = false;
bool bAutoFire = false;
int iTestComboMode = 0;
const char* const ppTestComboItems[] =
{
    "Default",
    "Legit",
    "Aggressive",
    "Rage",
    "Balanced",
    "Stealth",
    "Precise",
    "Fast",
    "Adaptive",
    "Custom"
};
int iAimCurve = 4;
float fDeadzone = 0.03f;
float fMicroAdjust = 0.55f;
float fSnapStrength = 0.35f;
float fFriction = 0.42f;
float fLeadFactor = 0.28f;
int iStepMode = 2;
int iJitter = 1;

bool bEnableEsp = true;
bool bBoxEsp = true;
bool bNameEsp = true;
bool bHealthEsp = true;
bool bDistanceEsp = true;
float fGlowPower = 0.55f;
int iEspMode = 0;
const char* const ppEspModes[] =
{
    "Clean",
    "Classic",
    "Bold",
    "Outline",
    "Minimal"
};
bool bChams = false;
bool bRadar = true;
bool bOffscreen = false;
bool bHitmarker = true;
bool bCrosshair = true;
bool bWorldGlow = false;
bool bViewmodelEdit = false;
bool bNoFlash = false;
char szVisualProfile[128] = {};
bool bVisualTintOne = false;
bool bVisualTintTwo = false;
bool bVisualTintThree = false;
float fVisualTintOneR = 0.8f;
float fVisualTintOneG = 0.45f;
float fVisualTintOneB = 0.3f;
float fVisualTintTwoR0 = 0.25f;
float fVisualTintTwoG0 = 0.8f;
float fVisualTintTwoB0 = 0.55f;
float fVisualTintTwoR1 = 0.35f;
float fVisualTintTwoG1 = 0.6f;
float fVisualTintTwoB1 = 0.95f;
float fVisualTintThreeR0 = 0.95f;
float fVisualTintThreeG0 = 0.55f;
float fVisualTintThreeB0 = 0.2f;
float fVisualTintThreeR1 = 0.35f;
float fVisualTintThreeG1 = 0.75f;
float fVisualTintThreeB1 = 0.3f;
float fVisualTintThreeR2 = 0.5f;
float fVisualTintThreeG2 = 0.4f;
float fVisualTintThreeB2 = 0.95f;
float fVisualColorPicker1R = 0.9f;
float fVisualColorPicker1G = 0.3f;
float fVisualColorPicker1B = 0.35f;
float fVisualColorPicker2R = 0.25f;
float fVisualColorPicker2G = 0.85f;
float fVisualColorPicker2B = 0.55f;
float fVisualColorPicker2R1 = 0.45f;
float fVisualColorPicker2G1 = 0.55f;
float fVisualColorPicker2B1 = 0.95f;
float fVisualColorPicker3R = 0.35f;
float fVisualColorPicker3G = 0.5f;
float fVisualColorPicker3B = 0.95f;
float fVisualColorPicker3R1 = 0.95f;
float fVisualColorPicker3G1 = 0.6f;
float fVisualColorPicker3B1 = 0.25f;
float fVisualColorPicker3R2 = 0.5f;
float fVisualColorPicker3G2 = 0.85f;
float fVisualColorPicker3B2 = 0.45f;

bool bAutoHop = false;
int iFakePing = 24;
float fMenuOpacity = 0.95f;
bool bAirStrafe = true;
bool bEdgeJump = false;
bool bAutoStop = true;
bool bRoundTimer = true;
bool bWatermark = true;
int iMovementMode = 1;
int iLatencyMode = 0;
float fStrafePower = 0.62f;
int iTickShift = 8;
int iChoke = 4;
const char* const ppMovementModes[] =
{
    "Manual",
    "Assist",
    "Full"
};
const char* const ppLatencyModes[] =
{
    "Off",
    "Dynamic",
    "Force"
};

bool bPreviewToggle = false;
float fPreviewMix = 0.4f;
int iPreviewTier = 3;
bool bDoubleTap = false;
bool bHideShots = false;
bool bRapidFire = false;
bool bTickFreeze = false;
bool bResolver = true;
bool bLagExploit = false;
bool bSafeMode = true;
int iDoubleTapMode = 1;
int iResolverMode = 0;
float fChargeSpeed = 0.7f;
float fDesyncAngle = 35.f;
int iFakelagTicks = 6;
const char* const ppDoubleTapModes[] =
{
    "Off",
    "Shift",
    "Adaptive"
};
const char* const ppResolverModes[] =
{
    "Auto",
    "Strong",
    "Safe"
};

float fMenuAccentR = 188.f / 255.f;
float fMenuAccentG = 244.f / 255.f;
float fMenuAccentB = 33.f / 255.f;
int iMenuHotkey = 0;
bool bMenuVisible = true;

bool bDemoCheckboxColor1 = false;
float fDemoCheckboxColor1R = 0.85f;
float fDemoCheckboxColor1G = 0.35f;
float fDemoCheckboxColor1B = 0.35f;
bool bDemoCheckboxColor2 = false;
float fDemoCheckboxColor2R0 = 0.35f;
float fDemoCheckboxColor2G0 = 0.75f;
float fDemoCheckboxColor2B0 = 0.45f;
float fDemoCheckboxColor2R1 = 0.4f;
float fDemoCheckboxColor2G1 = 0.45f;
float fDemoCheckboxColor2B1 = 0.9f;
bool bDemoCheckboxColor3 = false;
float fDemoCheckboxColor3R0 = 0.95f;
float fDemoCheckboxColor3G0 = 0.55f;
float fDemoCheckboxColor3B0 = 0.2f;
float fDemoCheckboxColor3R1 = 0.25f;
float fDemoCheckboxColor3G1 = 0.65f;
float fDemoCheckboxColor3B1 = 0.95f;
float fDemoCheckboxColor3R2 = 0.55f;
float fDemoCheckboxColor3G2 = 0.35f;
float fDemoCheckboxColor3B2 = 0.75f;
bool bAutoSave = true;
bool bCloudSync = false;
bool bConfirmOnLoad = true;
bool bShowDebug = false;
int iSelectedConfig = 0;
int iLanguage = 0;
int iThemePreset = 0;
float fFontScale = 1.f;
const char* const ppConfigSlots[] =
{
    "Default",
    "Legit",
    "Rage",
    "HvH",
    "Scout",
    "Custom"
};
const char* const ppLanguages[] =
{
    "English",
    "German",
    "French",
    "Spanish"
};
const char* const ppThemePresets[] =
{
    "Green",
    "Blue",
    "Purple",
    "Red"
};
char szConfigName[64] = {};
bool bShowChangelog = true;
bool bEnableTelemetry = false;
bool bBetaChannel = false;
int iBuildTrack = 1;
const char* const ppBuildTracks[] =
{
    "Stable",
    "Beta",
    "Nightly"
};

}
