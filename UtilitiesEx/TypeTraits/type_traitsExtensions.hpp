#pragma once
#include <utility>
#include <type_traits>

/** @file This file contains functions that should be viewed as extensions of
 *        the STL's type_traits library.
 *
 *  The main intent of this file is so people can ensure their classes adhere
 *  to the STL concepts.  Heavy usage of them is likely to balloon compile time
 *  as they are template heavy.
 *
 *  @note These checks should be taken as necessary, but not sufficient at this
 *        time.
 *
 *  @note My capitalization syntax for the most part matches that of the STL.
 *        The reason being much of this is code that is generated by macros to
 *        look for standard features mandated by the STL.  It's easier and more
 *        intuitive to adopt to their syntax.   Only when there may be a
 *        conflict with newer versions of the STL do I revert back to CamelCase.
 *
 *  Contents:
 *  - VoidType    : Type that always defines a type "type" that is void
 *  - Conjunction : performs multiple checks returning true if all are true
 *  - has_value_type  : Checks if type defines typedef "value_type"
 *  - has_reference   : Checks if type defines typedef "reference"
 *  - has_const_reference   : Checks if type defines typedef "const_reference"
 *  - has_pointer   : Checks if type defines typedef "pointer"
 *  - has_const_pointer  : Checks if type defines typedef "const_pointer"
 *  - has_iterator   : Checks if type defines typedef "iterator"
 *  - has_const_iterator   : Checks if type defines typedef "const_iterator"
 *  - has_difference_type   : Checks if type defines typedef "difference_type"
 *  - has_size_type  : Checks if type defines typedef "size_type"
 *  - has_iterator_category : Checks for typedef  "iterator_category"
 *  - has_begin : Checks for member function "begin"
 *  - has_end : Checks for member function "end"
 *  - has_cbegin : Checks for member function "cbegin"
 *  - has_cend : Checks for member function "cend"
 *  - has_size : Checks for member function "size"
 *  - has_max_size : Checks for member function "max_size"
 *  - has_empty : Checks for member function "empty"
 *  - has_equal_to : Checks for operator==
 *  - has_not_equal_to : Checks for operator!=
 *  - has_less_than : Checks for operator<
 *  - has_greater_than : Checks for operator>
 *  - has_less_than_equal : Checks for operator<=
 *  - has_greater_than_equal : Checks for operator>=
 *  - has_increment_by : Checks for operator+=
 *  - has_decrement_by : Checks for operator-=
 *  - has_plus : Checks for operator+
 *  - has_minus : Checks for operator-
 *  - has_dereference : Checks for operator*()
 *  - has_arrow : Checks for operator->
 *  - has_prefix_increment : Checks for operator++()
 *  - has_postfix_increment : Checks for operator++(int)
 *  - has_prefix_decrement : Checks for operator--()
 *  - has_postfix_decrement : Checks for operator--(int)
 *  - is_indexable : Checks for operator[]
 *  - is_container : Checks if type satisfies concept of "container"
 *  - is_iterator : Checks if type satisfies concept of "iterator"
 *  - is_input_iterator : Checks if type satisfies concept of input iterator
 *  - is_forward_iterator : Checks if type satisfies concept of forward iterator
 *  - is_bidirectional_iterator : Checks if type satisfies concept of
 *                                bidirectional iterator
 *  - is_random_access_iterator : Checks if type satisfies concept of random
 *                                access iterator
 *
 */


