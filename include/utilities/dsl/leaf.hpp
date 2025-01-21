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
#include <utilities/dsl/detail_/leaf_holder.hpp>
#include <utility>
#include <stdexcept>

namespace utilities::dsl {

/** @brief Type-erases a node of the abstract syntax tree.
 *
 *  This class type-erases a true leaf of the AST (nodes with no descendents)
 *  or an effective leaf (node and descendents of that node are treated as
 *  a single entity).
 */
class Leaf {
private:
    /// Base type of the holder
    using leaf_holder = detail_::LeafHolderBase;

    /// Type of a pointer to the holder
    using leaf_pointer = typename leaf_holder::leaf_pointer;

public:
    /** @brief Creates an empty Leaf object.
     *
     *  The object resulting from the default ctor is equivalent to one value-
     *  initialized with a nullptr. Either way, the resulting object wraps no
     *  object and attempting to unwrap it will result in an error. Empty leaf
     *  objects are primarily meant for use as place holders.
     *
     *  @throw None No throw guarantee.
     */
    Leaf() noexcept = default;

    /** @brief Creates a Leaf which wraps the provided value.
     *
     *  This ctor will create a Leaf object from an already type-erased value.
     *  Under most circumstances users will not call this ctor directly, rather
     *  they will call it indirectly via the make_leaf function.
     *
     *  @param[in] holder A pointer (may be null) to the type-erased value *this
     *                    will wrap. If @p holder is null, *this is empty.
     *
     *  @throw None No throw guarantee.
     */
    explicit Leaf(leaf_pointer holder) : m_holder_(std::move(holder)) {}

    /** @brief Creates a new leaf by copying @p rhs.
     *
     *  This method will create a new leaf by copying @p rhs. The new leaf will
     *  contain a deep copy of the type-erased object in @p rhs if @p rhs owns
     *  its object and will alias the type-erased object in @p rhs if @p rhs
     *  aliases the object.
     *
     *  @param[in] rhs The Leaf to copy.
     *
     *  @throw std::bad_alloc if there is a problem copying @p rhs. Strong throw
     *                        guarantee.
     */
    Leaf(const Leaf& rhs) :
      m_holder_(rhs.has_value() ? rhs.m_holder_->clone() : nullptr) {}

    /** @brief Creates a new Leaf by taking the state from @p rhs.
     *
     *  This method transfers the type-erased object in @p rhs into *this. No
     *  reallocation happens and all references to the object remain valid.
     *
     *  @param[in,out] rhs The object to take the state from. After the call
     *                     @p rhs will be in a valid, but otherwise undefined
     *                     state.
     *
     *  @throw None No throw guarantee.
     */
    Leaf(Leaf&& rhs) noexcept = default;

    /** @brief Overwrites *this with a copy of @p rhs.
     *
     *  This method will release the state held in *this and replace it with
     *  a copy of @p rhs. The copy follows the same semantics as the copy
     *  ctor.
     *
     *  @param[in] rhs The object to copy.
     *
     *  @return *this after setting its state to a copy of @p rhs.
     *
     *  @throw std::bad_alloc if there is a problem copying @p rhs. Strong throw
     *                        guarantee.
     */
    Leaf& operator=(const Leaf& rhs) {
        if(this == &rhs) return *this;
        if(rhs.has_value())
            Leaf(rhs).swap((*this));
        else
            reset();
        return *this;
    }

    /** @brief Overwrites *this with the state of @p rhs.
     *
     *  This method will release the state held in *this and replace it with the
     *  state in @p rhs. After the operation *this will be in the same state
     *  as if it had been initialized by the move ctor.
     *
     *  @param[in,out] rhs The object to take the state from. After this
     *                     operation @p rhs will be in a valid, but otherwise
     *                     undefined state.
     *
     *  @throw None No throw guarantee.
     */
    Leaf& operator=(Leaf&& rhs) noexcept = default;

    /** @brief Determines if *this contains a type-erased object.
     *
     *  At any given time a Leaf object is in one of two states. Either it holds
     *  a type-erased object or it is empty. This method is used to check if
     *  *this contains an object.
     *
     *  @return True if *this contains a value and false otherwise.
     *
     *  @throw None No throw guarantee.
     */
    bool has_value() const noexcept { return static_cast<bool>(m_holder_); }

    /** @brief Provides type-safe access to the value.
     *
     *  @param T The cv-qualified type of object to return. The resulting
     *           reference will have the same cv-qualifiers as @p T.
     *
     *  Users are encouraged to unwrap Leaf objects via the unwrap_leaf free
     *  function, on account of the free function providing a nicer API. That
     *  said the free function is ultimately implemented in terms of this (and
     *  the const-version) method and users may call it directly if they want.
     *
     *  @return A possibly mutable reference to the wrapped object. The
     *          reference is mutable if @p T is NOT cv-qualified and read-only
     *          if @p T is cv-qualified.
     *
     *  @throw std::runtime_error if *this does not contain a type-erased
     *                            object or if the object is not of type @p T.
     *                            Strong throw guarantee.
     */
    template<typename T>
    decltype(auto) value() {
        const auto& rv = std::as_const(*this).template value<T>();
        if constexpr(!std::is_const_v<T>) {
            return const_cast<T&>(rv);
        } else {
            return rv;
        }
    }

