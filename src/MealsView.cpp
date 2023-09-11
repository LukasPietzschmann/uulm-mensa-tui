#include "MealsView.hpp"

#include <string>

#include "MealView.hpp"
#include "MealsView.hpp"

const std::set<std::string_view> MealsView::m_main_meal_categories{"Topf und Pfanne", "Prima Klima",
	"Fleisch und Fisch", "Sattmacher", "Pizza I", "Pizza II", "Pizza III", "Pasta I", "Pasta II", "Pasta III"};

MealsView::MealsView(const std::vector<Meal>& meals) {
	m_main_meals.reserve(4);
	m_secondary_meals.reserve(std::max(((int)meals.size()) - 4, 1));
	for(const Meal& meal : meals) {
		const Element& new_element = MealView(meal).Render();
		if(m_main_meal_categories.contains(meal.category))
			m_main_meals.push_back(new_element);
		else
			m_secondary_meals.push_back(new_element);
	}

	m_layout = Container::Vertical({
		Container::Horizontal({
			Renderer([&] { return text("") | flex; }),
			Toggle(&m_categories, &m_selected_tab),
			Renderer([&] { return text("") | flex; }),
		}),
		Renderer([&] { return separator(); }),
		Container::Tab(
			{
				Renderer([&] { return vflow(m_main_meals); }),
				Renderer([&] { return vflow(m_secondary_meals); }),
			},
			&m_selected_tab),
	});
}

Element MealsView::Render() { return m_layout->Render(); }

bool MealsView::OnEvent(Event e) { return m_layout->OnEvent(e); }

void MealsView::OnAnimation(animation::Params& params) { m_layout->OnAnimation(params); }

Component MealsView::ActiveChild() { return m_layout->ActiveChild(); }

bool MealsView::Focusable() const { return m_layout->Focusable(); }
