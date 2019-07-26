#pragma once
#include "utilities/iterators/offset_iterator.hpp"
#include <memory>
namespace utilities::detail_ {

template<typename ElementType>
class MathSetPIMPL {
private:
    using my_type = MathSetPIMPL<ElementType>;

public:
    using value_type      = ElementType;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using size_type       = std::size_t;
    using iterator        = iterators::OffsetIterator<my_type>;
    using const_iterator  = iterators::OffsetIterator<const my_type>;

    virtual ~MathSetPIMPL() = default;

    reference operator[](size_type i) { return const_cast<reference>(get_(i)); }
    const_reference operator[](size_type i) const { return get_(i); }

    /** @brief Adds an element to the set.
     *
     *  This function will first check if the provided element is in the set.
     *  If it is the element will not be added again. If it is not then the
     *  element is added and the size of the set increases by one.
     *
     *  @param[in] elem The element to add to the set.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the element
     *                        to the set. Strong throw guarantee.
     */
    void insert(value_type elem) { insert(end(), std::move(elem)); }

    void insert(iterator off, value_type el) { insert_(off, std::move(el)); }

    /** @brief Computes the number of elements in this set.
     *
     *  This function returns the number of elements in the set. It should be
     *  noted that the set only contains unique elements, thus the resulting
     *  value is the number of unique elements that have been inserted into the
     *  set.
     *
     *  @return The number of elements currently stored in the set.
     *
     *  @throw none No throw guarantee.
     */
    size_type size() const noexcept { return size_(); }

    /** @brief Determines whether an element appears in the set or not.
     *
     *  This function is used to determine the number of times an element
     *  appears in the set. Since an element can only appear at most once. The
     *  returns safely map to boolean values of `false` when the element is not
     *  present and `true` when it is.
     *
     *  @param[in] el The element whose inclusion in the current set is in
     *                  question.
     *  @return The number of times (either 0 or 1) @p el appears in the set.
     *
     *  @throw none No throw guarantee.
     */
    size_type count(const_reference elem) const noexcept;

    /** @brief Returns an iterator that points to the first element of the set.
     *
     *
     *  This function returns an iterator that is pointing at the first element
     *  of the container. If the set is empty the resulting iterator is the
     *  same as the iterator returned by `end`. The iterator resulting from this
     *  function can be used to modify the contents of the set.
     *
     *  @return An iterator pointing to the first element of the set.
     *
     *  @throw none No throw guarantee.
     */
    iterator begin() noexcept { return iterator(0, this); }

    /** @brief Returns an iterator that points to the first element of the set.
     *
     *
     *  This function returns an iterator that is pointing at the first element
     *  of the container. If the set is empty the resulting iterator is the
     *  same as the iterator returned by `end`. The iterator resulting from this
     *  function can only be used to read the contents of the set and not to
     *  modify it.
     *
     *  @return An iterator pointing to the first element of the set.
     *
     *  @throw none No throw guarantee.
     */
    const_iterator begin() const noexcept { return const_iterator(0, this); }

    /** @brief Returns an iterator just past the last element of the set.
     *
     *  This function returns an iterator that is just past the last element of
     *  the set. Iteration over the set is then possible by incrementing the
     *  iterator returned by begin until it equals the iterator returned by this
     *  function. Attempting to access the value pointed to by this iterator is
     *  undefined behavior.
     *
     *  @return An iterator just past the last element in the set.
     *
     *  @throw none No throw guarantee.
     */
    iterator end() noexcept { return iterator(size(), this); }

    /** @brief Returns an iterator just past the last element of the set.
     *
     *  This function returns an iterator that is just past the last element of
     *  the set. Iteration over the set is then possible by incrementing the
     *  iterator returned by begin until it equals the iterator returned by this
     *  function. Attempting to access the value pointed to by this iterator is
     *  undefined behavior.
     *
     *  @return An iterator just past the last element in the set.
     *
     *  @throw none No throw guarantee.
     */
    const_iterator end() const noexcept { return const_iterator(size(), this); }

private:
    virtual const_reference get_(size_type i) const        = 0;
    virtual void insert_(iterator offset, value_type elem) = 0;
    virtual size_type size_() const noexcept               = 0;
};

/** @brief Compares two SetPIMPL instances for equality
 *  @related SetPIMPL
 *
 *  Two SetPIMPL instances are equal if they have the same number of elements
 *  and if they contain the same elements, in the same order. It must be
 *  possible to implicitly compare elements from @p lhs with elements from
 *  @p rhs using `operator==`.
 *
 * @tparam T The type of the elements in @p lhs
 * @tparam U The type of the elements in @p rhs.
 *
 * @param[in] lhs The SetPIMPL on the left side of the comparison.
 * @param[in] rhs The SetPIMPL on the right side of the comparison.
 *
 * @return True if the two PIMPL instances hold equivalent sets and false
 *         otherwise.
 *
 * @throw none No throw guarantee.
 */
template<typename T, typename U>
bool operator==(const MathSetPIMPL<T>& lhs,
                const MathSetPIMPL<U>& rhs) noexcept {
    if(lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/** @brief Determines if two SetPIMPL instances are different.
 *  @related SetPIMPL
 *
 *  Two SetPIMPL instances are equal if they have the same number of elements
 *  and if they contain the same elements, in the same order. It must be
 *  possible to implicitly compare elements from @p lhs with elements from
 *  @p rhs using `operator==`.
 *
 * @tparam T The type of the elements in @p lhs
 * @tparam U The type of the elements in @p rhs.
 *
 * @param[in] lhs The SetPIMPL on the left side of the comparison.
 * @param[in] rhs The SetPIMPL on the right side of the comparison.
 *
 * @return False if the two PIMPL instances hold equivalent sets and true
 *         otherwise.
 *
 * @throw none No throw guarantee.
 */
template<typename T, typename U>
bool operator!=(const MathSetPIMPL<T>& lhs,
                const MathSetPIMPL<U>& rhs) noexcept {
    return !(lhs == rhs);
}

//------------------------Implementations---------------------------------------
#define MATH_SET_PIMPL_TYPE MathSetPIMPL<ElementType>

template<typename ElementType>
typename MATH_SET_PIMPL_TYPE::size_type MATH_SET_PIMPL_TYPE::count(
  const_reference elem) const noexcept {
    return std::count(begin(), end(), elem);
}

#undef MATH_SET_PIMPL_TYPE
} // namespace utilities::detail_
