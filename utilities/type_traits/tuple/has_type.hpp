#pragma once
#include <tuple>
#include <type_traits>

namespace utilities::type_traits::tuple {
namespace detail_ {

/** @brief Primary template for determining if a type is in a tuple.
 *
 *  This struct can be used to determine if @p Type2Look4 appears in the tuple
 *  @p Tuple. The primary template is selected when @p Tuple is not an
 *  std::tuple; the primary template is not defined and thus a compilation error
 *  will arise if @p Tuple is not a tuple.
 *
 *  @tparam Type2Look4 The type we are attempting to find in @p Tuple
 *  @tparam Tuple If this version of has_type is selected @p Tuple will be a
 *                type other than std::tuple.
 */
template<typename Type2Look4, typename Tuple>
struct has_type;

/** @brief Specializes has_type for when the std::tuple is empty.
 *
 *  Looping over the types in the provided std::tuple ends when there are no
 *  more types in the std::tuple. The recursion endpoint is implemented by this
 *  class which contains a static constexpr member `value` which will be set to
 *  false (if the type had been found we would have stopped recursing).
 *
 *  @tparam Type2Look4 The type we are looking for in the std::tuple.
 */
template<typename Type2Look4>
struct has_type<Type2Look4, std::tuple<>> : std::false_type {};

/** @brief Specializes has_type for when there are types in the std::tuple, but
 *         the 0-th type is not the type we are looking for.
 *
 *  This specialization is the loop body for determining if the type
 *  @p Type2Look4 appears in the tuple. This specialization is selected every
 *  time there is at least one type in the tuple, but the 0-th type is not the
 *  type we are looking for. This specialization inherits from the
 *  specialization of has_type for std::tuple<OtherTypes...> (i.e., the tuple
 *  without the 0-th type in the present tuple).
 *
 *  Eventually the class hierarchy either inherits from has_type with an empty
 *  tuple, in which case this class contains a static constexpr member value set
 *  to false, or it inherits from has_type with a std::tuple where the 0-th type
 *  is the type we are looking for. In the latter case this class contains a
 *  static constexpr member `value` set to true.
 *
 *  @tparam Type2Look4 The type we are attempting to find in the tuple
 *  @tparam Type0 The 0-th type in the tuple we are looking at.
 *  @tparam OtherTypes The other types in the tuple
 */
template<typename Type2Look4, typename Type0, typename... OtherTypes>
struct has_type<Type2Look4, std::tuple<Type0, OtherTypes...>>
  : has_type<Type2Look4, std::tuple<OtherTypes...>> {};

/** @brief Specializes has_type for the scenario in which the 0-th type of the
 *         tuple is the type we are looking for.
 *
 *  This specialization is selected when the 0-th type in tuple is the type we
 *  are looking for. It is the end point for recursion through tuples which
 *  have the type we are looking for. Inheriting from this specialization will
 *  provide a static constexpr member `value` which is set to true.
 *
 *  @tparam Type2Look4 The type we are looking for in the tuple.
 *  @tparam OtherTypes The other types in the tuple.
 */
template<typename Type2Look4, typename... OtherTypes>
struct has_type<Type2Look4, std::tuple<Type2Look4, OtherTypes...>>
  : std::true_type {};
} // namespace detail_

/** @brief Global variable indicating whether @p Type2Look4 is in the tuple
 *         @p Tuple.
 *
 *  This is the public API for determining if a type appears in a tuple. If
 *  @p Type2Look4 appears in @p Tuple this variable will be set to true,
 *  otherwise it is set to false.
 *
 *  @tparam Type2Look4 The type we are looking for.
 *  @tparam Tuple The tuple we are searching.
 */
template<typename Type2Look4, typename Tuple>
static constexpr bool has_type_v = detail_::has_type<Type2Look4, Tuple>::value;

template<typename Type2Look4, typename Tuple, typename T = void>
using enable_if_has_type_t = std::enable_if_t<has_type_v<Type2Look4, Tuple>, T>;

} // namespace utilities::type_traits::tuple