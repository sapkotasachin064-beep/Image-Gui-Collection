#define IMGUI_DEFINE_MATH_OPERATORS
#include "custom_imgui.h"
#include "custom_settings.h"
#include "var.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "fonts.h"
#include "iconsfontawesome/fa.h"
#include "iconsfontawesome/IconsFontAwesome6.h"
#include <cfloat>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <map>

namespace
{
// kingsleydotdev
ImFont* gpFontBold = nullptr;
ImFont* gpFontMedium = nullptr;
ImFont* gpFontRegular = nullptr;
ImFont* gpFontBracket = nullptr;
ImFont* gpFontIcon = nullptr;
ImFont* gpFontLogo = nullptr;
ImFont* gpFontStaticTitle = nullptr;
float gfIconBlend[32] = {};
float gfTextBlend[32] = {};
constexpr float fCheckboxSliderAlignX = 0.f;

struct CollapsibleSectionCtx
{
    ImVec2 vMinScreen;
    float fWidth;
    float fHeaderHeight;
    bool* pbOpen;
    bool bPushedItemWidth;
    bool bPushedClipRect;
};

struct GroupBoxCtx
{
    ImVec2 vMinScreen;
    float fWidth;
    float fHeaderHeight;
    float fHeight;
    float fCutLeftX;
    float fCutRightX;
    ImVec2 vTextPos;
    const char* pLabel;
    bool bPushedItemWidth;
    bool bPushedClipRect;
    bool bBeganChild;
};

constexpr int kiCollapsibleStackMax = 16;
CollapsibleSectionCtx gCollapsibleStack[kiCollapsibleStackMax];
int giCollapsibleStackCount = 0;
GroupBoxCtx gGroupBoxStack[kiCollapsibleStackMax];
int giGroupBoxStackCount = 0;

ImVec4 LerpColor(const ImVec4& from, const ImVec4& to, float fT)
{
    const float fClamped = fT < 0.f ? 0.f : (fT > 1.f ? 1.f : fT);
    return ImVec4(
        from.x + (to.x - from.x) * fClamped,
        from.y + (to.y - from.y) * fClamped,
        from.z + (to.z - from.z) * fClamped,
        from.w + (to.w - from.w) * fClamped
    );
}

float EaseOutQuad(float fT)
{
    const float fC = fT < 0.f ? 0.f : (fT > 1.f ? 1.f : fT);
    return fC * (2.f - fC);
}

ImU32 LerpU32(ImU32 uA, ImU32 uB, float fT)
{
    const float fC = fT < 0.f ? 0.f : (fT > 1.f ? 1.f : fT);
    const int ra = (int)(uA >> 0) & 0xFF;
    const int ga = (int)(uA >> 8) & 0xFF;
    const int ba = (int)(uA >> 16) & 0xFF;
    const int aa = (int)(uA >> 24) & 0xFF;
    const int rb = (int)(uB >> 0) & 0xFF;
    const int gb = (int)(uB >> 8) & 0xFF;
    const int bb = (int)(uB >> 16) & 0xFF;
    const int ab = (int)(uB >> 24) & 0xFF;
    return IM_COL32(
        (int)(ra + (rb - ra) * fC),
        (int)(ga + (gb - ga) * fC),
        (int)(ba + (bb - ba) * fC),
        (int)(aa + (ab - aa) * fC)
    );
}

void StepAnimFloat(float* pfValue, float fTarget, float fSpeed, float fDt)
{
    const float fStep = fDt * fSpeed;
    if (*pfValue < fTarget)
        *pfValue = (*pfValue + fStep < fTarget) ? (*pfValue + fStep) : fTarget;
    else if (*pfValue > fTarget)
        *pfValue = (*pfValue - fStep > fTarget) ? (*pfValue - fStep) : fTarget;
}

bool ColorSwatchCompactAt(ImGuiWindow* pWindow, float* pr, float* pg, float* pb, const ImVec2& vSwMin, const ImVec2& vSwMax)
{
    const ImGuiID idSwatch = pWindow->GetID("sw");
    if (!ImGui::ItemAdd(ImRect(vSwMin, vSwMax), idSwatch))
        return false;
    const int ir = (int)(ImClamp(*pr * 255.f, 0.f, 255.f));
    const int ig = (int)(ImClamp(*pg * 255.f, 0.f, 255.f));
    const int ib = (int)(ImClamp(*pb * 255.f, 0.f, 255.f));
    const ImU32 uTop = IM_COL32(ir, ig, ib, 255);
    const ImU32 uBot = IM_COL32((ir * 15) >> 4, (ig * 15) >> 4, (ib * 15) >> 4, 255);
    ImDrawList* const pDraw = pWindow->DrawList;
    pDraw->AddRectFilledMultiColor(vSwMin, vSwMax, uTop, uTop, uBot, uBot);
    pDraw->AddRect(vSwMin, vSwMax, IM_COL32(55, 55, 60, 255), 0.f, 0, 1.f);
    bool bSwHover = false;
    bool bSwHeld = false;
    const bool bSwPressed = ImGui::ButtonBehavior(ImRect(vSwMin, vSwMax), idSwatch, &bSwHover, &bSwHeld);
    char szPopup[40];
    ImFormatString(szPopup, IM_ARRAYSIZE(szPopup), "##pk%u", (unsigned int)idSwatch);
    if (bSwPressed)
        ImGui::OpenPopup(szPopup);
    bool bChanged = false;
    if (ImGui::BeginPopup(szPopup))
    {
        float fRgb[3] = { *pr, *pg, *pb };
        if (ksd::ColorPickerCompact3("##p3", fRgb, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview))
        {
            *pr = fRgb[0];
            *pg = fRgb[1];
            *pb = fRgb[2];
            bChanged = true;
        }
        ImGui::EndPopup();
    }
    return bChanged;
}

bool CheckboxWithColorSwatchesRowImpl(const char* label, bool* pbValue, int iSwatchCount, float* pfRgb)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;
    if (iSwatchCount < 1 || iSwatchCount > 3)
        return false;
    ImGui::PushID(label);
    ImGuiContext& g = *GImGui;
    const float fDt = g.IO.DeltaTime;
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vLabelSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);
    const float fBoxW = CustomSettings::fSettingsSliderOuterH;
    const float fBoxH = CustomSettings::fSettingsSliderOuterH;
    constexpr float fRowPadY = 4.f;
    constexpr float fGapLabelBox = 6.f;
    constexpr float fGapSw = 4.f;
    const float fSwatchW = CustomSettings::fAccentSwatchW;
    const float fSwatchH = CustomSettings::fAccentSwatchH;
    const float fRowH = ImMax(vLabelSize.y, ImMax(fBoxH, fSwatchH)) + fRowPadY;
    const float fAvail = ImGui::GetContentRegionAvail().x;
    const ImVec2 vRowMin = pWindow->DC.CursorPos;
    float fRowRightX = vRowMin.x + fAvail;
    if (pWindow->ClipRect.Max.x > vRowMin.x && pWindow->ClipRect.Max.x < fRowRightX)
        fRowRightX = pWindow->ClipRect.Max.x;
    const float fPadX = CustomSettings::fRowControlLabelAlignX;
    const float fRowInnerRight = fRowRightX - fPadX;
    const float fSwatchBlockW = fSwatchW * (float)iSwatchCount + fGapSw * (float)(iSwatchCount - 1);
    const ImRect totalBB(vRowMin, ImVec2(fRowRightX, vRowMin.y + fRowH));
    const ImGuiID idCb = pWindow->GetID("##cb");
    ImGui::ItemSize(totalBB);
    if (!ImGui::ItemAdd(totalBB, idCb))
    {
        ImGui::PopID();
        return false;
    }
    const float fBoxX = vRowMin.x + fCheckboxSliderAlignX;
    const float fBoxY = vRowMin.y + (fRowH - fBoxH) * 0.5f;
    const ImVec2 vBoxMin(fBoxX, fBoxY);
    const ImVec2 vBoxMax(fBoxX + fBoxW, fBoxY + fBoxH);
    const float fTextStartX = fBoxX + fBoxW + fGapLabelBox;
    const float fSwatchStartX = fRowInnerRight - fSwatchBlockW;
    const float fCbHitMaxX = fSwatchStartX - 8.f;
    ImRect rCheckboxHit(vRowMin, ImVec2(ImMax(fCbHitMaxX, fTextStartX + 4.f), vRowMin.y + fRowH));
    if (rCheckboxHit.Max.x < rCheckboxHit.Min.x + 1.f)
        rCheckboxHit.Max.x = rCheckboxHit.Min.x + 1.f;
    bool bCbHovered = false;
    bool bCbHeld = false;
    const bool bCbPressed = ImGui::ButtonBehavior(rCheckboxHit, idCb, &bCbHovered, &bCbHeld);
    if (bCbPressed)
    {
        *pbValue = !*pbValue;
        ImGui::MarkItemEdited(idCb);
    }
    ImGuiStorage* const pStorage = ImGui::GetStateStorage();
    float* const pfFill = pStorage->GetFloatRef(idCb, 0.f);
    float* const pfHover = pStorage->GetFloatRef(idCb + 9173u, 0.f);
    const float fFadeSpeed = 1.f / CustomSettings::fUiAnimSeconds;
    StepAnimFloat(pfFill, *pbValue ? 1.f : 0.f, fFadeSpeed, fDt);
    StepAnimFloat(pfHover, bCbHovered ? 1.f : 0.f, fFadeSpeed, fDt);
    const ImU32 uBorder = IM_COL32(55, 55, 60, 255);
    ImDrawList* const pDraw = pWindow->DrawList;
    pDraw->AddRectFilled(vBoxMin, vBoxMax, IM_COL32(35, 35, 40, 255), 0.f);
    pDraw->AddRect(vBoxMin, vBoxMax, uBorder, 0.f, 0, 1.f);
    if (*pfFill > 0.01f)
    {
        const ImU32 uFill = ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, *pfFill);
        pDraw->AddRectFilled(vBoxMin, vBoxMax, uFill, 0.f);
    }
    const ImU32 uTextMuted = IM_COL32(140, 140, 145, 255);
    const ImU32 uTextBright = IM_COL32(255, 255, 255, 255);
    const float fTextLerp = (*pfHover > *pfFill) ? *pfHover : *pfFill;
    const ImU32 uTextCol = LerpU32(uTextMuted, uTextBright, fTextLerp);
    const ImVec2 vTextPos(fTextStartX, vRowMin.y + (fRowH - vLabelSize.y) * 0.5f);
    pDraw->AddText(pLabelFont, fLabelPx, vTextPos, uTextCol, label);
    const float fSwatchY = vRowMin.y + (fRowH - fSwatchH) * 0.5f;
    bool bColorChanged = false;
    for (int i = 0; i < iSwatchCount; ++i)
    {
        ImGui::PushID(i);
        const float fX = fSwatchStartX + (fSwatchW + fGapSw) * (float)i;
        const ImVec2 vSwMin(fX, fSwatchY);
        const ImVec2 vSwMax(fX + fSwatchW, fSwatchY + fSwatchH);
        float* const pr = pfRgb + i * 3;
        if (ColorSwatchCompactAt(pWindow, pr, pr + 1, pr + 2, vSwMin, vSwMax))
            bColorChanged = true;
        ImGui::PopID();
    }
    ImGui::PopID();
    return bCbPressed || bColorChanged;
}

