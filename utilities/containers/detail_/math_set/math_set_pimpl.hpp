#pragma once
#include "utilities/containers/detail_/math_set/element_type_traits.hpp"
#include "utilities/iterators/offset_iterator.hpp"

#include <memory>
namespace utilities::detail_ {

template<typename T>
class SelectionViewPIMPL;

/** @brief Base class common to all PIMPLs used to implement MathSet instances.
 *
 *  Each of the derived MathSetPIMPL instances holds its data in a different
 *  manner. The MathSetPIMPL class defines the API that the MathSet class will
 *  use to interact with the various derived classes.
 *
 * @tparam ElementType The type of the elements in the set. Must satisfy the
 *                     same concepts as the template type parameter of the
 *                     MathSet class.
 */
template<typename ElementType>
class MathSetPIMPL {
private:
    /// Type of this class
    using my_type = MathSetPIMPL<ElementType>;
    /// Type of the type traits for this set's elements
    using elem_traits_t = ElementTypeTraits<ElementType>;

public:
    /// Type of a single element
    using value_type = typename elem_traits_t::value_type;
    /// Type of a read/write reference to an element
    using reference = typename elem_traits_t::reference;
    /// Type of a read-only reference to an element
    using const_reference = typename elem_traits_t::const_reference;
    /// Type used for an index/offset
    using size_type = typename elem_traits_t::size_type;
    /// Read/write iterator over the set
    using iterator = iterators::OffsetIterator<my_type>;
    /// Read-only iterator over the set
    using const_iterator = iterators::OffsetIterator<const my_type>;

    /// Default no-throw ctor
    MathSetPIMPL() noexcept = default;

    // Deleted to avoid slicing, copy/move is done via MathSet class
    //@{
    MathSetPIMPL(const my_type&) = delete;
    MathSetPIMPL(my_type&&)      = delete;
    my_type& operator=(const my_type&) = delete;
    my_type& operator=(my_type&&) = delete;
    //@}

    /// Standard default polymorphic dtor
    virtual ~MathSetPIMPL() = default;

    /** @brief Retrieves the i-th element in the set.
     *
     *  Sets are ordered such that the i-th element is the i-th element that
     *  was inserted into the set. This function can be used to retrieve the
     *  element in a read/write state.
     *
     *  @param[in] i  The index of the element to retrieve. Should be in the
     *                range [0, size()).
     *  @return A read/write reference to the i-th element in the set.
     *
     *  @throw std::out_of_range if @p i is not in the range [0, size()). Strong
     *                           throw guarantee.
     */
    reference operator[](size_type i);

    /** @brief Retrieves the i-th element in the set.
     *
     *  Sets are ordered such that the i-th element is the i-th element that
     *  was inserted into the set. This function can be used to retrieve the
     *  element in a read-only state. This function ultimately calls the derived
     *  class via its `get_` method.
     *
     *  @param[in] i  The index of the element to retrieve. Should be in the
     *                range [0, size()).
     *  @return A read-only reference to the i-th element in the set.
     *
     *  @throw std::out_of_range if @p i is not in the range [0, size()). Strong
     *                           throw guarantee.
     */
    const_reference operator[](size_type i) const;

    /** @brief Adds an element to the set.
     *
     *  This function is a convenience function for calling insert(end(), elem).
     *  It will insert the element onto the end of the set.
     *
     *  @param[in] elem The element to add to the set.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the element
     *                        to the set. Strong throw guarantee.
     */
    void insert(value_type elem);

    /** @brief Adds an element to the set.
     *
     *  This function will add an element to the set before the element pointed
     *  at by @p off. Calling this function will invalidate all references and
     *  iterators to this class and its members. In general insertion will be
     *  fastest if @p off is equal to end(). This function ultimately calls the
     *  derived class via it's `insert_` method.
     *
     *  @param[in] off An iterator pointing at the element which should come
     *                 after @p elem. Setting @p off to end() will cause the
     *                 element to be inserted at the end of the container.
     *  @param[in] elem The element to add to the set.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the element
     *                        to the set. Strong throw guarantee.
     */
    void insert(iterator off, value_type el);

    /// Can be called by other PIMPLs to force insertion
    void no_check_insert(value_type x) { insert_(end(), std::move(x)); }

    /** @brief Computes the number of elements in this set.
     *
     *  This function returns the number of elements in the set. This function
     *  works by ultimately calling the derived class's `size_` method.
     *
     *  @return The number of elements currently stored in the set.
     *
     *  @throw none No throw guarantee.
     */
    size_type size() const noexcept { return size_(); }

