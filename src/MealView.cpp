#include "MealView.hpp"

#include <sstream>
#include <vector>

Element MealView::Render() {
	std::stringstream attributes;
	for(auto it = m_meal.attributes.begin(); it != m_meal.attributes.end(); ++it) {
		if(it != m_meal.attributes.begin())
			attributes << ", ";
		attributes << Meal::attibute_to_string(*it);
	}
	std::vector<Element> body_elements = {text(std::string(m_meal.name)), text(std::string(m_meal.price))};
	if(!m_meal.attributes.empty())
		body_elements.emplace_back(text(attributes.str()));
	return window(text(std::string(m_meal.category)), vbox(body_elements));
}
