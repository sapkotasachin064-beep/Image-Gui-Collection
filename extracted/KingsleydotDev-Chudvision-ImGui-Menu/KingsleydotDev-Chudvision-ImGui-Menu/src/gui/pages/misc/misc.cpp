#include "gui/pages/pages.hpp"

#include "config/config.hpp"
#include "gui/framework/badge/badge.hpp"
#include "gui/framework/bind/bind.hpp"
#include "gui/framework/checkbox/checkbox.hpp"
#include "gui/framework/groupbox/groupbox.hpp"
#include "gui/framework/layout/layout.hpp"
#include "gui/framework/slider/slider.hpp"

namespace vars
{
    bool miscFreecam = true;
    int miscFreecamKey = 0;
    int miscFreecamMode = 0;
    float miscFreecamSpeed = 0.64f;
    bool miscFastLoot = false;
    bool miscExtendedMelee = false;
    bool miscInstantRevive = false;
    bool miscInstantPickup = false;
    bool miscInstantUntie = false;
    bool miscAntiAfk = false;
    bool miscFishingBot = false;

    bool miscNoclip = false;
    int miscNoclipKey = ImGuiKey_T;
    int miscNoclipMode = 0;
    bool miscNoclipGrounded = true;
    bool miscNoclipSmallWaist = true;
    float miscNoclipSpeed = 2.22f;
    bool miscInteractive = false;
    int miscInteractiveKey = ImGuiKey_H;
    int miscInteractiveMode = 0;
    bool miscInteractiveCollision = false;
    float miscInteractiveSpeed = 1.f;
    bool miscJumpMidair = false;
    bool miscNoJumpCooldown = false;
    bool miscSnapTap = false;
    bool miscOmniSprint = false;
    bool miscAntiAim = false;
    bool miscNoSlowDown = false;
}

namespace pages
{
    void RenderMisc() noexcept
    {
        ksd::BeginColumn(0.5f);

        if (ksd::BeginColumnBox("General"))
        {
            ksd::Checkbox("Freecam", &vars::miscFreecam);
            ksd::Bind("##miscFreecamBind", &vars::miscFreecam, &vars::miscFreecamKey, &vars::miscFreecamMode);

            ksd::Indent();
            ksd::SliderFloat("Freecam speed", &vars::miscFreecamSpeed, 0.1f, 3.f, "%.2fx");
            ksd::Unindent();

            ksd::Checkbox("Fast loot", &vars::miscFastLoot);
            ksd::Checkbox("Extended melee", &vars::miscExtendedMelee);
            ksd::Checkbox("Instant revive", &vars::miscInstantRevive);
            ksd::Checkbox("Instant pickup", &vars::miscInstantPickup);
            ksd::Checkbox("Instant untie", &vars::miscInstantUntie);
            ksd::Checkbox("Anti afk", &vars::miscAntiAfk);
            ksd::Checkbox("Fishing bot", &vars::miscFishingBot);
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
        ksd::BeginColumn();

        if (ksd::BeginColumnBox("Movement"))
        {
            ksd::Checkbox("Noclip", &vars::miscNoclip);
            ksd::Bind("##miscNoclipBind", &vars::miscNoclip, &vars::miscNoclipKey, &vars::miscNoclipMode);

            ksd::Checkbox("Noclip grounded", &vars::miscNoclipGrounded);

            ksd::Checkbox("Noclip small waist", &vars::miscNoclipSmallWaist);
            ksd::StatusBadge(vars::miscNoclipSmallWaist);

            ksd::SliderFloat("Noclip speed", &vars::miscNoclipSpeed, 0.1f, 10.f, "%.2fx");

            ksd::Checkbox("Interactive", &vars::miscInteractive);
            ksd::Bind("##miscInteractiveBind", &vars::miscInteractive, &vars::miscInteractiveKey, &vars::miscInteractiveMode);

            ksd::Indent();
            ksd::Checkbox("Interactive collision", &vars::miscInteractiveCollision);
            ksd::SliderFloat("Interactive speed", &vars::miscInteractiveSpeed, 0.1f, 5.f, "%.2fx");
            ksd::Unindent();

            ksd::Checkbox("Jump while midair", &vars::miscJumpMidair);
            ksd::Checkbox("No jump cooldown", &vars::miscNoJumpCooldown);
            ksd::Checkbox("Snap tap", &vars::miscSnapTap);
            ksd::Checkbox("Omni sprint", &vars::miscOmniSprint);
            ksd::Checkbox("Anti aim", &vars::miscAntiAim);
            ksd::Checkbox("No slow down", &vars::miscNoSlowDown);
        }
        ksd::EndGroupBox();

        ksd::EndColumn();
    }

    void RegisterMiscVars(config::VarRegistry& registry)
    {
        registry.AddBool("miscFreecam", &vars::miscFreecam);
        registry.AddInt("miscFreecamKey", &vars::miscFreecamKey);
        registry.AddInt("miscFreecamMode", &vars::miscFreecamMode);
        registry.AddFloat("miscFreecamSpeed", &vars::miscFreecamSpeed);
        registry.AddBool("miscFastLoot", &vars::miscFastLoot);
        registry.AddBool("miscExtendedMelee", &vars::miscExtendedMelee);
        registry.AddBool("miscInstantRevive", &vars::miscInstantRevive);
        registry.AddBool("miscInstantPickup", &vars::miscInstantPickup);
        registry.AddBool("miscInstantUntie", &vars::miscInstantUntie);
        registry.AddBool("miscAntiAfk", &vars::miscAntiAfk);
        registry.AddBool("miscFishingBot", &vars::miscFishingBot);
        registry.AddBool("miscNoclip", &vars::miscNoclip);
        registry.AddInt("miscNoclipKey", &vars::miscNoclipKey);
        registry.AddInt("miscNoclipMode", &vars::miscNoclipMode);
        registry.AddBool("miscNoclipGrounded", &vars::miscNoclipGrounded);
        registry.AddBool("miscNoclipSmallWaist", &vars::miscNoclipSmallWaist);
        registry.AddFloat("miscNoclipSpeed", &vars::miscNoclipSpeed);
        registry.AddBool("miscInteractive", &vars::miscInteractive);
        registry.AddInt("miscInteractiveKey", &vars::miscInteractiveKey);
        registry.AddInt("miscInteractiveMode", &vars::miscInteractiveMode);
        registry.AddBool("miscInteractiveCollision", &vars::miscInteractiveCollision);
        registry.AddFloat("miscInteractiveSpeed", &vars::miscInteractiveSpeed);
        registry.AddBool("miscJumpMidair", &vars::miscJumpMidair);
        registry.AddBool("miscNoJumpCooldown", &vars::miscNoJumpCooldown);
        registry.AddBool("miscSnapTap", &vars::miscSnapTap);
        registry.AddBool("miscOmniSprint", &vars::miscOmniSprint);
        registry.AddBool("miscAntiAim", &vars::miscAntiAim);
        registry.AddBool("miscNoSlowDown", &vars::miscNoSlowDown);
    }
}