///Namespace for all functionality in the UtilitiesEx library
namespace UtilitiesEx {

/** @brief A type that is always void.
 *
 *  Basically this class provides us a SFINAE vessel for checking if a type
 *  exists, without specializing the call to that type.
 *
 *  @todo C++17 provides this type, switch when we can use C++17
 *
 *  @tparam T An arbitrary type
 */
template<typename /*T*/>
struct VoidType {
    using type=void;
};

/** @brief Sets the member value to the opposite of the input value
 *
 * @tparam T the class containing the value to negate
 * @todo C++17 provides this type
 */
template<typename T>
struct Negation {
    constexpr static bool value = !T::value;
};

/** @brief Returns true if all the types are true too.
 *
 *  @todo C++17 provides this class.
 *
 */
template<typename...> struct Conjunction : std::true_type { };
template<typename B1> struct Conjunction<B1> : B1 { };
template<typename B1, typename... Bn>
struct Conjunction<B1, Bn...>
        : std::conditional_t<bool(B1::value), Conjunction<Bn...>, B1> {};

/** @brief This macro will define a class gas_xxx for determining if a
 *         type defines a typedef xxx.
 *
 *  Although at first it may seem like we could make the requested type a
 *  template type parameter a simple example shows why this won't work:
 *  @code
 *  //Won't work, type_2_check_for is not a type declared in this scope
 *  bool answer=HasType<type_2_check, type_2_check_for>::value;
 *  @endcode
 *  We could do something like:
 *  @code
 *  bool answer=HasType<typename type_2_check::type_2_check_for>::value;
 *  @endcode
 *  but that simply won't compile if it doesn't have the type (in other words,
 *  it doesn't actually return false as intended).
 *
 *  The only way I can see to do this is to declare a has_type_xxx for each type
 *  we want to check for.  This isn't so bad because we really only need to
 *  check for a few types, assuming usual nomenclature.  These classes really
 *  only differ in their names, and the type they try to call, hence a macro is
 *  needed to reduce the boiler-plate.
 *
 *  @param[in] NAME name of the type to define a check for.
 */
#define HAS_TYPE(NAME)\
template<typename, typename = void> \
struct has_type_##NAME :std::false_type {};\
template<typename T>\
struct has_type_##NAME<T, typename VoidType<typename T::NAME>::type > : \
std::true_type{}

HAS_TYPE(value_type);
HAS_TYPE(reference);
HAS_TYPE(const_reference);
HAS_TYPE(pointer);
HAS_TYPE(const_pointer);
HAS_TYPE(iterator);
HAS_TYPE(const_iterator);
HAS_TYPE(difference_type);
HAS_TYPE(size_type);
HAS_TYPE(iterator_category);

#undef HAS_TYPE

/** @brief This macro will define a class has_xxx for determining if a
 *         type has a function xxx.
 *
 *  @note The function must not return void for this macro to work
 *
 *  @param[in] type_name name of the member to define a check for.
 */
#define HAS_MEMBER(NAME)\
template<typename T, typename = void>\
struct has_##NAME : std::false_type {};\
template<typename T>\
struct has_##NAME<T,typename VoidType<decltype(\
    std::declval<T>().NAME())>::type> : std::true_type{}

HAS_MEMBER(begin);
HAS_MEMBER(end);
HAS_MEMBER(cbegin);
HAS_MEMBER(cend);
HAS_MEMBER(size);
HAS_MEMBER(max_size);
HAS_MEMBER(empty);

#undef HAS_MEMBER

/** @brief This macro will define a class has_xxx for determining if a
 *         type has an operator corresponding to description xxx.
 *
 *  @param[in] NAME name of the member for function
 *  @param[in] symbol the symbol the check for
 */
#define HAS_OPERATOR(NAME,symbol)\
template<typename T, typename U, typename = void>\
struct has_##NAME : std::false_type {};\
template<typename T, typename U>\
struct has_##NAME<T, U, typename VoidType<decltype(\
    std::declval<T>() symbol std::declval<U>())>::type> :\
std::true_type{}

HAS_OPERATOR(equal_to,==);
HAS_OPERATOR(not_equal_to,!=);
HAS_OPERATOR(less_than,<);
HAS_OPERATOR(greater_than,>);
HAS_OPERATOR(less_than_equal,<=);
HAS_OPERATOR(greater_than_equal,>=);
HAS_OPERATOR(increment_by,+=);
HAS_OPERATOR(decrement_by,-=);
HAS_OPERATOR(plus,+);
HAS_OPERATOR(minus,-);



