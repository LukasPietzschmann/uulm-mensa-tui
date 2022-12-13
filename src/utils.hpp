#pragma once

#include <algorithm>
#include <regex>
#include <string>
#include <type_traits>

enum class SelectAction { INCREMENT, DECREMENT, SELECT };

template <typename T, typename... Args>
using All = typename std::enable_if_t<std::conjunction_v<std::is_convertible<Args, T>...>>;

template <typename T, typename B>
using InstanceOf = typename std::enable_if_t<std::is_base_of_v<B, T>>;

template <typename T, typename... Args>
using ConstructableWith = typename std::enable_if_t<std::is_constructible_v<T, Args...>>;

template <typename T>
inline const T& max(const T& t1) {
	return t1;
}

template <typename T, typename... Args, typename = All<T, Args...>>
inline const T& max(const T& t1, const T& t2, const Args&... args) {
	return std::max(t1, max(t2, args...));
}

template <typename T>
inline const T& min(const T& t1) {
	return t1;
}

template <typename T, typename... Args, typename = All<T, Args...>>
inline const T& min(const T& t1, const T& t2, const Args&... args) {
	return std::min(t1, min(t2, args...));
}

inline std::string replace_sub(const std::string& orig, const std::string& sub, const std::string& repl) {
	return std::regex_replace(orig, std::regex(sub), repl);
}

struct Showable {
	virtual ~Showable() = default;
	virtual std::string as_string() const = 0;
};
