#include "list_view.hpp"

#include "constants.hpp"

void list_view::add_str(const std::string& s, unsigned x, unsigned y) {
	mvwaddnstr(m_underlying_window, y, x, s.c_str(), s.size());
}

void list_view::clear() { werase(m_underlying_window); }

void list_view::prepare_refresh() {
	clear();
	for(unsigned i = 0; i < size(); ++i) {
		if(m_selected.has_value() && m_selected.value() == i)
			wattron(m_underlying_window, A_HIGHLIGHT);
		add_str(at(i), 0, i);
		if(m_selected.has_value() && m_selected.value() == i)
			wattroff(m_underlying_window, A_HIGHLIGHT);
	}
	wnoutrefresh(m_underlying_window);
}

bool list_view::select_elem(unsigned index, SelectAction action) {
	if(action == INCREMENT)
		index = m_selected.value_or(0) + index;
	else if(action == DECREMENT)
		index = m_selected.value_or(size() - 1) - index;

	if(index >= size())
		return false;

	m_selected.emplace(index);
	return true;
}

void list_view::unselect_elem() { m_selected.reset(); }

std::optional<unsigned> list_view::get_selected_index() const { return m_selected; }

std::optional<std::string> list_view::get_selected_element() const {
	if(!m_selected.has_value())
		return {};
	return at(m_selected.value());
}
