#pragma once

#include <cassert>
#include <optional>
#include <set>
#include <string>
#include <string_view>

struct Meal {
	enum class Attribute { BIO, VEGAN, VEGETARIAN, PORK, BEEF };

	Meal(std::string_view name, std::string_view category, std::string_view price,
		const std::set<Attribute>& attributes) :
		name(name),
		category(category), price(price), attributes(attributes){};

	std::string_view name;
	std::string_view category;
	std::string_view price;
	std::set<Attribute> attributes;

	static const std::string_view attibute_to_string(Attribute attribute) {
		switch(attribute) {
			case Attribute::BIO: return "Bio";
			case Attribute::VEGAN: return "Vegan";
			case Attribute::VEGETARIAN: return "Vegetarisch";
			case Attribute::PORK: return "Schwein";
			case Attribute::BEEF: return "Rind";
			default: assert(0);
		}
	}

	static std::optional<Attribute> string_to_attribute(const std::string& string) {
		if(string.compare("Bio") == 0)
			return std::make_optional(Attribute::BIO);
		if(string.compare("Vegan") == 0)
			return std::make_optional(Attribute::VEGAN);
		if(string.compare("Vegetarisch") == 0)
			return std::make_optional(Attribute::VEGETARIAN);
		if(string.compare("Schwein") == 0)
			return std::make_optional(Attribute::PORK);
		if(string.compare("Rind") == 0)
			return std::make_optional(Attribute::BEEF);
		return {};
	}
};
