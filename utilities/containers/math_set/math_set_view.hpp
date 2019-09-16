#pragma once
#include "utilities/containers/math_set/detail_/math_set_view_pimpl.hpp"
#include "utilities/containers/math_set/math_set_class.hpp"
namespace utilities {

/**
 *
 *
 *
 * @tparam ElementType
 */
template<typename ElementType>
class MathSetView {
private:
    using pimpl_type = detail_::MathSetViewPIMPL<ElementType>;
    using base_type  = MathSet<ElementType>;

public:
    using view_type       = std::reference_wrapper<ElementType>;
    using const_reference = typename base_type::const_reference;
    using size_type       = typename base_type::size_type;

    operator MathSet<ElementType>() const { m_pimpl_; }
    operator const MathSet<ElementType>&() const { return m_pimpl_; }

    MathSetView() : m_pimpl_(std::make_unique<pimpl_type>()) {}
    explicit MathSetView(std::vector<view_type> values) :
      m_pimpl_(std::make_unique<pimpl_type>(std::move(values))) {}

    auto begin() const noexcept { return m_pimpl_.begin(); }
    auto end() const noexcept { return m_pimpl_.end(); }

    auto cbegin() const noexcept { return m_pimpl_.cbegin(); }
    auto cend() const noexcept { return m_pimpl_.cend(); }
    const_reference operator[](size_type i) const { return m_pimpl_[i]; }
    auto count(const_reference e) const noexcept { return m_pimpl_.count(e); }
    auto empty() const noexcept { return m_pimpl_.empty(); }
    auto size() const noexcept { return m_pimpl_.size(); }

private:
    MathSet<ElementType> m_pimpl_;
};

template<typename T>
MathSetView(const std::vector<std::reference_wrapper<T>>&)->MathSetView<T>;

} // namespace utilities
