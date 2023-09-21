#include "MealView.hpp"

#include <sstream>

Element MealView::Render() {
	std::stringstream attributes;
	for(auto it = m_meal.attributes.begin(); it != m_meal.attributes.end(); ++it) {
		if(it != m_meal.attributes.begin())
			attributes << ", ";
		attributes << Meal::attibute_to_string(*it);
	}
	return window(text(std::string(m_meal.category)),
		vbox({text(std::string(m_meal.name)), text(std::string(m_meal.price)), text(attributes.str())}));
}
