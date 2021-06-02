#pragma once
#include <tuple>

namespace utilities::type_traits::tuple {

/** @brief Type of resulting from concatentating several tuple types together.
 *
 *  This typedef
 *
 *  As an example:
 *
 *  @code
 *  using tuple0_t = std::tuple<int, double>;
 *  using tuple1_t = std::tuple<char, bool>;
 *  using tuple0p1_t = tuple_cat_t<tuple0_t, tuple1_t>;
 *  @endcode
 *
 *  Results in the typedef `tuple0p1_t` being a typedef of
 *  `std::tuple<int, double, char, bool>`.
 *
 *  @tparam Args A series of 0 or more tuple types. Each type in the template
 *               parameter pack is expected to be an instantiation of
 *               std::tuple.
 */
template<typename... Args>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<Args>()...));

} // namespace utilities::type_traits::tuple