bool ColorPickerWithSwatchesRowImpl(const char* label, int iSwatchCount, float* pfRgb)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;
    if (iSwatchCount < 1 || iSwatchCount > 3)
        return false;
    ImGui::PushID(label);
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vLabelSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);
    constexpr float fRowPadY = 4.f;
    constexpr float fGapSw = 4.f;
    const float fSwatchW = CustomSettings::fAccentSwatchW;
    const float fSwatchH = CustomSettings::fAccentSwatchH;
    const float fRowH = ImMax(vLabelSize.y, fSwatchH) + fRowPadY;
    const float fAvail = ImGui::GetContentRegionAvail().x;
    const ImVec2 vRowMin = pWindow->DC.CursorPos;
    float fRowRightX = vRowMin.x + fAvail;
    if (pWindow->ClipRect.Max.x > vRowMin.x && pWindow->ClipRect.Max.x < fRowRightX)
        fRowRightX = pWindow->ClipRect.Max.x;
    const float fPadX = CustomSettings::fRowControlLabelAlignX;
    const float fRowInnerRight = fRowRightX - fPadX;
    const float fLeftX = vRowMin.x + fPadX;
    const float fSwatchBlockW = fSwatchW * (float)iSwatchCount + fGapSw * (float)(iSwatchCount - 1);
    const ImRect totalBB(vRowMin, ImVec2(fRowInnerRight, vRowMin.y + fRowH));
    const ImGuiID idRow = pWindow->GetID("##cprow");
    ImGui::ItemSize(totalBB);
    if (!ImGui::ItemAdd(totalBB, idRow))
    {
        ImGui::PopID();
        return false;
    }
    pWindow->DrawList->AddText(pLabelFont, fLabelPx, ImVec2(fLeftX, vRowMin.y + (fRowH - vLabelSize.y) * 0.5f), IM_COL32(255, 255, 255, 255), label);
    const float fSwatchStartX = fRowInnerRight - fSwatchBlockW;
    const float fSwatchY = vRowMin.y + (fRowH - fSwatchH) * 0.5f;
    bool bColorChanged = false;
    for (int i = 0; i < iSwatchCount; ++i)
    {
        ImGui::PushID(i);
        const float fSwatchX = fSwatchStartX + (fSwatchW + fGapSw) * (float)i;
        const ImVec2 vSwMin(fSwatchX, fSwatchY);
        const ImVec2 vSwMax(fSwatchX + fSwatchW, fSwatchY + fSwatchH);
        float* const pr = pfRgb + i * 3;
        if (ColorSwatchCompactAt(pWindow, pr, pr + 1, pr + 2, vSwMin, vSwMax))
            bColorChanged = true;
        ImGui::PopID();
    }
    ImGui::PopID();
    return bColorChanged;
}

bool CheckboxInternal(const char* label, bool* pbValue, float fLabelStartX)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;
    ImGuiContext& g = *GImGui;
    const ImGuiID id = pWindow->GetID(label);
    const float fDt = g.IO.DeltaTime;
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vLabelSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);
    const float fBoxW = CustomSettings::fSettingsSliderOuterH;
    const float fBoxH = CustomSettings::fSettingsSliderOuterH;
    constexpr float fRowPadY = 4.f;
    constexpr float fGapLabelBox = 6.f;
    const float fRowH = ImMax(vLabelSize.y, fBoxH) + fRowPadY;
    const float fAvail = ImGui::GetContentRegionAvail().x;
    const ImVec2 vRowMin = pWindow->DC.CursorPos;
    float fRowRightX = vRowMin.x + fAvail;
    if (pWindow->ClipRect.Max.x > vRowMin.x && pWindow->ClipRect.Max.x < fRowRightX)
        fRowRightX = pWindow->ClipRect.Max.x;
    const ImRect totalBB(vRowMin, ImVec2(fRowRightX, vRowMin.y + fRowH));
    ImGui::ItemSize(totalBB);
    if (!ImGui::ItemAdd(totalBB, id))
        return false;
    bool bHovered = false;
    bool bHeld = false;
    const bool bPressed = ImGui::ButtonBehavior(totalBB, id, &bHovered, &bHeld);
    if (bPressed)
    {
        *pbValue = !*pbValue;
        ImGui::MarkItemEdited(id);
    }
    ImGuiStorage* const pStorage = ImGui::GetStateStorage();
    float* const pfFill = pStorage->GetFloatRef(id, 0.f);
    float* const pfHover = pStorage->GetFloatRef(id + 9173u, 0.f);
    const float fFadeSpeed = 1.f / CustomSettings::fUiAnimSeconds;
    StepAnimFloat(pfFill, *pbValue ? 1.f : 0.f, fFadeSpeed, fDt);
    StepAnimFloat(pfHover, bHovered ? 1.f : 0.f, fFadeSpeed, fDt);
    const ImU32 uBorder = IM_COL32(55, 55, 60, 255);
    const float fBoxX = vRowMin.x + fLabelStartX;
    const float fBoxY = vRowMin.y + (fRowH - fBoxH) * 0.5f;
    const ImVec2 vBoxMin(fBoxX, fBoxY);
    const ImVec2 vBoxMax(fBoxX + fBoxW, fBoxY + fBoxH);
    ImDrawList* const pDraw = pWindow->DrawList;
    pDraw->AddRectFilled(vBoxMin, vBoxMax, IM_COL32(35, 35, 40, 255), 0.f);
    pDraw->AddRect(vBoxMin, vBoxMax, uBorder, 0.f, 0, 1.f);
    if (*pfFill > 0.01f)
    {
        const ImU32 uFill = ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, *pfFill);
        pDraw->AddRectFilled(vBoxMin, vBoxMax, uFill, 0.f);
    }
    const ImU32 uTextMuted = IM_COL32(140, 140, 145, 255);
    const ImU32 uTextBright = IM_COL32(255, 255, 255, 255);
    const float fTextLerp = (*pfHover > *pfFill) ? *pfHover : *pfFill;
    const ImU32 uTextCol = LerpU32(uTextMuted, uTextBright, fTextLerp);
    const float fTextStartX = fBoxX + fBoxW + fGapLabelBox;
    const ImVec2 vTextPos(fTextStartX, vRowMin.y + (fRowH - vLabelSize.y) * 0.5f);
    pDraw->AddText(pLabelFont, fLabelPx, vTextPos, uTextCol, label);
    return bPressed;
}
}

void ksd::ApplyStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = CustomSettings::fWindowRounding;
    style.ChildRounding = CustomSettings::fChildRounding;
    style.FrameRounding = CustomSettings::fFrameRounding;
    style.PopupRounding = CustomSettings::fPopupRounding;
    style.ScrollbarRounding = CustomSettings::fScrollbarRounding;
    style.GrabRounding = CustomSettings::fGrabRounding;
    style.TabRounding = CustomSettings::fTabRounding;
    style.ScrollbarSize = 10.f;
    style.WindowBorderSize = 0.f;
    style.WindowPadding = ImVec2(0.f, 0.f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
}

void ksd::Initialize(float fDpiScale)
{
    ImGui::StyleColorsDark();
    ApplyStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(fDpiScale);
    style.FontScaleDpi = fDpiScale;
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    gpFontBracket = io.Fonts->AddFontDefault();
    ImFontConfig fontMainConfig;
    fontMainConfig.FontDataOwnedByAtlas = false;
    gpFontBold = io.Fonts->AddFontFromMemoryTTF(EncampTestBold, sizeof(EncampTestBold), CustomSettings::fDefaultFontSize, &fontMainConfig);
    ImFontConfig fontMediumConfig;
    fontMediumConfig.FontDataOwnedByAtlas = false;
    gpFontMedium = io.Fonts->AddFontFromMemoryTTF(EncampTestMedium, sizeof(EncampTestMedium), CustomSettings::fDefaultFontSize, &fontMediumConfig);
    ImFontConfig fontRegularConfig;
    fontRegularConfig.FontDataOwnedByAtlas = false;
    gpFontRegular = io.Fonts->AddFontFromMemoryTTF(EncampTestRegular, sizeof(EncampTestRegular), CustomSettings::fCheckboxLabelFontSize, &fontRegularConfig);
    io.FontDefault = gpFontMedium;
    ImFontConfig fontLogoConfig;
    fontLogoConfig.FontDataOwnedByAtlas = false;
    gpFontLogo = io.Fonts->AddFontFromMemoryTTF(risopop, sizeof(risopop), CustomSettings::fDefaultFontSize + 8.f, &fontLogoConfig);
    ImFontConfig fontStaticTitleConfig;
    fontStaticTitleConfig.FontDataOwnedByAtlas = false;
    gpFontStaticTitle = io.Fonts->AddFontFromMemoryTTF(CitadelofBlackrose, sizeof(CitadelofBlackrose), CustomSettings::fDefaultFontSize + 8.f, &fontStaticTitleConfig);

    static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig fontIconConfig;
    fontIconConfig.FontDataOwnedByAtlas = false;
    fontIconConfig.PixelSnapH = true;
    fontIconConfig.GlyphMinAdvanceX = CustomSettings::fDefaultFontSize;
    gpFontIcon = io.Fonts->AddFontFromMemoryTTF(freesolid900, sizeof(freesolid900), CustomSettings::fDefaultFontSize, &fontIconConfig, iconRanges);
}

ImFont* ksd::GetBoldFont()
{
    return gpFontBold;
}

ImFont* ksd::GetMediumFont()
{
    return gpFontMedium;
}

ImFont* ksd::GetIconFont()
{
    return gpFontIcon;
}

ImFont* ksd::GetLogoFont()
{
    return gpFontLogo;
}

ImFont* ksd::GetStaticTitleFont()
{
    return gpFontStaticTitle;
}

ImFont* ksd::GetRegularFont()
{
    return gpFontRegular;
}

bool ksd::BeginCollapsibleSection(const char* strId, const char* label, float fX, float fY, float fHeaderHeight, float fWidth, bool bDefaultOpen)
{
    if (giCollapsibleStackCount >= kiCollapsibleStackMax)
        return false;
    const float fUseW = fWidth < 8.f ? 8.f : fWidth;
    const float fUseHeaderH = fHeaderHeight < 6.f ? 6.f : fHeaderHeight;
    ImGui::SetCursorPos(ImVec2(fX, fY));
    const ImVec2 vMinScreen = ImGui::GetCursorScreenPos();
    ImGui::PushID(strId);
    bool* const pbOpen = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID("open"), bDefaultOpen);
    ImGui::InvisibleButton("##hdr", ImVec2(fUseW, fUseHeaderH));
    if (ImGui::IsItemClicked())
        *pbOpen = !*pbOpen;

    const ImVec2 vHeaderMax(vMinScreen.x + fUseW, vMinScreen.y + fUseHeaderH);
    ImDrawList* const pDraw = ImGui::GetWindowDrawList();
    const ImU32 uAccent = ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, Colors::fAccentA);
    const ImU32 uOutline = ImColor(Colors::fSectionOutlineR, Colors::fSectionOutlineG, Colors::fSectionOutlineB, Colors::fSectionOutlineA);
    const ImU32 uTitleText = IM_COL32(255, 255, 255, 255);
    const float fTitleFontSize = CustomSettings::fCollapsibleSectionTitleFontSize;
    ImFont* const pTitleFont = gpFontMedium ? gpFontMedium : ImGui::GetFont();
    const ImVec2 vTextSize = pTitleFont->CalcTextSizeA(fTitleFontSize, FLT_MAX, 0.f, label);
    const float fTextX = vMinScreen.x + (fUseW - vTextSize.x) * 0.5f;
    const float fTextY = vMinScreen.y + (fUseHeaderH - vTextSize.y) * 0.5f;
    pDraw->AddText(pTitleFont, fTitleFontSize, ImVec2(fTextX, fTextY), uTitleText, label);

    constexpr float fArrowHalf = 3.25f;
    const float fApexX = vHeaderMax.x - 11.f;
    const float fApexY = vMinScreen.y + fUseHeaderH * 0.5f;
    if (*pbOpen)
    {
        pDraw->AddTriangleFilled(
            ImVec2(fApexX - fArrowHalf, fApexY - fArrowHalf * 0.35f),
            ImVec2(fApexX + fArrowHalf, fApexY - fArrowHalf * 0.35f),
            ImVec2(fApexX, fApexY + fArrowHalf),
            uAccent
        );
    }
    else
    {
        pDraw->AddTriangleFilled(
            ImVec2(fApexX - fArrowHalf * 0.35f, fApexY - fArrowHalf),
            ImVec2(fApexX - fArrowHalf * 0.35f, fApexY + fArrowHalf),
            ImVec2(fApexX + fArrowHalf, fApexY),
            uAccent
        );
    }

    const float fHeaderLineY = vHeaderMax.y - 0.5f;
    pDraw->AddLine(ImVec2(vMinScreen.x + 1.f, fHeaderLineY), ImVec2(vHeaderMax.x - 1.f, fHeaderLineY), uOutline, 1.f);

    CollapsibleSectionCtx& ctx = gCollapsibleStack[giCollapsibleStackCount++];
    ctx.vMinScreen = vMinScreen;
    ctx.fWidth = fUseW;
    ctx.fHeaderHeight = fUseHeaderH;
    ctx.pbOpen = pbOpen;
    ctx.bPushedItemWidth = false;
    ctx.bPushedClipRect = false;
    constexpr float fBodyPadX = 10.f;
    constexpr float fBodyPadTop = 6.f;
    if (*pbOpen)
    {
        ImGui::PushItemWidth(fUseW - fBodyPadX * 2.f);
        ctx.bPushedItemWidth = true;
        ImGui::SetCursorScreenPos(ImVec2(vMinScreen.x + fBodyPadX, vHeaderMax.y + fBodyPadTop));
        const ImVec2 vClipMin(vMinScreen.x + fBodyPadX - 6.f, vHeaderMax.y + 1.f);
        const ImVec2 vWinPos = ImGui::GetWindowPos();
        const ImVec2 vClipMax(vMinScreen.x + fUseW - fBodyPadX, vWinPos.y + ImGui::GetWindowSize().y);
        ImGui::PushClipRect(vClipMin, vClipMax, true);
        ctx.bPushedClipRect = true;
        return true;
    }
    ImGui::SetCursorScreenPos(ImVec2(vMinScreen.x, vHeaderMax.y));
    return false;
}

