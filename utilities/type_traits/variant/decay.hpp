#pragma once
#include <type_traits> // for std::decay_t
#include <variant>     // for std::variant

namespace utilities::type_traits::variant {
namespace detail_ {

/** @brief Primary template for removing qualifiers from types in a std::variant
 *
 *  The primary template is selected when @p T is not an std::variant. Since the
 *  primary template is not defined, a compilation error will occur.
 *
 *  @tparam T The std::variant whose types are being cleaned up. If the primary
 *            template is selected @p T is not actually an std::variant.
 */
template<typename T>
struct Decay;

/** @brief Specializes Decay for when the template parameter is an std::variant
 *
 *  This struct contains a member `type` which will be a typedef of an
 *  std::variant with the same types as in @p Args except that each type will be
 *  stripped of all cv-qualifiers and references.
 *
 *  @note This actually calls std::decay on each type so if the types contain
 *        function types, those become function pointers. And if it contains
 *        array types, those become pointers.
 *
 *  @tparam Args The parameter pack of types we are cleaning up.
 */
template<typename... Args>
struct Decay<std::variant<Args...>> {
    using type = std::variant<std::decay_t<Args>...>;
};

} // namespace detail_

/** @brief Public API for removing qualifiers from the types in a std::variant
 *
 *  This type trait can be used to remove references and cv-qualifiers from the
 *  types in a std::variant.
 *
 *  @note This actually calls std::decay on each type so if the types contain
 *        function types, those become function pointers. And if it contains
 *        array types, those become pointers.
 *
 *  @tparam T The std::variant we are removing references and cv-qualifiers
 *            from. If @p T is not a std::variant a compiler error will occur.
 */
template<typename T>
using decay_t = typename detail_::Decay<T>::type;

} // namespace utilities::type_traits::variant
