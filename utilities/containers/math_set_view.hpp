#pragma once
#include "utilities/containers/detail_/math_set_pimpl.hpp"

namespace utilities {

template<typename ElementType>
class MathSetView {
private:
    using pimpl_type = detail_::MathSetPIMPL<ElementType>;

public:
    using value_type      = typename pimpl_type::value_type;
    using size_type       = typename pimpl_type::size_type;
    using reference       = typename pimpl_type::reference;
    using const_reference = typename pimpl_type::const_reference;
    using iterator        = typename pimpl_type::iterator;
    using const_iterator  = typename pimpl_type::const_iterator;

    explicit MathSetView(std::shared_ptr<pimpl_type> pimpl) : m_pimpl_(pimpl) {}

    iterator begin() { return m_pimpl_->begin(); }
    auto begin() const { return m_pimpl_->begin(); }
    auto cbegin() const { return begin(); }

    iterator end() { return m_pimpl_->end(); }
    auto end() const { return m_pimpl_->end(); }
    auto cend() const { return end(); }

    reference operator[](size_type i) { return (*m_pimpl_)[i]; }
    const_reference operator[](size_type i) const { return (*m_pimpl_)[i]; }

    size_type size() const noexcept { return m_pimpl_->size(); }
    bool empty() const { return cbegin() == cend(); }

private:
    std::shared_ptr<detail_::MathSetPIMPL<ElementType>> m_pimpl_;
};

//---------------------------Implementations------------------------------------
#define MATH_SET_VIEW_TYPE MathSetView<ElementType>

#undef MATH_SET_VIEW_TYPE
} // namespace utilities