    /** @brief Determines whether an element appears in the set or not.
     *
     *  This function is used to determine the number of times an element
     *  appears in the set. This PIMPL makes no attempt to enforce element
     *  uniqueness thus the returned value can theoretically lie anywhere in
     *  the range [0, size()).
     *
     *  @param[in] el The element whose inclusion in the current set is in
     *                question.
     *  @return The number of times @p el appears in the set.
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

    void clear() { return clear_(); }

    void erase(const_reference elem) {
        if(count(elem) > 0) erase_(elem);
    }

    /** @brief Compares MathSetPIMPL instances for equality
     *
     *  Two MathSetPIMPL instances are equal if they have the same number of
     *  elements and if they contain the same elements, in the same order. It
     *  must be possible to implicitly compare elements from this container with
     *  elements from @p rhs using `operator==`.
     *
     * @tparam T The type of the elements in @p rhs
     *
     * @param[in] rhs The MathSetPIMPL on the right side of the comparison.
     *
     * @return True if the two PIMPL instances hold equivalent sets and false
     *         otherwise.
     *
     * @throw none No throw guarantee.
     */
    template<typename T>
    bool operator==(const MathSetPIMPL<T>& rhs) const noexcept;

    /** @brief Determines if two MathSetPIMPL instances are different
     *
     *  Two MathSetPIMPL instances are equal if they have the same number of
     *  elements and if they contain the same elements, in the same order. It
     *  must be possible to implicitly compare elements from this container with
     *  elements from @p rhs using `operator==`.
     *
     * @tparam T The type of the elements in @p rhs
     *
     * @param[in] rhs The MathSetPIMPL on the right side of the comparison.
     *
     * @return False if the two PIMPL instances hold equivalent sets and false
     *         otherwise.
     *
     * @throw none No throw guarantee.
     */
    template<typename T>
    bool operator!=(const MathSetPIMPL<T>& rhs) const noexcept;

private:
    /// Code factorization for ensuring index @p i is in bounds
    void check_index_(size_type i) const;

    /// To be overridden by derived class to implement getting an element
    virtual const_reference get_(size_type i) const = 0;

    /// To be overridden by derived class to implement inserting an element
    virtual void insert_(iterator offset, value_type elem) = 0;

    /// To be overridden by the derived class to compute the size of the set
    virtual size_type size_() const noexcept = 0;

    /// To be overridden by the derived class to make the set empty again
    virtual void clear_() = 0;

