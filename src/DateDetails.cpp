#include "DateDetails.hpp"

#include <sstream>

DateDetails::DateDetails(unsigned width, unsigned height, unsigned x, unsigned y, WINDOW* w) :
	CanShowDetailsOf<ListElement>(w ? w : newwin(height, width, y, x), width, height, x, y) {}

void DateDetails::show(const ListElement& element) {
	std::stringstream ss;
	for(const Meal& meal : element.meals)
		ss << meal.name << "\n\n";
	const auto& s = ss.str();

	mvwaddnstr(m_underlying_window, 0, 0, s.c_str(), s.size());
}

void DateDetails::render_meal(const Meal& meal, unsigned max_width, unsigned x, unsigned y) {

}
