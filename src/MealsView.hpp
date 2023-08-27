#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <vector>

#include "Meal.hpp"

using namespace ftxui;

class MealsView : public ComponentBase {
public:
	MealsView(const std::vector<Meal>& meals);

	Element Render() override;
	bool Focusable() const override;

private:
	std::vector<Element> m_meals;
};
