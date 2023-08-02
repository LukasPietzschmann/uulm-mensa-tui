#include "DateDetails.hpp"

#include <algorithm>
#include <array>

#include "utils.hpp"

DateDetails::DateDetails(unsigned width, unsigned height, unsigned x, unsigned y, WINDOW* w) :
	CanShowDetailsOf<ListElement>(w ? w : newwin(height, width, y, x), width, height, x, y) {}

void DateDetails::show(const ListElement& element) {
	clear();
	for(unsigned i = 0; i < element.meals.size(); ++i)
		render_meal(element.meals.at(i), 0, 6 * i);
}

void DateDetails::render_meal(const Meal& meal, unsigned x, unsigned y) {
	unsigned width = min(max(meal.name.size(), meal.category.size()), (size_t)m_screen_width);
	unsigned height = 1 + 1 + 1;
	std::array<std::string, 3> elements = {meal.category, meal.name, meal.price};

	box_area_inside(x, y, x + width + 1, y + height);

	for(unsigned i = 0; i < elements.size(); ++i) {
		const std::string elem =
			replace_sub(elements.at(i), rp("ß", "ss"), rp("ä", "ae"), rp("ö", "oe"), rp("ü", "ue"), rp("€", "Euro"));
		mvwaddnstr(m_underlying_window, y + i + 1, x + 2, elem.c_str(), elem.size());
	}
}

void DateDetails::box_area_inside(unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
	unsigned width = x2 - x1 + 2;
	unsigned height = y2 - y1;

	// Horizontal lines
	wmove(m_underlying_window, y1, x1 + 1);
	whline(m_underlying_window, ACS_HLINE, width);
	wmove(m_underlying_window, y2 + 1, x1 + 1);
	whline(m_underlying_window, ACS_HLINE, width);

	// Vertical lines
	wmove(m_underlying_window, y1 + 1, x1);
	wvline(m_underlying_window, ACS_VLINE, height);
	wmove(m_underlying_window, y1 + 1, x2 + 2);
	wvline(m_underlying_window, ACS_VLINE, height);

	// Corners
	wmove(m_underlying_window, y1, x1);
	waddch(m_underlying_window, ACS_ULCORNER);
	wmove(m_underlying_window, y1, x2 + 2);
	waddch(m_underlying_window, ACS_URCORNER);
	wmove(m_underlying_window, y2 + 1, x1);
	waddch(m_underlying_window, ACS_LLCORNER);
	wmove(m_underlying_window, y2 + 1, x2 + 2);
	waddch(m_underlying_window, ACS_LRCORNER);
}
