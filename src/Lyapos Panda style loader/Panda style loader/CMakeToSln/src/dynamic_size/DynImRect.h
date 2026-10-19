//
// Created by n08i40k on 09.04.2024.
//

#ifndef DYNIMRECT_H
#define DYNIMRECT_H

#include <imgui_internal.h>

#include "imgui_dynamic_size.h"

class DynImRect : public ImRect {
public:
	explicit
	DynImRect(const float x1,
	          const float y1,
	          const float x2,
	          const float y2) : ImRect(x1,
	                                   y1,
	                                   x1 + (x2 - x1) * imgui_dynamic_size::get_current_scale_factor(),
	                                   y1 + (y2 - y1) * imgui_dynamic_size::get_current_scale_factor()) {}

	explicit
	DynImRect(const ImRect& rect) : DynImRect(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y) {}

	DynImRect(const ImVec2& min,
	          const ImVec2& max) : DynImRect(min.x, min.y, max.x, max.y) {}

	explicit
	DynImRect(const ImVec4& vec4) : DynImRect(vec4.x, vec4.y, vec4.z, vec4.w) {}
};

#endif //DYNIMRECT_H
