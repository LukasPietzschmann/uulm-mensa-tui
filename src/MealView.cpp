#include "MealView.hpp"

#include <string>

Element MealView::Render() { return text(std::string(m_meal.name)); }
