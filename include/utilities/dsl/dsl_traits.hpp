#pragma once
#include <type_traits>
#include <utilities/dsl/dsl_fwd.hpp>

namespace utilities::dsl {

template<typename T>
struct IsTerm : public std::false_type {};

template<typename T>
struct IsTerm<Term<T>> : public std::true_type {};

template<typename T>
constexpr bool is_term_v = IsTerm<T>::value;

} // namespace utilities::dsl