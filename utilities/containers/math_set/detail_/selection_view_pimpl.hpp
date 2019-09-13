#pragma once
#include "utilities/containers/detail_/math_set/math_set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief A view of (possibly) non-contiguous elements in another MathSet
 *
 *  This PIMPL allows the resulting MathSet to be used as if it holds a subset
 *  of another MathSet's elements. Unlike the RangeViewPIMPL, this PIMPL does
 *  not assume that the elements are contiguous.
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
    /// Type of a vector of indices
    using idx_vector = std::vector<size_type>;

    /** @brief Creates an empty selection that is a view of @p parent
     *
     *  This ctor creates a new SelectionViewPIMPL that aliases @p parent and
     *  specifically holds the elements with indices in @p idxs. It is assumed
     *  that the values in @p idxs are valid (in range and not repeated).
     *
     *  @param[in] p The class this instance will be a view of.
     *
     *  @throw none No throw guarantee
     */
    SelectionViewPIMPL(const base_type* p, idx_vector idxs) noexcept;

    template<typename Itr1, typename Itr2>
    SelectionViewPIMPL(const base_type* p, Itr1 itr1, Itr2 itr2);

protected:
    /// Implements operator[] for MathSetPIMPL
    const_reference get_(size_type i) const override;
    /// Implements push_back() for MathSetPIMPL, throws an error if called
    void push_back_(value_type elem) override;
    /// Implements size() for MathSetPIMPL
    size_type size_() const noexcept override { return m_indices_.size(); }

private:
    /// A list of indices that are stored in this view
    idx_vector m_indices_;

    /// The PIMPL of the parent container
    const base_type* m_parent_;
};

//---------------------------------Implementations------------------------------
#define SELECTION_VIEW_PIMPL_TYPE SelectionViewPIMPL<ElementType>

template<typename ElementType>
SelectionViewPIMPL(const MathSetPIMPL<ElementType>*, std::vector<std::size_t>)
  ->SelectionViewPIMPL<ElementType>;

template<typename ElementType>
SELECTION_VIEW_PIMPL_TYPE::SelectionViewPIMPL(const base_type* p,
                                              idx_vector idxs) noexcept :
  m_indices_(std::move(idxs)),
  m_parent_(p) {}

template<typename ElementType>
template<typename Itr1, typename Itr2>
SELECTION_VIEW_PIMPL_TYPE::SelectionViewPIMPL(const base_type* p, Itr1 itr1,
                                              Itr2 itr2) :
  SelectionViewPIMPL(p, idx_vector(itr1, itr2)) {}

template<typename ElementType>
typename SELECTION_VIEW_PIMPL_TYPE::const_reference
SELECTION_VIEW_PIMPL_TYPE::get_(size_type i) const {
    return (*m_parent_)[m_indices_[i]];
}

template<typename ElementType>
void SELECTION_VIEW_PIMPL_TYPE::push_back_(value_type) {
    throw std::runtime_error(
      "SelectionView PIMPLs are read-only. Cannot insert new elements.");
}

#undef SELECTION_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
