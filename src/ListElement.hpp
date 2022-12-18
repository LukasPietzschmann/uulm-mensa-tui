#pragma once

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils.hpp"

struct Date : public Showable {
	Date(int year, int month, int day) : year(year), month(month), day(day) {}

	int year;
	int month;
	int day;

	std::string as_string() const override {
		static const time_t now = time(nullptr);
		static const tm* local_now = localtime(&now);
		if(local_now->tm_year + 1900 == year && local_now->tm_mon + 1 == month) {
			if(local_now->tm_mday == day)
				return "Today";
			if(local_now->tm_mday + 1 == day)
				return "Tomorrow";
		}

		static std::map<unsigned, std::string> month_names = {{1, "Jan."}, {2, "Feb."}, {3, "Mar."}, {4, "Apr."},
			{5, "May"}, {6, "Jun."}, {7, "Jul."}, {8, "Aug."}, {9, "Sep."}, {10, "Oct."}, {11, "Nov."}, {12, "Dec."}};

		std::stringstream ss;
		ss << day << ". " << month_names.at(month);
		return ss.str();
	}
};

struct Meal {
	Meal(const std::string& category, const std::string& name, const std::string& price) :
		category(category), name(name), price(price) {}
	std::string category;
	std::string name;
	std::string price;
};

struct ListElement : public Showable {
	ListElement(const Date& date, const std::vector<Meal> meals) : date(date), meals(meals) {}

	Date date;
	std::vector<Meal> meals;

	std::string as_string() const override { return date.as_string(); }
};