void ksd::EndCollapsibleSection()
{
    if (giCollapsibleStackCount <= 0)
        return;
    const CollapsibleSectionCtx ctx = gCollapsibleStack[--giCollapsibleStackCount];
    constexpr float fBodyPadTop = 6.f;
    constexpr float fBodyPadBottom = 8.f;
    if (ctx.bPushedClipRect)
        ImGui::PopClipRect();
    if (ctx.bPushedItemWidth)
        ImGui::PopItemWidth();

    float fBottomY;
    if (*ctx.pbOpen)
    {
        fBottomY = ImGui::GetCursorScreenPos().y + fBodyPadBottom;
        const float fMinOpenBottom = ctx.vMinScreen.y + ctx.fHeaderHeight + fBodyPadTop + fBodyPadBottom;
        if (fBottomY < fMinOpenBottom)
            fBottomY = fMinOpenBottom;
    }
    else
        fBottomY = ctx.vMinScreen.y + ctx.fHeaderHeight;

    const ImVec2 vMaxScreen(ctx.vMinScreen.x + ctx.fWidth, fBottomY);
    ImDrawList* const pDraw = ImGui::GetWindowDrawList();
    const ImU32 uOutlineRect = ImColor(Colors::fSectionOutlineR, Colors::fSectionOutlineG, Colors::fSectionOutlineB, Colors::fSectionOutlineA);
    pDraw->AddRect(ctx.vMinScreen, vMaxScreen, uOutlineRect, CustomSettings::fChildRounding, ImDrawFlags_RoundCornersAll, 1.f);

    ImGui::PopID();
    ImGui::SetCursorScreenPos(ImVec2(ctx.vMinScreen.x, vMaxScreen.y));
    ImGui::Dummy(ImVec2(1.f, CustomSettings::fSectionGapAfterPx));
}

bool ksd::BeginGroupBox(const char* strId, const char* label, float fX, float fY, float fHeaderHeight, float fWidth, float fHeight)
{
    if (giGroupBoxStackCount >= kiCollapsibleStackMax)
        return false;
    const float fUseW = fWidth < 8.f ? 8.f : fWidth;
    const float fUseHeaderH = fHeaderHeight < 6.f ? 6.f : fHeaderHeight;

    ImGui::SetCursorPos(ImVec2(fX, fY));
    const ImVec2 vMinScreen = ImGui::GetCursorScreenPos();
    ImGui::PushID(strId);
    const float fTitleFontSize = CustomSettings::fCollapsibleSectionTitleFontSize;
    ImFont* const pTitleFont = gpFontMedium ? gpFontMedium : ImGui::GetFont();
    const ImVec2 vTextSize = pTitleFont->CalcTextSizeA(fTitleFontSize, FLT_MAX, 0.f, label);
    const float fTextX = vMinScreen.x + 19.f;
    const float fTextY = vMinScreen.y - (vTextSize.y * 0.5f);
    float fContentStartLocalY = (fTextY - vMinScreen.y) + vTextSize.y + 4.f;
    if (fContentStartLocalY < fUseHeaderH * 0.5f)
        fContentStartLocalY = fUseHeaderH * 0.5f;

    GroupBoxCtx& ctx = gGroupBoxStack[giGroupBoxStackCount++];
    ctx.vMinScreen = vMinScreen;
    ctx.fWidth = fUseW;
    ctx.fHeaderHeight = fContentStartLocalY;
    ctx.fHeight = fHeight;
    ctx.fCutLeftX = fTextX - 6.f;
    ctx.fCutRightX = fTextX + vTextSize.x + 6.f;
    ctx.vTextPos = ImVec2(fTextX, fTextY);
    ctx.pLabel = label;
    ctx.bPushedItemWidth = false;
    ctx.bPushedClipRect = false;
    ctx.bBeganChild = false;
    constexpr float fBodyPadX = 10.f;
    constexpr float fBodyPadTop = 0.f;
    if (fHeight > 0.f)
    {
        const float fBodyH = fHeight - fContentStartLocalY - fBodyPadTop - 8.f;
        const float fUseBodyH = fBodyH < 30.f ? 30.f : fBodyH;
        ImGui::SetCursorScreenPos(ImVec2(vMinScreen.x + fBodyPadX, vMinScreen.y + fContentStartLocalY + fBodyPadTop));
        ImGui::BeginChild("##groupbody", ImVec2(fUseW - fBodyPadX * 2.f, fUseBodyH), false, ImGuiWindowFlags_NoBackground);
        ctx.bBeganChild = true;
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ctx.bPushedItemWidth = true;
    }
    else
    {
        ImGui::SetCursorScreenPos(ImVec2(vMinScreen.x + fBodyPadX, vMinScreen.y + fContentStartLocalY + fBodyPadTop));
        ImGui::BeginChild("##groupbody", ImVec2(fUseW - fBodyPadX * 2.f, 0.f), ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoBackground);
        ctx.bBeganChild = true;
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ctx.bPushedItemWidth = true;
    }
    return true;
}

void ksd::EndGroupBox()
{
    if (giGroupBoxStackCount <= 0)
        return;
    const GroupBoxCtx ctx = gGroupBoxStack[--giGroupBoxStackCount];
    constexpr float fBodyPadTop = 6.f;
    constexpr float fBodyPadBottom = 8.f;
    if (ctx.bBeganChild)
    {
        if (ctx.bPushedItemWidth)
            ImGui::PopItemWidth();
        ImGui::EndChild();
    }
    else
    {
        if (ctx.bPushedClipRect)
            ImGui::PopClipRect();
        if (ctx.bPushedItemWidth)
            ImGui::PopItemWidth();
    }

    float fBottomY;
    if (ctx.fHeight > 0.f)
        fBottomY = ctx.vMinScreen.y + ctx.fHeight;
    else
    {
        fBottomY = ImGui::GetCursorScreenPos().y + fBodyPadBottom;
        const float fMinOpenBottom = ctx.vMinScreen.y + ctx.fHeaderHeight + fBodyPadTop + fBodyPadBottom;
        if (fBottomY < fMinOpenBottom)
            fBottomY = fMinOpenBottom;
    }

    const ImVec2 vMaxScreen(ctx.vMinScreen.x + ctx.fWidth, fBottomY);
    ImDrawList* const pDraw = ImGui::GetWindowDrawList();
    const ImU32 uOutlineRect = ImColor(Colors::fSectionOutlineR, Colors::fSectionOutlineG, Colors::fSectionOutlineB, Colors::fSectionOutlineA);
    const ImU32 uMenuBg = ImColor(Colors::fBackgroundR, Colors::fBackgroundG, Colors::fBackgroundB, Colors::fBackgroundA);
    const ImU32 uTitleText = IM_COL32(255, 255, 255, 255);
    const float fTitleFontSize = CustomSettings::fCollapsibleSectionTitleFontSize;
    ImFont* const pTitleFont = gpFontMedium ? gpFontMedium : ImGui::GetFont();
    const ImVec2 vMin = ctx.vMinScreen;
    pDraw->AddRect(vMin, vMaxScreen, uOutlineRect, CustomSettings::fChildRounding, ImDrawFlags_RoundCornersAll, 1.f);
    pDraw->AddLine(ImVec2(ctx.fCutLeftX, vMin.y), ImVec2(ctx.fCutRightX, vMin.y), uMenuBg, 2.f);
    pDraw->AddText(pTitleFont, fTitleFontSize, ctx.vTextPos, uTitleText, ctx.pLabel);

    ImGui::PopID();
    ImGui::SetCursorScreenPos(ImVec2(ctx.vMinScreen.x, vMaxScreen.y));
    ImGui::Dummy(ImVec2(1.f, CustomSettings::fSectionGapAfterPx));
}

bool ksd::Checkbox(const char* label, bool* pbValue)
{
    return CheckboxInternal(label, pbValue, fCheckboxSliderAlignX);
}

bool ksd::SliderFloat(const char* displayName, float* pfValue, float fMin, float fMax, const char* fmt)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vNameSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, displayName);
    const float fOuterH = CustomSettings::fSettingsSliderOuterH;
    const float fInnerH = CustomSettings::fSettingsSliderInnerH;
    const float fBtnW = CustomSettings::fSettingsSliderBtnZoneW;
    const float fGapLabel = CustomSettings::fSettingsSliderLabelGapY;
    const float fGapVal = CustomSettings::fSettingsSliderValueGapY;
    const float fValuePx = fLabelPx - 1.f;
    const ImVec2 vRowMin = pWindow->DC.CursorPos;
    const float fAvail = ImGui::GetContentRegionAvail().x;
    float fRowRightX = vRowMin.x + fAvail;
    if (pWindow->ClipRect.Max.x > vRowMin.x && pWindow->ClipRect.Max.x < fRowRightX)
        fRowRightX = pWindow->ClipRect.Max.x;
    const float fPadX = CustomSettings::fRowControlLabelAlignX;
    const float fRowInnerRight = fRowRightX - fPadX;
    const float fLeftX = vRowMin.x + fPadX;
    const float fRightX = fRowInnerRight - 2.f;
    const float fInnerRowW = fRightX - fLeftX;
    float fTrackOuterW = fInnerRowW - fBtnW - 2.f;
    if (fTrackOuterW < 20.f)
        fTrackOuterW = 20.f;
    const float fInnerW = fTrackOuterW - 2.f;
    const float fValueH = pLabelFont->CalcTextSizeA(fValuePx, FLT_MAX, 0.f, "0.000").y;
    const float fRowBlockH = vNameSize.y + fGapLabel + fOuterH + fGapVal + fValueH;
    const ImRect totalBB(vRowMin, ImVec2(fRowInnerRight, vRowMin.y + fRowBlockH));
    ImGui::PushID(displayName);
    const ImGuiID idRoot = pWindow->GetID("##slroot");
    const ImVec2 vBlockTopLeft = ImGui::GetCursorScreenPos();
    ImGui::ItemSize(totalBB);
    if (!ImGui::ItemAdd(totalBB, idRoot))
    {
        ImGui::PopID();
        return false;
    }
    const ImU32 uOuter = IM_COL32(37, 37, 37, 255);
    const ImU32 uInnerBg = IM_COL32(23, 23, 23, 255);
    const ImU32 uAccent = ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, Colors::fAccentA);
    const ImU32 uGlyph = IM_COL32(205, 205, 205, 255);
    const ImU32 uValueText = IM_COL32(140, 140, 145, 255);
    ImDrawList* const pDraw = pWindow->DrawList;
    pDraw->AddText(pLabelFont, fLabelPx, ImVec2(fLeftX, vRowMin.y), IM_COL32(255, 255, 255, 255), displayName);
    const float fSliderY = vRowMin.y + vNameSize.y + fGapLabel;
    const float fTrackX = fLeftX;
    const float fMinusX = fTrackX - fBtnW - 2.f;
    const float fPlusX = fTrackX + fTrackOuterW + 2.f;
    const ImVec2 vTrackOuterMin(fTrackX, fSliderY);
    const ImVec2 vTrackOuterMax(fTrackX + fTrackOuterW, fSliderY + fOuterH);
    const ImVec2 vInnerMin(vTrackOuterMin.x + 1.f, vTrackOuterMin.y + (fOuterH - fInnerH) * 0.5f);
    const ImVec2 vInnerMax(vInnerMin.x + fInnerW, vInnerMin.y + fInnerH);
    pDraw->AddRectFilled(vTrackOuterMin, vTrackOuterMax, uOuter, 0.f);
    pDraw->AddRectFilled(vInnerMin, vInnerMax, uInnerBg, 0.f);
    const float fSpan = fMax - fMin;
    float fT = 0.f;
    if (ImFabs(fSpan) > 1e-20f)
        fT = (*pfValue - fMin) / fSpan;
    fT = ImSaturate(fT);
    const float fFillW = fInnerW * fT;
    if (fFillW > 0.5f)
        pDraw->AddRectFilled(vInnerMin, ImVec2(vInnerMin.x + fFillW, vInnerMax.y), uAccent, 0.f);
    const float fCx = fMinusX + fBtnW * 0.5f;
    const float fCy = fSliderY + fOuterH * 0.5f;
    pDraw->AddLine(ImVec2(fCx - 3.f, fCy), ImVec2(fCx + 3.f, fCy), uGlyph, 1.f);
    const float fPx2 = fPlusX + fBtnW * 0.5f;
    const float fPy2 = fSliderY + fOuterH * 0.5f;
    pDraw->AddLine(ImVec2(fPx2 - 3.f, fPy2), ImVec2(fPx2 + 3.f, fPy2), uGlyph, 1.f);
    pDraw->AddLine(ImVec2(fPx2, fPy2 - 3.f), ImVec2(fPx2, fPy2 + 3.f), uGlyph, 1.f);
    char szBuf[64];
    snprintf(szBuf, sizeof szBuf, fmt, *pfValue);
    const ImVec2 vValSize = pLabelFont->CalcTextSizeA(fValuePx, FLT_MAX, 0.f, szBuf);
    float fValX = vInnerMin.x + fFillW - vValSize.x * 0.5f;
    if (fValX < fTrackX)
        fValX = fTrackX;
    if (fValX + vValSize.x > fRightX)
        fValX = fRightX - vValSize.x;
    const float fValY = vInnerMax.y + fGapVal;
    pDraw->AddText(pLabelFont, fValuePx, ImVec2(fValX, fValY), uValueText, szBuf);
    bool bChanged = false;
    constexpr float fFloatStep = 0.1f;
    const float fDySlider = vNameSize.y + fGapLabel;
    const ImVec2 vMinusScr(fMinusX, vBlockTopLeft.y + fDySlider);
    const ImVec2 vTrackScr(fTrackX, vBlockTopLeft.y + fDySlider);
    const ImVec2 vPlusScr(fPlusX, vBlockTopLeft.y + fDySlider);
    const ImVec2 vInnerScrMin(vTrackScr.x + 1.f, vTrackScr.y + (fOuterH - fInnerH) * 0.5f);
    ImGui::SetCursorScreenPos(vMinusScr);
    ImGui::InvisibleButton("##minus", ImVec2(fBtnW, fOuterH));
    if (ImGui::IsItemClicked())
    {
        *pfValue = ImMax(fMin, *pfValue - fFloatStep);
        bChanged = true;
    }
    ImGui::SetCursorScreenPos(vTrackScr);
    ImGui::InvisibleButton("##track", ImVec2(fTrackOuterW, fOuterH));
    if (fInnerW > 1.f)
    {
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        {
            const float mx = ImGui::GetIO().MousePos.x - vInnerScrMin.x;
            const float nt = ImSaturate(mx / fInnerW);
            *pfValue = fMin + nt * fSpan;
            *pfValue = ImFloor(*pfValue * 10.f + 0.5f) / 10.f;
            bChanged = true;
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
        {
            const float mx = ImGui::GetIO().MousePos.x - vInnerScrMin.x;
            const float nt = ImSaturate(mx / fInnerW);
            *pfValue = fMin + nt * fSpan;
            *pfValue = ImFloor(*pfValue * 10.f + 0.5f) / 10.f;
            bChanged = true;
        }
    }
    ImGui::SetCursorScreenPos(vPlusScr);
    ImGui::InvisibleButton("##plus", ImVec2(fBtnW, fOuterH));
    if (ImGui::IsItemClicked())
    {
        *pfValue = ImMin(fMax, *pfValue + fFloatStep);
        bChanged = true;
    }
    *pfValue = ImClamp(*pfValue, fMin, fMax);
    *pfValue = ImFloor(*pfValue * 10.f + 0.5f) / 10.f;
    ImGui::SetCursorScreenPos(ImVec2(vBlockTopLeft.x, vBlockTopLeft.y + fRowBlockH));
    if (bChanged)
        ImGui::MarkItemEdited(idRoot);
    ImGui::PopID();
    return bChanged;
}


