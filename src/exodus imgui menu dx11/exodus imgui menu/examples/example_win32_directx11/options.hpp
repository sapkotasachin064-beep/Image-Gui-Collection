#pragma once

#include <map>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <imgui_internal.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#define DIRECTINPUT_VERSION 0x0800

class Options
{
public:
	ImFont* InterRegular;
	ImFont* InterRegular_small;
};

extern Options opt;