/** @brief Checks if a type is dereference-able via the star operator
 *
 * @tparam T the type to check.
 */
template<typename T, typename = void>
struct has_dereference : std::false_type {};
template<typename T>
struct has_dereference<T,
typename VoidType<decltype(*std::declval<T>())>::type> : std::true_type{};

/** @brief Checks if a type has the arrow operator
 *
 * @tparam T the type to check.
 */
template<typename T, typename = void>
struct has_arrow : std::false_type {};
template<typename T>
struct has_arrow<T,
            typename VoidType<decltype(std::declval<T>().operator->())>::type> :
        std::true_type{};

/** @brief Checks if a type implements the prefix increment operator
 *
 * @tparam T The type to check
 */
template<typename T, typename = void>
struct has_prefix_increment : std::false_type {};
template<typename T>
struct has_prefix_increment<T,
     typename VoidType<decltype(++std::declval<T>())>::type> : std::true_type{};

/** @brief Checks if a type implements the postfix increment operator
 *
 * @tparam T The type to check
 */
template<typename T, typename = void>
struct has_postfix_increment : std::false_type {};
template<typename T>
struct has_postfix_increment<T,
     typename VoidType<decltype(std::declval<T>()++)>::type> : std::true_type{};

/** @brief Checks if a type implements the prefix decrement operator
 *
 * @tparam T The type to check
 */
template<typename T, typename = void>
struct has_prefix_decrement : std::false_type {};
template<typename T>
struct has_prefix_decrement<T,
     typename VoidType<decltype(--std::declval<T>())>::type> : std::true_type{};

/** @brief Checks if a type implements the postfix decrement operator
 *
 * @tparam T The type to check
 */
template<typename T, typename = void>
struct has_postfix_decrement : std::false_type {};
template<typename T>
struct has_postfix_decrement<T,
     typename VoidType<decltype(std::declval<T>()--)>::type> : std::true_type{};

/** @brief Checks if a type implements the index operator
 *
 * @tparam T The type to check
 */
template<typename T, typename = void>
struct is_indexable : std::false_type {};
template<typename T>
struct is_indexable<T,typename VoidType<decltype(std::declval<T>()[0])>::type>
: std::true_type{};

#undef HAS_OPERATOR

/** @brief This struct will contain a value true if the type satisfies the C++
 *         concept of "Container"
 *
 *  @warning This type will not check if the type is swappable.  A type_trait,
 *           is_swappable exists in C++17 for this purpose.  Implementing our
 *           own is a royal pain on account of how the STL actually does
 *           swapping...
 *
 *  The full specificiation of what a "container" must have is available
 *  < a href="http://en.cppreference.com/w/cpp/concept/Container">here</a>.
 */
template<typename T>
struct is_container : Conjunction<has_type_value_type<T>,
                                 has_type_reference<T>,
                                 has_type_const_reference<T>,
                                 has_type_iterator<T>,
                                 has_type_const_iterator<T>,
                                 has_type_difference_type<T>,
                                 has_type_size_type<T>,
                                 std::is_default_constructible<T>,
                                 std::is_copy_constructible<T>,
                                 std::is_assignable<T,T>,
                                 std::is_destructible<T>,
                                 has_begin<T>,
                                 has_end<T>,
                                 has_cbegin<T>,
                                 has_cend<T>,
                                 has_equal_to<T,T>,
                                 has_not_equal_to<T,T>,
                                 has_size<T>,
                                 has_max_size<T>,
                                 has_empty<T>
                                 >{};

/** @brief This struct will contain a value true if the type satisfies the C++
 *         concept of "Iterator"
 *
 *  @warning This type will not check if the type is swappable.  A type_trait,
 *           is_swappable exists in C++17 for this purpose.  Implementing our
 *           own is a royal pain on account of how the STL actually does
 *           swapping...
 *
 *  The full specificiation of what an "iterator" must have is available
 *  < a href="http://en.cppreference.com/w/cpp/concept/iterator">here</a>.
 */