bool ksd::SliderInt(const char* displayName, int* piValue, int iMin, int iMax, const char* fmt)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vNameSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, displayName);
    const float fOuterH = CustomSettings::fSettingsSliderOuterH;
    const float fInnerH = CustomSettings::fSettingsSliderInnerH;
    const float fBtnW = CustomSettings::fSettingsSliderBtnZoneW;
    const float fGapLabel = CustomSettings::fSettingsSliderLabelGapY;
    const float fGapVal = CustomSettings::fSettingsSliderValueGapY;
    const float fValuePx = fLabelPx - 1.f;
    const ImVec2 vRowMin = pWindow->DC.CursorPos;
    const float fAvail = ImGui::GetContentRegionAvail().x;
    float fRowRightX = vRowMin.x + fAvail;
    if (pWindow->ClipRect.Max.x > vRowMin.x && pWindow->ClipRect.Max.x < fRowRightX)
        fRowRightX = pWindow->ClipRect.Max.x;
    const float fPadX = CustomSettings::fRowControlLabelAlignX;
    const float fRowInnerRight = fRowRightX - fPadX;
    const float fLeftX = vRowMin.x + fPadX;
    const float fRightX = fRowInnerRight - 2.f;
    const float fInnerRowW = fRightX - fLeftX;
    float fTrackOuterW = fInnerRowW - fBtnW - 2.f;
    if (fTrackOuterW < 20.f)
        fTrackOuterW = 20.f;
    const float fInnerW = fTrackOuterW - 2.f;
    const float fValueH = pLabelFont->CalcTextSizeA(fValuePx, FLT_MAX, 0.f, "000").y;
    const float fRowBlockH = vNameSize.y + fGapLabel + fOuterH + fGapVal + fValueH;
    const ImRect totalBB(vRowMin, ImVec2(fRowInnerRight, vRowMin.y + fRowBlockH));
    ImGui::PushID(displayName);
    const ImGuiID idRoot = pWindow->GetID("##slroot");
    const ImVec2 vBlockTopLeft = ImGui::GetCursorScreenPos();
    ImGui::ItemSize(totalBB);
    if (!ImGui::ItemAdd(totalBB, idRoot))
    {
        ImGui::PopID();
        return false;
    }
    const ImU32 uOuter = IM_COL32(37, 37, 37, 255);
    const ImU32 uInnerBg = IM_COL32(23, 23, 23, 255);
    const ImU32 uAccent = ImColor(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, Colors::fAccentA);
    const ImU32 uGlyph = IM_COL32(205, 205, 205, 255);
    const ImU32 uValueText = IM_COL32(140, 140, 145, 255);
    ImDrawList* const pDraw = pWindow->DrawList;
    pDraw->AddText(pLabelFont, fLabelPx, ImVec2(fLeftX, vRowMin.y), IM_COL32(255, 255, 255, 255), displayName);
    const float fSliderY = vRowMin.y + vNameSize.y + fGapLabel;
    const float fTrackX = fLeftX;
    const float fMinusX = fTrackX - fBtnW - 2.f;
    const float fPlusX = fTrackX + fTrackOuterW + 2.f;
    const ImVec2 vTrackOuterMin(fTrackX, fSliderY);
    const ImVec2 vTrackOuterMax(fTrackX + fTrackOuterW, fSliderY + fOuterH);
    const ImVec2 vInnerMin(vTrackOuterMin.x + 1.f, vTrackOuterMin.y + (fOuterH - fInnerH) * 0.5f);
    const ImVec2 vInnerMax(vInnerMin.x + fInnerW, vInnerMin.y + fInnerH);
    pDraw->AddRectFilled(vTrackOuterMin, vTrackOuterMax, uOuter, 0.f);
    pDraw->AddRectFilled(vInnerMin, vInnerMax, uInnerBg, 0.f);
    const float fSpan = (float)(iMax - iMin);
    float fT = 0.f;
    if (ImFabs(fSpan) > 1e-20f)
        fT = ((float)*piValue - (float)iMin) / fSpan;
    fT = ImSaturate(fT);
    const float fFillW = fInnerW * fT;
    if (fFillW > 0.5f)
        pDraw->AddRectFilled(vInnerMin, ImVec2(vInnerMin.x + fFillW, vInnerMax.y), uAccent, 0.f);
    const float fCx = fMinusX + fBtnW * 0.5f;
    const float fCy = fSliderY + fOuterH * 0.5f;
    pDraw->AddLine(ImVec2(fCx - 3.f, fCy), ImVec2(fCx + 3.f, fCy), uGlyph, 1.f);
    const float fPx2 = fPlusX + fBtnW * 0.5f;
    const float fPy2 = fSliderY + fOuterH * 0.5f;
    pDraw->AddLine(ImVec2(fPx2 - 3.f, fPy2), ImVec2(fPx2 + 3.f, fPy2), uGlyph, 1.f);
    pDraw->AddLine(ImVec2(fPx2, fPy2 - 3.f), ImVec2(fPx2, fPy2 + 3.f), uGlyph, 1.f);
    char szBuf[64];
    snprintf(szBuf, sizeof szBuf, fmt, *piValue);
    const ImVec2 vValSize = pLabelFont->CalcTextSizeA(fValuePx, FLT_MAX, 0.f, szBuf);
    float fValX = vInnerMin.x + fFillW - vValSize.x * 0.5f;
    if (fValX < fTrackX)
        fValX = fTrackX;
    if (fValX + vValSize.x > fRightX)
        fValX = fRightX - vValSize.x;
    const float fValY = vInnerMax.y + fGapVal;
    pDraw->AddText(pLabelFont, fValuePx, ImVec2(fValX, fValY), uValueText, szBuf);
    bool bChanged = false;
    const float fDySlider = vNameSize.y + fGapLabel;
    const ImVec2 vMinusScr(fMinusX, vBlockTopLeft.y + fDySlider);
    const ImVec2 vTrackScr(fTrackX, vBlockTopLeft.y + fDySlider);
    const ImVec2 vPlusScr(fPlusX, vBlockTopLeft.y + fDySlider);
    const ImVec2 vInnerScrMin(vTrackScr.x + 1.f, vTrackScr.y + (fOuterH - fInnerH) * 0.5f);
    ImGui::SetCursorScreenPos(vMinusScr);
    ImGui::InvisibleButton("##minus", ImVec2(fBtnW, fOuterH));
    if (ImGui::IsItemClicked())
    {
        *piValue = ImMax(iMin, *piValue - 1);
        bChanged = true;
    }
    ImGui::SetCursorScreenPos(vTrackScr);
    ImGui::InvisibleButton("##track", ImVec2(fTrackOuterW, fOuterH));
    if (fInnerW > 1.f)
    {
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        {
            const float mx = ImGui::GetIO().MousePos.x - vInnerScrMin.x;
            const float nt = ImSaturate(mx / fInnerW);
            const int iNew = iMin + (int)ImFloor(nt * fSpan + 0.5f);
            *piValue = ImClamp(iNew, iMin, iMax);
            bChanged = true;
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
        {
            const float mx = ImGui::GetIO().MousePos.x - vInnerScrMin.x;
            const float nt = ImSaturate(mx / fInnerW);
            const int iNew = iMin + (int)ImFloor(nt * fSpan + 0.5f);
            *piValue = ImClamp(iNew, iMin, iMax);
            bChanged = true;
        }
    }
    ImGui::SetCursorScreenPos(vPlusScr);
    ImGui::InvisibleButton("##plus", ImVec2(fBtnW, fOuterH));
    if (ImGui::IsItemClicked())
    {
        *piValue = ImMin(iMax, *piValue + 1);
        bChanged = true;
    }
    *piValue = ImClamp(*piValue, iMin, iMax);
    ImGui::SetCursorScreenPos(ImVec2(vBlockTopLeft.x, vBlockTopLeft.y + fRowBlockH));
    if (bChanged)
        ImGui::MarkItemEdited(idRoot);
    ImGui::PopID();
    return bChanged;
}

bool ksd::InputText(const char* label, char* buf, int iBufSize, ImGuiInputTextFlags flags)
{
    ImFont* pFont = GetRegularFont();
    if (!pFont)
        pFont = GetMediumFont();
    ImGui::PushFont(pFont);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CustomSettings::fRowControlLabelAlignX);
    float fFieldW = ImGui::GetContentRegionAvail().x - CustomSettings::fRowControlLabelAlignX;
    if (fFieldW < 1.f)
        fFieldW = 1.f;
    ImGui::PushItemWidth(fFieldW);
    const ImGuiStyle& style = ImGui::GetStyle();
    const float fLineH = ImGui::GetTextLineHeight();
    float fPadY = (CustomSettings::fRowControlLabelPadButtonH - fLineH) * 0.5f;
    if (fPadY < 2.f)
        fPadY = 2.f;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x + CustomSettings::fFieldTypedTextPadExtraX, fPadY));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, 0.48f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(30, 30, 33, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(36, 36, 40, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(33, 33, 37, 255));
    ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Border]));
    const bool bChanged = ImGui::InputText(label, buf, (size_t)iBufSize, flags);
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(3);
    ImGui::PopItemWidth();
    ImGui::PopFont();
    return bChanged;
}

