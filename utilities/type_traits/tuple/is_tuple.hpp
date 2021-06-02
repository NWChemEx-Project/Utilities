#pragma once
#include <type_traits>

namespace utilities::type_traits::tuple {

/// Type trait for when @p T is not an std::tuple or std::pair has  value=false
template<typename T>
struct IsTuple : std::false_type {};

/// Type trait for when @p T is an std::tuple, has value = true
template<typename... Args>
struct IsTuple<std::tuple<Args...>> : std::true_type {};

/// Type trait for when @p T is an std::pair, has value = true
template<typename T, typename U>
struct IsTuple<std::pair<T, U>> : std::true_type {};

/// Convenience global variable for easily getting the result of IsTuple
template<typename T>
static constexpr bool is_tuple_v = IsTuple<T>::value;

} // namespace utilities::type_traits::tuple
