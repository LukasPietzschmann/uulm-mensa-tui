#include "list_view.hpp"

void list_view::add_str(const std::string& s, unsigned x, unsigned y) {
	mvwaddnstr(m_underlying_window, y, x, s.c_str(), s.size());
}

void list_view::clear() { werase(m_underlying_window); }

void list_view::prepare_refresh() {
	clear();
	for(unsigned i = 0; i < size(); ++i)
		add_str(at(i), 0, i);
	wnoutrefresh(m_underlying_window);
}
