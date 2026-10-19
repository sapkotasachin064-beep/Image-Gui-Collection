//
// Created by n08i40k on 01.05.2024.
//

#ifndef ESPEDITOR_H
#define ESPEDITOR_H
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"
#include "animation/linear/linear_with_target_animation.h"

enum EspBlockType {
	EspBlockType_DragDrop = 0,
	EspBlockType_Text,
	EspBlockType_ProgressBar
};

enum EspDrawableBlockAlign {
	EspDrawableBlockAlign_X = 0,
	EspDrawableBlockAlign_Y,
};

enum EspTextAlign {
	EspTextAlign_Left = 0,
	EspTextAlign_Center,
	EspTextAlign_Right,
};

enum EspSideDirection {
	EspSideDirection_Top = 0,
	EspSideDirection_Bottom,
	EspSideDirection_Left,
	EspSideDirection_Right
};

namespace ImGui {
class EspTextBlock;
class EspDrawableBlock;
class EspSide;
struct EspData;

struct EspDragDropData {
	EspSide* p_source_side{nullptr};
	EspSide* p_target_side{nullptr};

	EspDrawableBlock* p_source_block{nullptr};
	EspDrawableBlock* p_target_block{nullptr};

	ImGuiID source_id{};
	ImGuiID target_id{};

	bool finalized{};

	void
	reset() { memset(this, 0, sizeof(EspDragDropData)); }
};

class EspDrawable {
public:
	ImRect prev_bb;
	const ImGuiID id;

	explicit
	EspDrawable(const ImGuiID id) : id(id) {}

	virtual ~EspDrawable() = default;

	virtual void
	draw(const ImRect& bb,
	     bool allow_overlap);

	[[nodiscard]] virtual ImVec2
	calculate_size(float side_min_size) = 0;

	[[nodiscard]] virtual bool
	cursor_at_next(const ImVec2& mouse_pos,
	               const ImVec2& middle_point,
	               EspDrawableBlockAlign align) const = 0;
};

template<class T>
concept EspDrawableDerived = std::is_base_of_v<EspDrawable, T>;

class EspDrawableBlock : public EspDrawable {
public:
	EspSide* p_parent;
	const EspBlockType element_type;

	std::vector<ImGuiID> elements_order;

	explicit
	EspDrawableBlock(ImGuiID id,
	                 EspSide* p_parent,
	                 EspBlockType element_type);

	void
	drag_drop(EspDragDropData* p_drag_drop_data,
	          ImGuiID id);

	virtual void
	insert_element(EspDrawableBlock* p_source_block,
	               ImGuiID source_id,
	               EspDrawableBlockAlign align) = 0;

	virtual bool
	can_drag_drop_from(EspDrawableBlock* p_source_block) = 0;

	void
	draw(const ImRect& bb,
	     bool allow_overlap) override;

	[[nodiscard]] virtual std::shared_ptr<EspDrawableBlock>
	create_itself(ImGuiID id,
	              EspSide* p_parent) = 0;
};

template<class T>
concept EspDrawableBlockDerived = std::is_base_of_v<EspDrawableBlock, T>;

template<EspDrawableDerived T>
class EspDrawableBlockDynamic : public EspDrawableBlock {
public:
	EspDrawableBlockDynamic(const ImGuiID id,
	                        EspSide* p_parent,
	                        const EspBlockType element_type) : EspDrawableBlock(id, p_parent, element_type) {}

protected:
	void
	insert_element_t(EspDrawableBlockDynamic* p_source_block,
	                 ImGuiID source_id,
	                 const EspDrawableBlockAlign align) {
		const auto mouse_pos = GetMousePos();

		std::shared_ptr<T> source_element = p_source_block->elements.at(source_id);

		p_source_block->elements.erase(source_id);
		std::erase(p_source_block->elements_order, source_id);

		auto element_id_it = elements_order.begin();
		while (element_id_it != elements_order.end()) {
			EspDrawable* p_target_element = elements.at(*element_id_it).get();

			if (auto middle_point = p_target_element->prev_bb.Min
					+ (p_target_element->prev_bb.Max - p_target_element->prev_bb.Min) / 2;
				!p_target_element->cursor_at_next(mouse_pos, middle_point, align))
				break;

			++element_id_it;
		}

		elements.emplace(source_id, source_element);
		elements_order.insert(element_id_it, source_id);
	}

public:
	std::unordered_map<ImGuiID, std::shared_ptr<T> > elements;
};

class EspTextElement final : public EspDrawable {
public:
	std::string text;