bool ksd::ColorPicker(const char* label, float* pr, float* pg, float* pb)
{
    float fRgb[3] = { *pr, *pg, *pb };
    const bool bChanged = ColorPickerWithSwatchesRowImpl(label, 1, fRgb);
    *pr = fRgb[0];
    *pg = fRgb[1];
    *pb = fRgb[2];
    return bChanged;
}

bool ksd::ColorPicker2(
    const char* label,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1)
{
    float fRgb[6] = { *pr0, *pg0, *pb0, *pr1, *pg1, *pb1 };
    const bool bChanged = ColorPickerWithSwatchesRowImpl(label, 2, fRgb);
    *pr0 = fRgb[0];
    *pg0 = fRgb[1];
    *pb0 = fRgb[2];
    *pr1 = fRgb[3];
    *pg1 = fRgb[4];
    *pb1 = fRgb[5];
    return bChanged;
}

bool ksd::ColorPicker3(
    const char* label,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1,
    float* pr2, float* pg2, float* pb2)
{
    float fRgb[9] = { *pr0, *pg0, *pb0, *pr1, *pg1, *pb1, *pr2, *pg2, *pb2 };
    const bool bChanged = ColorPickerWithSwatchesRowImpl(label, 3, fRgb);
    *pr0 = fRgb[0];
    *pg0 = fRgb[1];
    *pb0 = fRgb[2];
    *pr1 = fRgb[3];
    *pg1 = fRgb[4];
    *pb1 = fRgb[5];
    *pr2 = fRgb[6];
    *pg2 = fRgb[7];
    *pb2 = fRgb[8];
    return bChanged;
}

bool ksd::CheckboxColorPicker1(const char* label, bool* pbValue, float* pr, float* pg, float* pb)
{
    float fRgb[3] = { *pr, *pg, *pb };
    const bool bChanged = CheckboxWithColorSwatchesRowImpl(label, pbValue, 1, fRgb);
    *pr = fRgb[0];
    *pg = fRgb[1];
    *pb = fRgb[2];
    return bChanged;
}

bool ksd::CheckboxColorPicker2(
    const char* label,
    bool* pbValue,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1)
{
    float fRgb[6] = { *pr0, *pg0, *pb0, *pr1, *pg1, *pb1 };
    const bool bChanged = CheckboxWithColorSwatchesRowImpl(label, pbValue, 2, fRgb);
    *pr0 = fRgb[0];
    *pg0 = fRgb[1];
    *pb0 = fRgb[2];
    *pr1 = fRgb[3];
    *pg1 = fRgb[4];
    *pb1 = fRgb[5];
    return bChanged;
}

bool ksd::CheckboxColorPicker3(
    const char* label,
    bool* pbValue,
    float* pr0, float* pg0, float* pb0,
    float* pr1, float* pg1, float* pb1,
    float* pr2, float* pg2, float* pb2)
{
    float fRgb[9] = { *pr0, *pg0, *pb0, *pr1, *pg1, *pb1, *pr2, *pg2, *pb2 };
    const bool bChanged = CheckboxWithColorSwatchesRowImpl(label, pbValue, 3, fRgb);
    *pr0 = fRgb[0];
    *pg0 = fRgb[1];
    *pb0 = fRgb[2];
    *pr1 = fRgb[3];
    *pg1 = fRgb[4];
    *pb1 = fRgb[5];
    *pr2 = fRgb[6];
    *pg2 = fRgb[7];
    *pb2 = fRgb[8];
    return bChanged;
}

bool ksd::KeyBindRow(const char* label, int* piKey)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;
    ImGui::PushID(label);
    bool* pbListen = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID("listen"), false);
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const float fBindPx = CustomSettings::fKeyBindFontPx;
    const ImVec2 vNameSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);
    const ImVec2 vRowMin = pWindow->DC.CursorPos;
    const float fAvail = ImGui::GetContentRegionAvail().x;
    float fRowRightX = vRowMin.x + fAvail;
    if (pWindow->ClipRect.Max.x > vRowMin.x && pWindow->ClipRect.Max.x < fRowRightX)
        fRowRightX = pWindow->ClipRect.Max.x;
    const float fPadX = CustomSettings::fRowControlLabelAlignX;
    const float fRowInnerRight = fRowRightX - fPadX;
    const float fLeftX = vRowMin.x + fPadX;
    const char* inner = " ";
    if (*pbListen)
        inner = "...";
    else if (*piKey != 0)
    {
        inner = ImGui::GetKeyName((ImGuiKey)(*piKey));
        if (!inner || !inner[0])
            inner = " ";
    }
    const char* openB = "[";
    const char* closeB = "]";
    ImFont* const pBracketFont = gpFontBracket ? gpFontBracket : pLabelFont;
    const float fBracketPx = fBindPx;
    const float wOpen = pBracketFont->CalcTextSizeA(fBracketPx, FLT_MAX, 0.f, openB).x;
    const float wInner = pLabelFont->CalcTextSizeA(fBindPx, FLT_MAX, 0.f, inner).x;
    const float wClose = pBracketFont->CalcTextSizeA(fBracketPx, FLT_MAX, 0.f, closeB).x;
    const float fBindW = wOpen + wInner + wClose;
    const float fBindH = ImMax(
        ImMax(pLabelFont->CalcTextSizeA(fBindPx, FLT_MAX, 0.f, inner).y, pBracketFont->CalcTextSizeA(fBracketPx, FLT_MAX, 0.f, openB).y),
        pBracketFont->CalcTextSizeA(fBracketPx, FLT_MAX, 0.f, closeB).y);
    const float fRowH = ImMax(vNameSize.y, fBindH) + 4.f;
    const ImRect totalBB(vRowMin, ImVec2(fRowInnerRight, vRowMin.y + fRowH));
    const ImGuiID idBind = pWindow->GetID("bind");
    ImGui::ItemSize(totalBB);
    if (!ImGui::ItemAdd(totalBB, idBind))
    {
        ImGui::PopID();
        return false;
    }
    pWindow->DrawList->AddText(pLabelFont, fLabelPx, ImVec2(fLeftX, vRowMin.y + (fRowH - vNameSize.y) * 0.5f), IM_COL32(255, 255, 255, 255), label);
    const float fBindX = fRowInnerRight - fBindW;
    const float fBindY = vRowMin.y + (fRowH - fBindH) * 0.5f;
    const ImVec2 vBindMin(fBindX - 2.f, fBindY - 1.f);
    const ImVec2 vBindMax(fBindX + fBindW + 2.f, fBindY + fBindH + 1.f);
    bool bBindHover = false;
    bool bBindHeld = false;
    const bool bBindPressed = ImGui::ButtonBehavior(ImRect(vBindMin, vBindMax), idBind, &bBindHover, &bBindHeld);
    if (bBindPressed)
        *pbListen = !*pbListen;
    bool bKeyChanged = false;
    if (*pbListen)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
            *pbListen = false;
        else
        {
            for (int k = (int)ImGuiKey_NamedKey_BEGIN; k < (int)ImGuiKey_NamedKey_END; ++k)
            {
                const ImGuiKey key = (ImGuiKey)k;
                if (key == ImGuiKey_Escape)
                    continue;
                if (key >= ImGuiKey_MouseLeft && key <= ImGuiKey_MouseWheelY)
                    continue;
                if (key >= ImGuiKey_LeftCtrl && key <= ImGuiKey_RightSuper)
                    continue;
                if (ImGui::IsKeyPressed(key, false))
                {
                    *piKey = k;
                    *pbListen = false;
                    bKeyChanged = true;
                    break;
                }
            }
        }
    }
    const ImU32 uBindCol = IM_COL32(140, 140, 145, 255);
    float fX = fBindX;
    pWindow->DrawList->AddText(pBracketFont, fBracketPx, ImVec2(fX, fBindY), uBindCol, openB);
    fX += wOpen;
    pWindow->DrawList->AddText(pLabelFont, fBindPx, ImVec2(fX, fBindY), uBindCol, inner);
    fX += wInner;
    pWindow->DrawList->AddText(pBracketFont, fBracketPx, ImVec2(fX, fBindY), uBindCol, closeB);
    ImGui::PopID();
    return bKeyChanged;
}

float ComboPopupHeightForItems(int iItemCount);
bool BeginComboPopupWindow(ImGuiID popupId, const ImRect& bb, ImGuiComboFlags flags);

bool ksd::BeginCombo(const char* label, const char* previewValue, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CustomSettings::fRowControlLabelAlignX);
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = pWindow->GetID(label);
    const float fArrowSize = (flags & ImGuiComboFlags_NoArrowButton) ? 0.f : ImGui::GetFrameHeight();
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vLabelSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);
    float fW = ImGui::GetContentRegionAvail().x;
    if (fW < 1.f)
        fW = 1.f;
    fW -= CustomSettings::fRowControlLabelAlignX;
    if (fW < 1.f)
        fW = 1.f;

    const ImVec2 vPos = pWindow->DC.CursorPos;
    ImVec2 vOffset(0.f, vLabelSize.x > 0.f ? vLabelSize.y + style.ItemInnerSpacing.y : 0.f);
    ImVec2 vBbMin(vPos.x + vOffset.x, vPos.y + vOffset.y);
    ImVec2 vBbMax(vPos.x + fW + vOffset.x, vPos.y + style.FramePadding.y * 2.f + vLabelSize.y + vOffset.y + 2.f);
    ImRect bb(vBbMin, vBbMax);
    ImRect totalBb(vPos, vBbMax);

    ImGui::ItemSize(totalBb, style.FramePadding.y);
    if (!ImGui::ItemAdd(totalBb, id, &bb))
        return false;

    bool bHovered = false;
    bool bHeld = false;
    const bool bPressed = ImGui::ButtonBehavior(bb, id, &bHovered, &bHeld);
    const ImGuiID popupId = ImHashStr("##ComboPopup", 0, id);
    bool bPopupOpen = ImGui::IsPopupOpen(popupId, ImGuiPopupFlags_None);
    if (bPressed && !bPopupOpen)
    {
        ImGui::OpenPopupEx(popupId, ImGuiPopupFlags_None);
        bPopupOpen = true;
    }

    struct ComboAnimState
    {
        float fTopFactor;
    };
    static std::map<ImGuiID, ComboAnimState> mAnimState;
    auto pAnimIt = mAnimState.find(id);
    if (pAnimIt == mAnimState.end())
        pAnimIt = mAnimState.insert({ id, ComboAnimState{0.15f} }).first;

    const float fTarget = bHovered ? 1.f : 0.f;
    const float fSpeed = CustomSettings::fUiAnimSeconds > 0.f ? (1.f / CustomSettings::fUiAnimSeconds) : 1.f;
    pAnimIt->second.fTopFactor = ImLerp(pAnimIt->second.fTopFactor, fTarget, g.IO.DeltaTime * fSpeed);

    const float fValueX2 = ImMax(bb.Min.x, bb.Max.x - fArrowSize);
    ImGui::RenderNavHighlight(bb, id);

    const float fTopMix = pAnimIt->second.fTopFactor;
    const ImU32 uTopCol = IM_COL32(
        (int)ImLerp(37.f, 35.f, fTopMix),
        (int)ImLerp(37.f, 35.f, fTopMix),
        (int)ImLerp(37.f, 40.f, fTopMix),
        255
    );
    const ImU32 uBottomCol = IM_COL32(23, 23, 23, 255);
    pWindow->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, uTopCol, uTopCol, uBottomCol, uBottomCol);

    if (!(flags & ImGuiComboFlags_NoArrowButton))
    {
        ImVec2 vCenter(bb.Max.x - fArrowSize * 0.5f, bb.Min.y + (bb.Max.y - bb.Min.y) * 0.5f);
        ImVec2 p1(vCenter.x - 2.f, vCenter.y);
        ImVec2 p2(vCenter.x + 2.f, vCenter.y);
        ImVec2 p3(vCenter.x, vCenter.y - 2.f);
        ImVec2 p4(vCenter.x, vCenter.y + 2.f);
        const ImU32 uPlusCol = IM_COL32(255, 255, 255, 255);
        pWindow->DrawList->AddLine(p1, p2, uPlusCol, 1.5f);
        pWindow->DrawList->AddLine(p3, p4, uPlusCol, 1.5f);
    }

    pWindow->DrawList->AddRect(bb.Min, bb.Max, IM_COL32(55, 55, 60, 255), 0.f, 0, 1.f);

    if (previewValue != nullptr && !(flags & ImGuiComboFlags_NoPreview))
    {
        ImVec2 vPreviewPos(bb.Min.x + style.FramePadding.x + CustomSettings::fFieldTypedTextPadExtraX, bb.Min.y + style.FramePadding.y);
        ImGui::PushFont(pLabelFont);
        pWindow->DrawList->AddText(pLabelFont, fLabelPx, vPreviewPos, ImGui::GetColorU32(ImGuiCol_Text), previewValue);
        ImGui::PopFont();
    }

    if (vLabelSize.x > 0.f)
    {
        ImVec2 vLabelPos(totalBb.Min.x, totalBb.Min.y);
        pWindow->DrawList->AddText(pLabelFont, fLabelPx, vLabelPos, ImGui::GetColorU32(ImGuiCol_Text), label);
    }

    (void)fValueX2;

    if (!bPopupOpen)
        return false;

    return BeginComboPopupWindow(popupId, bb, flags);
}

