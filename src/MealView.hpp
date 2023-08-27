#pragma once

#include <ftxui/component/component_base.hpp>

#include "Meal.hpp"

using namespace ftxui;

class MealView : public ComponentBase {
public:
	MealView(const Meal& meal) : m_meal(meal) {}

	Element Render() override;

private:
	const Meal& m_meal;
};
