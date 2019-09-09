#pragma once
#include <tuple>

namespace utilities {

/** @brief Typedef for getting the type of concatenating many tuples.
 *
 *  Using a tuple to hold an arbitrary number of types is a common metatemplate
 *  programming technique. This handy typedef can be used to get the type of the
 *  tuple that results from concatenating a series of tuple instances. This is
 *  probably easier explained with an example:
 *
 *  @code
 *  using tuple1 = std::tuple<int, double>;
 *  using tuple2 = std::tuple<char, std::tuple<int>>;
 *  using tuple_union = tuple_cat_t<tuple1, tuple2>;
 *  using corr_tuple = std::tuple<int, double, char, std::tuple<int>>;
 *  static_assert(std::is_same<tuple_union, corr_tuple>::value);
 *  @endcode
 *
 *  @tparam Args the tuples to be concatenated. Each type is expected to be an
 *          std::tuple.
 */
template<typename... Args>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<Args>()...));

} // namespace utilities
