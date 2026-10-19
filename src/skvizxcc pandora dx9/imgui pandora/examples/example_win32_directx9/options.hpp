#pragma once

#include <map>

struct ragebot_settings {
	bool enabled = true;
	bool hpbar = false;
	bool silent = false;
	int priority = 123;
};

class Options
{
public:
	std::map<int, ragebot_settings> ragebot = { };
};

extern Options opt;