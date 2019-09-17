#include "utilities/containers/math_set/math_set_class.hpp"
#include "utilities/containers/math_set/math_set_view.hpp"
#include "utilities/containers/math_set/operators.hpp"

namespace utilities {
namespace detail_ {

/** @brief Creates a view from the indices in the provided range.
 *
 *  @relates MathSet
 *  @relates MathSetView
 *
 *  @tparam T Expected to be either MathSet or MathSetView
 *  @tparam Itr1 The type of the iterator pointing to the first index to
 *          include.
 *  @tparam Itr2 The type of the iterator pointing to just past the last index
 *          to include.
 *
 *  @param[in] lhs The instance we are aliasing elements of.
 *  @param[in] begin An iterator pointing to the first index in a range that
 *             should be included.
 *  @param[in] end An iterator pointing to the just past the last index in a
 *             range that should be included.
 *
 *  @return A MathSetView which contains aliases of the elements in the provided
 *          range.
 */
template<typename T, typename Itr1, typename Itr2>
auto select_guts_(const T& lhs, Itr1&& begin, Itr2&& end) {
    using element_t = decltype(*begin);
    std::vector<std::reference_wrapper<const element_t>> elems;
    while(begin != end) {
        elems.push_back(std::ref(lhs[*begin]));
        ++begin;
    }
    return MathSetView<element_t>{std::move(elems)};
}

} // namespace detail_

template<typename T, typename Itr1, typename Itr2>
auto select(const MathSet<T>& lhs, Itr1&& begin, Itr2&& end) {
    return select_guts_(lhs, std::forward<Itr1>(begin),
                        std::forward<Itr2>(end));
}

template<typename T, typename Itr1, typename Itr2>
auto select(const MathSetView<T>& lhs, Itr1&& begin, Itr2&& end) {
    return select_guts_(lhs, std::forward<Itr1>(begin),
                        std::forward<Itr2>(end));
}

} // namespace utilities
