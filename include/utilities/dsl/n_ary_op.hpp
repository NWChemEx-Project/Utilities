/*
 * Copyright 2024 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <tuple>
#include <type_traits>
#include <utilities/dsl/term.hpp>
#include <utilities/dsl/term_traits.hpp>

namespace utilities::dsl {

/** @brief Code factorization for n-ary operations.
 *
 *  @tparam DerivedType the operation *this is implementing.
 *  @tparam LHSType The const-qualified type of the object on the left side of
 *                  the operation.
 *  @tparam RHSType The const-qualified type of the object on the right side of
 *                  the operation.
 *
 *  The DSL implementation of most of the n-ary operations are the same and is
 *  implemented by this class.
 */
template<typename DerivedType, typename... Args>
class NAryOp : public Term<DerivedType> {
private:
    /// Works out the types associated with each type
    using traits_types = std::tuple<TermTraits<Args>...>;

    /// Grabs the traits for the I-th type
    template<std::size_t I>
    using traits_i_type = std::tuple_element_t<I, traits_types>;

public:
    /// Unqualified type of the I-th object
    template<std::size_t I>
    using object_type = typename traits_i_type<I>::value_type;

    /// Type acting like `object_type<I>&`, but respecting const-ness
    template<std::size_t I>
    using object_reference = typename traits_i_type<I>::reference;

    /// Type acting like `const object_type<I>&`
    template<std::size_t I>
    using const_object_reference = typename traits_i_type<I>::const_reference;

    /** @brief Creates a new N-ary operation by aliasing @p args.
     *
     *  Generally speaking n-ary operations will want to alias the arguments to
     *  the operation (as opposed to copying them or taking
     *  ownership). This ctor takes references to the objects and stores
     *  them internally as `TermTraits<Args>::holder_type...` objects. Thus
     *  whether *this ultimately owns the objects referenced by @p args is
     *  controlled by the respective specializations of `TermTraits`.
     *
     *  @param[in] args Aliases to the objects.
     *
     *  @throw ??? Throws if converting any of @p args to their holder type
     *             throws. Same throw guarantee.
     */
    template<typename... Args2>
    NAryOp(Args2&&... args) :
      m_objects_(TermTraits<std::decay_t<Args2>>::make_holder(
        std::forward<Args2>(args))...) {}

    // -------------------------------------------------------------------------
    // -- Getters and setters
    // -------------------------------------------------------------------------

    /** @brief Returns a (possibly) mutable reference to the `I`-th object in
     *         the operation.
     *
     *  *this is associated with a number of objects. This method is used to
     *  retrieve references to them.
     *
     *  @tparam I The offset of the object the user wants.
     *
     *  @return A (possibly) mutable reference to the `I`-th object. The
     *          mutable-ness of the return is controlled by TermTraits<Args>.
     *
     *  @throw ??? Throws if converting from the held type to object_reference
     *             throws. Same throw guarantee.
     */
    template<std::size_t I>
    object_reference<I> object() {
        return traits_i_type<I>::unwrap_holder(std::get<I>(m_objects_));
    }

    /** @brief Returns a read-only reference to the `I`-th object in the
     *         operation.
     *
     *  @tparam I The offset of the object the user wants.
     *
     *  This method is identical to the non-const version except that the return
     *  is guaranteed to be read-only.
     *
     *  @return A read-only reference to the `I`-th object in the
     *          operation.
     *
     *  @throw ??? Throws if converting from the held type to
     *             const_object_reference<I> throws. Same throw guarantee.
     */
    template<std::size_t I>
    const_object_reference<I> object() const {
        return traits_i_type<I>::unwrap_holder(std::get<I>(m_objects_));
    }

    // -------------------------------------------------------------------------
    // -- Utility methods
    // -------------------------------------------------------------------------

    /** @brief Is *this the same n-ary op as @p other?
     *
     *  @tparam DerivedType2 The type @p other implements.
     *  @tparam Args2 The types of the arguments in @p other.
     *
     *  Two NaryOp objects are the same if they:
     *  - Implement the same operation, e.g., both are implementing addition,
     *    and
     *  - both have the same argument values
     *
     *  It should be noted that following C++ convention, value comparisons are
     *  done with const references and thus the const-ness of @tparam Args
     *  vs the respective const-ness of @tparam Args2 is not considered.
     *
     *  @param[in] other The object to compare to.
     *
     *  @return True if *this is value equal and false otherwise.
     *
     *  @throw None No throw guarantee.
     */
    template<typename DerivedType2, typename... Args2>
    bool operator==(const NAryOp<DerivedType2, Args2...>& other) const noexcept;

    /** @brief Is *this different than @p other?
     *
     *  @tparam DerivedType2 The type @p other implements.
     *  @tparam Args2 The types of the arguments to @p other.
     *
     *  This method defines "different" as not value equal. See the description
     *  for operator== for the definition of value equal.
     *
     *  @param[in] other The object to compare to *this.
     *
     *  @return False if *this is value equal to @p other and true otherwise.
     *
     *  @throw None No throw guarantee.
     */
    template<typename DerivedType2, typename... Args2>
    bool operator!=(
      const NAryOp<DerivedType2, Args2...>& other) const noexcept {
        return !((*this) == other);
    }

private:
    /// Lets other specializations access m_objects_
    template<typename DerivedType2, typename... Args2>
    friend class NAryOp;

    /// A tuple containing the arguments to *this
    std::tuple<typename TermTraits<Args>::holder_type...> m_objects_;
};

// -----------------------------------------------------------------------------
// -- Out of line inline definitions
// -----------------------------------------------------------------------------

template<typename DerivedType, typename... Args>
template<typename DerivedType2, typename... Args2>
bool NAryOp<DerivedType, Args...>::operator==(
  const NAryOp<DerivedType2, Args2...>& other) const noexcept {
    using value_type1 = std::tuple<typename TermTraits<Args>::value_type...>;
    using value_type2 = std::tuple<typename TermTraits<Args2>::value_type...>;
    if constexpr(std::is_same_v<value_type1, value_type2>) {
        return m_objects_ == other.m_objects_;
    } else {
        return false;
    }
}

} // namespace utilities::dsl