    virtual void erase_(const_reference elem) = 0;
};

/** @brief Determines if @p lhs comes before @p rhs sequentially.
 *
 *  All ordering comparisons among MathSet instances assume lexicographical
 *  ordering. Namely, {a, b} comes before {c, d} if a <= c and in the event
 *  a == c, that b < d.
 *
 *  @warning This operator does **NOT** compare @p lhs and @p rhs for
 *           subset/superset relationships.
 *
 * @tparam T The type of the elements in the MathSet instance on the left side
 *           of the comparison operator.
 * @tparam U The type of the elements in the MathSet instance on the right side
 *           of the comparison operator.
 *
 * @param[in] lhs The instance on the left side of the comparison operator.
 * @param[in] rhs The instance on the right side of the comparison operator.
 *
 * @return True if @p lhs comes before @p rhs and false otherwise.
 */
template<typename T, typename U>
bool operator<(const MathSetPIMPL<T>& lhs,
               const MathSetPIMPL<U>& rhs) noexcept {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}

/** @brief Determines if @p lhs comes after @p rhs sequentially.
 *
 *  All ordering comparisons among MathSet instances assume lexicographical
 *  ordering. Namely, {a, b} comes before {c, d} if a <= c and in the event
 *  a == c, that b < d.
 *
 *  @warning This operator does **NOT** compare @p lhs and @p rhs for
 *           subset/superset relationships.
 *
 * @tparam T The type of the elements in the MathSet instance on the left side
 *           of the comparison operator.
 * @tparam U The type of the elements in the MathSet instance on the right side
 *           of the comparison operator.
 *
 * @param[in] lhs The instance on the left side of the comparison operator.
 * @param[in] rhs The instance on the right side of the comparison operator.
 *
 * @return True if @p lhs comes after @p rhs and false otherwise.
 */
template<typename T, typename U>
bool operator>(const MathSetPIMPL<T>& lhs,
               const MathSetPIMPL<U>& rhs) noexcept {
    return rhs < lhs;
}

/** @brief Determines if @p lhs is @p rhs or if @p lhs comes after @p rhs
 *         sequentially.
 *
 *  All ordering comparisons among MathSet instances assume lexicographical
 *  ordering. Namely, {a, b} comes before {c, d} if a <= c and in the event
 *  a == c, that b < d.
 *
 *  @warning This operator does **NOT** compare @p lhs and @p rhs for
 *           subset/superset relationships.
 *
 * @tparam T The type of the elements in the MathSet instance on the left side
 *           of the comparison operator.
 * @tparam U The type of the elements in the MathSet instance on the right side
 *           of the comparison operator.
 *
 * @param[in] lhs The instance on the left side of the comparison operator.
 * @param[in] rhs The instance on the right side of the comparison operator.
 *
 * @return True if @p lhs comes after @p rhs or if @p lhs compares equivalent to
 *         @p rhs and false otherwise.
 */
template<typename T, typename U>
bool operator>=(const MathSetPIMPL<T>& lhs,
                const MathSetPIMPL<U>& rhs) noexcept {
    return !(lhs < rhs);
}

/** @brief Determines if @p lhs is @p rhs or if @p lhs comes before @p rhs
 *         sequentially.
 *
 *  All ordering comparisons among MathSet instances assume lexicographical
 *  ordering. Namely, {a, b} comes before {c, d} if a <= c and in the event
 *  a == c, that b < d.
 *
 *  @warning This operator does **NOT** compare @p lhs and @p rhs for
 *           subset/superset relationships.
 *
 * @tparam T The type of the elements in the MathSet instance on the left side
 *           of the comparison operator.
 * @tparam U The type of the elements in the MathSet instance on the right side
 *           of the comparison operator.
 *
 * @param[in] lhs The instance on the left side of the comparison operator.
 * @param[in] rhs The instance on the right side of the comparison operator.
 *
 * @return True if @p lhs comes before @p rhs or if @p lhs compares equivalent
 * to
 *         @p rhs and false otherwise.
 */
template<typename T, typename U>
bool operator<=(const MathSetPIMPL<T>& lhs,
                const MathSetPIMPL<U>& rhs) noexcept {
    return !(rhs < lhs);
}

//------------------------Implementations---------------------------------------
#define MATH_SET_PIMPL_TYPE MathSetPIMPL<ElementType>

template<typename ElementType>
typename MATH_SET_PIMPL_TYPE::reference MATH_SET_PIMPL_TYPE::operator[](
  size_type i) {
    check_index_(i);
    return const_cast<reference>(get_(i));
}

template<typename ElementType>
typename MATH_SET_PIMPL_TYPE::const_reference MATH_SET_PIMPL_TYPE::operator[](
  size_type i) const {
    check_index_(i);
    return get_(i);
}

template<typename ElementType>
typename MATH_SET_PIMPL_TYPE::size_type MATH_SET_PIMPL_TYPE::count(
  const_reference elem) const noexcept {
    return std::count(begin(), end(), elem);
}

template<typename ElementType>
void MATH_SET_PIMPL_TYPE::insert(value_type elem) {
    if(count(elem) > 0) return;
    insert(end(), std::move(elem));
}

template<typename ElementType>
void MATH_SET_PIMPL_TYPE::insert(iterator off, value_type el) {
    if(count(el) > 0) return;
    insert_(off, std::move(el));
}

template<typename ElementType>
void MATH_SET_PIMPL_TYPE::check_index_(size_type i) const {
    if(i < size()) return;
    const std::string msg =
      "Index i = " + std::to_string(i) + " is not in the range [0, ";
    throw std::out_of_range(msg + std::to_string(size()) + ").");
}

template<typename ElementType>
template<typename T>
bool MATH_SET_PIMPL_TYPE::operator==(const MathSetPIMPL<T>& rhs) const
  noexcept {
    if(size() != rhs.size()) return false;
    return std::equal(begin(), end(), rhs.begin());
}

template<typename ElementType>
template<typename T>
bool MATH_SET_PIMPL_TYPE::operator!=(const MathSetPIMPL<T>& rhs) const
  noexcept {
    return !(*this == rhs);
}

#undef MATH_SET_PIMPL_TYPE
} // namespace utilities::detail_
