#pragma once
#include "utilities/iterators/input_iterator_base.hpp"

namespace utilities::iterators {

/** @brief This class is designed to facilitate making your own bidirectional
 *         iterator class.
 *
 *  This class works off of CRTP. Let's say your trying to write iterators for
 *  the container `MyContainer<T>` (where `T` is the type of elements stored in
 *  your container). A reasonable name for your iterator is then something like
 *  `MyContainerIterator<U>`. You'll then define two typedefs in your class:
 *  `using iterator = MyContainerIterator<T>` and
 *  `using const_iterator = MyContainerIterator<const T>`, which respectively
 *  are read-write and read-only iterators. Note they are only typedefs, you
 *  only need to define one class `MyContainerIterator<U>`, which will
 *  inherit from `BidirectionalIteratorBase<MyContainerIterator<U>, U>` and will
 *  properly implement the cases where `U=T` and `U=const T` if you implement
 *  the functions:
 *
 *  - increment
 *  - decrement
 *  - dereference
 *  - are_equal
 *
 *  @tparam ParentType The type of your iterator (including its template
 *                     parameters).
 *  @tparam ValueType The type of the elements you are storing in your
 *                    container.
 *  @tparam SizeType The type of an index into your container. Default:
 *          std::size_t.
 *  @tparam DifferenceType The type of the difference between two SizeType
 *          instances.  Default: long int.
 */
template<typename ParentType, typename ValueType,
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

} //namespace utilities::iterators
