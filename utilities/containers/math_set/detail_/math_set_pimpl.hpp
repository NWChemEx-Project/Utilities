#pragma once
#include "utilities/containers/math_set/detail_/element_type_traits.hpp"
#include "utilities/iterators/offset_iterator.hpp"

#include <memory>
namespace utilities::detail_ {

/** @brief Base class common to all PIMPLs used in the MathSet class hierarchy
 *
 *  Each of the derived MathSetPIMPL instances holds its data in a different
 *  manner. The MathSetPIMPL class defines the API that the MathSet and the
 *  MathSetView classes will use to interact with the various derived classes.
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

    // Deleted to avoid slicing, copy/move is done via MathSet/MathSetView
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
     *  Sets are ordered such that the i-th element is the i-th unique element
     *  that was inserted into the set. This function can be used to retrieve
     *  the element as a read/write reference. This function ultimately works
     *  by const casting the result of the derived class's `get_` method.
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
     *  Sets are ordered such that the i-th element is the i-th unique element
     *  that was inserted into the set. This function can be used to retrieve
     *  the element as a read-only reference. This function ultimately calls the
     *  derived class via its `get_` method.
     *
     *  @param[in] i  The index of the element to retrieve. Should be in the
     *                range [0, size()).
     *  @return A read-only reference to the i-th element in the set.
     *
     *  @throw std::out_of_range if @p i is not in the range [0, size()). Strong
     *                           throw guarantee.
     */
    const_reference operator[](size_type i) const;

    /** @brief Determines whether an element appears in the set or not.
     *
     *  This function is used to determine the number of times an element
     *  appears in the set. This value will be either 0 or 1 because elements
     *  must be unique. This function works by calling the derived class's
     *  `count_` method.
     *
     *  @param[in] el The element whose inclusion in the current set is in
     *                question.
     *  @return The number of times @p el appears in the set.
     *
     *  @throw none No throw guarantee.
     */
    size_type count(const_reference elem) const noexcept;

    /** @brief Adds an element to the end of the set.
     *
     *  This function can be used to append an element into the set. The element
     *  will only be added if it is unique.
     *
     *  @param[in] elem The element to add to the set.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the element
     *                        to the set. Strong throw guarantee.
     */
    void push_back(value_type elem);

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

    /** @brief Returns an iterator that points to the first element of the set.
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
    /// Code factorization for ensuring index @p i is in bounds
    void check_index_(size_type i) const;

    /// To be overridden by derived class to implement getting an element
    virtual const_reference get_(size_type i) const = 0;

    /// To be overridden by derived class to determine how many times an element
    virtual size_type count_(const_reference elem) const noexcept = 0;

    /// To be overridden by derived class to implement inserting an element
    virtual void push_back_(value_type elem) = 0;

    /// To be overridden by the derived class to compute the size of the set
    virtual size_type size_() const noexcept = 0;
};

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
    return count_(elem);
}

template<typename ElementType>
void MATH_SET_PIMPL_TYPE::push_back(value_type elem) {
    if(count(elem) > 0) return;
    push_back_(std::move(elem));
}

template<typename ElementType>
void MATH_SET_PIMPL_TYPE::check_index_(size_type i) const {
    if(i < size()) return;
    const std::string msg =
      "Index i = " + std::to_string(i) + " is not in the range [0, ";
    throw std::out_of_range(msg + std::to_string(size()) + ").");
}

#undef MATH_SET_PIMPL_TYPE
} // namespace utilities::detail_
