#pragma once

#include <sstream>

#include "CanShowDetailsOf.hpp"
#include "ListElement.hpp"

class DateDetails : public CanShowDetailsOf<ListElement> {
public:
	DateDetails(unsigned width, unsigned height, unsigned x, unsigned y, WINDOW* w = nullptr) :
		CanShowDetailsOf<ListElement>(w ? w : newwin(height, width, y, x), width, height, x, y) {}

	void show(const ListElement& element) override {
		std::stringstream ss;
		for(const Meal& meal : element.meals)
			ss << meal.name << "\n\n";
		const auto& s = ss.str();

		mvwaddnstr(m_underlying_window, 0, 0, s.c_str(), s.size());
	}
};
