#pragma once
#include <type_traits>

namespace utilities {

/// Forward declare MathSet for template meta-programming purposes
template<typename T>
class MathSet;

namespace detail_ {

template<typename T>
class NestedSetPIMPL;
template<typename T>
class SetPIMPL;

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

    /// Type of IsMathSet for this type_trait
    using is_math_set_t = IsMathSet<ElementType>;

    /// Convenience variable for whether ElementType is a MathSet
    static constexpr bool is_math_set_v = is_math_set_t::value;

    /// Type of the normal PIMPL
    using normal_pimpl = detail_::SetPIMPL<ElementType>;

    /// Triggers if @p ElementType is a nested MathSet
    template<typename T>
    using if_nested =
      std::conditional_t<is_math_set_v, detail_::NestedSetPIMPL<ElementType>,
                         T>;

    /// Type of the default, non-view PIMPL for ElementType
    using default_pimpl = if_nested<normal_pimpl>;
};

} // namespace detail_
} // namespace utilities
