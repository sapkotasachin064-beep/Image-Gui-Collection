//
// Created by n08i40k on 01.05.2024.
//

#include "ESPEditor.h"
#include <iostream>

#include <algorithm>
#include <chrono>

#include "animation/linear/linear_color_animation.h"
#include "dynamic_size/DynImVec2.h"
#include "dynamic_size/imgui_dynamic_size.h"
#include "animation/animation_manager.h"

#include <print>
#include <ranges>

ImGui::EspDrawableBlock::EspDrawableBlock(const uint32_t id,
                                          EspSide* p_parent,
                                          const EspBlockType element_type): EspDrawable(id)
                                                                          , p_parent(p_parent)
                                                                          , element_type(element_type) {}

void
ImGui::EspDrawable::draw(const ImRect& bb,
                         const bool allow_overlap) {
	prev_bb = bb;

	auto* const p_window = GetCurrentWindow();
	auto* const p_draw_list = p_window->DrawList;
	const auto& style = GetStyle();

	if (allow_overlap)
		SetNextItemAllowOverlap();

	ItemAdd(bb, id);

	auto& hover_animation =
		ngui::animation_manager::get_default<linear_with_target_animation>(
			"hover##EspDrawable"_sh,
			6.F,
			vector2f{0.F, 0.5F});

	const auto half_inner_spacing = style.ItemInnerSpacing / 2;
	const ImRect btw_inner_outer_bb{bb.Min + half_inner_spacing, bb.Max - half_inner_spacing};

	bool hovered{};

	if (!allow_overlap)
		ButtonBehavior(btw_inner_outer_bb, id, &hovered, nullptr);

	hover_animation.set_target(hovered
		                           ? 0.5F
		                           : 0.F);

	// hover outline
	p_draw_list->AddRect(btw_inner_outer_bb.Min,
	                     btw_inner_outer_bb.Max,
	                     ColorConvertFloat4ToU32({1.F, 1.F, 1.F, hover_animation.get_current()}),
	                     style.FrameRounding,
	                     0,
	                     half_inner_spacing.x);

#ifdef _DEBUG
	p_window->DrawList->AddRect(bb.Min, bb.Max, 0x55FF0000);
	// TODO: Сделать флаг "блок ли это", ибо у блоков нету "внутреннего" размера, только внешний.
	// p_window->DrawList->AddRect(bb.Min + style.ItemInnerSpacing, bb.Max - style.ItemInnerSpacing, 0x550000FF);
#endif
}

void
ImGui::EspDrawableBlock::drag_drop(EspDragDropData* p_drag_drop_data,
                                   const uint32_t id) {
	if (GetKeyData(GetCurrentContext(), ImGuiKey_LeftCtrl)->Down)
		return;

	if (BeginDragDropSource()) {
		p_drag_drop_data->reset();

		p_drag_drop_data->p_source_side = p_parent;
		p_drag_drop_data->p_source_block = this;
		p_drag_drop_data->source_id = id;

		SetDragDropPayload("ESPDragDropElement##Payload", p_drag_drop_data, sizeof(void*), ImGuiCond_Once);

#ifdef _DEBUG
		const auto* p_payload = GetDragDropPayload();
		
		Text("[Block] Type: %s", p_payload->DataType);
#endif

		EndDragDropSource();
	}
}

