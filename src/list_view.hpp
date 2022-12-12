#pragma once

#include <ncurses.h>

#include <optional>
#include <vector>

#include "utils.hpp"
#include "window_like.hpp"

class list_view : public window_like, private std::vector<std::string> {
public:
	using std::vector<std::string>::push_back;
	using std::vector<std::string>::pop_back;

	enum SelectAction { INCREMENT, DECREMENT, SELECT };

	list_view(unsigned list_width, unsigned list_height, unsigned x, unsigned y) :
		window_like(newwin(list_height, list_width, y, x), list_width, list_height, x, y) {}

	void add_str(const std::string& s, unsigned x, unsigned y) override;
	void clear() override;

	void prepare_refresh() override;

	bool select_elem(unsigned index, SelectAction action = SELECT);
	void unselect_elem();

	std::optional<unsigned> get_selected_index() const;
	std::optional<std::string> get_selected_element() const;

private:
	std::optional<unsigned> m_selected;
};
