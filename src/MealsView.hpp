#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <set>
#include <string_view>
#include <vector>

#include "Meal.hpp"

using namespace ftxui;

class MealsView : public ComponentBase {
public:
	MealsView(const std::vector<Meal>& meals);

	Element Render() override;
	bool OnEvent(Event e) override;
	void OnAnimation(animation::Params& params) override;
	Component ActiveChild() override;
	bool Focusable() const override;

private:
	static const std::set<std::string_view> m_main_meal_categories;

	int m_selected_tab{0};
	std::vector<std::string> m_categories{"Main courses", "Side dishes"};
	Component m_layout;

	std::vector<Element> m_main_meals;
	std::vector<Element> m_secondary_meals;
};