void
ImGui::EspDrawableBlock::draw(const ImRect& bb,
                              const bool allow_overlap) {
	if (!allow_overlap)
		SetNextItemAllowOverlap();
	EspDrawable::draw(bb, false);

	auto* const p_drag_drop_data = p_parent->p_parent->drag_drop_data.get();

	if (element_type != EspBlockType_DragDrop && BeginDragDropSource()) {
		p_drag_drop_data->reset();

		p_drag_drop_data->p_source_side = p_parent;
		p_drag_drop_data->source_id = id;

		SetDragDropPayload("ESPDragDropBlock##Payload",
		                   p_drag_drop_data,
		                   sizeof(void*),
		                   ImGuiCond_Once);

#ifdef _DEBUG
		const auto* p_payload = GetDragDropPayload();
		
		Text("[Side] Type: %s", p_payload->DataType);
#endif
		EndDragDropSource();
	}

	if (p_drag_drop_data->p_source_side != nullptr
		&& p_drag_drop_data->p_source_block == nullptr
		&& BeginDragDropTarget()) {
		if (const auto* p_payload = AcceptDragDropPayload("ESPDragDropBlock##Payload",
		                                                  ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
			p_payload != nullptr) {
			// if (IsMouseReleased(ImGuiMouseButton_Left)) {
			p_drag_drop_data->p_target_side = p_parent;
			p_drag_drop_data->target_id = id;
			p_drag_drop_data->finalized = true;

#ifdef _DEBUG
			std::printf("[Side] Type: {}", p_payload->DataType);
#endif
			// }
			//
			// auto& draw_list = GetCurrentWindow()->DrawList;
			//
			// const auto middle_point = bb.Min + (bb.Max - bb.Min) / 2;
			// const bool after = p_parent->align == EspDrawableBlockAlign_X
			// 	                   ? GetMousePos().y > middle_point.y
			// 	                   : GetMousePos().x > middle_point.x;
			//
			// static std::array<ImU32, 4> side_colors{
			// 	0x00FFFFFF,
			// 	0x00FFFFFF,
			// 	0x40FFFFFF,
			// 	0x40FFFFFF
			// };
			//
			// size_t color_offset{};
			//
			// if (p_parent->align == EspDrawableBlockAlign_Y)
			// 	color_offset += 3;
			// if (after)
			// 	color_offset += 2;
			//
			// auto get_next_color = [&] {
			// 	return side_colors.at(color_offset++ % 4);
			// };
			//
			// ShadeVertsLinearUV()
			//
			// draw_list->AddRectFilledMultiColor(bb.Min - ImVec2{10, 10},
			//                                    bb.Max,
			//                                    get_next_color(),
			//                                    get_next_color(),
			//                                    get_next_color(),
			//                                    get_next_color());
			//
			// std::shared_ptr<EspDrawableBlock> next_drawable_block{nullptr};
			//
			// if (auto order_it = std::ranges::find(p_parent->drawable_blocks_order, id);
			// 	after) {
			// 	if (order_it + 1 != p_parent->drawable_blocks_order.end())
			// 		next_drawable_block = p_parent->drawable_blocks.at(*(order_it + 1));
			// }
			// else {
			// 	if (order_it != p_parent->drawable_blocks_order.begin())
			// 		next_drawable_block = p_parent->drawable_blocks.at(*(order_it - 1));
			// }
			//
			// if (next_drawable_block != nullptr) {
			// 	color_offset += 2;
			//
			// 	draw_list->PushClipRect(next_drawable_block->prev_bb.Min, next_drawable_block->prev_bb.Max);
			//
			// 	draw_list->AddRectFilledMultiColor(next_drawable_block->prev_bb.Min,
			// 					   next_drawable_block->prev_bb.Max,
			// 					   get_next_color(),
			// 					   get_next_color(),
			// 					   get_next_color(),
			// 					   get_next_color());
			//
			// 	draw_list->PopClipRect();
			// }
		}
		EndDragDropTarget();
	}
	if (p_drag_drop_data->p_source_side != nullptr
		&& p_drag_drop_data->p_source_block != nullptr
		&& can_drag_drop_from(p_drag_drop_data->p_source_block)
		&& p_drag_drop_data->source_id != 0
		&& BeginDragDropTarget()) {
		if (const auto* p_payload = AcceptDragDropPayload("ESPDragDropElement##Payload",
		                                                  ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
			p_payload != nullptr) {
			p_drag_drop_data->p_target_side = p_parent;
			p_drag_drop_data->p_target_block = this;
			p_drag_drop_data->finalized = true;

#ifdef _DEBUG
			std::printf("[Side] Type: {}", p_payload->DataType);
#endif
		}
		EndDragDropTarget();
	}
}

void
ImGui::EspTextElement::draw(const ImRect& bb,
                            const bool allow_overlap) {
	EspDrawable::draw(bb, allow_overlap);

	const auto inner_spacing = GetStyle().ItemInnerSpacing;

	ImRect text_inner_bb{bb.Min + inner_spacing, bb.Max - inner_spacing};

	auto* const p_window = GetCurrentWindow();
	auto* const p_draw_list = p_window->DrawList;

	// text
	p_draw_list->AddText(text_inner_bb.Min, 0xFFFFFFFF, text.c_str(), text.c_str() + text.size());
}

ImVec2
ImGui::EspTextElement::calculate_size([[maybe_unused]] float side_min_size) {
	return CalcTextSize(text.c_str(), text.c_str() + text.size()) + GetStyle().ItemInnerSpacing * 2;
}

bool
ImGui::EspTextElement::cursor_at_next(const ImVec2& mouse_pos,
                                      const ImVec2& middle_point,
                                      EspDrawableBlockAlign align) const { return mouse_pos.y > middle_point.y; }

ImGui::EspTextBlock::EspTextBlock(const uint32_t id,
                                  EspSide* p_parent,
                                  const EspTextAlign text_align): EspDrawableBlockDynamic(
	                                                                id,
	                                                                p_parent,
	                                                                EspBlockType_Text)
                                                                , text_align(text_align) {}

void
ImGui::EspTextBlock::draw(const ImRect& bb,
                          const bool allow_overlap) {
	EspDrawableBlock::draw(bb, allow_overlap);

	auto cursor = bb.Min;
	auto* const p_drag_drop_data = p_parent->p_parent->drag_drop_data.get();

	for (const auto id : elements_order) {
		const auto element = elements.at(id);

		ImRect element_outer_bb{cursor, cursor + element->calculate_size(0.F)};
		if (text_align != EspTextAlign_Left) {
			float offset = bb.Max.x - element_outer_bb.Max.x;
			if (text_align == EspTextAlign_Center)
				offset /= 2;

			element_outer_bb.Min.x += offset;
			element_outer_bb.Max.x += offset;
		}
		cursor.y = element_outer_bb.Max.y;

		element->draw(element_outer_bb, allow_overlap);

		drag_drop(p_drag_drop_data, id);
	}
}

std::pair<const ImGuiID, std::shared_ptr<ImGui::EspTextElement> >
ImGui::EspTextBlock::add(const char* label,
                         const char* text) {
	const ImGuiID id = p_parent->p_parent->id_storage.generate(label);

	elements.emplace(id, std::make_shared<EspTextElement>(id, text));
	elements_order.emplace_back(id);

	return *elements.find(id);
}

ImVec2
ImGui::EspTextBlock::calculate_size(const float side_min_size) {
	ImVec2 total_size{0.F, 0.F};

	for (const auto& element : elements | std::ranges::views::values) {
		auto size = element->calculate_size(0.F);

		total_size.x = std::max<float>(size.x, total_size.x);
		total_size.y += size.y;
	}

	if (p_parent->align == EspDrawableBlockAlign_X)
		total_size.x = std::max<float>(side_min_size, total_size.x);
	else
		total_size.y = std::max<float>(side_min_size, total_size.y);

	return total_size;
}

bool
ImGui::EspTextBlock::cursor_at_next(const ImVec2& mouse_pos,
                                    const ImVec2& middle_point,
                                    const EspDrawableBlockAlign align) const {
	assert(p_parent->align == align);

	if (align == EspDrawableBlockAlign_X)
		return mouse_pos.y > middle_point.y;
	return mouse_pos.x > middle_point.x;
}

void
ImGui::EspTextBlock::insert_element(EspDrawableBlock* p_source_block,
                                    const uint32_t source_id,
                                    const EspDrawableBlockAlign align) {
	insert_element_t(dynamic_cast<EspTextBlock*>(p_source_block), source_id, align);
}

bool
ImGui::EspTextBlock::can_drag_drop_from(EspDrawableBlock* p_source_block) {
	return p_source_block->element_type == element_type;
}

std::shared_ptr<ImGui::EspDrawableBlock>
ImGui::EspTextBlock::create_itself(ImGuiID id,
                                   EspSide* p_parent) {
	return std::make_shared<EspTextBlock>(id, p_parent, text_align);
}

ImGui::EspProgressBarElement::EspProgressBarElement(
	const uint32_t id,
	const vector2f limits,
	const float target,
	const ImGuiCol color,
	const ImGuiCol background_color,
	const ImGuiCol heal_color,
	const ImGuiCol damage_color
): EspDrawable(id)
 , color(color)
 , background_color(background_color)
 , heal_color(heal_color)
 , damage_color(damage_color)
 , limits(limits)
 , target(target) {}

void
ImGui::EspProgressBarElement::draw(const ImRect& bb,
                                   const bool allow_overlap,
                                   const EspDrawableBlockAlign align) {
	EspDrawable::draw(bb, allow_overlap);

	auto* const p_window = GetCurrentWindow();
	auto* p_draw_list = p_window->DrawList;
	auto& style = GetStyle();

	auto& first_animation =
		ngui::animation_manager::get_default<linear_with_target_animation>(
			"first"_sh,
			3.F,
			limits,
			target);

	auto& second_animation =
		ngui::animation_manager::get_default<linear_with_target_animation>(
			"second"_sh,
			3.F,
			limits,
			target);

	first_animation.set_target(target);

	if (first_animation.is_finished())
		second_animation.set_target(target);

	const float first_current = first_animation.get_current(true);
	const float second_current = second_animation.get_current(true);

	const auto item_inner_spacing = style.ItemInnerSpacing;
	auto p_min = bb.Min + item_inner_spacing;
	auto p_max = bb.Max - item_inner_spacing;

	const float rounding = style.FrameRounding;
	p_draw_list->AddRectFilled(p_min, p_max, background_color, rounding);

	const auto length = align == EspDrawableBlockAlign_X
		                    ? ImVec2{p_max.x - p_min.x, 0.F}
		                    : ImVec2{0.F, p_max.y - p_min.y};

	if (first_current != second_current) {
		const auto front_length = first_current < second_current
			                          ? p_max - length * (1.F - first_current)
			                          : p_max - length * (1.F - second_current);
		const auto back_length = first_current < second_current
			                         ? p_max - length * (1.F - second_current)
			                         : p_max - length * (1.F - first_current);

		// back
		p_draw_list->AddRectFilled(
			p_min,
			back_length,
			first_current >= second_current
				? heal_color
				: damage_color,
			rounding);

		// front
		p_draw_list->AddRectFilled(
			p_min,
			front_length,
			color,
			rounding);
	}
	else {
		p_draw_list->AddRectFilled(
			p_min,
			p_max - length * (1.F - first_current),
			color,
			rounding);
	}
}

ImVec2
ImGui::EspProgressBarElement::calculate_size(float side_min_size) {
	const auto bar_size = imgui_dynamic_size::get_current_scale_factor() * 15.F;
	const auto inner_spacing = GetStyle().ItemInnerSpacing;

	return ImVec2{bar_size, bar_size} + inner_spacing;
}

bool
ImGui::EspProgressBarElement::cursor_at_next(const ImVec2& mouse_pos,
                                             const ImVec2& middle_point,
                                             const EspDrawableBlockAlign align) const {
	if (align == EspDrawableBlockAlign_X)
		return mouse_pos.y > middle_point.y;
	return mouse_pos.x > middle_point.x;
}

ImGui::EspProgressBarBlock::EspProgressBarBlock(
	const uint32_t id,
	EspSide* p_parent): EspDrawableBlockDynamic(id,
	                                            p_parent,
	                                            EspBlockType_ProgressBar) {}

void
ImGui::EspProgressBarBlock::draw(const ImRect& bb,
                                 const bool allow_overlap) {
	EspDrawableBlock::draw(bb, allow_overlap);

	auto* const p_drag_drop_data = p_parent->p_parent->drag_drop_data.get();

	auto bar_size = elements.empty()
		                ? ImVec2{0.F, 0.F}
		                : elements.begin()->second->calculate_size(0.F);
	if (p_parent->align == EspDrawableBlockAlign_X)
		bar_size.x = bb.Max.x - bb.Min.x;
	else
		bar_size.y = bb.Max.y - bb.Min.y;

	auto cursor = bb.Min;

	for (const auto id : elements_order) {
		const auto bar = elements.at(id);

		bar->draw(ImRect{cursor, cursor + bar_size}, allow_overlap, p_parent->align);

		drag_drop(p_drag_drop_data, id);

		if (p_parent->align == EspDrawableBlockAlign_X)
			cursor.y += bar_size.y;
		else
			cursor.x += bar_size.x;
	}
}

ImVec2
ImGui::EspProgressBarBlock::calculate_size(const float side_min_size) {
	const auto bar_size = elements.empty()
		                      ? ImVec2{0.F, 0.F}
		                      : elements.begin()->second->calculate_size(0.F);

	return p_parent->align == EspDrawableBlockAlign_X
		       ? ImVec2{
			       side_min_size,
			       bar_size.y * static_cast<float>(elements_order.size())
		       }
		       : ImVec2{
			       bar_size.x * static_cast<float>(elements_order.size()),
			       side_min_size
		       };
}

bool
ImGui::EspProgressBarBlock::cursor_at_next(const ImVec2& mouse_pos,
                                           const ImVec2& middle_point,
                                           const EspDrawableBlockAlign align) const {
	if (align == EspDrawableBlockAlign_X)
		return mouse_pos.y > middle_point.y;
	return mouse_pos.x > middle_point.x;
}

void
ImGui::EspProgressBarBlock::insert_element(EspDrawableBlock* p_source_block,
                                           const uint32_t source_id,
                                           const EspDrawableBlockAlign align) {
	insert_element_t(dynamic_cast<EspProgressBarBlock*>(p_source_block), source_id, align);
}

bool
ImGui::EspProgressBarBlock::can_drag_drop_from(EspDrawableBlock* p_source_block) {
	return p_source_block->element_type == element_type;
}

std::shared_ptr<ImGui::EspDrawableBlock>
ImGui::EspProgressBarBlock::create_itself(ImGuiID id,
                                          EspSide* p_parent) {
	return std::make_shared<EspProgressBarBlock>(id, p_parent);
}

ImGui::EspDragDropBlock::EspDragDropBlock(const ImGuiID id,
                                          EspSide* p_parent): EspDrawableBlock(id, p_parent, EspBlockType_DragDrop) {}

ImVec2
ImGui::EspDragDropBlock::calculate_size(const float side_min_size) {
	return p_parent->align == EspDrawableBlockAlign_X
		       ? ImVec2{side_min_size, 25.F * imgui_dynamic_size::get_current_scale_factor()}
		       : ImVec2{25.F * imgui_dynamic_size::get_current_scale_factor(), side_min_size};
}

bool
ImGui::EspDragDropBlock::cursor_at_next(const ImVec2& mouse_pos,
                                        const ImVec2& middle_point,
                                        EspDrawableBlockAlign align) const {
	return p_parent->direction == EspSideDirection_Top || p_parent->direction == EspSideDirection_Left;
}

void
ImGui::EspDragDropBlock::insert_element(EspDrawableBlock* p_source_block,
                                        const ImGuiID source_id,
                                        const EspDrawableBlockAlign align) {
	auto* const p_parent_side = p_parent;
	auto* const p_esp_data = p_parent_side->p_parent;

	const ImGuiID new_block_id = p_esp_data->id_storage.generate();
	auto new_block = p_source_block->create_itself(new_block_id, p_parent_side);
	new_block->insert_element(p_source_block, source_id, align);

	p_parent_side->drawable_blocks.emplace(new_block_id, new_block);
	p_parent_side->drawable_blocks_order.insert(
		p_parent_side->direction == EspSideDirection_Top || p_parent_side->direction == EspSideDirection_Left
			? p_parent_side->drawable_blocks_order.begin() + 1
			: p_parent_side->drawable_blocks_order.end() - 1,
		new_block_id);
}

void
ImGui::EspDragDropBlock::draw(const ImRect& bb,
                              const bool allow_overlap) { EspDrawableBlock::draw(bb, allow_overlap); }

bool
ImGui::EspDragDropBlock::can_drag_drop_from(EspDrawableBlock* p_source_block) { return true; }

std::shared_ptr<ImGui::EspDrawableBlock>
ImGui::EspDragDropBlock::create_itself(ImGuiID id,
                                       EspSide* p_parent) {
	throw std::exception("Этот метод не поддерживается в EspDragDropBlock");
}

ImGui::EspSide::EspSide(
	EspData* const p_parent,
	const EspSideDirection direction
): direction(direction)
 , align(
	 direction == EspSideDirection_Top
	 || direction == EspSideDirection_Bottom
		 ? EspDrawableBlockAlign_X
		 : EspDrawableBlockAlign_Y)
 , p_parent(p_parent)
 , id(std::chrono::system_clock::now().time_since_epoch().count() & 0x00000000FFFFFFFF) {
	this->add_drawable_block<EspDragDropBlock>();
}

void
ImGui::EspSide::draw(const ImRect& bb) const {
#ifdef _DEBUG
	GetCurrentWindow()->DrawList->AddRect(bb.Min, bb.Max, 0x55FF0000);
#endif

	auto cursor = bb.Min;

	SetCursorScreenPos(cursor);
	if (BeginChildEx(nullptr,
	                 id,
	                 bb.Max - bb.Min,
	                 ImGuiChildFlags_None,
	                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
	                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize)) {
		const auto side_min_size = align == EspDrawableBlockAlign_X
			                           ? bb.Max.x - bb.Min.x
			                           : bb.Max.y - bb.Min.y;

		for (const auto id : drawable_blocks_order) {
			auto* drawable = drawable_blocks.at(id).get();
			const auto drawable_size = drawable->calculate_size(side_min_size);
			ImRect drawable_block_bb{cursor, cursor + drawable_size};

			SetCursorScreenPos(cursor);

			if (BeginChildEx(nullptr,
			                 id,
			                 drawable_size,
			                 ImGuiChildFlags_None,
			                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
			                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize)) {
				drawable->draw(drawable_block_bb,
				               drawable_block_bb.Contains(GetMousePos())
				               && GetKeyData(GetCurrentContext(), ImGuiKey_LeftCtrl)->Down);
				if (align == EspDrawableBlockAlign_X)
					cursor.y += drawable_size.y;
				else
					cursor.x += drawable_size.x;
			}
			EndChild();
		}
	}
	EndChild();
}

ImVec2
ImGui::EspSide::calculate_size(const float side_min_size) {
	std::erase_if(drawable_blocks,
	              [](const decltype(drawable_blocks)::value_type& pair) {
		              return pair.second->element_type != EspBlockType_DragDrop && pair.second->elements_order.empty();
	              });
	std::erase_if(drawable_blocks_order, [this](const uint32_t id) { return !drawable_blocks.contains(id); });

	ImVec2 size;

	for (const auto& drawable : drawable_blocks | std::views::values) {
		const auto calculated_size = drawable->calculate_size(side_min_size);
		if (align == EspDrawableBlockAlign_X) {
			size.x = std::max<float>(size.x, calculated_size.x);
			size.y += calculated_size.y;
		}
		else {
			size.x += calculated_size.x;
			size.y = std::max<float>(size.y, calculated_size.y);
		}
	}

	if (align == EspDrawableBlockAlign_X)
		size.x = std::max<float>(size.x, side_min_size);
	else
		size.y = std::max<float>(size.y, side_min_size);

	return size;
}

void
ImGui::EspSide::insert_block(const ImGuiID drag_drop_target_id,
                             EspSide* p_source_side,
                             const ImGuiID source_id) {
	auto source_element = p_source_side->drawable_blocks.at(source_id);
	source_element->p_parent = this;

	std::erase(p_source_side->drawable_blocks_order, source_id);
	p_source_side->drawable_blocks.erase(source_id);

	auto drag_drop_target_element = drawable_blocks.at(drag_drop_target_id);
	const auto mouse_pos = GetMousePos();
	const auto prev_bb = drag_drop_target_element->prev_bb;
	const auto middle_point = prev_bb.Min + (prev_bb.Max - prev_bb.Min) / 2;

	bool after;
	if (drag_drop_target_element->element_type == EspBlockType_DragDrop) {
		after = direction == EspSideDirection_Top
			|| direction == EspSideDirection_Left;
	}
	else {
		after = align == EspDrawableBlockAlign_X
			        ? mouse_pos.y > middle_point.y
			        : mouse_pos.x > middle_point.x;
	}

	auto dd_order_index_it = std::ranges::find(drawable_blocks_order, drag_drop_target_id);
	if (after)
		++dd_order_index_it;

	drawable_blocks.emplace(source_id, source_element);
	drawable_blocks_order.insert(dd_order_index_it, source_id);
}

void
ImGui::EspData::draw(const ImRect& bb,
                     const ImVec2& center_box_size) {
	const auto left_size = left.calculate_size(center_box_size.y);
	const auto right_size = right.calculate_size(center_box_size.y);

	const auto top_size = top.calculate_size(center_box_size.x);
	const auto bottom_size = bottom.calculate_size(center_box_size.x);

	float top_offset = top_size.y;
	float left_offset = left_size.x;

	const auto cursor = bb.Min;

	const auto left_cursor = cursor + ImVec2{0.F, top_offset};
	const auto right_cursor = left_cursor + ImVec2{left_size.x + center_box_size.x, 0.F};

	const auto top_cursor = cursor + ImVec2{left_offset, 0.F};
	const auto bottom_cursor = top_cursor + ImVec2{0.F, center_box_size.y + top_size.y};

	ImRect left_bb{left_cursor, left_cursor + left_size};
	ImRect right_bb{right_cursor, right_cursor + right_size};

	ImRect top_bb{top_cursor, top_cursor + top_size};
	ImRect bottom_bb{bottom_cursor, bottom_cursor + bottom_size};

	const auto left_center_offset = ImVec2{0.F, left_size.y - center_box_size.y} / 2;
	left_bb.Min -= left_center_offset;
	left_bb.Max -= left_center_offset;

	const auto right_center_offset = ImVec2{0.F, right_size.y - center_box_size.y} / 2;
	right_bb.Min -= right_center_offset;
	right_bb.Max -= right_center_offset;

	const auto top_center_offset = ImVec2{top_size.x - center_box_size.x, 0.F} / 2;
	top_bb.Min -= top_center_offset;
	top_bb.Max -= top_center_offset;

	const auto bottom_center_offset = ImVec2{bottom_size.x - center_box_size.x, 0.F} / 2;
	bottom_bb.Min -= bottom_center_offset;
	bottom_bb.Max -= bottom_center_offset;

	left.draw(left_bb);
	right.draw(right_bb);
	top.draw(top_bb);
	bottom.draw(bottom_bb);

	const ImVec2 center_box_bb{
		cursor.x + left_offset,
		cursor.y + top_offset
	};

	GetCurrentWindow()->DrawList->AddRectFilled(center_box_bb,
	                                            center_box_bb + center_box_size,
	                                            ColorConvertFloat4ToU32({1.F, 1.F, 1.F, 0.3F}),
	                                            10);
}

ImVec2
ImGui::EspData::calculate_size(const ImVec2& center_box_size) {
	if (drag_drop_data->finalized) {
		// ибо читать больно
		// ReSharper disable once CppTooWideScopeInitStatement
		auto& [p_source_side,
			p_target_side,
			p_source_block,
			p_target_block,
			source_id,
			target_id,
			finalised] = *drag_drop_data;

		if (p_source_side != nullptr
			&& p_target_side != nullptr
			&& p_source_block == nullptr
			&& p_target_block == nullptr
			&& source_id != 0
			&& target_id != 0)
			p_target_side->insert_block(target_id, p_source_side, source_id);
			// p_source_side->outsert_block(source_id, p_target_side, target_id);
		else if (p_source_side != nullptr
			&& p_target_side != nullptr
			&& p_source_block != nullptr
			&& p_target_block != nullptr
			&& source_id != 0
			&& target_id == 0) {
			if (p_target_block->can_drag_drop_from(p_source_block))
				p_target_block->insert_element(p_source_block, source_id, p_target_side->align);
		}

		drag_drop_data->reset();
	}

	auto size = center_box_size;

	size += ImVec2{
		left.calculate_size(center_box_size.x).x + right.calculate_size(center_box_size.x).x,
		top.calculate_size(center_box_size.y).y + bottom.calculate_size(center_box_size.y).y
	};

	return size;
}

bool
ImGui::EspEditor(const char* label,
                 EspData& data,
                 const ImVec2& size) {
	const ImVec2 center_box_size = DynImVec2{100, 200};

	auto esp_editor_size = data.calculate_size(center_box_size);

	auto* const p_window = GetCurrentWindow();
	auto* const p_storage = p_window->DC.StateStorage;
	const auto& style = GetStyle();

	const ImGuiID id = GetID(label);
	bool is_open = p_storage->GetBool(id, false);

	auto label_size = CalcTextSize(label, nullptr, true);
	auto button_expected_size = CalcItemSize(size,
	                                         label_size.x + style.FramePadding.x * 2.0F,
	                                         label_size.y + style.FramePadding.y * 2.0F);

	const vector2f button_x_anim_limits{0.F, esp_editor_size.x};

	auto& button_x_animation =
		ngui::animation_manager::get<linear_with_target_animation>(p_window, id, "x_button"_sh);
	auto& child_y_animation =
		ngui::animation_manager::get<linear_with_target_animation>(p_window, id, "y_chlild"_sh);

	button_x_animation.set_limits(button_x_anim_limits);

	if (button_x_animation.first_frame) {
		button_x_animation.set_target(button_expected_size.x);
		button_x_animation.reset_current();
	}

	if (button_x_animation.get_current() < button_expected_size.x) {
		button_x_animation.set_target(button_expected_size.x);
		button_x_animation.reset_current();
	}

	if (Button(label, ImVec2{button_x_animation.get_current(), button_expected_size.y})) {
		p_storage->SetBool(id, is_open = !is_open);

		if (is_open)
			button_x_animation.set_target(esp_editor_size.x);
		else
			child_y_animation.set_target(0.F);
	}

	if (is_open) {
		if (button_x_animation.is_finished())
			child_y_animation.set_target(1.F);
	}
	else if (child_y_animation.is_finished())
		button_x_animation.set_target(button_expected_size.x);

	if ((is_open || !child_y_animation.is_finished()) && button_x_animation.is_finished() && child_y_animation.
		get_current()
		!= 0) {
		BeginChildEx(nullptr,
		             id,
		             {esp_editor_size.x, esp_editor_size.y * child_y_animation.get_current()},
		             0,
		             ImGuiWindowFlags_NoScrollbar);
		auto* current_window = GetCurrentWindow();
		ImRect total_bb{GetCursorScreenPos(), GetCursorScreenPos() + current_window->Size};

		data.draw(total_bb, center_box_size);
		EndChild();
	}

	return false;
}
