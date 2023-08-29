#include "Date.hpp"

#include <ctime>

RelativeDate Date::get_relative_id() const {
	const Date& today = get_today();

	if(today.day == day && today.month == month && today.year == year)
		return RelativeDate::TODAY;
	if(today.day + 1 == day && today.month == month && today.year == year)
		return RelativeDate::TOMORROW;
	return RelativeDate::OTHER;
}

bool Date::is_in_the_past() const {
	const Date& today = get_today();
	if(year < today.year) {
		return true;
	} else if(year == today.year) {
		if(month < today.month)
			return true;
		else if(month == today.month && day < today.day)
			return true;
	}
	return false;
}

const Date& Date::get_today() const {
	static time_t now = time(0);
	static tm* local_time = localtime(&now);
	static Date today(local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
	return today;
}
