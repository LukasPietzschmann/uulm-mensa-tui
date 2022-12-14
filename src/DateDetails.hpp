#pragma once

#include <ncurses.h>

#include "CanShowDetailsOf.hpp"
#include "ListElement.hpp"

class DateDetails : public CanShowDetailsOf<ListElement> {
public:
	DateDetails(unsigned width, unsigned height, unsigned x, unsigned y, WINDOW* w = nullptr);

	void show(const ListElement& element) override;

private:
	void render_meal(const Meal& meal, unsigned x, unsigned y);
	void box_area_inside(unsigned x1, unsigned y1, unsigned x2, unsigned y2);
};
