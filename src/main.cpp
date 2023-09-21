#include <cassert>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <cxxopts.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>

#include "Day.hpp"
#include "DaysView.hpp"

using namespace nlohmann;
using namespace ftxui;

static const std::map<std::string_view, std::string_view> mensen_nice_names = {
	{"Mensa", "Mensa Uni Süd"}, {"West", "Mensa Uni West"}, {"Bistro", "Bistro Uni Nord"}};
static const std::string_view default_mensa = mensen_nice_names.at("Mensa");

Date parse_date(const std::string& date_string);
std::optional<Meal::Attribute> parse_attribute(const std::string& string);

int main(int argc, const char** argv) {
	cxxopts::Options options(argv[0]);
	options.add_options()("f,filter",
		"Only show Meals with the given attributes. Posisble values are: Bio, Vegetarisch, Vegan, Schwein, Rind. "
		"Example usage: -f Bio,Vegan",
		cxxopts::value<std::vector<std::string>>());
	const cxxopts::ParseResult& cmdline = options.parse(argc, argv);
	std::set<Meal::Attribute> filter_attributes;
	if(cmdline.count("filter")) {
		for(const std::string& filter : cmdline["filter"].as<std::vector<std::string>>()) {
			const auto& attribute = Meal::string_to_attribute(filter);
			if(!attribute) {
				std::cout << options.help();
				return 1;
			}
			filter_attributes.emplace(*attribute);
			if(*attribute == Meal::Attribute::VEGETARIAN)
				filter_attributes.emplace(Meal::Attribute::VEGAN);
		}
	}

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
			for(const auto& [key, _] : mensen_nice_names) {
				const json& mensa = day.at(key);
				bool is_open = mensa.at("open");
				Day parsed_day(parse_date(day.at("date").get<std::string>()), is_open);
				for(const json& meal : mensa.at("meals")) {
					std::set<Meal::Attribute> attributes;
					for(const auto& raw_attribute : meal.at("allergy")) {
						const std::optional<Meal::Attribute>& attribute = parse_attribute(raw_attribute);
						if(attribute)
							attributes.emplace(*attribute);
					}
					bool matches_filter = filter_attributes.empty();
					for(const Meal::Attribute& filter : filter_attributes) {
						if(attributes.contains(filter)) {
							matches_filter = true;
							break;
						}
					}
					if(matches_filter) {
						parsed_day.meals.emplace_back(meal.at("meal").get<std::string_view>(),
							meal.at("category").get<std::string_view>(), meal.at("price").get<std::string_view>(),
							attributes);
					}
				}
				data.at(key).emplace_back(parsed_day);
			}
		}
	}

	auto screen = ScreenInteractive::Fullscreen();
	std::vector<std::string> mensen;
	mensen.reserve(data.size());
	for(const auto& [mensa, _] : data)
		mensen.emplace_back(mensen_nice_names.at(mensa));
	const auto& it = std::find(mensen.begin(), mensen.end(), default_mensa);
	assert(it != mensen.end());
	int selected_mensa = std::distance(mensen.begin(), it);

	Components day_views;
	day_views.reserve(data.size());
	for(const auto& [_, days] : data)
		day_views.push_back(Make<DaysView>(days));

	std::stringstream attributes;
	if(!filter_attributes.empty())
		attributes << "Filter: ";
	for(auto it = filter_attributes.begin(); it != filter_attributes.end(); ++it) {
		if(it != filter_attributes.begin())
			attributes << ", ";
		attributes << Meal::attibute_to_string(*it);
	}
	// clang-format off
	auto container = Container::Vertical({
		Container::Horizontal({
			Renderer([] { return text("Mensa Plan") | bold | color(Color::Red) | borderEmpty | flex; }),
			Renderer([&attributes] { return text(attributes.str()) | borderEmpty | flex; }),
			Dropdown(&mensen, &selected_mensa)
		}),
		Renderer([] { return separator(); }),
		Container::Tab(day_views, &selected_mensa) | yflex_grow,
		Renderer([] { return separator(); }),
		Container::Horizontal({
			Renderer([] { return text("Natigate using ← ↑ → ↓ or the mouse. Press ↵ or click to select. Press ? for more.") | dim | borderEmpty | flex; }),
			Button("Quit", [&screen] { screen.Exit(); })
		}),
	});
	// clang-format on

	bool modal_shown = false;
	// clang-format off
	Component modal = Container::Vertical({
		Renderer([] { return vbox({
			hbox({text("") | flex, text("Help"), text("") | flex}),
			separator(),
			vbox({
				paragraph("▶ You can generally navigate the app using the arrow keys or the mouse (using the mouse might be easier though)"),
				paragraph("▶ ? Toggles this popup"),
				paragraph("▶ q closes the whole app"),
				paragraph("▶ You can select your canteen of choice in the top right"),
				paragraph("▶ Selecting a date on the left will show every meal of that day"),
				paragraph("▶ Meals are split into main and secondary dishes: you can choose in the top center which category gets displayed"),
			}),
		});}),
		Button("Quit", [&modal_shown] { modal_shown = false; })
	}) | size(WIDTH, LESS_THAN, 80) | border;
	// clang-format on

	container |= Modal(modal, &modal_shown);

	container = CatchEvent(container, [&screen, &modal_shown](Event e) {
		if(e == Event::Character('q')) {
			screen.ExitLoopClosure()();
			return true;
		} else if(e == Event::Character('?')) {
			modal_shown = !modal_shown;
			return true;
		}
		return false;
	});

	screen.Loop(container);
}

Date parse_date(const std::string& date_string) {
	std::stringstream date(date_string);
	char delimiter;
	unsigned year, month, day;
	date >> year >> delimiter >> month >> delimiter >> day;
	return Date(day, month, year);
}

std::optional<Meal::Attribute> parse_attribute(const std::string& string) {
	if(string == "veg")
		return Meal::Attribute::VEGETARIAN;
	else if(string == "van")
		return Meal::Attribute::VEGAN;
	else if(string == "bio")
		return Meal::Attribute::BIO;
	else if(string == "S")
		return Meal::Attribute::PORK;
	else if(string == "R")
		return Meal::Attribute::BEEF;
	return {};
}
