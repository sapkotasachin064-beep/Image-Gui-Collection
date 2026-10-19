#pragma once

#include "imgui.h"

namespace config
{
    class VarRegistry;
}

namespace pages
{
    void RenderAimbot() noexcept;
    void RenderEsp() noexcept;
    void RenderPlayers() noexcept;
    void RenderVisuals() noexcept;
    void RenderMisc() noexcept;
    void RenderConfig() noexcept;

    void ApplyMenuScale() noexcept;

    void RegisterAimbotVars(config::VarRegistry& registry);
    void RegisterEspVars(config::VarRegistry& registry);
    void RegisterVisualsVars(config::VarRegistry& registry);
    void RegisterMiscVars(config::VarRegistry& registry);
    void RegisterPlayersVars(config::VarRegistry& registry);
    void RegisterConfigVars(config::VarRegistry& registry);
}
