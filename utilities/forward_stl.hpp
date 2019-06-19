#pragma once

/** @file forward_stl.hpp
 *
 *  This header file forward declares the classes in the STL. It is useful for
 *  when you are designing classes/functions that should work with all of the
 *  STL, but you don't want to force a user to include all of the STL. With this
 *  file you can define classes/functions that use all of the STL and the user
 *  only needs to include the STL header files for the classes they actually
 *  use.
 */

namespace std {

/// Forward delcare std::array
template<typename T, std::size_t N>
class array;

/// Forward declare std::deque
template<typename T, typename Alloc>
class deque;

/// Forward declare std::forward_list
template<typename T, typename Alloc>
class forward_list;

/// Forward declare std::list
// template<typename T, typename Alloc> class list;

/// Forward declare std::map
template<typename Key, typename T, typename Compare, typename Alloc>
class map;

/// Forward declare std::multimap
template<typename Key, typename T, typename Compare, typename Alloc>
class multimap;

/// Forward declare std::multiset
template<typename T, typename Compare, typename Alloc>
class multiset;

/// Forward declare std::optional
template<typename T>
class optional;

/// Forward declare std::pair
template<typename T1, typename T2>
class pair;

/// Forward declare std::priority_queue
template<typename T, typename Container, typename Compare>
class priority_queue;

/// Forward declare std::queue
template<typename T, typename Container>
class queue;

/// Forward declare std::reference_wrapper
template<typename T>
class reference_wrapper;

/// Forward declare std::set
template<typename T, typename Compare, typename Alloc>
class set;

// Forward declare std::stack
template<typename T, typename Container>
class stack;

// Forward declare std::tuple
template<typename... Types>
class tuple;

// Forward declare std::unordered_map
template<typename Key, typename T, typename Hash, typename Pred, typename Alloc>
class unordered_map;

// Forward declare std::unordered_multimap
template<typename Key, typename T, typename Hash, typename Pred, typename Alloc>
class unordered_multimap;

// Forward declare std::unordered_set
template<typename Key, typename Hash, typename Pred, typename Alloc>
class unordered_multiset;

// Forward declare std::unordered_set
template<typename Key, typename Hash, typename Pred, typename Alloc>
class unordered_set;

// Forward declare std::vector
template<typename T, typename Alloc>
class vector;

} // namespace std
