#pragma once
#include "utilities/iterators/bidirectional_iterator_base.hpp"

namespace utilities::iterators {

/** @brief This class is designed to facilitate making your own random access
 *  iterator class.
 *
 *  When making your own iterator there's a lot of boiler plate involved.  This
 *  class factors out as much boiler plate as possible for a bidirectional
 *  iterator, which is an iterator that allows you to access either the next
 *  element or the previous element.
 *
 *  To use this class you'll need to implement in your derived class:
 *  - dereference
 *  - increment
 *  - are_equal
 *  - decrement
 *  - advance
 *  - distance_to
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
template<typename ParentType, typename ValueType,
         typename SizeType = std::size_t, typename DifferenceType = long int>
struct RandomAccessIteratorBase
  : public BidirectionalIteratorBase<ParentType, ValueType, SizeType,
                                     DifferenceType> {
    using iterator_category = std::random_access_iterator_tag;

    /// Implement to provide advance
    virtual ParentType& advance(DifferenceType n) = 0;

    /// Implement to provide ordering
    virtual DifferenceType distance_to(const ParentType& rhs) const = 0;

    /** @brief Provides random access to any element in the container relative
     *  to the element currently pointed to by this iterator.
     *
     *  This function ultimately works by calling operator+
     *
     *  @param[in] n The desired number of iterations away.
     *  @return The element that is @p n iterations away
     *  @throws exception if operator + throws
     */
    ValueType operator[](DifferenceType n) const { return *((*this) + n); }

    /** @brief Compares two iterators and returns true if the current iterator
     *  points to an element appearing earlier in the container
     *
     *  This function ultimately calls distance_to and then uses the fact
     *  that a positive distance means that @p rhs points to an element
     *  further in the sequence.
     *
     *  @param[in] rhs The iterator to compare to.
     *  @returns True if the current iterator points to an element indexed
     *  earlier then the element pointed to by @p rhs.
     *  @throws None. No throw guarantee.
     */
    bool operator<(const ParentType& rhs) const noexcept {
        return distance_to(rhs) > 0;
    }

    /** @brief Compares two iterators and returns true if the current iterator
     *  points to an element appearing earlier in the container or if the two
     *  iterators point to the same element.
     *
     *  This function ultimately works by checking the less than operator and
     *  the equality operator.  Hence the definitions of less than and equal to
     *  are set by those functions.
     *
     *  @param[in] rhs The iterator to compare to.
     *  @returns True if the current iterator points to an element indexed
     *  earlier than or the same as the element pointed to by @p rhs.
     *  @throws None. No throw guarantee.
     */
    bool operator<=(const ParentType& rhs) const noexcept {
        return (*this) < rhs || (*this) == rhs;
    }

    /** @brief Compares two iterators and returns true if this iterator points
     *  to an element later in the container than the rhs iterator.
     *
     *  This function ultimately checks if @p rhs is less than the current
     *  iterator and thus uses the same concept of order.
     *
     *  @param[in] rhs The iterator to compare to.
     *  @returns True if the current element is indexed after that of @p rhs.
     *  @throws None. No throw guarantee.
     */
    bool operator>(const ParentType& rhs) const noexcept {
        return rhs < static_cast<const ParentType&>(*this);
    }

    /** @brief Compares two iterators and returns true if they point to the
     *  same element or the element pointed to by this iterator occurs later in
     *  the sequence than that of the rhs iterator.
     *
     *  This function ultimately checks if @p rhs is less than or equal to
     *  the current iterator and hence uses the same definitions of equality
     *  and order.
     *
     *  @param[in] rhs The iterator to compare to.
     *  @returns true if the current iterator points to the same element or one
     *  later in the sequence than @p rhs.
     *  @throws None. No throw guarantee.
     */
    bool operator>=(const ParentType& rhs) const noexcept {
        return rhs <= static_cast<const ParentType&>(*this);
    }

    /** @brief Advances the current iterator a specified number of iterations.
     *
     *  This function ultimately relies on advance.
     *
     *  @param[in] n The number of iterations to move forward by.  If @p n is
     *  negative then the iterator will actually move backward by @p n.
     *  @returns The iterator advanced @p n elements.
     *  @throws exception if advance throws.
     */
    ParentType& operator+=(DifferenceType n) { return advance(n); }

    /** @brief Creates a copy of the current iterator that points to the element
     *  a specified number of iterations away.
     *
     *  This function ultimately relies on operator+= and the copy constructor.
     *
     *  @param[in] n The number of iterations to move forward by.  If @p n is
     *  negative then the iterator will actually move backward by @p n.
     *  @returns A copy of the current iterator pointing to the element @n
     *  iterations away.
     *  @throws exception if either the copy constructor or operator+= throw.
     */
    ParentType operator+(DifferenceType n) const {
        ParentType copy_of_me(static_cast<const ParentType&>(*this));
        copy_of_me += n;
        return copy_of_me;
    }

    /** @brief Moves the current iterator backwards by the specified number of
     *  iterations.
     *
     *  This function ultimately negates its input and then calls operator+.
     *
     *  @param[in] n The number of iterations to move backward by.  If @p n is
     *  negative then the iterator will actually move forward by @p n.
     *  @returns The current iterator pointing to the element @p n iterations
     *  away.
     *  @throws exception if operator+ throws.
     */
    ParentType& operator-=(DifferenceType n) { return (*this) += (-n); }

    /** @brief Creates a copy of the current iterator that points to the element
     *  a specified number of iterations away.
     *
     *  This function ultimately relies on operator-= and the copy constructor.
     *
     *  @param[in] n The number of iterations to move backward by.  If @p n is
     *  negative then the iterator will actually move forward by @p n.
     *  @returns A copy of the current iterator pointing to the element @n
     *  iterations away.
     *  @throws exception if either the copy constructor or operator-= throw.
     */
    ParentType operator-(DifferenceType n) const {
        ParentType copy_of_me(static_cast<const ParentType&>(*this));
        copy_of_me -= n;
        return copy_of_me;
    }

    /** @brief Returns the distance between two iterators
     *
     * This ultimately works by calling distance_to.
     *
     * @param[in] rhs The iterator to compare to.
     * @returns The distance between this and rhs.
     * @throws exception if distance_to throws.
     */
    DifferenceType operator-(const ParentType& rhs) const {
        return distance_to(rhs);
    }
};

/**
 * Template meta-programming to select the correct base type given the iterator
 * tag type.
 */
///@{
template<typename ParentType, typename ValueType, typename iterator_type>
struct Iterator2Base {};

template<typename ParentType, typename ValueType>
struct Iterator2Base<ParentType, ValueType, std::input_iterator_tag> {
    using type = InputIteratorBase<ParentType, ValueType>;
};

template<typename ParentType, typename ValueType>
struct Iterator2Base<ParentType, ValueType, std::bidirectional_iterator_tag> {
    using type = BidirectionalIteratorBase<ParentType, ValueType>;
};

template<typename ParentType, typename ValueType>
struct Iterator2Base<ParentType, ValueType, std::random_access_iterator_tag> {
    using type = RandomAccessIteratorBase<ParentType, ValueType>;
};
///@}

} // namespace utilities::iterators
