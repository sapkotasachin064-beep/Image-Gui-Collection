//
// Created by n08i40k on 09.04.2024.
//

#ifndef DYNIMVEC2_H
#define DYNIMVEC2_H

#include <imgui.h>
#include <utility>

#include "imgui_dynamic_size.h"

class imgui_dynamic_size;

class DynImVec2 : public ImVec2 {
public:
	DynImVec2(const float x,
	          const float y) : ImVec2(
		                         x == -1
			                         ? -1
			                         : x * imgui_dynamic_size::get_current_scale_factor(),
		                         y == -1
			                         ? -1
			                         : y * imgui_dynamic_size::get_current_scale_factor()) {}

	explicit
	DynImVec2(const ImVec2& vec2) : DynImVec2(vec2.x,
	                                          vec2.y) {}
};

#endif //DYNIMVEC2_H
