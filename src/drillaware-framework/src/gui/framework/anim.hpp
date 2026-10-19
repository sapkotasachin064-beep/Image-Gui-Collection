#pragma once
#include <unordered_map>

#include "gui/framework/common.hpp"

namespace anim
{
	template <typename T> T& State(ImGuiID id) noexcept
	{
		static std::unordered_map<ImGuiID, T> states;
		return states[id];
	}

	inline float speed = 1.f;

	inline float Step(float rate) noexcept
	{
		return ImClamp(rate * speed * ImGui::GetIO().DeltaTime, 0.f, 1.f);
	}

	inline void EaseLerp(float& value, float target, float rate) noexcept
	{
		value = ImLerp(value, target, Step(rate));
	}

	inline void EaseLerp(ImVec4& value, const ImVec4& target, float rate) noexcept
	{
		value = ImLerp(value, target, Step(rate));
	}
}