bool ksd::Selectable(const char* label, bool selected, const ImVec2& sizeArg)
{
    ImGuiWindow* pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = pWindow->GetID(label);
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    ImVec2 vLabelSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);
    ImVec2 vPos = pWindow->DC.CursorPos;
    ImVec2 vSize = ImGui::CalcItemSize(sizeArg, vLabelSize.x + style.FramePadding.x * 2.f, vLabelSize.y + style.FramePadding.y * 2.f);

    ImRect bb(vPos, ImVec2(vPos.x + vSize.x, vPos.y + vSize.y));
    ImGui::ItemSize(vSize);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool bHovered = false;
    bool bHeld = false;
    bool bPressed = ImGui::ButtonBehavior(bb, id, &bHovered, &bHeld);

    ImGuiStorage* const pStorage = ImGui::GetStateStorage();
    float* const pfHover = pStorage->GetFloatRef(id + 311u, 0.f);
    float* const pfSelect = pStorage->GetFloatRef(id + 677u, 0.f);
    const float fFadeSpeed = 1.f / CustomSettings::fUiAnimSeconds;
    StepAnimFloat(pfHover, bHovered ? 1.f : 0.f, fFadeSpeed, g.IO.DeltaTime);
    StepAnimFloat(pfSelect, selected ? 1.f : 0.f, fFadeSpeed, g.IO.DeltaTime);
    const ImU32 uIdle = IM_COL32(23, 23, 23, 255);
    const ImU32 uSelected = IM_COL32(37, 37, 37, 255);
    const ImU32 uHover = IM_COL32(35, 35, 40, 255);
    ImU32 uBgCol = LerpU32(uIdle, uSelected, *pfSelect);
    uBgCol = LerpU32(uBgCol, uHover, *pfHover);
    pWindow->DrawList->AddRectFilled(bb.Min, bb.Max, uBgCol, 0.f);

    ImVec2 vTextPos(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y);
    pWindow->DrawList->AddText(pLabelFont, fLabelPx, vTextPos, ImGui::GetColorU32(ImGuiCol_Text), label);
    return bPressed;
}

float ComboPopupHeightForItems(int iItemCount)
{
    if (iItemCount <= 0)
        return FLT_MAX;
    return (ImGui::GetTextLineHeightWithSpacing() * iItemCount) - ImGui::GetStyle().ItemSpacing.y + (ImGui::GetStyle().WindowPadding.y * 2.f);
}

bool BeginComboPopupWindow(ImGuiID popupId, const ImRect& bb, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    if (!ImGui::IsPopupOpen(popupId, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    const float fW = bb.GetWidth();
    if (g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint)
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, fW);
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_));
        int iPopupMaxHeightInItems = -1;
        if (flags & ImGuiComboFlags_HeightRegular)
            iPopupMaxHeightInItems = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)
            iPopupMaxHeightInItems = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)
            iPopupMaxHeightInItems = 20;
        ImVec2 vConstraintMin(0.f, 0.f);
        ImVec2 vConstraintMax(FLT_MAX, FLT_MAX);
        if ((g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.x <= 0.f)
            vConstraintMin.x = fW;
        if ((g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.y <= 0.f)
            vConstraintMax.y = ComboPopupHeightForItems(iPopupMaxHeightInItems);
        ImGui::SetNextWindowSizeConstraints(vConstraintMin, vConstraintMax);
    }

    char szName[16];
    ImFormatString(szName, IM_ARRAYSIZE(szName), "##Combo_%02d", g.BeginPopupStack.Size);

    ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + g.Style.ItemSpacing.y), ImGuiCond_Always, ImVec2(0.f, 0.f));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, g.Style.FramePadding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.f);
    bool bRet = ImGui::BeginPopupEx(popupId, windowFlags);
    ImGui::PopStyleVar(2);
    if (!bRet)
    {
        ImGui::EndPopup();
        IM_ASSERT(0);
        return false;
    }
    return true;
}

namespace
{
const char* ComboItemsArrayGetter(void* pData, int iIndex)
{
    const char* const* ppItems = (const char* const*)pData;
    return ppItems[iIndex];
}

const char* ComboItemsSingleStringGetter(void* pData, int iIndex)
{
    const char* pItemsSeparatedByZeros = (const char*)pData;
    int iCurrent = 0;
    const char* pWalker = pItemsSeparatedByZeros;
    while (*pWalker != 0)
    {
        if (iCurrent == iIndex)
            return pWalker;
        pWalker += strlen(pWalker) + 1;
        ++iCurrent;
    }
    return nullptr;
}
}

bool ksd::Combo(const char* label, int* currentItem, const char* (*getter)(void* userData, int iIndex), void* userData, int iItemCount, int iPopupMaxHeightInItems)
{
    ImGuiContext& g = *GImGui;
    const char* pPreviewValue = nullptr;
    if (*currentItem >= 0 && *currentItem < iItemCount)
        pPreviewValue = getter(userData, *currentItem);

    if (iPopupMaxHeightInItems != -1 && !(g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.f, 0.f), ImVec2(FLT_MAX, ComboPopupHeightForItems(iPopupMaxHeightInItems)));

    if (!BeginCombo(label, pPreviewValue, ImGuiComboFlags_None))
        return false;

    bool bValueChanged = false;
    for (int iIndex = 0; iIndex < iItemCount; ++iIndex)
    {
        const char* pItemText = getter(userData, iIndex);
        if (pItemText == nullptr)
            pItemText = "*Unknown item*";
        ImGui::PushID(iIndex);
        const bool bItemSelected = (iIndex == *currentItem);
        if (Selectable(pItemText, bItemSelected, ImVec2(-0.1f, 0.f)) && *currentItem != iIndex)
        {
            bValueChanged = true;
            *currentItem = iIndex;
        }
        if (bItemSelected)
            ImGui::SetItemDefaultFocus();
        ImGui::PopID();
    }

    ImGui::EndPopup();

    if (bValueChanged)
        ImGui::MarkItemEdited(g.LastItemData.ID);
    return bValueChanged;
}

bool ksd::Combo(const char* label, int* currentItem, const char* const items[], int iItemCount, int iHeightInItems)
{
    return Combo(label, currentItem, ComboItemsArrayGetter, (void*)items, iItemCount, iHeightInItems);
}

bool ksd::Combo(const char* label, int* currentItem, const char* itemsSeparatedByZeros, float fWidth, int iHeightInItems)
{
    int iItemCount = 0;
    const char* pWalk = itemsSeparatedByZeros;
    while (*pWalk != 0)
    {
        pWalk += strlen(pWalk) + 1;
        ++iItemCount;
    }

    const float fUseW = ImGui::CalcItemSize(ImVec2(fWidth, 0.f), ImGui::CalcItemWidth(), 0.f).x;
    ImGui::PushItemWidth(fUseW);
    bool bValueChanged = Combo(label, currentItem, ComboItemsSingleStringGetter, (void*)itemsSeparatedByZeros, iItemCount, iHeightInItems);
    ImGui::PopItemWidth();
    return bValueChanged;
}

bool ksd::ButtonEx(const char* label, const ImVec2& sizeArg, ImGuiButtonFlags flags)
{
    return Button(label, sizeArg, flags);
}

bool ksd::Button(const char* label, const ImVec2& sizeArg, ImGuiButtonFlags flags)
{
    ImGuiWindow* const pWindow = ImGui::GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CustomSettings::fRowControlLabelAlignX);
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = pWindow->GetID(label);
    ImFont* const pLabelFont = gpFontRegular ? gpFontRegular : gpFontMedium;
    const float fLabelPx = CustomSettings::fCheckboxLabelFontSize;
    const ImVec2 vLabelSize = pLabelFont->CalcTextSizeA(fLabelPx, FLT_MAX, 0.f, label);

    ImVec2 vPos = pWindow->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < pWindow->DC.CurrLineTextBaseOffset)
        vPos.y += pWindow->DC.CurrLineTextBaseOffset - style.FramePadding.y;

    ImVec2 vSize = ImGui::CalcItemSize(sizeArg, vLabelSize.x + style.FramePadding.x * 6.f, vLabelSize.y + style.FramePadding.y * 2.6f);
    if (sizeArg.x <= 0.f)
    {
        const float fAvailW = ImGui::GetContentRegionAvail().x;
        const float fPadX = CustomSettings::fRowControlLabelAlignX;
        float fBtnOuterW = fAvailW - fPadX;
        if (fBtnOuterW < 1.f)
            fBtnOuterW = 1.f;
        if (fAvailW > 0.f)
            vSize.x = fBtnOuterW;
    }
    const float fMinButtonHeight = vLabelSize.y + style.FramePadding.y * 2.6f;
    if (vSize.y < fMinButtonHeight)
        vSize.y = fMinButtonHeight;
    const ImRect bb(vPos, ImVec2(vPos.x + vSize.x, vPos.y + vSize.y));
    ImGui::ItemSize(vSize, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool bHovered = false;
    bool bHeld = false;
    const bool bPressed = ImGui::ButtonBehavior(bb, id, &bHovered, &bHeld, flags);

    struct ButtonAnimState
    {
        float fTopFactor;
        float fClickFactor;
    };
    static std::map<ImGuiID, ButtonAnimState> mAnimState;
    auto pAnimIt = mAnimState.find(id);
    if (pAnimIt == mAnimState.end())
        pAnimIt = mAnimState.insert({ id, ButtonAnimState{0.15f, 0.f} }).first;

    const float fHoverTarget = bHovered ? 1.f : 0.f;
    const float fClickTarget = bHeld ? 1.f : 0.f;
    pAnimIt->second.fTopFactor = ImLerp(pAnimIt->second.fTopFactor, fHoverTarget, g.IO.DeltaTime * 10.f);
    pAnimIt->second.fClickFactor = ImLerp(pAnimIt->second.fClickFactor, fClickTarget, g.IO.DeltaTime * 18.f);
    const float fTopMix = pAnimIt->second.fTopFactor;
    const float fBottomMix = pAnimIt->second.fClickFactor;
    const ImU32 uTopCol = IM_COL32(
        (int)ImLerp(37.f, 35.f, fTopMix),
        (int)ImLerp(37.f, 35.f, fTopMix),
        (int)ImLerp(37.f, 40.f, fTopMix),
        255
    );
    const ImU32 uBottomCol = IM_COL32(
        (int)ImLerp(23.f, 35.f, fBottomMix),
        (int)ImLerp(23.f, 35.f, fBottomMix),
        (int)ImLerp(23.f, 40.f, fBottomMix),
        255
    );

    pWindow->DrawList->AddRectFilledMultiColor(
        bb.Min,
        bb.Max,
        uTopCol,
        uTopCol,
        uBottomCol,
        uBottomCol
    );
    pWindow->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border), 0.f, 0, 1.f);
    const ImVec2 vCenter = bb.GetCenter();
    const ImVec2 vTextPos(vCenter.x - vLabelSize.x * 0.5f, vCenter.y - vLabelSize.y * 0.5f);
    pWindow->DrawList->AddText(
        pLabelFont,
        fLabelPx,
        vTextPos,
        ImGui::GetColorU32(style.Colors[ImGuiCol_Text]),
        label,
        ImGui::FindRenderedTextEnd(label)
    );

    return bPressed;
}

bool ksd::Button(const char* label, const ImVec2& sizeArg)
{
    return Button(label, sizeArg, ImGuiButtonFlags_None);
}


