#pragma once

#include <cassert>

enum class RelativeDate { TODAY, TOMORROW, OTHER };

struct Date {
	Date(unsigned day, unsigned month, unsigned year) : day(day), month(month), year(year) {
		assert(day >= 1 && day <= 31);
		assert(month >= 1 && month <= 31);
	}

	unsigned day;
	unsigned month;
	unsigned year;

	RelativeDate get_relative_id() const;
	bool is_in_the_past() const;

private:
	const Date& get_today() const;
};
