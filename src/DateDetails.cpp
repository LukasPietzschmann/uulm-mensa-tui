#include "DateDetails.hpp"

#include <algorithm>
#include <array>
#include <sstream>

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
	std::array<std::string, 3> elements = {meal.category, meal.name, "Price"};

	for(unsigned i = 0; i < width + 2; ++i) {
		mvwaddch(m_underlying_window, y, x + i, '_');
		mvwaddch(m_underlying_window, y + height + 1, x + i, '-');
	}

	for(unsigned i = 0; i < elements.size(); ++i) {
		mvwaddch(m_underlying_window, y + i + 1, x, '|');
		mvwaddch(m_underlying_window, y + i + 1, x + width + 2, '|');

		const std::string elem = replace_sub(elements.at(i), "ß", "ss");
		mvwaddnstr(m_underlying_window, y + i + 1, x + 1, elem.c_str(), elem.size());
	}
}
