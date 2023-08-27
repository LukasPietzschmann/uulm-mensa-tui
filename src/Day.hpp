#pragma once

#include <vector>

#include "Date.hpp"
#include "Meal.hpp"

struct Day {
	Day(const Date& date) : date(date) {}

	Date date;
	std::vector<Meal> meals{};
};
