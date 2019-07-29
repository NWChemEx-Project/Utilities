#pragma once
#include "utilities/iterators/input_iterator_base.hpp"

namespace utilities::iterators {

/** @brief This class is designed to facilitate making your own bidirectional
 *  iterator class.
 *
 *  When making your own iterator there's a lot of boiler plate involved.  This
 *  class factors out as much boiler plate as possible for a bidirectional
 *  iterator, which is an iterator that allows you to access either the next
 *  element or the previous element.
 *
 *  To use this class you'll need to implement in your derived class:
 *  - dereference
 *  - are_equal
 *  - increment
 *  - decrement
 *
 *  @tparam ParentType The type of iterator that you are making.  This class
 *          works off CRTP so it needs to know the most derived class.
 *  @tparam ValueType The type of the elements you are storing in your
 *          container.
 *  @tparam SizeType The type of an index into your container. Default:
            std::size_t.
 *  @tparam DifferenceType The type of the difference between two SizeType
 *          instances.  Default: long int.
 */
template<typename ParentType,
         typename ValueType = typename ParentType::value_type,
         typename SizeType = std::size_t, typename DifferenceType = long int>
struct BidirectionalIteratorBase
  : public InputIteratorBase<ParentType, ValueType, SizeType, DifferenceType> {
    using iterator_category = std::bidirectional_iterator_tag;

    /// Implement to provide decrement functionality
    virtual ParentType& decrement() = 0;

    /** @brief Decrements the current iterator before returning the value.
     *
     *  This operator relies on decrement for its functionality.
     *
     *  @returns The current iterator after decrementing it.
     *  @throws exception if decrement throws.
     */
    ParentType& operator--() { return decrement(); }

    /** @brief Decrements the current iterator after returning the value.
     *
     *  Like the postfix increment operator this operator relies on the copy
     *  constructor and the prefix decrement operator.
     *
     *  @returns A copy of the current iterator before decrementing it.
     *  @throws exception if either the copy constructor or prefix decrement
     *  operator throws.
     */
    ParentType operator--(int) {
        ParentType& up_me = static_cast<ParentType&>(*this);
        ParentType copy_of_me(up_me);
        --(*this);
        return copy_of_me;
    }
};

} // namespace utilities::iterators
