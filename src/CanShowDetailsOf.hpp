#pragma once

#include "WindowLike.hpp"

template <typename T>
class CanShowDetailsOf : public WindowLike {
public:
	using WindowLike::WindowLike;

	virtual void show(const T& t) = 0;
};
