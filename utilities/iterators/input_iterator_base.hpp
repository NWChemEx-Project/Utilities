#pragma once
#include <iterator>

namespace utilities::iterators {

/** @brief This class is designed to facilitate making your own input iterator
 *  class.
 *
 *  This class works off of CRTP. Let's say your trying to write iterators for
 *  the container `MyContainer<T>` (where `T` is the type of elements stored in
 *  your container). A reasonable name for your iterator is then something like
 *  `MyContainerIterator<U>`. You'll then define two typedefs in your class:
 *  `using iterator = MyContainerIterator<T>` and
 *  `using const_iterator = MyContainerIterator<const T>`, which respectively
 *  are read-write and read-only iterators. Note they are only typedefs, you
 *  only need to define one class `MyContainerIterator<U>`, which will
 *  inherit from `InputIteratorBase<MyContainerIterator<U>, U>` and will
 *  properly implement the cases where `U=T` and `U=const T` if you implement
 *  the functions:
 *
 *  - increment
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
struct InputIteratorBase {
    /// The type of an element returned by this iterator.
    using value_type = ValueType;

    /// The type of a mutable reference to an element
    using reference = value_type&;

    /// The type of a non-mutable reference to an element
    using const_reference = const value_type&;

    /// The type of a pointer to an element
    using pointer = value_type*;

    /// The type of a pointer to a non-mutable element
    using const_pointer = const value_type*;

    /// The type of the index
    using size_type = SizeType;

    /// The type of the difference between two indices
    using difference_type = DifferenceType;

    /// The concept tag this iterator obeys
    using iterator_category = std::input_iterator_tag;

    /// Implement this so we can increment your iterator
    virtual ParentType& increment() = 0;

    /// Implement this so we can dereference your iterator
    virtual reference dereference() = 0;

    /// Implement to provide equality comparisons
    virtual bool are_equal(const ParentType& rhs) const noexcept = 0;

    /** @brief Allows access to the element currently pointed at by this
     *  iterator.
     *
     *  This function simply calls dereference.
     *
     *  @return The element this iterator currently points to.
     *  @throws exception if dereference throws.
     */
    const_reference operator*() const { return dereference(); }

    ///@copydoc operator*()const
    reference operator*() { return dereference(); }

    /** @brief Provides access to an element's member functions directly.
     *
     * @returns The address of the current element for use with the arrow
     * operator.
     * @throws exception if the dereference operation throws.
     */
    const_pointer operator->() const { return &(operator*()); }

    /** @brief Provides access to an element's member functions directly.
     *
     * If the user specified the type of the element to be const then this
     * ultimately will still return a const pointer
     *
     * @returns The address of the current element for use with the arrow
     * operator.
     * @throws exception if the dereference operation throws.
     */
    pointer operator->() { return &(operator*()); }

    /** @brief Implements prefix increment.
     *
     *  Increments an iterator before returning the current value.  This
     *  operation is implemented by calling increment.
     *
     *  @returns The iterator pointing to the value resulting from the
     * increment.
     *  @throws exception if increment throws
     *
     */
    ParentType& operator++() { return increment(); }

    /** @brief Implements postfix increment.
     *
     *  Increments an iterator after returning the current value.  This
     *  operation is implemented in terms of the copy constructor and the prefix
     *  increment operator.
     *
     *  @returns A copy of the current iterator pointing to the element before
     *          the increment.
     *  @throws exception if either the copy constructor or the prefix increment
     *  operator throw.
     *
     */
    ParentType operator++(int) {
        ParentType copy_of_me(static_cast<ParentType&>(*this));
        ++(*this);
        return copy_of_me;
    }

    /** @brief Determines if two iterators are equivalent.
     *
     * This function works by calling are_equal.  Hence the definition of
     * equality used is that of the are_equal function.
     *
     * @param[in] rhs The iterator to compare to.
     * @return true if the two iterators are equal
     * @throws None. No throw guarantee
     */
    bool operator==(const ParentType& rhs) const noexcept {
        return are_equal(rhs);
    }

    /** @brief Check to ensure two iterators are not identical.
     *
     *  The definition of iterator equality is taken from operator==.  This
     *  function
     *  simply negates the result.
     *
     *  @param[in] rhs The iterator to compare against.
     *  @returns True if the current iterator is not identical to @p rhs.
     *  @throws None. No throw guarantee.
     */
    bool operator!=(const ParentType& rhs) const noexcept {
        return !((*this) == rhs);
    }
};

} //namespace utilities::iterators