void ksd::RenderMenuStrip(const char* const* ppText, const char* const* ppIcon, int iItemCount, int& iCurrentIndex, const ImVec2& vButtonSize, float fSpacing)
{
    if (ppText == nullptr || ppIcon == nullptr || iItemCount <= 0)
        return;

    const int iSafeCount = iItemCount > 32 ? 32 : iItemCount;
    const float fMenuTextSize = CustomSettings::fDefaultFontSize - 2.f;
    const float fDelta = ImGui::GetIO().DeltaTime;
    const float fSpeed = fDelta <= 0.f ? 1.f : (fDelta / CustomSettings::fUiAnimSeconds);
    ImDrawList* pDraw = ImGui::GetWindowDrawList();
    const ImVec4 vAccent(MenuVars::fMenuAccentR, MenuVars::fMenuAccentG, MenuVars::fMenuAccentB, Colors::fAccentA);
    const ImVec4 vDimWhite(0.35f, 0.35f, 0.35f, 1.f);
    const ImVec4 vWhite(1.f, 1.f, 1.f, 1.f);
    const float fRowStartX = ImGui::GetCursorPosX();

    for (int iItem = 0; iItem < iSafeCount; ++iItem)
    {
        ImGui::PushID(iItem);
        ImGui::SetCursorPosX(fRowStartX);
        ImGui::InvisibleButton("entry", vButtonSize);
        const bool bHovered = ImGui::IsItemHovered();
        if (ImGui::IsItemClicked())
            iCurrentIndex = iItem;
        const bool bSelected = iCurrentIndex == iItem;

        const float fIconTarget = bSelected ? 1.f : 0.f;
        const float fTextTarget = bHovered || bSelected ? 1.f : 0.f;
        gfIconBlend[iItem] = gfIconBlend[iItem] + ((fIconTarget - gfIconBlend[iItem]) * fSpeed);
        gfTextBlend[iItem] = gfTextBlend[iItem] + ((fTextTarget - gfTextBlend[iItem]) * fSpeed);

        const ImVec2 vMin = ImGui::GetItemRectMin();
        const float fCenterY = vMin.y + (vButtonSize.y * 0.5f);
        const float fRowOffsetX = CustomSettings::fMenuStripItemOffsetX;
        const float fIconBoxX = vMin.x + 8.f + fRowOffsetX;
        const float fIconBoxW = 22.f;
        const float fTextX = fIconBoxX + fIconBoxW + 8.f;
        const ImVec4 vIconColor = LerpColor(vDimWhite, vAccent, gfIconBlend[iItem]);
        const ImVec4 vTextColor = LerpColor(vDimWhite, vWhite, gfTextBlend[iItem]);

        const float fRowOffsetY = 0.f;
        const float fTextOnlyOffsetY = 0.f;
        if (ImFont* pIconFont = GetIconFont())
        {
            const float fIconY = fCenterY - (CustomSettings::fDefaultFontSize * 0.5f) + fRowOffsetY;
            pDraw->AddText(
                pIconFont,
                CustomSettings::fDefaultFontSize,
                ImVec2(fIconBoxX, fIconY),
                ImColor(vIconColor),
                ppIcon[iItem]
            );
        }
        else
        {
            const float fIconY = fCenterY - (CustomSettings::fDefaultFontSize * 0.5f) + fRowOffsetY;
            pDraw->AddText(ImVec2(fIconBoxX, fIconY), ImColor(vIconColor), ppIcon[iItem]);
        }

        if (ImFont* pLabelFont = GetMediumFont())
        {
            const float fTextY = fCenterY - (fMenuTextSize * 0.5f) + fRowOffsetY + fTextOnlyOffsetY;
            pDraw->AddText(pLabelFont, fMenuTextSize, ImVec2(fTextX, fTextY), ImColor(vTextColor), ppText[iItem]);
        }
        else
        {
            const float fTextY = fCenterY - (fMenuTextSize * 0.5f) + fRowOffsetY + fTextOnlyOffsetY;
            pDraw->AddText(ImVec2(fTextX, fTextY), ImColor(vTextColor), ppText[iItem]);
        }

        ImGui::PopID();
        if (iItem + 1 < iSafeCount)
            ImGui::Dummy(ImVec2(1.f, fSpacing));
    }
}

namespace
{
using namespace ImGui;

void ColorEditRestoreH(const float* col, float* H)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.ColorEditCurrentID != 0);
    if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
        return;
    *H = g.ColorEditSavedHue;
}

void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.ColorEditCurrentID != 0);
    if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
        return;
    if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
        *H = g.ColorEditSavedHue;
    if (*V == 0.0f)
        *S = g.ColorEditSavedSat;
}

void RenderArrowsForVerticalBar(ImDrawList* drawList, ImVec2 pos, ImVec2 halfSz, float fBarW, float fAlpha)
{
    ImU32 uAlpha8 = IM_F32_TO_INT8_SAT(fAlpha);
    ImGui::RenderArrowPointingAt(drawList, ImVec2(pos.x + halfSz.x + 1,         pos.y), ImVec2(halfSz.x + 2, halfSz.y + 1), ImGuiDir_Right, IM_COL32(0,0,0,uAlpha8));
    ImGui::RenderArrowPointingAt(drawList, ImVec2(pos.x + halfSz.x,             pos.y), halfSz,                              ImGuiDir_Right, IM_COL32(255,255,255,uAlpha8));
    ImGui::RenderArrowPointingAt(drawList, ImVec2(pos.x + fBarW - halfSz.x - 1, pos.y), ImVec2(halfSz.x + 2, halfSz.y + 1), ImGuiDir_Left,  IM_COL32(0,0,0,uAlpha8));
    ImGui::RenderArrowPointingAt(drawList, ImVec2(pos.x + fBarW - halfSz.x,     pos.y), halfSz,                              ImGuiDir_Left,  IM_COL32(255,255,255,uAlpha8));
}