    /** @brief Provides type-safe, read-only access to the value.
     *
     *  @tparam T The type to unwrap the object to. Since the resulting
     *            reference will always be const, passing @p T with
     *            cv-qualifiers is allowed, but will have no effect.
     *
     *  This method is the same as the non-const version except that the
     *  returned value is guaranteed to be read-only. See the description of
     *  the non-const version for more information.
     *
     *  @return A read-only reference to the wrapped object.
     *
     *  @throw std::runtime_error under the same conditions as the non-const
     *                            version.
     */
    template<typename T>
    const std::decay_t<T>& value() const;

    /** @brief Destroys the contained object (if *this contains an object).
     *
     *  This method wraps the process of releasing the object contained in
     *  *this, while ensuring *this itself still exists.
     *
     *  @throw None No throw guarantee.
     */
    void reset() noexcept { m_holder_ = nullptr; }

    /** @brief Exchanges the state of *this with that of @p other.
     *
     *  @param[in,out] other The object to swap state with. After this operation
     *                       @p other will contain the state which was
     *                       previously in *this.
     *
     *  @throw None No throw guarantee.
     */
    void swap(Leaf& other) { m_holder_.swap(other.m_holder_); }

    /** @brief Determines if *this and @p rhs are value equal.
     *
     *  Two leaves are value equal if:
     *  1. They both are empty, or
     *  2. They both contain objects and the objects compare value equal.
     *
     *  @param[in] rhs The leaf to compare to.
     *
     *  @return True if *this and @p rhs are value equal and false otherwise.
     *
     *  @throw None No throw guarantee.
     */
    bool operator==(const Leaf& rhs) const noexcept {
        if(has_value() != rhs.has_value()) return false;
        if(!has_value()) return true;
        return m_holder_->are_equal(*rhs.m_holder_);
    }

    /** @brief Determines if *this differs from @p rhs.
     *
     *  Two leaves differ if they are not value equal. See the description of
     *  operator== for the definition of value equal.
     *
     *  @param[in] rhs The leaf to compare to.
     *
     *  @return False if *this and @p rhs are value equal and true otherwise.
     *
     *  @throw None No throw guarantee.
     */
    bool operator!=(const Leaf& rhs) const noexcept {
        return !((*this) == rhs);
    }

private:
    /// Wraps asserting that *this holds a value
    void assert_value_() const {
        if(has_value()) return;
        throw std::runtime_error("Leaf does not wrap a value.");
    }

    /// Wraps asserting that *this can be converted to type @p T
    template<typename T>
    void assert_convertible_() const {
        assert_value_();
        if(m_holder_->template is_convertible<T>()) return;
        throw std::runtime_error("Wrapped object is not convertible to T");
    }

    /// The type-erased object
    leaf_pointer m_holder_;
};

/** @brief Wraps an object in a Leaf object.
 *
 *  @tparam T The type of the object to wrap.
 *
 *  @param[in] value The object to wrap in a Leaf.
 *
 *  @return A newly constructed Leaf object wrapping @p value.
 */
template<typename T>
auto make_leaf(T&& value) {
    using holder_type = detail_::QualifiedLeafHolder<T&&>;
    return Leaf(std::make_unique<holder_type>(std::forward<T>(value)));
}

/** @brief Extracts an object from a Leaf object.
 *
 *  @param T The cv-qualified type of the object to unwrap.
 *
 *  This free function is the preferred means of unwrapping a Leaf object. This
 *  call wraps calling `value` method of @p leaf.
 *
 *  @param[in] leaf The Leaf object to extract the object from.
 *
 *  @return The unwrapped object.
 *
 *  @throw std::runtime_error if @p leaf does not contain a value or if the
 *                            object in @p leaf can not be unwrapped to type
 *                            @p T.
 */
template<typename T, typename LeafType>
decltype(auto) unwrap_leaf(LeafType&& leaf) {
    using clean_type = std::decay_t<LeafType>;
    static_assert(std::is_same_v<clean_type, Leaf>, "Input must be a Leaf.");

    return leaf.template value<T>();
}

// -- Out of line implementations

template<typename T>
const std::decay_t<T>& Leaf::value() const {
    assert_convertible_<T>(); // Stops us from trying to do const->non-const

    using value_type  = std::decay_t<T>;
    using holder_type = detail_::LeafHolder<value_type>;
    auto p            = dynamic_cast<const holder_type*>(m_holder_.get());
    if(p != nullptr) return p->value();

    using const_holder = detail_::LeafHolder<const value_type>;
    auto const_p       = dynamic_cast<const const_holder*>(m_holder_.get());
    if(const_p != nullptr) return const_p->value();
    throw std::runtime_error("Wrapped value is not of type T");
}

} // namespace utilities::dsl
