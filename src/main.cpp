#include <ncurses.h>

#include <cassert>
#include <csignal>
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

#include "ListView.hpp"
#include "constants.hpp"

using namespace nlohmann;

struct Date {
	int year;
	int month;
	int day;

	std::string as_string() const {
		std::stringstream ss;
		ss << day << '.' << month << '.' << year;
		return ss.str();
	}
};

WINDOW* main_viewport;
WINDOW* footer;
ListView* date_list;

unsigned width;
unsigned height;

std::string get_url_for_date(const Date&);
Date parse_date(const std::string& date_string);
void handle_signal(int sig);
void setup_colors();
void setup_windows();

int main() {
	curlpp::Cleanup raii_cleanup;
	curlpp::Easy req;
	std::stringstream result;
	req.setOpt(cURLpp::Options::WriteStream(&result));
	req.setOpt<curlpp::options::Url>("https://uulm.anter.dev/api/v1/canteens/ul_uni_sued/all");
	req.perform();

	std::string decompressed_data = gzip::decompress(result.view().data(), result.view().size());

	json api_response = json::parse(decompressed_data);

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

	for(const auto& el : api_response.at("ul_uni_sued").items()) {
		const Date& date = parse_date(el.key());
		date_list->push_back(date.as_string());
	}
	date_list->select_elem(0);
	date_list->prepare_refresh();

	doupdate();
	while(int c = wgetch(footer)) {
		bool had_error = false;
		if(c == 'q')
			break;
		if(c == KEY_ARROW_UP || c == KEY_ARROW_DOWN) {
			had_error =
				!date_list->select_elem(1, c == KEY_ARROW_UP ? ListView::DECREMENT : ListView::INCREMENT) || had_error;
			date_list->prepare_refresh();
		} else {
			wmove(main_viewport, 0, 0);
			wprintw(main_viewport, "%c", c);
			wnoutrefresh(main_viewport);
		}

		if(had_error) {
			beep();
			flash();
		}

		doupdate();
	}

	delwin(footer);
	endwin();

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
	delwin(footer);
	delwin(main_viewport);
	endwin();
	exit(sig);
}

void setup_colors() {
	start_color();
	init_pair(STD_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
	init_pair(FOOTER_COLOR_PAIR, COLOR_BLACK, COLOR_RED);
	init_pair(HIGHLIGHT_EXPR_COLOR_PAR, COLOR_RED, COLOR_BLACK);
}

void setup_windows() {
	wresize(stdscr, 0, 0);
	wnoutrefresh(stdscr);

	main_viewport = newwin(height - FOOTER_HEIGHT, width - DATE_LIST_WIDTH, 0, DATE_LIST_WIDTH);
	wbkgd(main_viewport, COLOR_PAIR(STD_COLOR_PAIR));
	wnoutrefresh(main_viewport);

	date_list = new ListView(DATE_LIST_WIDTH, height - FOOTER_HEIGHT, 0, 0);
	wbkgd(**date_list, COLOR_PAIR(STD_COLOR_PAIR));
	date_list->prepare_refresh();

	footer = newwin(FOOTER_HEIGHT, width, height - FOOTER_HEIGHT, 0);
	wbkgd(footer, COLOR_PAIR(FOOTER_COLOR_PAIR));
	keypad(footer, true);
	wnoutrefresh(footer);
}
