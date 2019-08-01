#pragma once
#include "utilities/containers/detail_/math_set/selection_view_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Stores a contiguous range of elements
 *
 *  The RangeViewPIMPL is a specialization of the SelectionViewPIMPL that
 *  optimizes for the case when the selection we are interested in is a
 *  contiguous range of indices from the parent. In general what happens is that
 *  this class stores the beginning index and ending index for the range it
 *  aliases.
 *
 *  For the most part this class works as would be expected by a class
 *  holding offsets; we use the offsets to convert from the requested indices to
 *  the indices in the parent set; however, things get tricky if the user
 *  modifies this set. If the user wants to add a completely new element life is
 *  good; we add that element into the correct place in the parent container and
 *  expand our range by 1. If, however, the user wants to add an element that is
 *  already in the parent container care is needed. In this case the index of
 *  the element in the parent container is typically not compatible with the
 *  range we are storing. In this scenario we fall back to the base class which
 *  allows us to store an arbitrary list of elements, in an arbitrary order. The
 *  elements of the view are either held in this class or in the base class,
 *  never in a mixture of the two.
 *
 * @tparam ElementType The cv-qualified type of elements in the parent
 *                     container.
 */
template<typename ElementType>
class RangeViewPIMPL : public SelectionViewPIMPL<ElementType> {
private:
    /// The type of the base class
    using base_type = SelectionViewPIMPL<ElementType>;

    /// The type of the PIMPL common to all PIMPLs
    using pimpl_base = MathSetPIMPL<ElementType>;

    /// The type of this PIMPL
    using my_type = RangeViewPIMPL<ElementType>;

public:
    /// The type of the elements in this set
    using value_type = typename base_type::value_type;
    /// The type of an index or offset
    using size_type = typename base_type::size_type;
    /// A read-only reference to an element in this set
    using const_reference = typename base_type::const_reference;
    /// A read/write iterator over this set
    using iterator = typename base_type::iterator;

    /** @brief Creates an empty subset of another set.
     *
     *  This ctor will create an alias of the empty set in @p parent. Elements
     *  added to this set will also be added to @p parent if they do not exist.
     *
     *  @param[in] parent The set we are aliasing.
     *
     *  @throw none No throw guarantee.
     */
    explicit RangeViewPIMPL(pimpl_base* parent) noexcept;

    /** @brief Creates a view of elements from another container.
     *
     *  This is the main ctor for RangeViewPIMPL instances. Given an index to
     *  the first element in the range and the first element **NOT** in the
     *  range this PIMPL will implement a MathSet that simply aliases the parent
     *  container.
     *
     *  @param[in] start  The index of the first element in the set.
     *  @param[in] end    The index of the first element not in the set.
     *  @param[in] parent A poitner to the parent container.
     *
     *  @throw none No throw guarantee.
     */
    RangeViewPIMPL(size_type start, size_type end, pimpl_base* parent) noexcept;

private:
    /** @brief Code factorization for determining if this class or the base
     *         class is storing the elements.
     *
     *  @return True if this class is storing the elements and false otherwise.
     *
     *  @throw none No throw guarantee.
     */
    bool am_storing_() const noexcept;

    const_reference get_(size_type i) const override;
    void insert_(iterator offset, value_type elem) override;
    size_type size_() const noexcept override;
    void erase_(const_reference elem) noexcept override;
    void clear_() override;

    /// Index of the first element in the set we are a view of
    size_type m_start_ = 0;
    /// Index of the first element NOT in the set we are a view of
    size_type m_end_ = 0;
};

//---------------------------------Implementations------------------------------
#define RANGE_VIEW_PIMPL_TYPE RangeViewPIMPL<ElementType>

template<typename ElementType>
RangeViewPIMPL(std::size_t, std::size_t, MathSetPIMPL<ElementType>*)
  ->RangeViewPIMPL<ElementType>;

template<typename ElementType>
RANGE_VIEW_PIMPL_TYPE::RangeViewPIMPL(pimpl_base* parent) noexcept :
  SelectionViewPIMPL<ElementType>(parent) {}

template<typename ElementType>
RANGE_VIEW_PIMPL_TYPE::RangeViewPIMPL(size_type start, size_type end,
                                      pimpl_base* parent) noexcept :
  SelectionViewPIMPL<ElementType>(parent),
  m_start_(start),
  m_end_(end) {}

template<typename ElementType>
bool RANGE_VIEW_PIMPL_TYPE::am_storing_() const noexcept {
    return m_end_ - m_start_ > 0 || base_type::size_() == 0;
}

template<typename ElementType>
typename RANGE_VIEW_PIMPL_TYPE::const_reference RANGE_VIEW_PIMPL_TYPE::get_(
  size_type i) const {
    // If our size is 0 then we're holding the elements, if our size is 0 then
    // either the base class is holding the elements or we're the empty set
    //(which if we're the empty set then calling operator[] will trip an
    // exception so we don't have to worry about it)
    return am_storing_() ? this->parent_()[m_start_ + i] : base_type::get_(i);
}

template<typename ElementType>
void RANGE_VIEW_PIMPL_TYPE::insert_(iterator offset, value_type elem) {
    // Handle when base class is already holding values
    if(!am_storing_()) {
        base_type::insert_(offset, std::move(elem));
        return;
    }

    // Two scenarios: parent set has element and we need to alias it or its new
    // to both and we need to add it.

    auto idx     = this->index_(elem);
    auto& parent = this->parent_();

    const auto my_idx = offset - this->begin();

    if(idx == parent.size()) { // Not in the parent's range
        const auto parent_idx = my_idx + m_start_;
        parent.insert(parent.begin() + parent_idx, std::move(elem));
        // Doesn't matter where elem was added, net result is ++m_end_
        ++m_end_;
        return;
    }

    // Handle alias cases where we just need to expand range by 1
    if(idx == m_start_ - 1 && my_idx == 0) {
        --m_start_;
        return;
    } else if(idx == m_end_ && my_idx == m_end_) {
        ++m_end_;
        return;
    }

    // Insertion breaks our range, fall back to base class

    // Insert uses iterators make sure ours come back correct for empty
    auto old_start = m_start_;
    auto old_end   = m_end_;
    m_start_       = 0;
    m_end_         = 0;
    for(auto i = old_start; i < old_end; ++i)
        base_type::insert_(this->end(), parent[i]);
    base_type::insert_(this->begin() + my_idx, std::move(elem));
}

template<typename ElementType>
typename RANGE_VIEW_PIMPL_TYPE::size_type RANGE_VIEW_PIMPL_TYPE::size_() const
  noexcept {
    return am_storing_() ? m_end_ - m_start_ : base_type::size_();
}

template<typename ElementType>
void RANGE_VIEW_PIMPL_TYPE::erase_(const_reference elem) noexcept {
    if(!am_storing_()) {
        base_type::erase_(elem);
        return;
    }

    auto idx = this->index_(elem);
    // Our range is now effectively one shorter
    --m_end_;
    this->parent_().erase(elem);
}

template<typename ElementType>
void RANGE_VIEW_PIMPL_TYPE::clear_() {
    if(!am_storing_()) {
        base_type::clear_();
        return;
    }

    /* Since we are modifying this container we need to be careful using the
     * iterators to do it (each modification ivalidates the iterators and the
     * range-based for loop uses the same pair until completion). To that end
     * we copy the elements, erase them, and reset our range.
     */
    std::vector<ElementType> old(this->begin(), this->end());
    for(const auto& x : old) this->parent_().erase(x);
    m_start_ = 0;
    m_end_   = 0;
}

#undef RANGE_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
