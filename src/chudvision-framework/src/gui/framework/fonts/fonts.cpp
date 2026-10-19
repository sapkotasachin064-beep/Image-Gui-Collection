#include "gui/framework/fonts/fonts.hpp"

#include "tahoma.h"
#include "tahomabd.h"

namespace fonts
{
    ImFont* body = nullptr;
    ImFont* bold = nullptr;

    void Init() noexcept
    {
        ImGuiIO& io = ImGui::GetIO();

        ImFontConfig config;
        config.FontDataOwnedByAtlas = false;

        body = io.Fonts->AddFontFromMemoryTTF(tahoma, sizeof(tahoma), 16.f, &config);
        bold = io.Fonts->AddFontFromMemoryTTF(tahomabold, sizeof(tahomabold), 16.f, &config);

        io.FontDefault = body;
    }
}
