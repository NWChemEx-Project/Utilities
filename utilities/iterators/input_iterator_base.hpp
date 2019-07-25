#pragma once
#include <iterator>

namespace utilities::iterators {

/** @brief This class is designed to facilitate making your own input iterator
 *  class.
 *
 *  To use this class to quickly define an input iterator you'll need to define:
 *  - increment
 *  - dereference
 *  - are_equal
 *
 *  @tparam ParentType The type of the iterator we are implementing.
 *  @tparam ValueType The type of the elements you are storing in your
 *          container.
 *  @tparam SizeType The type of an index into your container. Default:
 *          std::size_t.
 *  @tparam DifferenceType The type of the difference between two SizeType
 *          instances.  Default: long int.
 */
template<typename ParentType,
         typename ValueType = typename ParentType::value_type,
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

    /** @brief Implemented by the derived class to retrieve the element of the
     *         container currently being pointed to.
     *
     *  It is only necessary for the derived class to implement the const
     *  version of this function. The reason is that if the class is iterating
     *  over a container in a const fashion this is what we want anyways. If
     *  on the other hand we want the non-reference version we can const_cast
     *  the return, knowing that the state of the container respected const. If
     *  the caller didn't want the elements to always be const then `value_type`
     *  is `const T` and reference is `const T&` so that when we const cast we
     *  don't actually remove the const.
     *
     *  @return A reference to the element currently being pointed at.
     *
     *  @throw ??? If the implementation in the derived class throws. Same throw
     *             guarantee.
     */
    virtual const_reference dereference() const = 0;

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
    reference operator*() {
        const_reference temp = dereference();
        return const_cast<reference>(temp); // NOLINT
    }

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

} // namespace utilities::iterators
