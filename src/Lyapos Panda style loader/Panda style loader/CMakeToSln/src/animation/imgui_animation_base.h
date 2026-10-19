//
// Created by n08i40k on 09.04.2024.
//

#ifndef IMGUI_ANIMATION_H
#define IMGUI_ANIMATION_H
#include <cstdint>

// ReSharper disable once CppUnusedIncludeDirective
#include "hash_string.h"

using animation_id = uint32_t;

class imgui_animation_base {
protected:
	bool finished{false};

public:
	const animation_id current_animation_id;
	bool queried{};

	bool first_frame{true};
	size_t update_frames{};
	bool should_update{true};


	explicit
	imgui_animation_base(const animation_id animation_id) : current_animation_id(animation_id) {}

	virtual ~imgui_animation_base() = default;

	virtual void
	update(float delta) = 0;

	[[nodiscard]] bool
	is_finished() const {
		return finished;
	}
};

#endif //IMGUI_ANIMATION_H
