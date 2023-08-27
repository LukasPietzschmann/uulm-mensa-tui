#include "MealsView.hpp"

#include <string>

#include "MealView.hpp"
#include "MealsView.hpp"

MealsView::MealsView(const std::vector<Meal>& meals) {
	m_meals.reserve(meals.size());
	for(const Meal& meal : meals)
		m_meals.push_back(MealView(meal).Render());
}

Element MealsView::Render() { return vflow(m_meals); }

bool MealsView::Focusable() const { return false; }
