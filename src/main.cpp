#include <ncurses.h>

#include <cassert>
#include <csignal>
#include <ctime>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <gzip/decompress.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>

#include "DateDetails.hpp"
#include "ListElement.hpp"
#include "MasterSlave.hpp"
#include "constants.hpp"
#include "utils.hpp"

using namespace nlohmann;

MasterSlave<ListElement, DateDetails>* ms;
WINDOW* footer;

unsigned width;
unsigned height;

std::string get_url_for_date(const Date&);
Date parse_date(const std::string& date_string);
void handle_signal(int sig);
void end_curses_mode();
void setup_colors();
void setup_windows();

bool is_date_in_the_past(const Date& date);

int main() {
	curlpp::Cleanup raii_cleanup;
	curlpp::Easy req;
	std::stringstream result;
	req.setOpt(cURLpp::Options::WriteStream(&result));
	req.setOpt<curlpp::options::Url>("https://uulm.anter.dev/api/v1/canteens/ul_uni_sued/all");
	try {
		req.perform();
	} catch(const curlpp::LibcurlRuntimeError& e) {
		std::cerr << "[Network Error] " << e.what() << std::endl;
		return 1;
	}

	std::string decompressed_data = gzip::decompress(result.view().data(), result.view().size());

	json api_response = json::parse(decompressed_data);
	const auto& uni_sued_data = api_response.value("ul_uni_sued", json::array());

	initscr();

	signal(SIGINT, handle_signal);
	signal(SIGABRT, handle_signal);
	signal(SIGKILL, handle_signal);

	cbreak();
	timeout(-1);
	noecho();
	curs_set(0);
	getmaxyx(stdscr, height, width);

	setup_colors();
	setup_windows();

	for(const auto& element : uni_sued_data.items()) {
		const Date& date = parse_date(element.key());

		if(is_date_in_the_past(date))
			continue;

		std::vector<Meal> meals;
		meals.reserve(4);
		for(const auto& meal : element.value().items()) {
			const auto& meal_value = meal.value();
			meals.emplace_back(meal_value.at("category").get<std::string>(), meal_value.at("name").get<std::string>(),
				meal_value.at("prices").at("students").get<std::string>());
		}
		ms->emplace_back(date, meals);
	}
	ms->select_elem(0, SelectAction::SELECT);
	ms->prepare_refresh();

	if(ms->size() == 0) {
		end_curses_mode();
		std::cout << "No meals available :(\n" << std::endl;
		return 0;
	}

	doupdate();
	while(int c = wgetch(footer)) {
		bool had_error = false;
		if(c == 'q')
			break;
		if(c == KEY_ARROW_UP || c == KEY_ARROW_DOWN || c == 'j' || c == 'k') {
			SelectAction action = c == KEY_ARROW_UP || c == 'k' ? SelectAction::DECREMENT : SelectAction::INCREMENT;
			had_error = !ms->select_elem(1, action) || had_error;
			ms->prepare_refresh();
		} else if(c == KEY_ESCAPE) {
			ms->unselect_elem();
			ms->prepare_refresh();
		}

		if(had_error) {
			beep();
			flash();
		}

		doupdate();
	}

	handle_signal(0);
}

std::string get_url_for_date(const Date& date) {
	static const std::string_view api_base = "https://uulm.anter.dev/api/v1/canteens/ul_uni_sued/days/";
	std::stringstream ss;
	ss << api_base << date.year << "-" << date.month << "-" << date.day << "/meals";
	return ss.str();
}

Date parse_date(const std::string& date_string) {
	std::smatch match;
	bool found_match = std::regex_match(
		date_string, match, std::regex("(\\d{4})-(\\d{2})-(\\d{2})", std::regex_constants::ECMAScript));
	assert(found_match);
	return {std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3])};
}

void handle_signal(int sig) {
	end_curses_mode();
	exit(sig);
}

void end_curses_mode() {
	delete ms;
	delwin(footer);
	endwin();
}

void setup_colors() {
	start_color();
	init_color(COLOR_REAL_BLACK, 0, 0, 0);
	init_pair(STD_COLOR_PAIR, COLOR_WHITE, COLOR_REAL_BLACK);
	init_pair(FOOTER_COLOR_PAIR, COLOR_REAL_BLACK, COLOR_RED);
	init_pair(HIGHLIGHT_EXPR_COLOR_PAR, COLOR_RED, COLOR_REAL_BLACK);
}

void setup_windows() {
	wresize(stdscr, 0, 0);
	wnoutrefresh(stdscr);

	ms = new MasterSlave<ListElement, DateDetails>(width, height - FOOTER_HEIGHT, 0, 0);
	ms->set_bg(STD_COLOR_PAIR);
	ms->prepare_refresh();

	footer = newwin(FOOTER_HEIGHT, width, height - FOOTER_HEIGHT, 0);
	wbkgd(footer, COLOR_PAIR(FOOTER_COLOR_PAIR));
	const std::string& string = pad_center_string_to_width("Arrow up/down: Select date\tq: Quit", width);
	mvwaddnstr(footer, 0, 0, string.c_str(), string.size());

	keypad(footer, true);
	wnoutrefresh(footer);
}

bool is_date_in_the_past(const Date& date) {
	std::tm futureDate;
	futureDate.tm_year = date.year - 1900;
	futureDate.tm_mon = date.month - 1;
	// A little bit hacky, but it works
	// This is needed to show "today"
	futureDate.tm_mday = date.day + 1;

	std::mktime(&futureDate);
	std::time_t t = std::time(nullptr);
	std::time_t futureTime = std::mktime(&futureDate);

	return t > futureTime;
}
