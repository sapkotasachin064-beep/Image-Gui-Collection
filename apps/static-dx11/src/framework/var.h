#pragma once

namespace MenuVars
{

constexpr int kiTestComboItemCount = 10;
constexpr int kiEspModeCount = 5;
constexpr int kiMovementModeCount = 3;
constexpr int kiLatencyModeCount = 3;
constexpr int kiDoubleTapModeCount = 3;
constexpr int kiResolverModeCount = 3;
constexpr int kiConfigSlotCount = 6;
constexpr int kiLanguageCount = 4;
constexpr int kiThemePresetCount = 4;
constexpr int kiBuildTrackCount = 3;

enum class PageFadeState
{
    Idle,
    FadeOut,
    FadeIn
};

// menu index and page fade

extern int giCurrentPage;
extern int giShownPage;
extern float gfPageAlpha;
extern PageFadeState ePageFade;

// aimbot page

extern bool bEnableAimbot;
extern float fAimFov;
extern bool bAutoSwitch;
extern bool bIgnoreSmoke;
extern bool bAimLock;
extern bool bVisibleOnly;
extern int iTargetBone;
extern float fMaxDistance;
extern float fReactionDelay;
extern int iBurstShots;
extern float fTargetWeight;
extern int iSmoothness;
extern bool bPrediction;
extern bool bRecoilControl;
extern bool bHumanize;
extern bool bAutoFire;
extern int iTestComboMode;
extern const char* const ppTestComboItems[];
extern int iAimCurve;
extern float fDeadzone;
extern float fMicroAdjust;
extern float fSnapStrength;
extern float fFriction;
extern float fLeadFactor;
extern int iStepMode;
extern int iJitter;

extern bool bEnableEsp;
extern bool bBoxEsp;
extern bool bNameEsp;
extern bool bHealthEsp;
extern bool bDistanceEsp;
extern float fGlowPower;
extern int iEspMode;
extern const char* const ppEspModes[];
extern bool bChams;
extern bool bRadar;
extern bool bOffscreen;
extern bool bHitmarker;
extern bool bCrosshair;
extern bool bWorldGlow;
extern bool bViewmodelEdit;
extern bool bNoFlash;
extern char szVisualProfile[128];
extern bool bVisualTintOne;
extern bool bVisualTintTwo;
extern bool bVisualTintThree;
extern float fVisualTintOneR;
extern float fVisualTintOneG;
extern float fVisualTintOneB;
extern float fVisualTintTwoR0;
extern float fVisualTintTwoG0;
extern float fVisualTintTwoB0;
extern float fVisualTintTwoR1;
extern float fVisualTintTwoG1;
extern float fVisualTintTwoB1;
extern float fVisualTintThreeR0;
extern float fVisualTintThreeG0;
extern float fVisualTintThreeB0;
extern float fVisualTintThreeR1;
extern float fVisualTintThreeG1;
extern float fVisualTintThreeB1;
extern float fVisualTintThreeR2;
extern float fVisualTintThreeG2;
extern float fVisualTintThreeB2;
extern float fVisualColorPicker1R;
extern float fVisualColorPicker1G;
extern float fVisualColorPicker1B;
extern float fVisualColorPicker2R;
extern float fVisualColorPicker2G;
extern float fVisualColorPicker2B;
extern float fVisualColorPicker2R1;
extern float fVisualColorPicker2G1;
extern float fVisualColorPicker2B1;
extern float fVisualColorPicker3R;
extern float fVisualColorPicker3G;
extern float fVisualColorPicker3B;
extern float fVisualColorPicker3R1;
extern float fVisualColorPicker3G1;
extern float fVisualColorPicker3B1;
extern float fVisualColorPicker3R2;
extern float fVisualColorPicker3G2;
extern float fVisualColorPicker3B2;

// other page

extern bool bAutoHop;
extern int iFakePing;
extern float fMenuOpacity;
extern bool bAirStrafe;
extern bool bEdgeJump;
extern bool bAutoStop;
extern bool bRoundTimer;
extern bool bWatermark;
extern int iMovementMode;
extern int iLatencyMode;
extern float fStrafePower;
extern int iTickShift;
extern int iChoke;
extern const char* const ppMovementModes[];
extern const char* const ppLatencyModes[];

extern bool bPreviewToggle;
extern float fPreviewMix;
extern int iPreviewTier;
extern bool bDoubleTap;
extern bool bHideShots;
extern bool bRapidFire;
extern bool bTickFreeze;
extern bool bResolver;
extern bool bLagExploit;
extern bool bSafeMode;
extern int iDoubleTapMode;
extern int iResolverMode;
extern float fChargeSpeed;
extern float fDesyncAngle;
extern int iFakelagTicks;
extern const char* const ppDoubleTapModes[];
extern const char* const ppResolverModes[];

extern float fMenuAccentR;
extern float fMenuAccentG;
extern float fMenuAccentB;
extern int iMenuHotkey;
extern bool bMenuVisible;

extern bool bDemoCheckboxColor1;
extern float fDemoCheckboxColor1R;
extern float fDemoCheckboxColor1G;
extern float fDemoCheckboxColor1B;
extern bool bDemoCheckboxColor2;
extern float fDemoCheckboxColor2R0;
extern float fDemoCheckboxColor2G0;
extern float fDemoCheckboxColor2B0;
extern float fDemoCheckboxColor2R1;
extern float fDemoCheckboxColor2G1;
extern float fDemoCheckboxColor2B1;
extern bool bDemoCheckboxColor3;
extern float fDemoCheckboxColor3R0;
extern float fDemoCheckboxColor3G0;
extern float fDemoCheckboxColor3B0;
extern float fDemoCheckboxColor3R1;
extern float fDemoCheckboxColor3G1;
extern float fDemoCheckboxColor3B1;
extern float fDemoCheckboxColor3R2;
extern float fDemoCheckboxColor3G2;
extern float fDemoCheckboxColor3B2;
extern bool bAutoSave;
extern bool bCloudSync;
extern bool bConfirmOnLoad;
extern bool bShowDebug;
extern int iSelectedConfig;
extern int iLanguage;
extern int iThemePreset;
extern float fFontScale;
extern const char* const ppConfigSlots[];
extern const char* const ppLanguages[];
extern const char* const ppThemePresets[];
extern char szConfigName[64];
extern bool bShowChangelog;
extern bool bEnableTelemetry;
extern bool bBetaChannel;
extern int iBuildTrack;
extern const char* const ppBuildTracks[];

// config page and about page

}
