#pragma once
#include "utilities/type_traits/tuple/tuple_cat.hpp"
#include "utilities/type_traits/tuple/variant.hpp"
#include "utilities/type_traits/variant/to_tuple.hpp"

namespace utilities::type_traits::variant {

/** @brief Creates a new variant by concatenating 0 or more variants.
 *
 *  @tparam Args The v
 */
template<typename... Args>
using cat_t = tuple::to_variant_t<tuple::tuple_cat_t<to_tuple_t<Args>...>>;

} // namespace utilities::type_traits::variant