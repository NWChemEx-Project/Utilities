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
class MathSetView : private MathSet<ElementType> {
private:
    using pimpl_type = detail_::MathSetViewPIMPL<ElementType>;
    using base_type  = MathSet<ElementType>;

public:
    using view_type       = std::reference_wrapper<ElementType>;
    using const_reference = typename base_type::const_reference;
    using size_type       = typename base_type::size_type;

    operator MathSet<ElementType>() const { MathSet<ElementType>(*this); }
    operator const MathSet<ElementType>&() const { return *this; }

    MathSetView() : base_type(std::make_unique<pimpl_type>()) {}
    explicit MathSetView(std::vector<view_type> values) :
      base_type(std::make_unique<pimpl_type>(std::move(values))) {}

    auto begin() const noexcept { return base_type::begin(); }
    auto end() const noexcept { return base_type::end(); }

    using base_type::cbegin;
    using base_type::cend;
    const_reference operator[](size_type i) const {
        return base_type::operator[](i);
    }
    using base_type::count;
    using base_type::empty;
    using base_type::size;
};

template<typename T>
MathSetView(const std::vector<std::reference_wrapper<T>>&)->MathSetView<T>;

} // namespace utilities
