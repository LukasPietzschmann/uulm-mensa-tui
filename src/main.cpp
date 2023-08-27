#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <string_view>

#include "Day.hpp"
#include "DaysView.hpp"

using namespace nlohmann;
using namespace ftxui;

static const std::map<std::string_view, std::string_view> mensen_nice_names = {
	{"Mensa", "Mensa Uni Süd"}, {"West", "Mensa Uni West"}};

Date parse_date(const std::string& date_string);

int main() {
	curlpp::Cleanup raii_cleanup;
	curlpp::Easy req;
	std::stringstream result;
	req.setOpt(cURLpp::Options::WriteStream(&result));
	req.setOpt<curlpp::options::Url>("https://uulm.anter.dev/api/v1/mensaplan.json");
	try {
		req.perform();
	} catch(const curlpp::LibcurlRuntimeError& e) {
		std::cerr << "[Network Error] " << e.what() << std::endl;
		return 1;
	}

	std::map<std::string_view, std::vector<Day>> data;
	for(const auto& [key, _] : mensen_nice_names)
		data.try_emplace(key, std::vector<Day>());

	const json& api_response = json::parse(result.view());
	for(const json& week : api_response.at("weeks")) {
		for(const json& day : week.at("days")) {
			Day parsed_day(parse_date(day.at("date").get<std::string>()));
			for(const auto& [key, _] : mensen_nice_names) {
				const json& mensa = day.at(key);
				for(const json& meal : mensa.at("meals")) {
					parsed_day.meals.emplace_back(meal.at("meal").get<std::string_view>(),
						meal.at("category").get<std::string_view>(), meal.at("price").get<std::string_view>());
				}
				data.at(key).emplace_back(parsed_day);
			}
		}
	}

	auto screen = ScreenInteractive::Fullscreen();
	int selected_mensa = 0;
	std::vector<std::string> mensen;
	mensen.reserve(data.size());
	for(const auto& [mensa, _] : data)
		mensen.emplace_back(mensen_nice_names.at(mensa));

	Components day_views;
	day_views.reserve(data.size());
	for(const auto& [_, days] : data)
		day_views.push_back(Make<DaysView>(days));

	// clang-format off
	auto container = Container::Vertical({
		Container::Horizontal({
			Renderer([] { return text("") | flex; }),
			Renderer([] { return text("Mensa Plan") | bold | color(Color::Red) | borderEmpty; }),
			Renderer([] { return text("") | flex; }),
			Dropdown(&mensen, &selected_mensa)
		}),
		Renderer([] { return separator(); }),
		Container::Tab(day_views, &selected_mensa),
		Renderer([] { return separator(); }),
		Container::Horizontal({
			Renderer([] { return text("Natigate using ← ↑ → ↓ or the mouse. Press ↵ or click to select. Press q to exit.") | dim | borderEmpty | flex; }),
			Button("Quit", [&screen] { screen.Exit(); })
		}),
	});
	// clang-format on

	container = CatchEvent(container, [&screen](Event e) {
		if(e == Event::Character('q')) {
			screen.ExitLoopClosure()();
			return true;
		}
		return false;
	});

	screen.Loop(container);
}

Date parse_date(const std::string& date_string) {
	std::stringstream date(date_string);
	char delimiter;
	unsigned year, month, a;
	date >> year >> delimiter >> month >> delimiter >> a;
	return Date(a, month, year);
}
