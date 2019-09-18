#pragma once
#include "utilities/containers/math_set/math_set_class.hpp"
#include "utilities/containers/math_set/math_set_view.hpp"

namespace utilities {
namespace detail_ {

/** @brief Implements operator== and operator!= for MathSet and MathSetView
 *
 *  @relates MathSet
 *  @relates MathSetView
 *
 *  This function is code factorization for checking if a MathSet or a
 *  MathSetView contains the same elements as another MathSet or MathSetView.
 *  Equality is defined as having the same number of elements, and having
 *  `lhs[i] == rhs[i]` hold true for all i in the range [0, size()).
 *
 *  @tparam T The type of the container on the left side of the operator.
 *            Assumed to be a cv-qualified MathSet or MathSetView.
 *  @tparam U The type of the container on the right side of the operator.
 *            Assumed to be a cv-qualified MathSet or MathSetView.
 *
 *  @param[in] lhs The MathSet/MathSetView on the left side of the operator.
 *  @param[in] rhs The MathSet/MathSetView on the right side of the operator.
 *
 *  @return True if @p lhs has the same number of elements as @p rhs and
 *          compares element-wise equivalent. False otherwise.
 *
 *  @throw ??? if the element's comparison operator throws same throw guarantee.
 */
template<typename T, typename U>
static auto are_equal_guts_(T&& lhs, U&& rhs) {
    if(lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/** @brief Implements MathSet::operator^ and MathSetView::operator^
 *
 *  @relates MathSet
 *  @relates MathSetView
 *
 *  This function is code factorization for finding the intersection of two
 *  MathSet or MathSetView instances. This function guarantees that all
 *  references in the returned view are to the elements in @p lhs.
 *
 *  @tparam T The type of the container on the left of the operator. Assumed to
 *            be either a MathSet or MathSetView instance.
 *  @tparam U The type of the container on the right of the operator. Assumed to
 *            be either a MathSet or MathSetView instance.
 * @param[in] lhs The MathSet/MathSetView instance on the left of the operator.
 * @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 * @return A MathSetView containing aliases of the elements in @p lhs that also
 *        appear in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename T, typename U>
auto intersection_guts_(T&& lhs, U&& rhs) {
    using value_type      = typename std::remove_reference_t<T>::value_type;
    using const_reference = std::reference_wrapper<const value_type>;
    std::vector<const_reference> idxs;
    for(auto& x : lhs)
        if(rhs.count(x)) idxs.push_back(std::ref(x));
    return MathSetView{idxs.begin(), idxs.end()};
}

/** @brief Implements MathSet::operator+ and MathSetView::operator+
 *
 *  @relates MathSet
 *  @relates MathSetView
 *
 *  This function is code factorization for finding the union of two
 *  MathSet or MathSetView instances. This function guarantees that the
 *  resulting view is made up of references to the elements in @p lhs followed
 *  by unique elements in @p rhs.
 *
 *  @tparam T The type of the container on the left of the operator. Assumed to
 *            be either a MathSet or MathSetView instance.
 *  @tparam U The type of the container on the right of the operator. Assumed to
 *            be either a MathSet or MathSetView instance.
 * @param[in] lhs The MathSet/MathSetView instance on the left of the operator.
 * @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 * @return A MathSetView containing aliases of the elements in @p lhs and the
 *         unique elements appearing in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename T, typename U>
auto union_guts_(T&& lhs, U&& rhs) {
    using value_type      = typename std::remove_reference_t<T>::value_type;
    using const_reference = std::reference_wrapper<const value_type>;
    std::vector<const_reference> idxs;
    for(auto& x : lhs) idxs.push_back(std::ref(x));
    for(auto& x : rhs)
        if(!lhs.count(x)) idxs.push_back(std::cref(x));
    return MathSetView{idxs.begin(), idxs.end()};
}

/** @brief Implements MathSet::operator- and MathSetView::operator-
 *
 *  @relates MathSet
 *  @relates MathSetView
 *
 *  This function is code factorization for finding the difference of two
 *  MathSet or MathSetView instances. This function guarantees that all
 *  references in the returned view are to the elements in @p lhs.
 *
 *  @tparam T The type of the container on the left of the operator. Assumed to
 *            be either a MathSet or MathSetView instance.
 *  @tparam U The type of the container on the right of the operator. Assumed to
 *            be either a MathSet or MathSetView instance.
 * @param[in] lhs The MathSet/MathSetView instance on the left of the operator.
 * @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 * @return A MathSetView containing aliases of the elements in @p lhs that are
 *         not in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename T, typename U>
auto set_difference_guts_(T&& lhs, U&& rhs) {
    using value_type      = typename std::remove_reference_t<T>::value_type;
    using const_reference = std::reference_wrapper<const value_type>;
    std::vector<const_reference> idxs;
    for(auto& x : lhs)
        if(!rhs.count(x)) idxs.push_back(std::ref(x));
    return MathSetView{idxs.begin(), idxs.end()};
}

} // namespace detail_

/** @brief Determines if a MathSet is equal to another MathSet or MathSetView
 *
 *  @relates MathSet
 *
 *  This function checks if a MathSet contains the same elements as another
 *  MathSet or MathSetView. Equality is defined as having the same number of
 *  elements, and having `lhs[i] == rhs[i]` hold true for all i in the range
 *  [0, size()).
 *
 *  @tparam ElementType The type of the element in @p lhs.
 *  @tparam RHS The type of the container on the right side of the operator.
 *              Assumed to be a cv-qualified MathSet or MathSetView.
 *
 *  @param[in] lhs The MathSet on the left side of the operator.
 *  @param[in] rhs The MathSet/MathSetView on the right side of the operator.
 *
 *  @return True if @p lhs has the same number of elements as @p rhs and
 *          compares element-wise equivalent. False otherwise.
 *
 *  @throw ??? if the element's comparison operator throws same throw guarantee.
 */
template<typename ElementType, typename RHS>
bool operator==(const MathSet<ElementType>& lhs, RHS&& rhs) {
    return detail_::are_equal_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Determines if a MathSetView is equal to another MathSet or
 *         MathSetView
 *
 *  @relates MathSetView
 *
 *  This function checks if a MathSetView contains the same elements as another
 *  MathSet or MathSetView. Equality is defined as having the same number of
 *  elements, and having `lhs[i] == rhs[i]` hold true for all i in the range
 *  [0, size()).
 *
 *  @tparam ElementType The type of the element in @p lhs.
 *  @tparam RHS The type of the container on the right side of the operator.
 *              Assumed to be a cv-qualified MathSet or MathSetView.
 *
 *  @param[in] lhs The MathSetView on the left side of the operator.
 *  @param[in] rhs The MathSet/MathSetView on the right side of the operator.
 *
 *  @return True if @p lhs has the same number of elements as @p rhs and
 *          compares element-wise equivalent. False otherwise.
 *
 *  @throw ??? if the element's comparison operator throws same throw guarantee.
 */
template<typename ElementType, typename RHS>
bool operator==(const MathSetView<ElementType>& lhs, RHS&& rhs) noexcept {
    return detail_::are_equal_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Determines if a MathSet is different from another MathSet or
 *         MathSetView
 *
 *  This function checks if a MathSet contains different elements from another
 *  MathSet or MathSetView. Equality is defined as having the same number of
 *  elements, and having `lhs[i] == rhs[i]` hold true for all i in the range
 *  [0, size()) and different is define as not equal.
 *
 *  @tparam ElementType The type of the element in @p lhs.
 *  @tparam RHS The type of the container on the right side of the operator.
 *              Assumed to be a cv-qualified MathSet or MathSetView.
 *
 *  @param[in] lhs The MathSet on the left side of the operator.
 *  @param[in] rhs The MathSet/MathSetView on the right side of the operator.
 *
 *  @return False if @p lhs is equal to @p rhs and true otherwise.
 *
 *  @throw ??? if the element's comparison operator throws same throw guarantee.
 */
template<typename ElementType, typename RHS>
bool operator!=(const MathSet<ElementType>& lhs, RHS&& rhs) noexcept {
    return !detail_::are_equal_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Determines if a MathSetView is different from another MathSet or
 *         MathSetView
 *
 *  This function checks if a MathSetView contains different elements from
 *  another MathSet or MathSetView. Equality is defined as having the same
 *  number of elements, and having `lhs[i] == rhs[i]` hold true for all i in the
 *  range [0, size()) and different is define as not equal.
 *
 *  @tparam ElementType The type of the element in @p lhs.
 *  @tparam RHS The type of the container on the right side of the operator.
 *              Assumed to be a cv-qualified MathSet or MathSetView.
 *
 *  @param[in] lhs The MathSetView on the left side of the operator.
 *  @param[in] rhs The MathSet/MathSetView on the right side of the operator.
 *
 *  @return False if @p lhs is equal to @p rhs and true otherwise.
 *
 *  @throw ??? if the element's comparison operator throws same throw guarantee.
 */
template<typename ElementType, typename RHS>
bool operator!=(const MathSetView<ElementType>& lhs, RHS&& rhs) noexcept {
    return !detail_::are_equal_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Computes the intersection of a MathSet and a MathSet/MathSetView
 *
 *  @relates MathSet
 *
 *  This function finds the intersection of two MathSet or MathSetView
 *  instances. This function guarantees that all  references in the returned
 *  view are to the elements in @p lhs.
 *
 *  @tparam ElementType The type of the elements in @p lhs.
 *  @tparam RHS The type of the container on the right of the operator. Assumed
 *              to be either a MathSet or MathSetView instance.
 *  @param[in] lhs The MathSet instance on the left of the operator.
 *  @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 *  @return A MathSetView containing aliases of the elements in @p lhs that also
 *          appear in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename ElementType, typename RHS>
auto operator^(const MathSet<ElementType>& lhs, RHS& rhs) {
    return detail_::intersection_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Computes the intersection of a MathSetView and a MathSet/MathSetView
 *
 *  @relates MathSetView
 *
 *  This function finds the intersection of two MathSet or MathSetView
 *  instances. This function guarantees that all  references in the returned
 *  view are to the elements in @p lhs.
 *
 *  @tparam ElementType The type of the elements in @p lhs.
 *  @tparam RHS The type of the container on the right of the operator. Assumed
 *              to be either a MathSet or MathSetView instance.
 *  @param[in] lhs The MathSetView instance on the left of the operator.
 *  @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 *  @return A MathSetView containing aliases of the elements in @p lhs that also
 *          appear in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename ElementType, typename RHS>
auto operator^(const MathSetView<ElementType>& lhs, RHS&& rhs) {
    return detail_::intersection_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Takes the union of a MathSet and another MathSet/MathSetView
 *
 *  @relates MathSet
 *
 *  This function finds the union of a MathSet and another MathSet/MathSetView.
 *  This function guarantees that the resulting view is made up of references to
 *  the elements in @p lhs followed by unique elements in @p rhs.
 *
 *  @tparam ElementType The type of the elements in @p lhs.
 *  @tparam RHS The type of the container on the right of the operator. Assumed
 * to be either a MathSet or MathSetView instance.
 * @param[in] lhs The MathSet instance on the left of the operator.
 * @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 * @return A MathSetView containing aliases of the elements in @p lhs and the
 *         unique elements appearing in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename ElementType, typename RHS>
auto operator+(const MathSet<ElementType>& lhs, RHS&& rhs) {
    return detail_::union_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Takes the union of a MathSetView and another MathSet/MathSetView
 *
 *  @relates MathSetView
 *
 *  This function finds the union of a MathSetView and another
 *  MathSet/MathSetView. This function guarantees that the resulting view is
 *  made up of references to  the elements in @p lhs followed by unique elements
 *  in @p rhs.
 *
 *  @tparam ElementType The type of the elements in @p lhs.
 *  @tparam RHS The type of the container on the right of the operator. Assumed
 * to be either a MathSet or MathSetView instance.
 *  @param[in] lhs The MathSetView instance on the left of the operator.
 *  @param[in] rhs The MathSet/MathSetView instance on the right of the
 *                 operator
 *
 * @return A MathSetView containing aliases of the elements in @p lhs and the
 *         unique elements appearing in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename ElementType, typename RHS>
auto operator+(const MathSetView<ElementType>& lhs, RHS&& rhs) {
    return detail_::union_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Computes the difference of a MathSet and a MathSet/MathSetView
 *
 *  @relates MathSet
 *
 *  This function finds the difference of two MathSet or MathSetView
 *  instances. This function guarantees that all  references in the returned
 *  view are to the elements in @p lhs.
 *
 *  @tparam ElementType The type of the elements in @p lhs.
 *  @tparam RHS The type of the container on the right of the operator. Assumed
 *              to be either a MathSet or MathSetView instance.
 *  @param[in] lhs The MathSet instance on the left of the operator.
 *  @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 *  @return A MathSetView containing aliases of the elements in @p lhs that are
 *          not in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename ElementType, typename RHS>
auto operator-(const MathSet<ElementType>& lhs, RHS& rhs) {
    return detail_::set_difference_guts_(lhs, std::forward<RHS>(rhs));
}

/** @brief Computes the difference of a MathSetView and a MathSet/MathSetView
 *
 *  @relates MathSetView
 *
 *  This function finds the difference of two MathSet or MathSetView
 *  instances. This function guarantees that all  references in the returned
 *  view are to the elements in @p lhs.
 *
 *  @tparam ElementType The type of the elements in @p lhs.
 *  @tparam RHS The type of the container on the right of the operator. Assumed
 *              to be either a MathSet or MathSetView instance.
 *  @param[in] lhs The MathSetView instance on the left of the operator.
 *  @param[in] rhs The MathSet/MathSetView instance on the right of the operator
 *
 *  @return A MathSetView containing aliases of the elements in @p lhs that are
 *          not in @p rhs.
 *
 * @throw std::bad_alloc if there is insufficient memory to create the new
 *        MathSetView. Strong throw guarantee.
 */
template<typename ElementType, typename RHS>
auto operator-(const MathSetView<ElementType>& lhs, RHS&& rhs) {
    return detail_::set_difference_guts_(lhs, std::forward<RHS>(rhs));
}

} // namespace utilities
