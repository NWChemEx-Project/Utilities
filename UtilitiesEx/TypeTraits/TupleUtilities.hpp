#pragma once
#include "UtilitiesEx/TypeTraits/type_traitsExtensions.hpp"
#include <tuple>
#include <type_traits>

/** @file TupleUtilities.hpp
 *
 *  This file collects a series of useful utilities for doing
 *  template meta-programming using tuples.
 *
 *  In particular this file contains:
 *
 *  - apply_functor_to_tuple which, as you may guess, applies a functor to a
 *       tuple.
 *  - reduce_tuple which utilizes a functor to reduce a tuple to a single value
 *  - combine_tuples which applies a functor element-wise to two tuples
 *
 */

namespace UtilitiesEx {
namespace detail_ {

/** @brief Type that will tell us if we have iterated over all elements of a
 *         tuple.
 *
 *  If @p I is the same as the number of elements in the tuple the resulting
 *  class will contain a bool member @p value set to true.  Otherwise @p value
 *  will be set to false.
 *
 *  @tparam I The iteration number.
 *  @tparam tuple_type The type of the tuple we are iterating over.
 */
template<std::size_t I, typename tuple_type>
using recursion_done = std::is_same<
    std::integral_constant<std::size_t, I>,
    std::integral_constant<std::size_t,
                           std::tuple_size<std::decay_t<tuple_type>>::value>>;

/** @brief Type that will tell us if we are not done iterating over all
 * elements of a tuple.
 *
 *  If @p I is the same as the number of elements in the tuple the resulting
 *  class will contain a bool member @p value set to false.  Otherwise @p value
 *  will be set to true.
 *
 *  @note One could negate the result of recursion_done instead of using this
 *  class; however, doing so results in the same type and can't be used for
 *  SFINAE.
 *
 *  @tparam I The iteration number.
 *  @tparam tuple_type The type of the tuple we are iterating over.
 */
template<std::size_t I, typename tuple_type>
using recursion_not_done = Negation<recursion_done<I, tuple_type>>;

/// Enables a function via SFINAE if recursion through a tuple has finished
template<std::size_t I, typename tuple_type>
using done_recursing =
    std::enable_if<recursion_done<I, tuple_type>::value, int>;

/// Enables a function via SFINAE if recursion through a tuple is still going
template<std::size_t I, typename tuple_type>
using recursing = std::enable_if<recursion_not_done<I, tuple_type>::value, int>;

/// Actually implements the apply_functor_to_tuple function
template<typename tuple_type, typename functor_type, std::size_t... I>
auto apply_functor_to_tuple_impl(tuple_type && tuple,
                                 functor_type && functor,
                                 std::index_sequence<I...>)
{
    return std::make_tuple(
        functor(std::get<I>(std::forward<tuple_type>(tuple)))...);
};

/// End-point for reducing a tuple
template<std::size_t I,
         typename tuple_type,
         typename functor_type,
         typename return_type,
         typename done_recursing<I, tuple_type>::type = 0>
return_type reduce_tuple_impl(tuple_type &&, functor_type &&, return_type val)
{
    return val;
};

/// The guts of the recursive process for reducing a tuple
template<std::size_t I,
         typename tuple_type,
         typename functor_type,
         typename return_type,
         typename recursing<I, tuple_type>::type = 0>
return_type reduce_tuple_impl(tuple_type && tuple,
                              functor_type && functor,
                              return_type val)
{
    auto new_val = functor(val, std::get<I>(std::forward<tuple_type>(tuple)));
    return reduce_tuple_impl<I + 1>(std::forward<tuple_type>(tuple),
                                    std::forward<functor_type>(functor),
                                    new_val);
};

/// The guts behind actually combining tuples via a functor
template<typename lhs_type,
         typename rhs_type,
         typename functor_type,
         std::size_t... I>
auto combine_tuples_impl(lhs_type && lhs,
                         rhs_type && rhs,
                         functor_type && functor,
                         std::index_sequence<I...>)
{
    return std::make_tuple(
        functor(std::get<I>(std::forward<lhs_type>(lhs)),
                std::get<I>(std::forward<rhs_type>(rhs)))...);
};

} // namespace detail_

/** @brief Given a tuple of objects this function will apply a functor to
 * each of them and return the results as a tuple.
 *
 * @param[in] tuple The tuple to apply the functor to.
 * @param[in] functor An instance of the functor to use.
 *
 * @tparam tuple_type The type of the std::tuple we are applying a functor to.
 * @tparam functor_type The type of the functor we are applying to the tuple.
 * The functor's operator() must be take a single argument, which will be one of
 * the tuple's elements (hence for tuples with hetrogenous types, it must be
 * templated on the input type).  Hint: one can use auto as the return type to
 * avoid having to do additional template meta-programming.
 *
 */
template<typename tuple_type, typename functor_type>
auto apply_functor_to_tuple(tuple_type && tuple, functor_type && functor)
{
    constexpr std::size_t nelems =
        std::tuple_size<std::decay_t<tuple_type>>::value;
    return detail_::apply_functor_to_tuple_impl(
        std::forward<tuple_type>(tuple),
        std::forward<functor_type>(functor),
        std::make_index_sequence<nelems>());
};

/** @brief Applies a reduction to a tuple
 *
 *  @param tuple The tuple to reduce
 *  @param functor The functor instance to use to do the reduction
 *  @param val The initial value for the reduction.
 *  @return The value resulting from reducing all elements of the tuple
 *
 *  @tparam tuple_type The type of the tuple we are reducing
 *
 *  @tparam functor_type The type of the functor doing the reduction.  Must
 *  define a member() with the signature:
 *  @code
 *  //Applies the functor to a tuple element combining it
 *  //param sum is the current reduction total
 *  //param element is the element to reduce into sum
 *  //return is the result of reducing sum and element
 *  return_type operator()(return_type sum, tuple_element element);
 *  @endcode
 *
 *  @tparam return_type The type resulting from a reduction.
 *
 *  @throws ??? Throws if any application of the functor to the tuple throws.
 *              Throw guarantee is same as that of functor.
 */
template<typename tuple_type, typename functor_type, typename return_type>
return_type reduce_tuple(tuple_type && tuple,
                         functor_type && functor,
                         return_type val)
{
    return detail_::reduce_tuple_impl<0>(std::forward<tuple_type>(tuple),
                                         std::forward<functor_type>(functor),
                                         val);
};

template<typename lhs_type, typename rhs_type, typename functor_type>
auto combine_tuples(lhs_type && lhs, rhs_type && rhs, functor_type && functor)
{
    constexpr std::size_t size = std::tuple_size<std::decay_t<lhs_type>>::value;
    static_assert(size == std::tuple_size<std::decay_t<rhs_type>>::value);
    return detail_::combine_tuples_impl(std::forward<lhs_type>(lhs),
                                        std::forward<rhs_type>(rhs),
                                        std::forward<functor_type>(functor),
                                        std::make_index_sequence<size>());
};
} // namespace UtilitiesEx
