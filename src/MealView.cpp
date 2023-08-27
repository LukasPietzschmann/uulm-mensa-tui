#include "MealView.hpp"

#include <string>

Element MealView::Render() {
	return window(
		text(std::string(m_meal.category)), vbox({text(std::string(m_meal.name)), text(std::string(m_meal.price))}));
}
