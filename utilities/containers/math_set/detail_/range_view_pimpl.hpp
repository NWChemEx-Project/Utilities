#pragma once
#include "utilities/containers/math_set/detail_/math_set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Stores a contiguous range of elements
 *
 *  The RangeViewPIMPL is a view optimization for the common case where we have
 *  selected a range of elements from some parent container. This class stores
 *  the starting and ending indices of the range as well as a pointer to the
 *  actual parent.
 *
 * @tparam ElementType The cv-qualified type of elements in the parent
 *                     container.
 */
template<typename ElementType>
class RangeViewPIMPL : public MathSetPIMPL<ElementType> {
private:
    /// The type of the base class
    using base_type = MathSetPIMPL<ElementType>;

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
     *  @tparam ParentContainer The type of the ParentContainer assumed to be
     *                          a MathSet, but nearly any container will work.
     *
     *  @param[in] start  The index of the first element in the set.
     *  @param[in] end    The index of the first element not in the set.
     *  @param[in] parent A poitner to the parent container.
     *
     *  @throw none No throw guarantee.
     */
    template<typename ParentContainer>
    RangeViewPIMPL(size_type start, size_type end,
                   ParentContainer* parent) noexcept;

private:
    const_reference get_(size_type i) const override;
    void insert_(iterator offset, value_type elem) override;
    size_type size_() const noexcept override { return m_end_ - m_start_; }

    /// Index of the first element in the set we are a view of
    size_type m_start_ = 0;
    /// Index of the first element NOT in the set we are a view of
    size_type m_end_ = 0;
    /// Call back for getting element i
    std::function<const_reference(size_type)> m_getter_;
    /// Call back for inserting an element before a specified index
    std::function<void(size_type, value_type)> m_inserter_;
};

//---------------------------------Implementations------------------------------
#define RANGE_VIEW_PIMPL_TYPE RangeViewPIMPL<ElementType>

template<typename ParentContainer>
RangeViewPIMPL(std::size_t, std::size_t, ParentContainer*)
  ->RangeViewPIMPL<typename ParentContainer::value_type>;

template<typename ElementType>
template<typename ParentContainer>
RANGE_VIEW_PIMPL_TYPE::RangeViewPIMPL(size_type start, size_type end,
                                      ParentContainer* parent) noexcept :
  m_start_(start),
  m_end_(end),
  m_getter_([=](size_type i) -> const_reference { return (*parent)[i]; }),
  m_inserter_([=](size_type off, value_type ei) {
      parent->insert(parent->begin() + off, std::move(ei));
  }) {}

template<typename ElementType>
typename RANGE_VIEW_PIMPL_TYPE::const_reference RANGE_VIEW_PIMPL_TYPE::get_(
  size_type i) const {
    return m_getter_(m_start_ + i);
}

template<typename ElementType>
void RANGE_VIEW_PIMPL_TYPE::insert_(iterator offset, value_type elem) {
    auto diff = offset - this->begin();
    m_inserter_(diff + m_start_, std::move(elem));
    ++m_end_;
}

#undef RANGE_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
