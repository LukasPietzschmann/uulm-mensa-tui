#pragma once

#include <string_view>
#include <vector>

struct Meal {
	enum class Attribute { BIO, VEGAN, VEGETARIAN, PORK, BEEF };

	Meal(std::string_view name, std::string_view category, std::string_view price,
		const std::vector<Attribute>& attributes) :
		name(name),
		category(category), price(price), attributes(attributes){};

	std::string_view name;
	std::string_view category;
	std::string_view price;
	std::vector<Attribute> attributes;
};
