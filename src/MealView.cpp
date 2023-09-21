#include "MealView.hpp"

#include <cassert>
#include <sstream>

Element MealView::Render() {
	std::stringstream attributes;
	for(auto it = m_meal.attributes.begin(); it != m_meal.attributes.end(); ++it) {
		switch(*it) {
			case Meal::Attribute::BIO: attributes << "Bio"; break;
			case Meal::Attribute::VEGAN: attributes << "Vegan"; break;
			case Meal::Attribute::VEGETARIAN: attributes << "Vegetarisch"; break;
			case Meal::Attribute::PORK: attributes << "Schwein"; break;
			case Meal::Attribute::BEEF: attributes << "Rind"; break;
			default: assert(0);
		}
		if(it != m_meal.attributes.end())
			attributes << " ";
	}
	return window(text(std::string(m_meal.category)),
		vbox({text(std::string(m_meal.name)), text(std::string(m_meal.price)), text(attributes.str())}));
}
