#pragma once
#include <tuple>

namespace utilities {
/// has `type` such that U = std::tuple<Args...> becomes std::tuple<T<Args>...>
template<template<typename> typename T, typename U>
struct ApplyTypeToTuple;

/// actually implements ApplyTypeToTuple
template<template<typename> typename T, typename... Args>
struct ApplyTypeToTuple<T, std::tuple<Args...>> {
    using type = std::tuple<T<Args>...>;
};

/// Convenience typedef of the type inside ApplyTypeToTuple
template<template<typename> typename T, typename U>
using apply_type_to_tuple_t = typename ApplyTypeToTuple<T, U>::type;

} // namespace utilities
