#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <vector>

#include "Day.hpp"

using namespace ftxui;

class DaysView : public ComponentBase {
public:
	DaysView(const std::vector<Day>& days);

	Element Render() override;
	bool OnEvent(Event e) override;
	void OnAnimation(animation::Params& params) override;
	Component ActiveChild() override;
	bool Focusable() const override;

private:
	// left margin + length of date + right margin
	int m_left_size{2 + 10 + 2};
	Component m_layout;

	int m_selected_date{0};
	std::vector<std::string> m_dates;

	std::vector<Component> m_meals_per_day;
};
