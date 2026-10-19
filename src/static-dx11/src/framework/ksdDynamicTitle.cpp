#include "ksdDynamicTitle.h"
#include "custom_settings.h"
#include "custom_imgui.h"
#include "imgui.h"
#include <Windows.h>
#include <string>
#include <cstdlib>
#include <cfloat>
#include <cmath>

namespace
{
std::string gMenuTitle = "Static by Kingsley";

void DrawGlitchTitle(ImDrawList* pDraw, ImFont* pFont, float fSize, ImVec2 vBasePos, const std::string& leftText)
{
    const float fTime = (float)ImGui::GetTime();
    const float fShake = (sinf(fTime * 3.5f) > 0.93f) ? 6.f * sinf(fTime * 88.f) : 0.f;
    vBasePos.x += fShake;
    const char* pFull = leftText.c_str();
    const float fAb1 = 2.f + 1.35f * sinf(fTime * 16.8f);
    const float fAb2 = 1.15f + 0.9f * sinf(fTime * 25.f);
    const float fAbVert = sinf(fTime * 31.f) * 1.05f;
    pDraw->AddText(pFont, fSize, ImVec2(vBasePos.x - fAb1, vBasePos.y), IM_COL32(255, 55, 85, 150), pFull);
    pDraw->AddText(pFont, fSize, ImVec2(vBasePos.x + fAb2, vBasePos.y + 0.7f), IM_COL32(50, 250, 255, 138), pFull);
    pDraw->AddText(pFont, fSize, ImVec2(vBasePos.x + fAbVert * 0.55f, vBasePos.y - 1.1f), IM_COL32(200, 255, 95, 88), pFull);
    if (fmodf(fTime * 12.f + 0.17f, 1.f) < 0.22f)
        pDraw->AddText(pFont, fSize, ImVec2(vBasePos.x + 3.8f * sinf(fTime * 53.f), vBasePos.y + 1.6f), IM_COL32(255, 90, 255, 58), pFull);
    const int iLen = (int)leftText.size();
    static const char kGlitchChars[] = "!<>-_\\|*";
    const int iGlitchN = (int)(sizeof(kGlitchChars) - 1);
    float fDrawX = vBasePos.x;
    for (int iChar = 0; iChar < iLen; ++iChar)
    {
        const char cTrue = leftText[(size_t)iChar];
        char szTrue[2] = { cTrue, '\0' };
        const float fCharW = pFont->CalcTextSizeA(fSize, FLT_MAX, 0.f, szTrue).x;
        const float fBurst = sinf(fTime * 49.f + (float)iChar * 0.44f);
        const float fGlitchAmt = (fBurst > 0.96f) ? 5.5f : (fBurst > 0.83f) ? 2.4f : 0.f;
        const float fJx = sinf(fTime * 12.5f + (float)iChar * 0.76f) * 1.05f + fGlitchAmt * sinf(fTime * 69.f);
        const float fJy = sinf(fTime * 9.4f + (float)iChar * 1.07f) * 0.72f;
        if (fmodf(fTime * 6.5f + (float)iChar * 0.34f, 1.f) < 0.14f)
        {
            char szG[2] = { kGlitchChars[(iChar + (int)(fTime * 26.f)) % iGlitchN], '\0' };
            pDraw->AddText(pFont, fSize, ImVec2(fDrawX + fJx + 3.2f * sinf(fTime * 43.f + (float)iChar), vBasePos.y + fJy + 1.8f), IM_COL32(255, 190, 255, 115), szG);
        }
        if (fmodf(fTime * 9.5f + (float)iChar * 0.19f, 1.f) < 0.28f)
            pDraw->AddText(pFont, fSize, ImVec2(fDrawX + fJx + 2.6f, vBasePos.y + fJy), IM_COL32(235, 245, 255, 92), szTrue);
        float fA = 236.f + 19.f * sinf(fTime * 24.f + (float)iChar * 0.52f);
        if (fA < 228.f)
            fA = 228.f;
        else if (fA > 255.f)
            fA = 255.f;
        pDraw->AddText(pFont, fSize, ImVec2(fDrawX + fJx, vBasePos.y + fJy), IM_COL32(255, 255, 255, (int)fA), szTrue);
        fDrawX += fCharW;
    }
}

void UpdateMenuTitle()
{
    static DWORD dLastUpdate = 0;
    static DWORD dCycleStart = GetTickCount();
    static bool bSeeded = false;
    const DWORD dCurrentTime = GetTickCount();

    if (!bSeeded)
    {
        srand((unsigned int)dCurrentTime);
        bSeeded = true;
    }

    if (dCurrentTime - dLastUpdate > 50)
    {
        dLastUpdate = dCurrentTime;
        const DWORD dTimeInCycle = (dCurrentTime - dCycleStart) % 1000;

        if (dTimeInCycle > 500)
        {
            std::string randomStr;
            const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
            for (size_t iIndex = 0; iIndex < 9; ++iIndex)
                randomStr += charset[rand() % (sizeof(charset) - 1)];
            gMenuTitle = randomStr + " by Kingsley";
        }
        else if (gMenuTitle != "Static by Kingsley")
        {
            gMenuTitle = "Static by Kingsley";
        }
    }
}
}