template<typename T>
struct is_iterator : Conjunction<has_type_value_type<T>,
                                 has_type_difference_type<T>,
                                 has_type_reference<T>,
                                 has_type_pointer<T>,
                                 has_type_iterator_category<T>,
                                 has_dereference<T,void>,
                                 has_prefix_increment<T,void>,
                                 std::is_copy_constructible<T>,
                                 std::is_assignable<T,T>,
                                 std::is_destructible<T>
                                 >{};

/** @brief This struct will contain a value true if the type satisfies the C++
 *         concept of "InputIterator"
 *
 *  @warning This type will not check if the type is swappable.  A type_trait,
 *           is_swappable exists in C++17 for this purpose.  Implementing our
 *           own is a royal pain on account of how the STL actually does
 *           swapping...
 *
 *  The full specificiation of what an "input iterator" must have is available
 *  < a href="http://en.cppreference.com/w/cpp/concept/InputIterator">here</a>.
 */
template<typename T>
struct is_input_iterator : Conjunction<is_iterator<T>,
                                       has_equal_to<T,T>,
                                       has_not_equal_to<T,T>,
                                       has_postfix_increment<T>,
                                       has_arrow<T>
                                      >{};

/** @brief This struct will contain a value true if the type satisfies the C++
 *         concept of "ForwardIterator"
 *
 *  @note A forward iterator is just an input iterator with a multipass
 *        guarantee.  We currently are not checking for this hence this struct
 *        is only a typedef for clarity.
 *
 *  @warning This type will not check if the type is swappable.  A type_trait,
 *           is_swappable exists in C++17 for this purpose.  Implementing our
 *           own is a royal pain on account of how the STL actually does
 *           swapping...
 *
 *  The full specificiation of what an "forward iterator" must have is available
 *  < a href="http://en.cppreference.com/w/cpp/concept/ForwardIterator">here</a>.
 */
template<typename T>
using is_forward_iterator = is_input_iterator<T>;


/** @brief This struct will contain a value true if the type satisfies the C++
 *         concept of "BidirectionalIterator"
 *
 *  @warning This type will not check if the type is swappable.  A type_trait,
 *           is_swappable exists in C++17 for this purpose.  Implementing our
 *           own is a royal pain on account of how the STL actually does
 *           swapping...
 *
 *  The full specificiation of what an "bidirectional iterator" must have is
 *  available
 *  < a href="http://en.cppreference.com/w/cpp/concept/BidirectionalIterator">here</a>.
 */
template<typename T>
struct is_bidirectional_iterator : Conjunction<is_forward_iterator<T>,
                                               has_prefix_decrement<T>,
                                               has_postfix_decrement<T>
>{};

/** @brief This struct will contain a value true if the type satisfies the C++
 *         concept of "RandomAccessIterator"
 *
 *  @warning This type will not check if the type is swappable.  A type_trait,
 *           is_swappable exists in C++17 for this purpose.  Implementing our
 *           own is a royal pain on account of how the STL actually does
 *           swapping...
 *
 *  The full specificiation of what an "random access iterator" must have is
 *  available
 *  < a href="http://en.cppreference.com/w/cpp/concept/RandomAccessIterator">here</a>.
 */
template<typename T, typename diff_type=long int>
struct is_random_access_iterator : Conjunction<is_bidirectional_iterator<T>,
                                               has_increment_by<T,diff_type>,
                                               has_decrement_by<T,diff_type>,
                                               has_plus<T,diff_type>,
                                               has_minus<T,diff_type>,
                                               has_minus<T,T>,
                                               is_indexable<T>,
                                               has_less_than<T,T>,
                                               has_greater_than<T,T>,
                                               has_less_than_equal<T,T>,
                                               has_greater_than_equal<T,T>
>{};

}//End namespace
