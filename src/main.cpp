#include <ncurses.h>

#include <csignal>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <gzip/decompress.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <string_view>

#include "constants.hpp"
#include "list_view.hpp"

using namespace nlohmann;

struct Date {
	int year;
	int month;
	int day;
};

WINDOW* main_viewport;
WINDOW* footer;
list_view* date_list;

unsigned width;
unsigned height;

std::string get_url_for_date(const Date&);
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

	for(const auto& el : api_response.at("ul_uni_sued").items())
		date_list->push_back(el.key());
	date_list->prepare_refresh();

	doupdate();
	while(char c = getch()) {
		if(c == 'q')
			break;
		wmove(main_viewport, 0, 0);
		wprintw(main_viewport, "%c", c);

		wnoutrefresh(main_viewport);
		wnoutrefresh(footer);
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
}

void setup_windows() {
	wresize(stdscr, 0, 0);
	wnoutrefresh(stdscr);

	main_viewport = newwin(height - FOOTER_HEIGHT, width - DATE_LIST_WIDTH, 0, DATE_LIST_WIDTH);
	wbkgd(main_viewport, COLOR_PAIR(STD_COLOR_PAIR));
	wnoutrefresh(main_viewport);

	date_list = new list_view(DATE_LIST_WIDTH, height - FOOTER_HEIGHT, 0, 0);
	wbkgd(**date_list, COLOR_PAIR(FOOTER_COLOR_PAIR));
	date_list->prepare_refresh();

	footer = newwin(FOOTER_HEIGHT, width, height - FOOTER_HEIGHT, 0);
	wbkgd(footer, COLOR_PAIR(FOOTER_COLOR_PAIR));
	wmove(footer, 0, 0);
	wprintw(footer, "n: next, p: prev");
	wnoutrefresh(footer);
}
