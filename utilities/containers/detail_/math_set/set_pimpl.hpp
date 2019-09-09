#pragma once
#include "utilities/containers/detail_/math_set/math_set_pimpl.hpp"
#include <memory>
#include <vector>

namespace utilities::detail_ {

/** @brief Backend for MathSet that owns its own data and that data is not more
 *         MathSet instances.
 *
 *  This class basically wraps an std::vector in the MathSetPIMPL API.
 *
 * @note This PIMPL serves as the ultimate backend for most sets. Of particular
 *       note is that it serves as the backend for the NestedPIMPL class. In
 *       order for that to work this class can **NOT** enforce uniqueness of the
 *       elements, that must be done above the PIMPL.
 *
 * @tparam ElementType The class type of the elements being stored in the set.
 */
template<typename ElementType>
class SetPIMPL : public MathSetPIMPL<ElementType> {
private:
    /// Type of the container used for storing the elements
    using container_type = std::vector<ElementType>;
    /// Type of the base class
    using base_type = MathSetPIMPL<ElementType>;

public:
    /// Type elements are stored as, implicitly converts to ElementType
    using value_type = typename base_type::value_type;
    /// Type of a read-only reference to an element in this PIMPL
    using const_reference = typename base_type::const_reference;
    /// Type of an iterator over a non-const PIMPL
    using iterator = typename base_type::iterator;
    /// Type used for indexing and offsets
    using size_type = typename base_type::size_type;

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
    SetPIMPL() = default;

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
    /// Implements operator[] of MathSetPIMPL
    const_reference get_(size_type i) const override { return m_data_[i]; }
    /// Implements insert() of MathSetPIMPL
    void push_back_(value_type elem) override;
    /// Implements size() of MathSetPIMPL
    size_type size_() const noexcept override { return m_data_.size(); }

    /// The actual data stored within this PIMPL
    container_type m_data_;
}; // class SetPIMPL

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
    m_data_.reserve(std::distance(itr1, itr2));
    while(itr1 != itr2) {
        if(this->count(*itr1) == 0) push_back_(*itr1);
        ++itr1;
    }
}

template<typename ElementType>
void SET_PIMPL_TYPE::push_back_(value_type elem) {
    m_data_.emplace_back(std::move(elem));
}

/// Ensure SET_PIMPL_TYPE does not pollute macro namespace
#undef SET_PIMPL_TYPE

} // namespace utilities::detail_