void KsdDynamicTitle::Draw(ImDrawList* pDraw, const ImVec2& vWindowPos)
{
    UpdateMenuTitle();

    std::string leftText = gMenuTitle;
    const std::string suffix = " by Kingsley";
    const size_t iFound = gMenuTitle.find(suffix);
    if (iFound != std::string::npos)
        leftText = gMenuTitle.substr(0, iFound);

    const float fTitleSize = CustomSettings::fDefaultFontSize + 9.f;
    const float fSubSize = CustomSettings::fDefaultFontSize - 4.f;
    const ImVec2 vTitlePos(vWindowPos.x + 16.f, vWindowPos.y + 8.f);
    const ImVec2 vSubPos(vWindowPos.x + 18.f, vWindowPos.y + 35.f);

    ImFont* pStaticTitleFont = ksd::GetStaticTitleFont();
    if (!pStaticTitleFont)
        pStaticTitleFont = ksd::GetLogoFont();
    if (pStaticTitleFont)
        DrawGlitchTitle(pDraw, pStaticTitleFont, fTitleSize, vTitlePos, leftText);
    else
    {
        const float fPulse = 0.9f + (0.1f * sinf(((float)ImGui::GetTime()) * 2.4f));
        pDraw->AddText(vTitlePos, ImColor(fPulse, fPulse, fPulse, 1.f), leftText.c_str());
    }

    if (ImFont* pSubFont = ksd::GetMediumFont())
    {
        pDraw->AddText(pSubFont, fSubSize, vSubPos, ImColor(0.7f, 0.7f, 0.7f, 1.f), "by ");
        float fDrawX = vSubPos.x + pSubFont->CalcTextSizeA(fSubSize, FLT_MAX, 0.f, "by ").x;
        const char* pName = "Kingsley";
        for (int iChar = 0; pName[iChar] != '\0'; ++iChar)
        {
            const float fHue = fmodf((((float)ImGui::GetTime()) * 0.35f) + (iChar * 0.1f), 1.f);
            const ImU32 uColor = ImColor::HSV(fHue, 0.9f, 1.f);
            char cBuffer[2] = { pName[iChar], '\0' };
            pDraw->AddText(pSubFont, fSubSize, ImVec2(fDrawX, vSubPos.y), uColor, cBuffer);
            fDrawX += pSubFont->CalcTextSizeA(fSubSize, FLT_MAX, 0.f, cBuffer).x;
        }
    }
    else
    {
        pDraw->AddText(vSubPos, ImColor(0.7f, 0.7f, 0.7f, 1.f), "by Kingsley");
    }
}
