#pragma once

#include <cassert>
#include <optional>
#include <set>
#include <string>
#include <string_view>

struct Meal {
	enum class Attribute { VEGAN, VEGETARIAN, PORK, BEEF, CHICKEN, FISH, LAMB };

	Meal(std::string_view name, std::string_view category, std::string_view price,
		const std::set<Attribute>& attributes, std::string_view calories) :
		name(name), category(category), price(price), attributes(attributes), calories(calories) {};

	std::string_view name;
	std::string_view category;
	std::string_view price;
	std::set<Attribute> attributes;
	std::string_view calories;

	static const std::string_view attibute_to_string(Attribute attribute) {
		switch(attribute) {
			case Attribute::VEGAN: return "Vegan";
			case Attribute::VEGETARIAN: return "Vegetarisch";
			case Attribute::PORK: return "Schwein";
			case Attribute::BEEF: return "Rind";
			case Attribute::CHICKEN: return "Geflügel";
			case Attribute::FISH: return "Fisch";
			case Attribute::LAMB: return "Lamm";
			default: assert(0);
		}
	}

	static std::optional<Attribute> string_to_attribute(std::string string) {
		if(string == "vegetarisch")
			return Meal::Attribute::VEGETARIAN;
		else if(string == "vegan")
			return Meal::Attribute::VEGAN;
		else if(string == "Schwein")
			return Meal::Attribute::PORK;
		else if(string == "Rind")
			return Meal::Attribute::BEEF;
		else if(string == "Lamm")
			return Meal::Attribute::LAMB;
		else if(string == "Fisch")
			return Meal::Attribute::FISH;
		else if(string == "Geflügel")
			return Meal::Attribute::CHICKEN;
		return {};
	}
};
