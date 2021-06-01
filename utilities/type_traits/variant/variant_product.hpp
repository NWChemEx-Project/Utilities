#pragma once
#include "utilities/type_traits/tuple/tuple_product.hpp"
#include "utilities/type_traits/tuple/variant.hpp"
#include "utilities/type_traits/variant/tuple.hpp"
#include <variant>

namespace utilities {
namespace detail_ {
/// Makes variant_prod_t easier to read
template<template<typename, typename> typename Op, typename LHSVariant,
         typename RHSVariant>
struct variant_prod {
    /// Convert LHSVariant to a tuple
    using lhs_as_tuple = utilities::variant_to_tuple_t<LHSVariant>;
    /// Convert RHSVariant to a tuple
    using rhs_as_tuple = utilities::variant_to_tuple_t<RHSVariant>;
    /// Take product of the two tuples, result is a tuple
    using result_type = utilities::tuple_prod_t<Op, lhs_as_tuple, rhs_as_tuple>;
    /// Convert product tuple to a variant
    using type = utilities::tuple_to_variant_t<result_type>;
};

} // namespace detail_

/** @brief Takes the Cartesian product of two variants.
 *
 *  This typedef will be set to a variant whose types are the Cartesian
 *  products of the types in @p LHSVariant with the types in @p RHSVariant. The
 *  type @p Op will be used to group the pairs. For example:
 *
 *  @code
 *  using variant0 = std::variant<int, double>;
 *  using variant1 = std::variant<float, char>;
 *  using variant01 = variant_prod_t<std::pair, variant0, variant1>;
 *  @endcode
 *
 *  will result in `variant01` being a typedef of
 * `std::variant<std::pair<int, float>, std::pair<int, char>, std::pair<double,
 *  float>, std::pair<double, char>>`.
 *
 *  @tparam Op A type templated on two types. @p Op will be used to group
 *             the pairs formed from taking the cartesian product of the types
 *             in @p LHSVariant and @p RHSVariant.
 *  @tparam LHSVariant The variant containing the first members of the pairs.
 *  @tparam RHSVariant The variant containing the second members of the pairs.
 */
template<template<typename, typename> typename Op, typename LHSVariant,
         typename RHSVariant>
using variant_prod_t =
  typename detail_::variant_prod<Op, LHSVariant, RHSVariant>::type;

} // namespace utilities