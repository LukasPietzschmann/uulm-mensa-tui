#pragma once

#include <string>
#include <type_traits>

enum class SelectAction { INCREMENT, DECREMENT, SELECT };

template <typename T, typename... Args>
using All = typename std::enable_if_t<std::conjunction_v<std::is_convertible<Args, T>...>>;

template <typename T, typename B>
using InstanceOf = typename std::enable_if_t<std::is_base_of_v<B, T>>;

struct Showable {
	virtual ~Showable() = default;
	virtual std::string as_string() const = 0;
};