bool ColorPickerCompact4Impl(const char* label, float col[4], ImGuiColorEditFlags flags, const float* refCol)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* pWindow = GetCurrentWindow();
    if (pWindow->SkipItems)
        return false;

    ImDrawList* drawList = pWindow->DrawList;
    ImGuiStyle& style = g.Style;
    ImGuiIO& io = g.IO;
    const float fWidth = CalcItemWidth();
    const bool bReadOnly = ((g.NextItemData.ItemFlags | g.CurrentItemFlags) & ImGuiItemFlags_ReadOnly) != 0;
    g.NextItemData.ClearFlags();

    PushID(label);
    const bool bSetCurrentColorEditId = (g.ColorEditCurrentID == 0);
    if (bSetCurrentColorEditId)
        g.ColorEditCurrentID = pWindow->IDStack.back();
    BeginGroup();

    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
        flags |= ImGuiColorEditFlags_NoSmallPreview;
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ColorPickerOptionsPopup(col, flags);
    if (!(flags & ImGuiColorEditFlags_PickerMask_))
        flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
    if (!(flags & ImGuiColorEditFlags_InputMask_))
        flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

    int iComponents = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
    bool bAlphaBar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
    ImVec2 vPickerPos = pWindow->DC.CursorPos;
    const float fPickerScale = CustomSettings::fAccentPickerScale;
    const float fHueStripMul = CustomSettings::fAccentPickerHueStripMul;
    float fSquareSz = GetFrameHeight() * fPickerScale;
    float fBarsW = GetFrameHeight() * fPickerScale * fHueStripMul;
    float fSvPickerSize = ImMax(fBarsW * 1, fWidth * fPickerScale - (bAlphaBar ? 2 : 1) * (fBarsW + style.ItemInnerSpacing.x));
    float fBar0PosX = vPickerPos.x + fSvPickerSize + style.ItemInnerSpacing.x;
    float fBar1PosX = fBar0PosX + fBarsW + style.ItemInnerSpacing.x;
    float fBarsTriangleHalfSz = IM_TRUNC(fBarsW * 0.20f);

    float fBackupInitialCol[4];
    memcpy(fBackupInitialCol, col, iComponents * sizeof(float));

    float fWheelThickness = fSvPickerSize * 0.08f;
    float fWheelROuter = fSvPickerSize * 0.50f;
    float fWheelRInner = fWheelROuter - fWheelThickness;
    ImVec2 vWheelCenter(vPickerPos.x + (fSvPickerSize + fBarsW) * 0.5f, vPickerPos.y + fSvPickerSize * 0.5f);
    float fTriangleR = fWheelRInner - (int)(fSvPickerSize * 0.027f);
    ImVec2 vTrianglePa = ImVec2(fTriangleR, 0.0f);
    ImVec2 vTrianglePb = ImVec2(fTriangleR * -0.5f, fTriangleR * -0.866025f);
    ImVec2 vTrianglePc = ImVec2(fTriangleR * -0.5f, fTriangleR * +0.866025f);

    float H = col[0], S = col[1], V = col[2];
    float R = col[0], G = col[1], B = col[2];
    if (flags & ImGuiColorEditFlags_InputRGB)
    {
        ColorConvertRGBtoHSV(R, G, B, H, S, V);
        ColorEditRestoreHS(col, &H, &S, &V);
    }
    else if (flags & ImGuiColorEditFlags_InputHSV)
    {
        ColorConvertHSVtoRGB(H, S, V, R, G, B);
    }

    bool bValueChanged = false;
    bool bValueChangedH = false;
    bool bValueChangedSv = false;
    PushItemFlag(ImGuiItemFlags_NoNav, true);
    if (flags & ImGuiColorEditFlags_PickerHueWheel)
    {
        InvisibleButton("hsv", ImVec2(fSvPickerSize + style.ItemInnerSpacing.x + fBarsW, fSvPickerSize));
        if (IsItemActive() && !bReadOnly)
        {
            ImVec2 vInitialOff = g.IO.MouseClickedPos[0] - vWheelCenter;
            ImVec2 vCurrentOff = g.IO.MousePos - vWheelCenter;
            float fInitialDist2 = ImLengthSqr(vInitialOff);
            if (fInitialDist2 >= (fWheelRInner - 1) * (fWheelRInner - 1) && fInitialDist2 <= (fWheelROuter + 1) * (fWheelROuter + 1))
            {
                H = ImAtan2(vCurrentOff.y, vCurrentOff.x) / IM_PI * 0.5f;
                if (H < 0.0f)
                    H += 1.0f;
                bValueChanged = bValueChangedH = true;
            }
            float fCosHueAngle = ImCos(-H * 2.0f * IM_PI);
            float fSinHueAngle = ImSin(-H * 2.0f * IM_PI);
            if (ImTriangleContainsPoint(vTrianglePa, vTrianglePb, vTrianglePc, ImRotate(vInitialOff, fCosHueAngle, fSinHueAngle)))
            {
                ImVec2 vCurrentOffUnrotated = ImRotate(vCurrentOff, fCosHueAngle, fSinHueAngle);
                if (!ImTriangleContainsPoint(vTrianglePa, vTrianglePb, vTrianglePc, vCurrentOffUnrotated))
                    vCurrentOffUnrotated = ImTriangleClosestPoint(vTrianglePa, vTrianglePb, vTrianglePc, vCurrentOffUnrotated);
                float uu, vv, ww;
                ImTriangleBarycentricCoords(vTrianglePa, vTrianglePb, vTrianglePc, vCurrentOffUnrotated, uu, vv, ww);
                V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
                S = ImClamp(uu / V, 0.0001f, 1.0f);
                bValueChanged = bValueChangedSv = true;
            }
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    }
    else if (flags & ImGuiColorEditFlags_PickerHueBar)
    {
        InvisibleButton("sv", ImVec2(fSvPickerSize, fSvPickerSize));
        if (IsItemActive() && !bReadOnly)
        {
            S = ImSaturate((io.MousePos.x - vPickerPos.x) / (fSvPickerSize - 1));
            V = 1.0f - ImSaturate((io.MousePos.y - vPickerPos.y) / (fSvPickerSize - 1));
            ColorEditRestoreH(col, &H);
            bValueChanged = bValueChangedSv = true;
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        SetCursorScreenPos(ImVec2(fBar0PosX, vPickerPos.y));
        InvisibleButton("hue", ImVec2(fBarsW, fSvPickerSize));
        if (IsItemActive() && !bReadOnly)
        {
            H = ImSaturate((io.MousePos.y - vPickerPos.y) / (fSvPickerSize - 1));
            bValueChanged = bValueChangedH = true;
        }
    }
    if (bAlphaBar)
    {
        SetCursorScreenPos(ImVec2(fBar1PosX, vPickerPos.y));
        InvisibleButton("alpha", ImVec2(fBarsW, fSvPickerSize));
        if (IsItemActive())
        {
            col[3] = 1.0f - ImSaturate((io.MousePos.y - vPickerPos.y) / (fSvPickerSize - 1));
            bValueChanged = true;
        }
    }
    PopItemFlag();

    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    {
        SameLine(0, style.ItemInnerSpacing.x);
        BeginGroup();
    }

    if (!(flags & ImGuiColorEditFlags_NoLabel))
    {
        const char* pLabelDisplayEnd = FindRenderedTextEnd(label);
        if (label != pLabelDisplayEnd)
        {
            if ((flags & ImGuiColorEditFlags_NoSidePreview))
                SameLine(0, style.ItemInnerSpacing.x);
            TextEx(label, pLabelDisplayEnd);
        }
    }

    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    {
        PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
        ImVec4 colV4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if ((flags & ImGuiColorEditFlags_NoLabel))
            Text("Current");
        ImGuiColorEditFlags subFlagsToForward = ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaMask_ | ImGuiColorEditFlags_NoTooltip;
        ColorButton("##current", colV4, (flags & subFlagsToForward), ImVec2(fSquareSz * 3, fSquareSz * 2));
        if (refCol != NULL)
        {
            Text("Original");
            ImVec4 refColV4(refCol[0], refCol[1], refCol[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : refCol[3]);
            if (ColorButton("##original", refColV4, (flags & subFlagsToForward), ImVec2(fSquareSz * 3, fSquareSz * 2)))
            {
                memcpy(col, refCol, iComponents * sizeof(float));
                bValueChanged = true;
            }
        }
        PopItemFlag();
        EndGroup();
    }

    if (bValueChangedH || bValueChangedSv)
    {
        if (flags & ImGuiColorEditFlags_InputRGB)
        {
            ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
            g.ColorEditSavedHue = H;
            g.ColorEditSavedSat = S;
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
        }
        else if (flags & ImGuiColorEditFlags_InputHSV)
        {
            col[0] = H;
            col[1] = S;
            col[2] = V;
        }
    }

    bool bValueChangedFixHueWrap = false;
    if ((flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        PushItemWidth((bAlphaBar ? fBar1PosX : fBar0PosX) + fBarsW - vPickerPos.x);
        ImGuiColorEditFlags subFlagsToForward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaMask_ | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSmallPreview;
        ImGuiColorEditFlags subFlags = (flags & subFlagsToForward) | ImGuiColorEditFlags_NoPicker;
        if (flags & ImGuiColorEditFlags_DisplayRGB || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
            if (ColorEdit4("##rgb", col, subFlags | ImGuiColorEditFlags_DisplayRGB))
            {
                bValueChangedFixHueWrap = (g.ActiveId != 0 && !g.ActiveIdAllowOverlap);
                bValueChanged = true;
            }
        if (flags & ImGuiColorEditFlags_DisplayHSV || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
            bValueChanged |= ColorEdit4("##hsv", col, subFlags | ImGuiColorEditFlags_DisplayHSV);
        if (flags & ImGuiColorEditFlags_DisplayHex || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
            bValueChanged |= ColorEdit4("##hex", col, subFlags | ImGuiColorEditFlags_DisplayHex);
        PopItemWidth();
    }

    if (bValueChangedFixHueWrap && (flags & ImGuiColorEditFlags_InputRGB))
    {
        float newH, newS, newV;
        ColorConvertRGBtoHSV(col[0], col[1], col[2], newH, newS, newV);
        if (newH <= 0 && H > 0)
        {
            if (newV <= 0 && V != newV)
                ColorConvertHSVtoRGB(H, S, newV <= 0 ? V * 0.5f : newV, col[0], col[1], col[2]);
            else if (newS <= 0)
                ColorConvertHSVtoRGB(H, newS <= 0 ? S * 0.5f : newS, newV, col[0], col[1], col[2]);
        }
    }

    if (bValueChanged)
    {
        if (flags & ImGuiColorEditFlags_InputRGB)
        {
            R = col[0];
            G = col[1];
            B = col[2];
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            ColorEditRestoreHS(col, &H, &S, &V);
        }
        else if (flags & ImGuiColorEditFlags_InputHSV)
        {
            H = col[0];
            S = col[1];
            V = col[2];
            ColorConvertHSVtoRGB(H, S, V, R, G, B);
        }
    }

    const int iStyleAlpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
    const ImU32 uColBlack = IM_COL32(0,0,0,iStyleAlpha8);
    const ImU32 uColWhite = IM_COL32(255,255,255,iStyleAlpha8);
    const ImU32 uColMidgrey = IM_COL32(128,128,128,iStyleAlpha8);
    const ImU32 uColHues[7] = { IM_COL32(255,0,0,iStyleAlpha8), IM_COL32(255,255,0,iStyleAlpha8), IM_COL32(0,255,0,iStyleAlpha8), IM_COL32(0,255,255,iStyleAlpha8), IM_COL32(0,0,255,iStyleAlpha8), IM_COL32(255,0,255,iStyleAlpha8), IM_COL32(255,0,0,iStyleAlpha8) };
    ImVec4 vHueColor(1, 1, 1, style.Alpha);
    ColorConvertHSVtoRGB(H, 1, 1, vHueColor.x, vHueColor.y, vHueColor.z);
    ImU32 uHueColor32 = ColorConvertFloat4ToU32(vHueColor);
    ImU32 uUserCol32NoAlpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha));
    ImVec2 vSvCursorPos;

    if (flags & ImGuiColorEditFlags_PickerHueWheel)
    {
        const float fAeps = 0.5f / fWheelROuter;
        const int iSegmentPerArc = ImMax(4, (int)fWheelROuter / 12);
        for (int i = 0; i < 6; i++)
        {
            const float fA0 = (i) / 6.0f * 2.0f * IM_PI - fAeps;
            const float fA1 = (i + 1.0f) / 6.0f * 2.0f * IM_PI + fAeps;
            const int iVertStart = drawList->VtxBuffer.Size;
            drawList->PathArcTo(vWheelCenter, (fWheelRInner + fWheelROuter) * 0.5f, fA0, fA1, iSegmentPerArc);
            drawList->PathStroke(uColWhite, 0, fWheelThickness);
            const int iVertEnd = drawList->VtxBuffer.Size;
            ImVec2 vGradientP0(vWheelCenter.x + ImCos(fA0) * fWheelRInner, vWheelCenter.y + ImSin(fA0) * fWheelRInner);
            ImVec2 vGradientP1(vWheelCenter.x + ImCos(fA1) * fWheelRInner, vWheelCenter.y + ImSin(fA1) * fWheelRInner);
            ShadeVertsLinearColorGradientKeepAlpha(drawList, iVertStart, iVertEnd, vGradientP0, vGradientP1, uColHues[i], uColHues[i + 1]);
        }
        float fCosHueAngle = ImCos(H * 2.0f * IM_PI);
        float fSinHueAngle = ImSin(H * 2.0f * IM_PI);
        ImVec2 vHueCursorPos(vWheelCenter.x + fCosHueAngle * (fWheelRInner + fWheelROuter) * 0.5f, vWheelCenter.y + fSinHueAngle * (fWheelRInner + fWheelROuter) * 0.5f);
        float fHueCursorRad = bValueChangedH ? fWheelThickness * 0.65f : fWheelThickness * 0.55f;
        int iHueCursorSegments = drawList->_CalcCircleAutoSegmentCount(fHueCursorRad);
        drawList->AddCircleFilled(vHueCursorPos, fHueCursorRad, uHueColor32, iHueCursorSegments);
        drawList->AddCircle(vHueCursorPos, fHueCursorRad + 1, uColMidgrey, iHueCursorSegments);
        drawList->AddCircle(vHueCursorPos, fHueCursorRad, uColWhite, iHueCursorSegments);
        ImVec2 vTra = vWheelCenter + ImRotate(vTrianglePa, fCosHueAngle, fSinHueAngle);
        ImVec2 vTrb = vWheelCenter + ImRotate(vTrianglePb, fCosHueAngle, fSinHueAngle);
        ImVec2 vTrc = vWheelCenter + ImRotate(vTrianglePc, fCosHueAngle, fSinHueAngle);
        ImVec2 vUvWhite = GetFontTexUvWhitePixel();
        drawList->PrimReserve(3, 3);
        drawList->PrimVtx(vTra, vUvWhite, uHueColor32);
        drawList->PrimVtx(vTrb, vUvWhite, uColBlack);
        drawList->PrimVtx(vTrc, vUvWhite, uColWhite);
        drawList->AddTriangle(vTra, vTrb, vTrc, uColMidgrey, 1.5f);
        vSvCursorPos = ImLerp(ImLerp(vTrc, vTra, ImSaturate(S)), vTrb, ImSaturate(1 - V));
    }
    else if (flags & ImGuiColorEditFlags_PickerHueBar)
    {
        drawList->AddRectFilledMultiColor(vPickerPos, vPickerPos + ImVec2(fSvPickerSize, fSvPickerSize), uColWhite, uHueColor32, uHueColor32, uColWhite);
        drawList->AddRectFilledMultiColor(vPickerPos, vPickerPos + ImVec2(fSvPickerSize, fSvPickerSize), 0, 0, uColBlack, uColBlack);
        RenderFrameBorder(vPickerPos, vPickerPos + ImVec2(fSvPickerSize, fSvPickerSize), 0.0f);
        vSvCursorPos.x = ImClamp(IM_ROUND(vPickerPos.x + ImSaturate(S) * fSvPickerSize), vPickerPos.x + 2, vPickerPos.x + fSvPickerSize - 2);
        vSvCursorPos.y = ImClamp(IM_ROUND(vPickerPos.y + ImSaturate(1 - V) * fSvPickerSize), vPickerPos.y + 2, vPickerPos.y + fSvPickerSize - 2);
        for (int i = 0; i < 6; ++i)
            drawList->AddRectFilledMultiColor(ImVec2(fBar0PosX, vPickerPos.y + i * (fSvPickerSize / 6)), ImVec2(fBar0PosX + fBarsW, vPickerPos.y + (i + 1) * (fSvPickerSize / 6)), uColHues[i], uColHues[i], uColHues[i + 1], uColHues[i + 1]);
        float fBar0LineY = IM_ROUND(vPickerPos.y + H * fSvPickerSize);
        RenderFrameBorder(ImVec2(fBar0PosX, vPickerPos.y), ImVec2(fBar0PosX + fBarsW, vPickerPos.y + fSvPickerSize), 0.0f);
        RenderArrowsForVerticalBar(drawList, ImVec2(fBar0PosX - 1, fBar0LineY), ImVec2(fBarsTriangleHalfSz + 1, fBarsTriangleHalfSz), fBarsW + 2.0f, style.Alpha);
    }

    float fSvCursorRad = bValueChangedSv ? fWheelThickness * 0.55f : fWheelThickness * 0.40f;
    int iSvCursorSegments = drawList->_CalcCircleAutoSegmentCount(fSvCursorRad);
    drawList->AddCircleFilled(vSvCursorPos, fSvCursorRad, uUserCol32NoAlpha, iSvCursorSegments);
    drawList->AddCircle(vSvCursorPos, fSvCursorRad + 1, uColMidgrey, iSvCursorSegments);
    drawList->AddCircle(vSvCursorPos, fSvCursorRad, uColWhite, iSvCursorSegments);

    if (bAlphaBar)
    {
        float fAlpha = ImSaturate(col[3]);
        ImRect rBar1(fBar1PosX, vPickerPos.y, fBar1PosX + fBarsW, vPickerPos.y + fSvPickerSize);
        RenderColorRectWithAlphaCheckerboard(drawList, rBar1.Min, rBar1.Max, 0, rBar1.GetWidth() / 2.0f, ImVec2(0.0f, 0.0f));
        drawList->AddRectFilledMultiColor(rBar1.Min, rBar1.Max, uUserCol32NoAlpha, uUserCol32NoAlpha, uUserCol32NoAlpha & ~IM_COL32_A_MASK, uUserCol32NoAlpha & ~IM_COL32_A_MASK);
        float fBar1LineY = IM_ROUND(vPickerPos.y + (1.0f - fAlpha) * fSvPickerSize);
        RenderFrameBorder(rBar1.Min, rBar1.Max, 0.0f);
        RenderArrowsForVerticalBar(drawList, ImVec2(fBar1PosX - 1, fBar1LineY), ImVec2(fBarsTriangleHalfSz + 1, fBarsTriangleHalfSz), fBarsW + 2.0f, style.Alpha);
    }

    EndGroup();
    if (bValueChanged && memcmp(fBackupInitialCol, col, iComponents * sizeof(float)) == 0)
        bValueChanged = false;
    if (bValueChanged && g.LastItemData.ID != 0)
        MarkItemEdited(g.LastItemData.ID);
    if (bSetCurrentColorEditId)
        g.ColorEditCurrentID = 0;
    PopID();
    return bValueChanged;
}
}

bool ksd::ColorPickerCompact3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
    float col4[4] = { col[0], col[1], col[2], 1.0f };
    flags |= ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions;
    if (!ColorPickerCompact4Impl(label, col4, flags, nullptr))
        return false;
    col[0] = col4[0];
    col[1] = col4[1];
    col[2] = col4[2];
    return true;
}
