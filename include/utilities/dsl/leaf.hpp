#pragma once
#include <utilities/dsl/detail_/leaf_holder.hpp>

namespace utilities::dsl {

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

    void swap(Leaf& other) { m_holder_.swap(other.m_holder_); }

    bool operator==(const Leaf& rhs) noexcept {
        if(has_value() != rhs.has_value()) return false;
        if(!has_value()) return true;
        return m_holder_->are_equal(*rhs.m_holder_);
    }

    bool operator!=(const Leaf& rhs) noexcept { return !((*this) == rhs); }

private:
    void assert_value_() const {
        if(has_value()) return;
        throw std::runtime_error("Leaf does not wrap a value.");
    }

    template<typename T>
    void assert_convertible_() const {
        assert_value_();
        if(m_holder_->template is_convertible<T>()) return;
        throw std::runtime_error("Wrapped object is not convertible to T");
    }

    leaf_pointer m_holder_;
};

template<typename T>
auto make_leaf(T&& value) {
    using holder_type = detail_::QualifiedLeafHolder<T&&>;
    return Leaf(std::make_unique<holder_type>(std::forward<T>(value)));
}

template<typename T>
auto unwrap_leaf(Leaf& leaf) {
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