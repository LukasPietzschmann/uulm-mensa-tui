#pragma once

#include <string_view>

struct Meal {
	Meal(std::string_view name, std::string_view category, std::string_view price) :
		name(name), category(category), price(price){};

	std::string_view name;
	std::string_view category;
	std::string_view price;
};
