#pragma once
#include "utilities/containers/math_set/detail_/math_set_pimpl.hpp"

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

public:
    /// Type of an element returned by this instance
    using value_type = typename base_type::value_type;
    /// Type of an index or offset into this container
    using size_type = typename base_type::size_type;
    /// Type of a read-only reference to this container
    using const_reference = typename base_type::const_reference;
    /// Type of an iterator over this container
    using iterator = typename base_type::iterator;

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
    template<typename Itr1, typename Itr2, typename ParentContainer>
    SelectionViewPIMPL(Itr1&& start, Itr2&& end, ParentContainer* parent);

private:
    const_reference get_(size_type i) const override;
    void insert_(iterator offset, value_type elem) override;
    size_type size_() const noexcept override { return m_indices_.size(); }

    /// A list of indices that are stored in this view
    std::vector<size_type> m_indices_;
    /// Callback for actually getting an element
    std::function<const_reference(size_type)> m_getter_;
    /// Callback for inserting an element into the set
    std::function<size_type(value_type)> m_inserter_;
};

//---------------------------------Implementations------------------------------
#define SELECTION_VIEW_PIMPL_TYPE SelectionViewPIMPL<ElementType>

template<typename Itr1, typename Itr2, typename ParentContainer>
SelectionViewPIMPL(Itr1&&, Itr2&&, ParentContainer*)
  ->SelectionViewPIMPL<typename ParentContainer::value_type>;

template<typename ElementType>
template<typename Itr1, typename Itr2, typename ParentContainer>
SELECTION_VIEW_PIMPL_TYPE::SelectionViewPIMPL(Itr1&& start, Itr2&& end,
                                              ParentContainer* parent) :
  m_indices_(std::forward<Itr1>(start), std::forward<Itr2>(end)),
  m_getter_([=](size_type i) -> const_reference { return (*parent)[i]; }),
  m_inserter_([=](value_type ei) {
      parent->insert(parent->end(), std::move(ei));
      return parent->size() - 1;
  }) {}

template<typename ElementType>
typename SELECTION_VIEW_PIMPL_TYPE::const_reference
SELECTION_VIEW_PIMPL_TYPE::get_(size_type i) const {
    auto itr = m_indices_.begin();
    std::advance(itr, i);
    return m_getter_(*itr);
}

template<typename ElementType>
void SELECTION_VIEW_PIMPL_TYPE::insert_(iterator offset, value_type elem) {
    auto diff = offset - this->begin();
    m_indices_.insert(m_indices_.begin() + diff, m_inserter_(std::move(elem)));
}

#undef SELECTION_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
