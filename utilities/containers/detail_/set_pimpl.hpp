#pragma once
#include <memory>
#include <vector>

namespace utilities::detail_ {

template<typename ElementType>
class SetPIMPL {
private:
    /// Type of the container used for storing the elements
    using container_type = std::vector<ElementType>;

public:
    /// Type of the elements in the set
    using value_type = typename container_type::value_type;
    /// Type used to convey counts and offsets
    using size_type = typename container_type::size_type;
    /// Type of a reference to an element in the set
    using reference = typename container_type::reference;
    /// Type of a read-only reference to an element in the set
    using const_reference = typename container_type::const_reference;
    /// Type of a random access iterator that can read/write elements in the set
    using iterator = typename container_type::iterator;
    /// Type of a random access iterator that can only read elements in this set
    using const_iterator = typename container_type::const_iterator;

    /** @brief Creates an empty set.
     *
     *  The set created by the default ctor has no elements in it. Elements can
     *  be added by calling `insert`.
     *
     *  @throw std::bad_alloc if there is insufficient memory on the heap to
     *                        allocate the container for holding the elements.
     *                        Strong throw guarantee.
     *
     */
    SetPIMPL() : m_data_(std::make_shared<container_type>()) {}

    /** @brief Creates a set by deep copying another set's state.
     *
     *  This ctor will create a new set, whose elements are allocated on the
     *  heap and are deep copies of the elements in @p rhs.
     *
     *  @param[in] rhs The set whose state is being copied.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        elements on the heap. Strong throw guarantee.
     */
    SetPIMPL(const SetPIMPL& rhs) : SetPIMPL(rhs.begin(), rhs.end()) {}

    /** @brief Overwrites the current set's state with a deep copy of another
     *         set.
     *
     *  This function sets the current PIMPL's state to a deep copy of @p rhs's
     *  state and returns the current instance to facilitate chaining. The
     *  previous state will be released making all references, and iterators
     *  invalid. Any views to the MathSet owning this PIMPL will alias the deep
     *  copy of this state and will thus likely be invalidated.
     *
     * @param[in] rhs The set whose state is being copied.
     *
     * @return The current SetPIMPL instance after its state has been set to a
     *         deep copy
     *
     * @throw std::bad_alloc if there is insufficient memory to copy the
     *                       elements from @p rhs. Strong throw guarantee.
     */
    SetPIMPL& operator=(const SetPIMPL& rhs);

    /** @brief Creates a new set by taking ownership of another set's state.
     *
     *  This ctor is used to create a new SetPIMPL instance by taking ownership
     *  of another instance's state.
     *
     *  @param[in] rhs The set whose state is being taken. After this operation
     *                 @p rhs's state is in a valid, but otherwise undefined
     *                 state.
     *
     *  @throw none No throw guarantee.
     *
     */
    SetPIMPL(SetPIMPL&& rhs) noexcept = default;

    /** @brief Transfers ownership of another set's state to this set.
     *
     *  This function is used to set the current SetPIMPL instance's state to
     *  the state of @p rhs. All references and iterators to this SetPIMPL's
     *  state will be invalidated. Views to the MathSet instance wrapping this
     *  PIMPL will continue to alias this instance and will thus likely be
     *  invalidated.
     *
     *  @param[in] rhs The set whose state is being taken. After this operation
     *                 @p rhs's state is in a valid, but otherwise undefined
     *                 state.
     *
     *  @return The current instance after its state has been set to @p rhs's
     *          state.
     *
     *  @throw none No throw guarantee.
     *
     */
    SetPIMPL& operator=(SetPIMPL&& rhs) noexcept = default;

    /** @brief Constructs a set with the provided values.
     *
     *  This ctor will add the values in the provided initializer list to the
     *  set. Insertion of the values will respect uniqueness (repeat values
     *  will only be added once).
     *
     * @param[in] il An initializer list of the values to add to the set.
     *
     * @throw std::bad_alloc if there is insufficient memory to store the new
     *                       elements. Strong throw guarantee.
     */
    SetPIMPL(std::initializer_list<ElementType> il);

