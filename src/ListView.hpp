#pragma once

#include <ncurses.h>

#include <optional>
#include <string>
#include <vector>

#include "WindowLike.hpp"
#include "constants.hpp"
#include "utils.hpp"

template <typename T, typename = InstanceOf<T, Showable>>
class ListView : public WindowLike, private std::vector<T> {
public:
	using std::vector<T>::emplace_back;
	using std::vector<T>::push_back;
	using std::vector<T>::pop_back;

	enum SelectAction { INCREMENT, DECREMENT, SELECT };

	ListView(unsigned list_width, unsigned list_height, unsigned x, unsigned y) :
		WindowLike(newwin(list_height, list_width, y, x), list_width, list_height, x, y) {}

	void add_str(const std::string& s, unsigned x, unsigned y) override {
		mvwaddnstr(m_underlying_window, y, x, s.c_str(), s.size());
	}

	void clear() override { werase(m_underlying_window); }

	void prepare_refresh() override {
		clear();
		for(unsigned i = 0; i < this->size(); ++i) {
			if(m_selected.has_value() && m_selected.value() == i)
				wattron(m_underlying_window, A_HIGHLIGHT);
			add_str(this->at(i).as_string(), 0, i);
			if(m_selected.has_value() && m_selected.value() == i)
				wattroff(m_underlying_window, A_HIGHLIGHT);
		}
		wnoutrefresh(m_underlying_window);
	}

	bool select_elem(unsigned index, SelectAction action = SELECT) {
		if(action == INCREMENT)
			index = m_selected.value_or(0) + index;
		else if(action == DECREMENT)
			index = m_selected.value_or(this->size() - 1) - index;

		if(index >= this->size())
			return false;

		m_selected.emplace(index);
		return true;
	}

	void unselect_elem() { m_selected.reset(); }

	std::optional<unsigned> get_selected_index() const { return m_selected; }

	std::optional<T> get_selected_element() const {
		if(!m_selected.has_value())
			return {};
		return this->at(m_selected.value());
	}

private:
	std::optional<unsigned> m_selected;
};