	EspTextElement(const ImGuiID id,
	               std::string text) : EspDrawable(id)
	                                 , text(std::move(text)) {}

	void
	draw(const ImRect& bb,
	     bool allow_overlap) override;

	ImVec2
	calculate_size(float side_min_size) override;

	[[nodiscard]] bool
	cursor_at_next(const ImVec2& mouse_pos,
	               const ImVec2& middle_point,
	               EspDrawableBlockAlign align) const override;
};

class EspTextBlock final : public EspDrawableBlockDynamic<EspTextElement> {
public:
	EspTextAlign text_align;

	EspTextBlock(ImGuiID id,
	             EspSide* p_parent,
	             EspTextAlign text_align);

	void
	draw(const ImRect& bb,
	     bool allow_overlap) override;

	std::pair<const ImGuiID, std::shared_ptr<EspTextElement> >
	add(const char* label,
	    const char* text);

	ImVec2
	calculate_size(float side_min_size) override;

	[[nodiscard]] bool
	cursor_at_next(const ImVec2& mouse_pos,
	               const ImVec2& middle_point,
	               EspDrawableBlockAlign align) const override;
	void
	insert_element(EspDrawableBlock* p_source_block,
	               ImGuiID source_id,
	               EspDrawableBlockAlign align) override;
	bool
	can_drag_drop_from(EspDrawableBlock* p_source_block) override;

	[[nodiscard]] std::shared_ptr<EspDrawableBlock>
	create_itself(ImGuiID id,
	              EspSide* p_parent) override;
};

class EspProgressBarElement final : public EspDrawable {
	const ImGuiCol color;
	const ImGuiCol background_color;
	const ImGuiCol heal_color;
	const ImGuiCol damage_color;

	float finished_result{};

	const vector2f limits;

public:
	float target;

	EspProgressBarElement(ImGuiID id,
	                      vector2f limits,
	                      float target = 1.F,
	                      ImGuiCol color = 0xFFFFFFFF,
	                      ImGuiCol background_color = 0xFF777777,
	                      ImGuiCol heal_color = 0xFF00FF00,
	                      ImGuiCol damage_color = 0xFF0000FF
	);

	void
	draw(const ImRect& bb,
	     bool allow_overlap,
	     EspDrawableBlockAlign align);

	ImVec2
	calculate_size(float side_min_size) override;

	[[nodiscard]] bool
	cursor_at_next(const ImVec2& mouse_pos,
	               const ImVec2& middle_point,
	               EspDrawableBlockAlign align) const override;
};

class EspProgressBarBlock final : public EspDrawableBlockDynamic<EspProgressBarElement> {
public:
	EspProgressBarBlock(ImGuiID id,
	                    EspSide* p_parent);

	void
	draw(const ImRect& bb,
	     bool allow_overlap) override;

	template<class... Args>
	std::pair<const ImGuiID, std::shared_ptr<EspProgressBarElement> >
	add(const char* label,
	    Args&&... args);

	ImVec2
	calculate_size(float side_min_size) override;

	[[nodiscard]] bool
	cursor_at_next(const ImVec2& mouse_pos,
	               const ImVec2& middle_point,
	               EspDrawableBlockAlign align) const override;
	void
	insert_element(EspDrawableBlock* p_source_block,
	               ImGuiID source_id,
	               EspDrawableBlockAlign align) override;
	bool
	can_drag_drop_from(EspDrawableBlock* p_source_block) override;

	[[nodiscard]] std::shared_ptr<EspDrawableBlock>
	create_itself(ImGuiID id,
	              EspSide* p_parent) override;
};

class EspDragDropBlock final : public EspDrawableBlock {
public:
	EspDragDropBlock(ImGuiID id,
	                 EspSide* p_parent);

	ImVec2
	calculate_size(float side_min_size) override;

