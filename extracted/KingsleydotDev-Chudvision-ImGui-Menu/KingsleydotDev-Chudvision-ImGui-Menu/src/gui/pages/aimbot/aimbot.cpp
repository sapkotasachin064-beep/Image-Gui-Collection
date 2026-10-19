#include "gui/pages/pages.hpp"

#include "config/config.hpp"
#include "gui/framework/bind/bind.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/colorswatch/colorswatch.hpp"
#include "gui/framework/groupbox/groupbox.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/slider/slider.hpp"

namespace vars
{
    bool aimbotEnabled = true;
    int aimbotKey = ImGuiKey_MouseRight;
    int aimbotKeyMode = 0;
    int aimbotFov = 4;
    bool aimbotSilent = false;
    bool aimbotSmoothing = true;
    int aimbotSmoothPercent = 87;
    bool aimbotTargetHead = true;
    bool aimbotTargetBody = true;
    bool aimbotTargetPlayers = true;
    bool aimbotTargetFriendly = false;
    bool aimbotTargetScientists = true;

    bool aimbotDrawFovCircle = true;
    float aimbotFovColor[4] = { 0.86f, 0.86f, 0.86f, 1.f };
    bool aimbotHighlightTarget = true;
    bool aimbotDrawTargetName = true;
    bool aimbotDrawSnapline = true;

    int aimbotRecoilPitch = 56;
    int aimbotRecoilYaw = 53;

    bool aimbotToggleFriendly = false;
    int aimbotFriendlyKey = ImGuiKey_F9;
    int aimbotFriendlyMode = 0;
    bool aimbotToggleEnemy = false;
    int aimbotEnemyKey = ImGuiKey_None;
    int aimbotEnemyMode = 0;
    bool aimbotNoSway = false;
    bool aimbotInstantEoka = false;
    bool aimbotForceAutomatic = false;
}

namespace pages
{
    void RenderAimbot() noexcept
    {
        ksd::BeginColumn(0.5f);

        if (ksd::BeginColumnBox("General", 0.68f))
        {
            ksd::Checkbox("Enabled", &vars::aimbotEnabled);
            ksd::Bind("##aimbotKey", &vars::aimbotEnabled, &vars::aimbotKey, &vars::aimbotKeyMode);

            ksd::Indent();
            ksd::SliderInt("Field of view", &vars::aimbotFov, 0, 180, "%d°");
            ksd::Unindent();

            ksd::Checkbox("Silent", &vars::aimbotSilent);
            ksd::Checkbox("Smoothing", &vars::aimbotSmoothing);

            ksd::Indent();
            ksd::SliderInt("Smoothing percent", &vars::aimbotSmoothPercent, 0, 100, "%d%%");
            ksd::Unindent();

            ksd::Checkbox("Target head", &vars::aimbotTargetHead);
            ksd::Checkbox("Target body", &vars::aimbotTargetBody);
            ksd::Checkbox("Target players", &vars::aimbotTargetPlayers);
            ksd::Checkbox("Target friendly", &vars::aimbotTargetFriendly);
            ksd::Checkbox("Target scientists", &vars::aimbotTargetScientists);
        }
        ksd::EndGroupBox();

        if (ksd::BeginColumnBox("Visualization"))
        {
            ksd::Checkbox("Field of view circle", &vars::aimbotDrawFovCircle);
            ksd::ColorSwatch("##aimbotFovColor", vars::aimbotFovColor);

            ksd::Checkbox("Highlight target", &vars::aimbotHighlightTarget);
            ksd::Checkbox("Draw target name under crosshair", &vars::aimbotDrawTargetName);
            ksd::Checkbox("Draw snapline", &vars::aimbotDrawSnapline);
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
        ksd::BeginColumn();

        if (ksd::BeginColumnBox("Modifiers", 0.58f))
        {
            ksd::SliderInt("Recoil percent pitch", &vars::aimbotRecoilPitch, 0, 100, "%d%%");
            ksd::SliderInt("Recoil percent yaw", &vars::aimbotRecoilYaw, 0, 100, "%d%%");
        }
        ksd::EndGroupBox();

        if (ksd::BeginColumnBox("Misc"))
        {
            ImGui::TextUnformatted("Toggle friendly");
            ksd::Bind("##aimbotFriendlyKey", &vars::aimbotToggleFriendly, &vars::aimbotFriendlyKey, &vars::aimbotFriendlyMode);

            ImGui::TextUnformatted("Toggle enemy");
            ksd::Bind("##aimbotEnemyKey", &vars::aimbotToggleEnemy, &vars::aimbotEnemyKey, &vars::aimbotEnemyMode);

            ksd::Checkbox("No sway", &vars::aimbotNoSway);
            ksd::Checkbox("Instant eoka", &vars::aimbotInstantEoka);
            ksd::Checkbox("Force automatic", &vars::aimbotForceAutomatic);
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
    }

    void RegisterAimbotVars(config::VarRegistry& registry)
    {
        registry.AddBool("aimbotEnabled", &vars::aimbotEnabled);
        registry.AddInt("aimbotKey", &vars::aimbotKey);
        registry.AddInt("aimbotKeyMode", &vars::aimbotKeyMode);
        registry.AddInt("aimbotFov", &vars::aimbotFov);
        registry.AddBool("aimbotSilent", &vars::aimbotSilent);
        registry.AddBool("aimbotSmoothing", &vars::aimbotSmoothing);
        registry.AddInt("aimbotSmoothPercent", &vars::aimbotSmoothPercent);
        registry.AddBool("aimbotTargetHead", &vars::aimbotTargetHead);
        registry.AddBool("aimbotTargetBody", &vars::aimbotTargetBody);
        registry.AddBool("aimbotTargetPlayers", &vars::aimbotTargetPlayers);
        registry.AddBool("aimbotTargetFriendly", &vars::aimbotTargetFriendly);
        registry.AddBool("aimbotTargetScientists", &vars::aimbotTargetScientists);

        registry.AddBool("aimbotDrawFovCircle", &vars::aimbotDrawFovCircle);
        registry.AddColor("aimbotFovColor", vars::aimbotFovColor);
        registry.AddBool("aimbotHighlightTarget", &vars::aimbotHighlightTarget);
        registry.AddBool("aimbotDrawTargetName", &vars::aimbotDrawTargetName);
        registry.AddBool("aimbotDrawSnapline", &vars::aimbotDrawSnapline);

        registry.AddInt("aimbotRecoilPitch", &vars::aimbotRecoilPitch);
        registry.AddInt("aimbotRecoilYaw", &vars::aimbotRecoilYaw);

        registry.AddBool("aimbotToggleFriendly", &vars::aimbotToggleFriendly);
        registry.AddInt("aimbotFriendlyKey", &vars::aimbotFriendlyKey);
        registry.AddInt("aimbotFriendlyMode", &vars::aimbotFriendlyMode);
        registry.AddBool("aimbotToggleEnemy", &vars::aimbotToggleEnemy);
        registry.AddInt("aimbotEnemyKey", &vars::aimbotEnemyKey);
        registry.AddInt("aimbotEnemyMode", &vars::aimbotEnemyMode);
        registry.AddBool("aimbotNoSway", &vars::aimbotNoSway);
        registry.AddBool("aimbotInstantEoka", &vars::aimbotInstantEoka);
        registry.AddBool("aimbotForceAutomatic", &vars::aimbotForceAutomatic);
    }
}
