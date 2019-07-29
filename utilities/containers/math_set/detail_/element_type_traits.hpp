#pragma once

namespace utilities::detail_ {

/** @brief Structure for collecting typedefs related to the MathSet class
 *
 *  This class collects the more basic typedefs for the MathSet class family.
 *  It can be specialized, if need be, for particular element types.
 *
 *  @tparam ElementType The type of the element in the MathSet instance.
 */
template<typename ElementType>
struct ElementTypeTraits {
    /// Type of an element in the set
    using value_type = ElementType;

    /// Type of a read-write reference to an element in the set
    using reference = value_type&;

    /// Type of the a read-only reference to an element in the set
    using const_reference = const value_type&;

    /// Type of an index or offset
    using size_type = std::size_t;
};

} // namespace utilities::detail_
