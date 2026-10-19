#include "gui/framework/fonts/fonts.hpp"
#include "gui/framework/fonts/SourceSans3-SemiBold.hpp"
#include "gui/framework/fonts/OpenSans-Regular.hpp"

namespace fonts
{
	ImFont* title = nullptr;
	ImFont* body = nullptr;
}

void fonts::Init() noexcept
{
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;

	title = io.Fonts->AddFontFromMemoryTTF((void*)SourceSans3SemiBold, sizeof(SourceSans3SemiBold), 16.f, &config);
	body = io.Fonts->AddFontFromMemoryTTF((void*)OpenSansRegular, sizeof(OpenSansRegular), 16.f, &config);

	io.FontDefault = title;
}