    /** @brief Constructs a set using the provided range of elements.
     *
     *  This ctor takes an iterator to the first element to add to the set and
     *  an iterator just past the last element to add. All elements in the
     *  resulting range will be added to the set, respecting uniqueness (each
     *  element will only be added once).
     *
     *  @tparam Itr1 The type of the iterator pointing to the first element.
     *  @tparam Itr2 The type of the iterator pointing to just past the last
     *               element.
     *  @param[in] begin An iterator pointing to the first element which should
     *                   be added to the set.
     *  @param[in] end   An iterator pointing to just past the last element to
     *                   add.
     *
     *  @throw std::bad_alloc if there is insufficient memory to store the new
     *                        elements. Strong throw guarantee.
     *  @throw ??? if any manipulation of the iterators throws. Strong throw
     *             guarantee.
     */
    template<typename Itr1, typename Itr2>
    SetPIMPL(Itr1 itr1, Itr2 itr2);

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
    iterator begin() noexcept { return m_data_->begin(); }

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
    const_iterator begin() const noexcept { return m_data_->begin(); }

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
    iterator end() noexcept { return m_data_->end(); }

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
    const_iterator end() const noexcept { return m_data_->end(); }

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
    size_type size() const noexcept { return m_data_->size(); }

    /** @brief Determines whether an element appears in the set or not.
     *
     *  This function is used to determine the number of times an element
     *  appears in the set. Since an element can only appear at most once. The
     *  returns safely map to boolean values of `false` when the element is not
     *  present and `true` when it is.
     *
     *  @param[in] elem The element whose inclusion in the current set is in
     *                  question.
     *  @return The number of times (either 0 or 1) @p elem appears in the set.
     *
     *  @throw none No throw guarantee.
     */
    size_type count(const ElementType& elem) const noexcept;

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
    void insert(ElementType elem);

private:
    /// The actual data stored within this PIMPL
    std::shared_ptr<container_type> m_data_;
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
bool operator==(const SetPIMPL<T>& lhs, const SetPIMPL<U>& rhs) noexcept {
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
bool operator!=(const SetPIMPL<T>& lhs, const SetPIMPL<U>& rhs) noexcept {
    return !(lhs == rhs);
}

//------------------------------Implementations---------------------------------

/// Improve readability of SetPIMPL's type in definitions
#define SET_PIMPL_TYPE SetPIMPL<ElementType>

template<typename Itr1, typename Itr2>
SetPIMPL(Itr1 itr1, Itr2 itr2)->SetPIMPL<typename Itr1::value_type>;

template<typename ElementType>
SET_PIMPL_TYPE::SetPIMPL(std::initializer_list<ElementType> il) :
  SetPIMPL(il.begin(), il.end()) {}

template<typename ElementType>
template<typename Itr1, typename Itr2>
SET_PIMPL_TYPE::SetPIMPL(Itr1 itr1, Itr2 itr2) : SetPIMPL() {
    m_data_->reserve(std::distance(itr1, itr2));
    while(itr1 != itr2) {
        insert(*itr1);
        ++itr1;
    }
}

template<typename ElementType>
SET_PIMPL_TYPE& SET_PIMPL_TYPE::operator=(const SET_PIMPL_TYPE& rhs) {
    SetPIMPL(rhs).m_data_->swap(*m_data_);
    return *this;
}

template<typename ElementType>
typename SET_PIMPL_TYPE::size_type SET_PIMPL_TYPE::count(
  const ElementType& elem) const noexcept {
    return m_data_->end() != std::find(m_data_->begin(), m_data_->end(), elem);
}

template<typename ElementType>
void SET_PIMPL_TYPE::insert(ElementType elem) {
    if(count(elem)) return;
    m_data_->push_back(elem);
}
#undef SET_PIMPL_TYPE
} // namespace utilities::detail_
