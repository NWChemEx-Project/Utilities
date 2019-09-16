#pragma once
#include "utilities/containers/math_set/math_set_class.hpp"
#include "utilities/containers/math_set/math_set_view.hpp"

namespace utilities {
namespace detail_ {

template<typename T, typename U>
auto are_equal_guts_(T&& lhs, U&& rhs) {
    if(lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, typename U>
auto intersection_guts_(T&& lhs, U&& rhs) {
    using value_type = std::reference_wrapper<typename T::value_type>;
    std::vector<value_type> idxs;
    for(auto& x : lhs)
        if(rhs.count(x)) idxs.push_back(std::ref(x));
    return MathSetView{std::move(idxs)};
}

} // namespace detail_

template<typename ElementType, typename RHS>
bool operator==(const MathSet<ElementType>& lhs, RHS&& rhs) noexcept {
    return detail_::are_equal_guts_(lhs, std::forward<RHS>(rhs));
}

template<typename ElementType, typename RHS>
bool operator==(const MathSetView<ElementType>& lhs, RHS&& rhs) noexcept {
    return detail_::are_equal_guts_(lhs, std::forward<RHS>(rhs));
}

template<typename ElementType, typename RHS>
auto operator^(const MathSet<ElementType>& lhs, RHS& rhs) {
    return detail_::intersection_guts_(lhs, std::forward<RHS>(rhs));
}

template<typename ElementType, typename RHS>
auto operator^(const MathSetView<ElementType>& lhs, RHS&& rhs) {
    return detail_::intersection_guts_(lhs, std::forward<RHS>(rhs));
}

} // namespace utilities
