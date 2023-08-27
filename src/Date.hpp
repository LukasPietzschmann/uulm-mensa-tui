#pragma once

#include <cassert>

struct Date {
	Date(unsigned day, unsigned month, unsigned year) : day(day), month(month), year(year) {
		assert(day >= 1 && day <= 31);
		assert(month >= 1 && month <= 31);
	}

	unsigned day;
	unsigned month;
	unsigned year;
};