	[[nodiscard]] bool
	cursor_at_next(const ImVec2& mouse_pos,
	               const ImVec2& middle_point,
	               EspDrawableBlockAlign align) const override;
	void
	insert_element(EspDrawableBlock* p_source_block,
	               ImGuiID source_id,
	               EspDrawableBlockAlign align) override;
	void
	draw(const ImRect& bb,
	     bool allow_overlap) override;
	bool
	can_drag_drop_from(EspDrawableBlock* p_source_block) override;

	[[nodiscard]] std::shared_ptr<EspDrawableBlock>
	create_itself(ImGuiID id,
	              EspSide* p_parent) override;
};

class EspSide final {
public:
	const EspSideDirection direction;
	const EspDrawableBlockAlign align;
	EspData* const p_parent;

	const ImGuiID id;

	explicit
	EspSide(EspData* p_parent,
	        EspSideDirection direction);

	std::unordered_map<ImGuiID, std::shared_ptr<EspDrawableBlock> > drawable_blocks;
	std::vector<ImGuiID> drawable_blocks_order;

	template<EspDrawableBlockDerived T, class... Args>
	std::pair<ImGuiID, T*>
	add_drawable_block(Args&&... args);

	template<EspDrawableBlockDerived T>
	T*
	get_drawable_block(const ImGuiID id) { return std::dynamic_pointer_cast<T>(drawable_blocks.at(id)).get(); }

	void
	draw(const ImRect& bb) const;

	ImVec2
	calculate_size(float side_min_size);

	void
	insert_block(ImGuiID drag_drop_target_id,
	             EspSide* p_source_side,
	             ImGuiID source_id);
};

class EspIdStorage {
	union EspAnyId {
		ImGuiID id;

		struct {
			ImGuiID storage_idx : 8;
			const ImGuiID other_data : 24;
		} data;
	};

	std::unordered_map<ImGuiID, std::string> ids_;

public:
	ImGuiID
	generate(const char* label = nullptr) {
		assert(ids_.size() < 255);

		if (label == nullptr)
			label = "unknown";

		EspAnyId new_id{ImHashStr(label)};
		new_id.data.storage_idx = ids_.size() + 1; // prevent 0 in total

		ids_.emplace(new_id.id, label);
		return new_id.id;
	}

	[[nodiscard]] const std::string&
	get(const ImGuiID id) const { return ids_.at(id); }
};

struct EspData {
	std::unique_ptr<EspDragDropData> drag_drop_data{std::make_unique<EspDragDropData>()};
	EspIdStorage id_storage{};

	EspSide top{this, EspSideDirection_Top};
	EspSide bottom{this, EspSideDirection_Bottom};
	EspSide left{this, EspSideDirection_Left};
	EspSide right{this, EspSideDirection_Right};

	constexpr EspSide&
	operator[](const EspSideDirection direction) {
		switch (direction) {
		case EspSideDirection_Top: return top;
		case EspSideDirection_Bottom: return bottom;
		case EspSideDirection_Left: return left;
		case EspSideDirection_Right: return right;
		}

		throw new std::runtime_error("Invalid direction provided!");
	}

	void
	draw(const ImRect& bb,
	     const ImVec2& center_box_size);

	ImVec2
	calculate_size(const ImVec2& center_box_size);
};

bool
EspEditor(const char* label,
          EspData& data,
          const ImVec2& size);

template<class... Args>
std::pair<const ImGuiID, std::shared_ptr<EspProgressBarElement> >
EspProgressBarBlock::add(const char* label,
                         Args&&... args) {
	const ImGuiID id = p_parent->p_parent->id_storage.generate(label);

	elements.emplace(id, std::make_shared<EspProgressBarElement>(id, std::forward<Args>(args)...));
	elements_order.emplace_back(id);

	return *elements.find(id);
}

template<EspDrawableBlockDerived T, class... Args>
std::pair<ImGuiID, T*>
EspSide::add_drawable_block(Args&&... args) {
	const ImGuiID id = p_parent->id_storage.generate();

	drawable_blocks.emplace(id, std::make_shared<T>(id, this, std::forward<Args>(args)...));

	if (direction == EspSideDirection_Top || direction == EspSideDirection_Left)
		drawable_blocks_order.emplace_back(id);
	else
		drawable_blocks_order.insert(drawable_blocks_order.begin(), id);

	return {id, std::dynamic_pointer_cast<T>(drawable_blocks.at(id)).get()};
}
}

#endif //ESPEDITOR_H
