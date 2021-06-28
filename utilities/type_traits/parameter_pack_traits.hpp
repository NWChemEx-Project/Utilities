#pragma once
#include <type_traits>

namespace utilities::type_traits {

/** Compile-time check if a parameter pack contains a specified type
 *
 *  std::true_type if SearchTypes contains @p Type2Look4, std::false_type
 *  otherwise.
 *
 *  @tparam Type2Look4  The query type
 *  @tparam SearchTypes The types to query
 */
template <typename Type2Look4, typename... SearchTypes>
struct parameter_pack_contains_type :
  public std::disjunction<std::is_same<Type2Look4, SearchTypes>...> { };

/** Value alias for parameter_pack_contains_type
 *
 *  Syntactic sugar to alias parameter_pack_container_type<...>::value.
 *  
 *  std::true_type::value if @p Type2Look4 is contained in @p SearchTypes,
 *  std::false_type::calue otherwise
 *
 *  @tparam Type2Look4  The query type
 *  @tparam SearchTypes The types to query
 */
template <typename Type2Look4, typename... SearchTypes>
static inline constexpr bool parameter_pack_contains_type_v =
  parameter_pack_contains_type<Type2Look4, SearchTypes...>::value;



/** Count the number of times a type appears in a parameter pack at compile time.
 *
 *  Returns an std::integral_constant<size_t,...> which represents the number 
 *  of times @p Type2Look4 appears in @p SearchTypes.
 *
 *  @tparam Type2Look4  The query type
 *  @tparam SearchTypes The types to query
 */
template <typename Type2Look4, typename... SearchTypes>
struct parameter_pack_count_type;

/** Specialization of `parameter_pack_count_type` for empty parameter packs
 *
 *  Alias to std::integral_constant<std::size_t,0>
 *
 *  @tparam Type2Look4  The query type
 */
template <typename Type2Look4>
struct parameter_pack_count_type<Type2Look4> : public std::integral_constant<std::size_t, 0> {};

/** Specialization of `parameter_pack_count_type` for 0-th types which do not match
 *  the query type.
 *
 *  Skips type by inhereting the parameter_pack_count_type instance for
 *  the remainter types. Does not increment the value counter.
 *
 *  @tparam Type2Look4  The query type
 *  @tparam Type0       Non-matching type
 *  @tparam OtherTypes  The trailing types
 */
template <typename Type2Look4, typename Type0, typename... OtherTypes>
struct parameter_pack_count_type<Type2Look4, Type0, OtherTypes...> :
  public parameter_pack_count_type<Type2Look4, OtherTypes...> {};


/** Specialization of `parameter_pack_count_type` for 0-th types which do match
 *  the query type.
 *
 *  Increments the counter of the trailing instance of paramter_pack_count_type
 *
 *  @tparam Type2Look4  The query type
 *  @tparam OtherTypes  The trailing types
 */
template <typename Type2Look4, typename... OtherTypes>
struct parameter_pack_count_type<Type2Look4, Type2Look4, OtherTypes...> :
  public std::integral_constant<std::size_t, 
    parameter_pack_count_type<Type2Look4, OtherTypes...>::value + 1 > {};

/** Value alias for parameter_pack_count_type
 *
 *  Syntactic sugar to alias parameter_pack_container_type<...>::value.
 *
 *  @tparam Type2Look4  The query type
 *  @tparam SearchTypes The types to query
 */
template <typename Type2Look4, typename... SearchTypes>
inline constexpr std::size_t parameter_pack_count_type_v =
  parameter_pack_count_type<Type2Look4, SearchTypes...>::value;

}
