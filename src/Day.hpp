#pragma once

#include <vector>

#include "Date.hpp"
#include "Meal.hpp"

struct Day {
	Day(const Date& date, bool is_open) : date(date), is_open(is_open) {}

	Date date;
	bool is_open;
	std::vector<Meal> meals{};
};
