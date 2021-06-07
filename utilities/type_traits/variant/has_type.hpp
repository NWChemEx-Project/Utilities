#pragma once
#include "utilities/type_traits/tuple/has_type.hpp"
#include "utilities/type_traits/variant/to_tuple.hpp"
#include <tuple>
#include <type_traits>
#include <variant>

namespace utilities::type_traits::variant {

/** @brief Global variable indicating whether @p Type2Look4 is in the variant
 *         @p Variant.
 *
 *  This is the public API for determining if a type appears in a variant. If
 *  @p Type2Look4 appears in @p Variant this variable will be set to true,
 *  otherwise it is set to false.
 *
 *  @tparam Type2Look4 The type we are looking for.
 *  @tparam Variant The std::variant we are searching.
 */
template<typename Type2Look4, typename Variant>
static constexpr bool has_type_v =
  tuple::has_type_v<Type2Look4, to_tuple_t<Variant>>;

template<typename Type2Look4, typename Variant, typename T = void>
using enable_if_has_type_t =
  std::enable_if_t<has_type_v<Type2Look4, Variant>, T>;

} // namespace utilities::type_traits::variant