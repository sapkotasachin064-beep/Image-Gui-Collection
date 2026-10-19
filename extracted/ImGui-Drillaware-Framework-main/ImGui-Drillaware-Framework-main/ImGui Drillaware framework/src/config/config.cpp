#include "config/config.hpp"

#include <cstdio>
#include "gui/gui.hpp"

static const char* configPath = "drillaware.cfg";

bool config::TrySave()
{
	FILE* file = nullptr;
	if (fopen_s(&file, configPath, "w") != 0 || file == nullptr)
	{
		return false;
	}

	fprintf(file, "accent=%f %f %f %f\n", vars::menuAccentColor[0], vars::menuAccentColor[1], vars::menuAccentColor[2], vars::menuAccentColor[3]);
	fprintf(file, "rainbow=%d\n", vars::menuAccentRainbow ? 1 : 0);
	fprintf(file, "rainbowSpeed=%f\n", vars::menuAccentRainbowSpeed);
	fprintf(file, "animationSpeed=%f\n", vars::menuAnimationSpeed);

	fclose(file);
	return true;
}

bool config::TryLoad()
{
	FILE* file = nullptr;
	if (fopen_s(&file, configPath, "r") != 0 || file == nullptr)
	{
		return false;
	}

	float accent[4] = { 0.f, 0.f, 0.f, 1.f };
	int rainbowInt = 0;
	float rainbowSpeed = 1.f;
	float animationSpeed = 1.f;

	const bool isParsed =
		fscanf_s(file, "accent=%f %f %f %f\n", &accent[0], &accent[1], &accent[2], &accent[3]) == 4 &&
		fscanf_s(file, "rainbow=%d\n", &rainbowInt) == 1 &&
		fscanf_s(file, "rainbowSpeed=%f\n", &rainbowSpeed) == 1 &&
		fscanf_s(file, "animationSpeed=%f\n", &animationSpeed) == 1;
	fclose(file);

	if (!isParsed)
	{
		return false;
	}

	for (int i = 0; i < 4; i++)
	{
		vars::menuAccentColor[i] = accent[i];
	}
	vars::menuAccentRainbow = rainbowInt != 0;
	vars::menuAccentRainbowSpeed = rainbowSpeed;
	vars::menuAnimationSpeed = animationSpeed;
	return true;
}
