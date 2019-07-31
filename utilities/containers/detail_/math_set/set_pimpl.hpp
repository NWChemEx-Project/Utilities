#pragma once
#include "utilities/containers/detail_/math_set/math_set_pimpl.hpp"
#include <memory>
#include <vector>

namespace utilities::detail_ {

/**
 *
 * @note This PIMPL serves as the ultimate backend for most sets. Of particular
 *       note is that it serves as the backend for the NestedPIMPL class. In
 *       order for that to work this class can **NOT** enforce uniqueness of the
 *       elements, that must be done above the PIMPL.
 * @tparam ElementType
 */
template<typename ElementType>
class SetPIMPL : public MathSetPIMPL<ElementType> {
private:
    /// Type of the container used for storing the elements
    using container_type = std::vector<ElementType>;

    /// Type of the base class
    using base_type = MathSetPIMPL<ElementType>;

public:
    using value_type      = typename base_type::value_type;
    using const_reference = typename base_type::const_reference;
    using iterator        = typename base_type::iterator;
    using size_type       = typename base_type::size_type;

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
     *  set. Insertion of the values will **NOT** respect uniqueness (repeat
     *  values will be added multiple times).
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
     *  resulting range will be added to the set, **NOT** respecting uniqueness
     *  (each element will be added regardless of whether it is already in the
     *  set).
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

private:
    const_reference get_(size_type i) const override { return (*m_data_)[i]; }

    void insert_(iterator offset, value_type elem) override;

    size_type size_() const noexcept override { return m_data_->size(); }

    void clear_() noexcept override { m_data_->clear(); }

    void erase_(const_reference elem) override;

    /// The actual data stored within this PIMPL
    std::shared_ptr<container_type> m_data_;
};

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
        if(this->count(*itr1) == 0) insert_(this->end(), *itr1);
        ++itr1;
    }
}

template<typename ElementType>
SET_PIMPL_TYPE& SET_PIMPL_TYPE::operator=(const SET_PIMPL_TYPE& rhs) {
    SetPIMPL(rhs).m_data_->swap(*m_data_);
    return *this;
}

template<typename ElementType>
void SET_PIMPL_TYPE::insert_(iterator offset, value_type elem) {
    auto diff = offset - this->begin();
    m_data_->insert(m_data_->begin() + diff, std::move(elem));
}

template<typename ElementType>
void SET_PIMPL_TYPE::erase_(const_reference elem) {
    m_data_->erase(std::find(m_data_->begin(), m_data_->end(), elem));
}
#undef SET_PIMPL_TYPE

} // namespace utilities::detail_
