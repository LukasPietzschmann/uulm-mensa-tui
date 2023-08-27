#include "DaysView.hpp"

#include <format>

#include "Meal.hpp"
#include "MealView.hpp"
#include "MealsView.hpp"

DaysView::DaysView(const std::vector<Day>& days) {
	std::size_t numof_days = days.size();
	m_dates.reserve(numof_days);
	m_meals_per_day.reserve(numof_days);
	for(const Day& day : days) {
		m_dates.push_back(std::format("{:02d}.{:02d}.{}", day.date.day, day.date.month, day.date.year));
		m_meals_per_day.push_back(Make<MealsView>(day.meals));
	}

	m_layout = ResizableSplitLeft(
		Menu(&m_dates, &m_selected_date),
		Container::Tab(m_meals_per_day, &m_selected_date),
		&m_left_size
	);
}

Element DaysView::Render() { return m_layout->Render(); }

bool DaysView::OnEvent(Event e) { return m_layout->OnEvent(e); }

void DaysView::OnAnimation(animation::Params& params) { m_layout->OnAnimation(params); }

Component DaysView::ActiveChild() { return m_layout->ActiveChild(); }

bool DaysView::Focusable() const { return m_layout->Focusable(); }
