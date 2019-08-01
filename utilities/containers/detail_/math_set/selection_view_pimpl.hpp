#pragma once
#include "utilities/containers/detail_/math_set/math_set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Stores sparse list of elements
 *
 *  The RangeViewPIMPL is used when the slice of the container we want a view to
 *  is contiguous. The SelectionViewPIMPL, on the other hand, is used when we
 *  want a non-contiguous range of elements. Under the hood this class just
 *  stores a vector of indices.
 *
 * @tparam ElementType The cv-qualified type of the parent set's elements. Must
 *                     satisfy the same concepts and criteria as MathSet's
 *                     template type parameter.
 */
template<typename ElementType>
class SelectionViewPIMPL : public MathSetPIMPL<ElementType> {
private:
    /// Type of the base class, used in other typedefs
    using base_type = MathSetPIMPL<ElementType>;

    /// Type of this class
    using my_type = SelectionViewPIMPL<ElementType>;

public:
    /// Type of an element returned by this instance
    using value_type = typename base_type::value_type;
    /// Type of an index or offset into this container
    using size_type = typename base_type::size_type;
    /// Type of a read-only reference to this container
    using const_reference = typename base_type::const_reference;
    /// Type of an iterator over this container
    using iterator = typename base_type::iterator;

    /** @brief Creates an empty selection that is a view of @p parent
     *
     *  This ctor creates a new SelectionViewPIMPL that aliases @p parent. The
     *  resulting instance is empty. Elements can be added to this instance by
     *  calling `insert`.
     *
     *  @param[in] p The class this instance will be a view of.
     *
     *  @throw none No throw guarantee
     */
    explicit SelectionViewPIMPL(base_type* p) noexcept : m_parent_(p) {}

    /** @brief Creates a MathSetPIMPL that holds an assortment of indices from a
     *         parent container.
     *
     *  This ctor is used to create a SelectionViewPIMPL instance. The inputs
     *  are a pointer to the parent container we are a view of and iterators to
     *  the range of indices this set contains. Ultimately this ctor adds all
     *  of the elements (*parent)[*start] to the set.
     *
     * @tparam Itr1 The type of the iterator pointing to the first element.
     * @tparam Itr2 The type of the iterator pointing to just past the last
     *              element.
     * @tparam ParentContainer The type of the container we are a view of. The
     *                         container is assumed to be a MatSet intance;
     *                         however, any object satisfying the concept of
     *                         container will work.
     *
     * @param[in] start An iterator pointing to the index of the first element
     *                  in this set.
     * @param[in] end   An iterator pointing to just past the last index in this
     *                 set.
     * @param[in] parent The container this instance is a view of.
     *
     * @throw std::bad_alloc if there is insufficient memory to store the
     *                       indices. Strong throw guarantee.
     */
    template<typename Itr1, typename Itr2>
    SelectionViewPIMPL(Itr1&& start, Itr2&& end, base_type* parent);

protected:
    /// Get the parent container's PIMPL in a read/write fashion
    base_type& parent_() noexcept { return *m_parent_; }
    /// Get the parent container's PIMPL in a read/only fashion
    const base_type& parent_() const noexcept { return *m_parent_; }
    /// Get the index of an element (code factorization)
    size_type index_(const_reference elem) const noexcept;

    const_reference get_(size_type i) const override;
    void insert_(iterator offset, value_type elem) override;
    size_type size_() const noexcept override { return m_indices_.size(); }
    void erase_(const_reference elem) override;
    void clear_() override;

private:
    /// A list of indices that are stored in this view
    std::vector<size_type> m_indices_;

    /// The PIMPL of the parent container
    base_type* m_parent_;
};

//---------------------------------Implementations------------------------------
#define SELECTION_VIEW_PIMPL_TYPE SelectionViewPIMPL<ElementType>

template<typename Itr1, typename Itr2, typename ElementType>
SelectionViewPIMPL(Itr1&&, Itr2&&, MathSetPIMPL<ElementType>*)
  ->SelectionViewPIMPL<ElementType>;

template<typename ElementType>
template<typename Itr1, typename Itr2>
SELECTION_VIEW_PIMPL_TYPE::SelectionViewPIMPL(Itr1&& start, Itr2&& end,
                                              base_type* parent) :
  m_indices_(std::forward<Itr1>(start), std::forward<Itr2>(end)),
  m_parent_(parent) {}

template<typename ElementType>
typename SELECTION_VIEW_PIMPL_TYPE::size_type SELECTION_VIEW_PIMPL_TYPE::index_(
  const_reference elem) const noexcept {
    auto itr = std::find(parent_().begin(), parent_().end(), elem);
    return itr - parent_().begin();
}

template<typename ElementType>
typename SELECTION_VIEW_PIMPL_TYPE::const_reference
SELECTION_VIEW_PIMPL_TYPE::get_(size_type i) const {
    return parent_()[m_indices_[i]];
}

template<typename ElementType>
void SELECTION_VIEW_PIMPL_TYPE::insert_(iterator offset, value_type elem) {
    // Two scenarios: parent has it and we need to alias it, or parent doesn't
    // and we need to add it.

    // Compute parent index
    auto idx = index_(elem);

    // Compute our index
    auto diff = offset - this->begin();

    if(idx == parent_().size()) {
        parent_().insert(parent_().end(), std::move(elem));
    }

    m_indices_.insert(m_indices_.begin() + diff, idx);
}

template<typename ElementType>
void SELECTION_VIEW_PIMPL_TYPE::clear_() {
    std::vector<ElementType> old_values(this->begin(), this->end());
    m_indices_.clear();
    for(const auto& x : old_values) parent_().erase(x);
}

template<typename ElementType>
void SELECTION_VIEW_PIMPL_TYPE::erase_(const_reference elem) {
    // Get the index we should erase
    auto idx = index_(elem);

    // Erase element from parent
    parent_().erase(elem);

    std::vector<size_type> new_indices;
    for(auto x : m_indices_) {
        if(x == idx) continue;
        new_indices.push_back(x < idx ? x : x - 1);
    }
    m_indices_.swap(new_indices);
}

#undef SELECTION_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
