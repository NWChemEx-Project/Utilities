#pragma once
#include "utilities/containers/detail_/math_set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Stores a contiguous range of elements
 *
 *  The RangeViewPIMPL is a view optimization for the common case where we have
 *  selected a range of elements from some parent container. This class stores
 *  the starting and ending indices of the range as well as a shared_ptr to the
 *  actual state.
 *
 * @tparam ElementType The cv-qualified type that
 */
template<typename ElementType>
class RangeViewPIMPL : public MathSetPIMPL<ElementType> {
private:
    using base_type = MathSetPIMPL<ElementType>;

public:
    using value_type      = typename base_type::value_type;
    using size_type       = typename base_type::size_type;
    using const_reference = typename base_type::const_reference;
    using iterator        = typename base_type::iterator;

    template<typename ParentContainer>
    RangeViewPIMPL(size_type start, size_type end,
                   std::shared_ptr<ParentContainer> parent);

private:
    const_reference get_(size_type i) const override;
    void insert_(iterator offset, value_type elem) override;
    size_type size_() const noexcept override { return m_end_ - m_start_; }

    size_type m_start_ = 0;
    size_type m_end_   = 0;
    std::function<const_reference(size_type)> m_getter_;
    std::function<void(size_type, value_type)> m_inserter_;
};

//---------------------------------Implementations------------------------------
#define RANGE_VIEW_PIMPL_TYPE RangeViewPIMPL<ElementType>

template<typename ParentContainer>
RangeViewPIMPL(std::size_t, std::size_t, std::shared_ptr<ParentContainer>)
  ->RangeViewPIMPL<typename ParentContainer::value_type>;

template<typename ElementType>
template<typename ParentContainer>
RANGE_VIEW_PIMPL_TYPE::RangeViewPIMPL(size_type start, size_type end,
                                      std::shared_ptr<ParentContainer> parent) :
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
}

#undef RANGE_VIEW_PIMPL_TYPE
} // namespace utilities::detail_
