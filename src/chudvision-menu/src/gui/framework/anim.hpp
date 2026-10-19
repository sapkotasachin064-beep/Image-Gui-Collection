#pragma once

#include "imgui.h"
#include <unordered_map>

namespace anim
{
    inline float speed = 1.f;

    template <typename T>
    T& State(ImGuiID id) noexcept
    {
        static std::unordered_map<ImGuiID, T> states;
        return states[id];
    }

    inline float Step(float rate) noexcept
    {
        const float step = rate * speed * ImGui::GetIO().DeltaTime;
        if (step > 1.f)
        {
            return 1.f;
        }
        return step;
    }

    inline void EaseLerp(float& value, float target, float rate) noexcept
    {
        value = value + (target - value) * Step(rate);
    }

    inline void EaseLerp(ImVec4& value, const ImVec4& target, float rate) noexcept
    {
        const float step = Step(rate);
        value.x = value.x + (target.x - value.x) * step;
        value.y = value.y + (target.y - value.y) * step;
        value.z = value.z + (target.z - value.z) * step;
        value.w = value.w + (target.w - value.w) * step;
    }
}
