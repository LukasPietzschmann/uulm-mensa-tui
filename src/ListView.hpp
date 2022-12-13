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


	ListView(unsigned list_width, unsigned list_height, unsigned x, unsigned y) :
		WindowLike(newwin(list_height, list_width, y, x), list_width, list_height, x, y) {}


	void prepare_refresh() override {
		werase(m_underlying_window);
		for(unsigned i = 0; i < this->size(); ++i) {
			if(m_selected.has_value() && m_selected.value() == i)
				wattron(m_underlying_window, A_HIGHLIGHT);
			const std::string& s = this->at(i).as_string();
			mvwaddnstr(m_underlying_window, i, 0, s.c_str(), s.size());
			if(m_selected.has_value() && m_selected.value() == i)
				wattroff(m_underlying_window, A_HIGHLIGHT);
		}
		wnoutrefresh(m_underlying_window);
	}

	bool select_elem(unsigned index, SelectAction action = SelectAction::SELECT) {
		if(action == SelectAction::INCREMENT)
			// Although value_or returns an unsigned, -1 will behave correctly in this case,
			// as adding some value to it, will wrap around and end up at the added value
			index = m_selected.value_or(-1) + index;
		else if(action == SelectAction::DECREMENT)
			index = m_selected.value_or(this->size()) - index;

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