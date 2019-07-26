#pragma once
#include "utilities/containers/detail_/nested_set_pimpl.hpp"
#include "utilities/containers/detail_/set_pimpl.hpp"
#include <type_traits>

namespace utilities {

/// Forward declare MathSet for template meta-programming purposes
template<typename T>
class MathSet;

namespace detail_ {

/** @brief Collects all of the template meta-programming for the MathSet class
 *
 *  The MathSet class relies on a decent amount of template meta-programming in
 *  order to function. The MathSetTraits class collects that programming into
 *  one place and makes the MathSet class easier to read.
 *
 *  @tparam ElementType The type provided to the MathSet.
 */
template<typename ElementType>
struct MathSetTraits {
    /// Helper class for determining if @p T is a MathSet
    template<typename T>
    struct IsMathSet : std::false_type {};

    /// Specializes IsMathSet for when @p T is a MathSet
    template<typename T>
    struct IsMathSet<MathSet<T>> : std::true_type {};

    using is_math_set_t = IsMathSet<ElementType>;

    /// Convenience variable for determining if @p T is a MathSet
    static constexpr bool is_math_set_v = is_math_set_t::value;

    /// Convenience variable for determining if @p T is not a MathSet
    static constexpr bool is_not_math_set_v = std::negation_v<is_math_set_t>;

    /// Type of elements actually stored in the PIMPL
    using value_type = ElementType;

    /// Type of const references returned by the PIMPL

    /// Type of the normal PIMPL
    using normal_pimpl = detail_::SetPIMPL<value_type>;

    /// Triggers if @p ElementType is a nested MathSet
    template<typename T>
    using if_nested =
      std::conditional_t<is_math_set_v, detail_::NestedSetPIMPL<value_type>, T>;

    /// Type of the default, non-view PIMPL for ElementType
    using default_pimpl = if_nested<normal_pimpl>;

    /// For convenience the type of the base class of all PIMPLs
    using pimpl_base = detail_::MathSetPIMPL<value_type>;

    /// Type of a reference to an element in the PIMPL
    using reference = typename default_pimpl::reference;

    /// Type of a read-only reference to an element in the PIMPL
    using const_reference = typename default_pimpl::const_reference;

    /// Type of an iterator over the elements in the PIMPL
    using iterator = typename default_pimpl::iterator;

    /// Type of an iterator that returns read-only elements in the PIMPL
    using const_iterator = typename default_pimpl::const_iterator;

    /// Type used for indexing/offsets with the PIPML
    using size_type = typename default_pimpl::size_type;
};

} // namespace detail_
} // namespace utilities
