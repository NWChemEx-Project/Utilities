#pragma once
#include "utilities/containers/detail_/math_set/math_set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Stores a contiguous range of elements
 *
 * @tparam ElementType The cv-qualified type of elements in the parent
 *                     container.
 */
template<typename ElementType>
class RangeViewPIMPL : public MathSetPIMPL<ElementType> {
private:
    /// The type of the PIMPL common to all PIMPLs
    using base_type = MathSetPIMPL<ElementType>;

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
    RangeViewPIMPL(size_type start, size_type end,
                   const base_type* parent) noexcept;

private:
    /// Implements operator[] for MathSetPIMPL
    const_reference get_(size_type i) const override;
    /// Implements push_back for MathSetPIMPL
    void push_back_(value_type elem) override;
    /// Implements size() for MathSetPIMPL
    size_type size_() const noexcept override;

    /// Pointer to MathSet we are a view of
    const base_type* m_parent_;
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
RANGE_VIEW_PIMPL_TYPE::RangeViewPIMPL(size_type start, size_type end,
                                      const base_type* parent) noexcept :
  m_parent_(parent),
  m_start_(start),
  m_end_(end) {}

template<typename ElementType>
typename RANGE_VIEW_PIMPL_TYPE::const_reference RANGE_VIEW_PIMPL_TYPE::get_(
  size_type i) const {
    return (*this->m_parent_)[m_start_ + i];
}

template<typename ElementType>
void RANGE_VIEW_PIMPL_TYPE::push_back_(value_type elem) {
    throw std::runtime_error("Can not add to a view");
}

template<typename ElementType>
typename RANGE_VIEW_PIMPL_TYPE::size_type RANGE_VIEW_PIMPL_TYPE::size_() const
  noexcept {
    return m_end_ - m_start_;
}

#undef RANGE_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
