#pragma once

#include <ncurses.h>

#include <string>

class window_like {
public:
	window_like(WINDOW* base, unsigned screen_width, unsigned screen_height, unsigned x, unsigned y) :
		m_underlying_window(base), m_screen_width(screen_width), m_screen_height(screen_height), m_x(x), m_y(y) {}
	virtual ~window_like() = default;

	virtual void add_str(const std::string& s, unsigned x, unsigned y) = 0;
	virtual void clear() = 0;

	virtual void prepare_refresh() = 0;

	unsigned get_height() const { return m_screen_height; }
	unsigned get_width() const { return m_screen_width; }

	unsigned get_x() const { return m_x; }
	unsigned get_y() const { return m_y; }

	WINDOW* operator*() { return m_underlying_window; }

protected:
	WINDOW* m_underlying_window;
	unsigned m_screen_width;
	unsigned m_screen_height;

	unsigned m_x;
	unsigned m_y;
};
