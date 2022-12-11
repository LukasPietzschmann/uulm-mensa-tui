#pragma once

#include <type_traits>

template <typename T, typename... Args>
using All = typename std::enable_if_t<std::conjunction_v<std::is_convertible<Args, T>...>>;
