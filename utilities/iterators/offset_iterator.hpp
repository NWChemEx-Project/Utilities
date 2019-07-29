#pragma once
#include "utilities/iterators/random_access_iterator_base.hpp"
#include "utilities/mathematician/integer_utils.hpp"

namespace utilities::iterators {
namespace detail_ {

/// Typedef that works out the cv-qualifiers for the values in an OffsetItr
template<typename T>
using offset_itr_value_t =
  std::conditional_t<std::is_const_v<T>, const typename T::value_type,
                     typename T::value_type>;

} // namespace detail_

/** @brief Implements a random access iterator given a random access container
 *         that defines `operator[] const`.
 *
 *  When writing a custom container it is often the case that we know how to get
 *  the `i`-th element from our container, but retrieving that element requires
 *  some indirection. Because of the indirection we typically can not use an
 *  underlying container's iterators as our class's iterators. In other words,
 *  our elements are not stored in, say an `std::vector`, so we can't simply
 *  call `std::vector::begin` to get an iterator to our class.
 *
 *  The current class is code factorization for this common scenario. Given a
 *  pointer to the class to be iterated over and the starting index, the current
 *  class implements a random access iterator. The resulting iterator works by
 *  advancing the internal index and then calling the parent container's
 *  `operator[](size_type) const` operator to retrieve the element currently
 *  being pointed at.
 *
 *  Instances of OffsetIterator are only valid as long as the wrapped container
 *  is valid. Since OffsetIterator only holds an offset, OffsetIterator
 *  instances remain valid even if the underlying container is modified;
 *  however, modification of the underlying container may invalidate references
 *  returned by the current instance and the current instance may no longer
 *  point to the same (or even a valid) element.
 *
 *  @tparam ContainerType The cv-qualified type of the container that we are
 *                        implementing the iterator for. It must define
 *                        `operator[](size_type) const` and have a typedef
 *                        `value_type` of the unqualified type of the elements.
 *                        The same cv qualifications present on `ContainerType`
 *                        will be applied to reference and pointer types of the
 *                        resulting iterator.
 */
template<typename ContainerType>
class OffsetIterator : public RandomAccessIteratorBase<
                         OffsetIterator<ContainerType>,
                         detail_::offset_itr_value_t<ContainerType>> {
private:
    /// Typedef of this class's type to make things more readable
    using my_type = OffsetIterator<ContainerType>;

    /// Typedef of the base class's type to make things more readable
    using base_type =
      RandomAccessIteratorBase<my_type,
                               detail_::offset_itr_value_t<ContainerType>>;

public:
    /// Pull in the type of a constant reference from the base class
    using const_reference = typename base_type::const_reference;

    /// Pull in the type used for indexing/offsets from the base class
    using size_type = typename base_type::size_type;

    /// Pull in the type of a signed offset
    using difference_type = typename base_type::difference_type;

    /** @brief Creates a new OffsetIterator instance that wraps the provided
     *         container and initially points at the element with the provided
     *         index.
     *
     *  This ctor creates a new OffsetIterator instance that is usable as an
     *  iterator for the container instance pointed to by @p parent. The
     *  resulting iterator points to the element in @p parent with index
     *  @p offset. No checks are performed on either @p parent (namely whether
     *  it is a valid pointer) or on @p offset (notably whether @p offset is in
     *  range of @p parent). The resulting iterator is only valid so long as
     *  @p parent is valid. Modifying @p parent may invalidate references
     *  obtained through this iterator and may cause this iterator to point to
     *  an invalid element.
     *
     * @param[in] offset The index of the element in @p parent that this
     *                   iterator will initially point at.
     * @param[in] parent The container this iterator runs over.
     *
     * @throw none No throw guarantee.
     */
    OffsetIterator(size_type offset, ContainerType* parent) noexcept;

    /** @brief Retrieves the element the iterator is currently pointing at.
     *
     *  This function calls the wrapped container's `operator[] const` function
     *  with the offset currently stored in the iterator.
     *
     *  @return A read-only reference to the element currently being pointed at.
     *
     *  @throw ??? If the wrapped container's `operator[] const` throws. Same
     *             guarantee as the wrapped container's `operator[] const`.
     */
    const_reference dereference() const override;

    /** @brief Advances the element currently being pointed at by 1
     *
     *  This function is what is ultimately called when you call `operator++` or
     *  `operator++(int)` on the resulting iterator. It works by advancing the
     *  internal offset by one. No bounds checks are performed on either the
     *  initial offset (namely whether it can be incremented) or on the
     *  resulting value (namely whether it is off the end of the container).
     *
     *  @return The current iterator with the internal counter incremented by
     *           one.
     *
     *  @throw none No throw guarantee.
     *
     */
    my_type& increment() noexcept override {
        ++m_offset_;
        return *this;
    }

    /** @brief Decreases the element currently being pointed at by 1
     *
     *  This function is what is ultimately called when you call `operator--` or
     *  `operator--(int)` on the resulting iterator. It works by decreasing the
     *  internal offset by one. No bounds checks are performed on either the
     *  initial offset (namely whether it can be decreased) or on the
     *  resulting value (namely whether it is off the end of the container).
     *
     *  @return The current iterator with the internal counter decreased by
     *           one.
     *
     *  @throw none No throw guarantee.
     *
     */
    my_type& decrement() noexcept override {
        --m_offset_;
        return *this;
    }

    /** @brief Compares this iterator to another iterator in order to determine
     *         whether they point to the same element.
     *
     *  Two OffsetIterator instances are equal if they are both iterating over
     *  the same instance (not just equivalent instances) and if they have the
     *  same offset.
     *
     * @param[in] rhs The other iterator to compare to.
     *
     * @return True if this iterator instance equals @p rhs and false otherwise.
     *
     * @throw none No throw guarantee.
     */
    bool are_equal(const my_type& rhs) const noexcept override;

    /** @brief Advances the iterator by @p n increments.
     *
     *  This function can be used to increment or by providing a negative value
     *  decrease the element currently being pointed at.This function performs
     *  no bounds checks. In particular this means that before the advance
     *  occurs this function will **not** ensure that the iterator is pointing
     *  to a valid element and after the advancement will **not** ensure that
     *  the iterator is still in range of the wrapped container.
     *
     *  @param[in] n If positive the number of increments to apply. If negative
     *               how mnay times the offset should be decreased by.
     *
     *  @return The current instance with its internal offset advanced by @p n.
     *
     *  @throw none No throw guarantee.
     */
    my_type& advance(difference_type n) noexcept override;

    /** @brief Computes the distance to another OffsetIterator instance.
     *
     *  This function simply returns the difference between the internal offsets
     *  of the current instance and @p rhs. No check is performed to ensure that
     *  @p rhs points to the same container or that it is even reachable by the
     *  current instance.
     *
     *  @return How many increments, if positive, @p rhs is ahead of this
     *          instance, or, if negative, how many increments this instance is
     *          ahead of @p rhs.
     *
     *  @throw none No throw guarantee.
     */
    difference_type distance_to(const my_type& rhs) const noexcept override;

private:
    /// The index of the element we are currently pointing at
    size_type m_offset_;

    /// The container we are an iterator for.
    ContainerType* m_parent_;
}; // class OffsetIterator

//-----------------------------Implementations----------------------------------
/// Macro to make the type of an OffsetIterator more readable
#define OFFSET_ITERATOR_TYPE OffsetIterator<ContainerType>

template<typename ContainerType>
OFFSET_ITERATOR_TYPE::OffsetIterator(size_type offset,
                                     ContainerType* parent) noexcept :
  m_offset_(offset),
  m_parent_(parent) {}

template<typename ContainerType>
typename OFFSET_ITERATOR_TYPE::const_reference
OFFSET_ITERATOR_TYPE::dereference() const {
    return (*m_parent_)[m_offset_];
}

template<typename ContainerType>
bool OFFSET_ITERATOR_TYPE::are_equal(const my_type& rhs) const noexcept {
    return std::tie(m_parent_, m_offset_) ==
           std::tie(rhs.m_parent_, rhs.m_offset_);
}

template<typename ContainerType>
OFFSET_ITERATOR_TYPE& OFFSET_ITERATOR_TYPE::advance(
  difference_type n) noexcept {
    m_offset_ += n;
    return *this;
}

template<typename ContainerType>
typename OFFSET_ITERATOR_TYPE::difference_type
OFFSET_ITERATOR_TYPE::distance_to(const my_type& rhs) const noexcept {
    return UnsignedSubtract(m_offset_, rhs.m_offset_);
}

#undef OFFSET_ITERATOR_TYPE
} // namespace utilities::iterators
