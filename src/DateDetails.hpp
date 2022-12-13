#pragma once

#include <ncurses.h>

#include "CanShowDetailsOf.hpp"
#include "ListElement.hpp"

class DateDetails : public CanShowDetailsOf<ListElement> {
public:
	DateDetails(unsigned width, unsigned height, unsigned x, unsigned y, WINDOW* w = nullptr);

	void show(const ListElement& element) override;

private:
	void render_meal(const Meal& meal, unsigned max_width, unsigned x, unsigned y);
};
