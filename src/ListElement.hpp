#pragma once

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
		std::stringstream ss;
		ss << day << '.' << month << '.' << year;
		return ss.str();
	}
};

struct Meal {
	Meal(const std::string& category, const std::string& name, float price) :
		category(category), name(name), price(price) {}
	std::string category;
	std::string name;
	float price;
};

struct ListElement : public Showable {
	ListElement(const Date& date, const std::vector<Meal> meals) : date(date), meals(meals) {}

	Date date;
	std::vector<Meal> meals;

	std::string as_string() const override { return date.as_string(); }
};
