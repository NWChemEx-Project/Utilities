#pragma once
#include <memory>
#include <type_traits>

namespace utilities::dsl::detail_ {

/** @brief Defines API for interacting with type-erased leaf objects.
 *
 *  Leaves of the DSL are held in a type-erased state. This class defines the
 *  interface for interacting with the type-erased object when the caller does
 *  not need to know the type of the object.
 */
class LeafHolderBase {
public:
    /// Type of a pointer to a LeafHolderBase object
    using leaf_pointer = std::unique_ptr<LeafHolderBase>;

    /// Defaulted polymorphic dtor
    virtual ~LeafHolderBase() noexcept = default;

    /// Deep polymorphic copy ctor
    leaf_pointer clone() const { return clone_(); }

    /// Polymorphic value comparison
    bool are_equal(const LeafHolderBase& rhs) const noexcept {
        return are_equal_(rhs) && rhs.are_equal_(*this);
    }

    /** @brief Used to determine if *this contains an object of type @p T.
     *
     *  @tparam The cv-qualified type to look for.
     *
     *  This method is used to check if the type-erased object in *this is of
     *  type @p T. Note that this is different than trying to determine if the
     *  type-erased object can be converted to an object of type @p T, e.g., if
     *  *this contains an object of type `int` and the user sets @p T to
     *  `const int` this method will return false since `int` and `const int`
     *  are not the same type, even though `int` is implicitly convertible to
     *  `const int`.
     *
     *  @return True if *this is type-erasing an object of type @p T and false
     *          otherwise.
     */
    template<typename T>
    bool contains_type() const noexcept {
        return contains_type_(std::is_const_v<T>, typeid(T));
    }

    /** @brief Determines if *this contains an object convertible to type @p T.
     *
     *  @param T The cv-qualified type to compare the wrapped object's type to.
     *
     *  This method is similar to contains_type, but additionally considers
     *  whether the cv-qualifiers on @p T are consistent with those of the
     *  wrapped object, e.g., if *this holds an object of type `int` and the
     *  user calls this method with @p T set to `const int` the result would be
     *  true.
     *
     *  @return True if the wrapped object can be returned as a reference to an
     *          object of type @p T and false otherwise.
     *
     *  @throw None No throw guarantee.
     */
    template<typename T>
    bool is_convertible() const noexcept {
        return is_convertible_(std::is_const_v<T>, typeid(T));
    }

protected:
    /// Type used for passing runtime time information (RTTI)
    using rtti_type = std::type_info;

    /// Instances of LeafHolderBase will always be created by derived class
    ///@{
    LeafHolderBase()                                 = default;
    LeafHolderBase(const LeafHolderBase&)            = default;
    LeafHolderBase(LeafHolderBase&&)                 = delete;
    LeafHolderBase& operator=(const LeafHolderBase&) = delete;
    LeafHolderBase& operator=(LeafHolderBase&&)      = delete;
    ///@}

    /// Derived class overrides to implement clone. See clone description.
    virtual leaf_pointer clone_() const = 0;

    /// Derived class overrides to implement are_equal. See are_equal desc.
    virtual bool are_equal_(const LeafHolderBase& rhs) const noexcept = 0;

    /// Derived class overrides to implement contains_type
    virtual bool contains_type_(bool is_const,
                                const rtti_type& type) const noexcept = 0;

    /// Derived class overrides to implement is_convertible
    virtual bool is_convertible_(bool is_const,
                                 const rtti_type& type) const noexcept = 0;
};

/** @brief API for interacting with the leaf when its type is known.
 *
 *  @tparam T The possibly cv-qualified type of the object being held.
 *
 *  This layer knows the type of the leaf, including cv-qualifiers. It does not
 *  however, know whether we aliased or copied the original object.
 */
template<typename T>
class LeafHolder : public LeafHolderBase {
public:
    /// Does @p T include const?
    static constexpr bool is_const = std::is_const_v<T>;

    /// Type of @p T with cv and reference qualifiers removed
    using value_type = std::decay_t<T>;

    /// Type of a read-only reference to an object of type value_type
    using const_reference = const value_type&;

    /** @brief Type of a (possibly) mutable reference to a value_type object.
     *
     *  If `!is_const` then this will be a mutable reference to an object of
     *  value_type. If `is_const` then the reference will be read-only. Thus
     *  this typedef ensures const-consistness with @p T.
     */
    using reference =
      std::conditional_t<is_const, const_reference, value_type&>;

    /// Type of a read-only pointer to an object of type value_type
    using const_pointer = const value_type*;

    /** @brief Type of a (possibly) mutable pointer to an object of value_type.
     *
     *  If `!is_const` then this will be a mutable pointer to an object of
     *  value_type. If `is_const` then the pointer will be read-only. Thus
     *  this typedef ensures const-consistness with @p T.
     */
    using pointer = std::conditional_t<is_const, const_pointer, value_type*>;

    /** @brief Provides a (possibly) mutable reference to the wrapped object.
     *
     *  @return A (possibly) mutable reference to the wrapped object. The
     *          reference is mutable if @p T is not const-qualified.
     *
     *  @throw None No throw guarantee.
     */
    reference value() { return value_(); }

    /** @brief Provides a read-only reference to the wrapped object.
     *
     *  This method is the same as the non-const method, aside from guaranteeing
     *  that the resulting reference is read-only.
     *
     *  @return A read-only reference to the wrapped object.
     *
     *  @throw None No throw guarantee.
     */
    const_reference value() const { return value_(); }

protected:
    LeafHolder() {
        // This class shouldn't have references
        static_assert(std::is_same_v<std::remove_reference_t<T>, T>);
    }

    /// Implements are_equal by downcasting and comparing returns of value()
    bool are_equal_(const LeafHolderBase& rhs) const noexcept override {
        using mutable_type = LeafHolder<value_type>;
        auto prhs          = dynamic_cast<const mutable_type*>(&rhs);
        if(prhs != nullptr) return value() == prhs->value();

        using immutable_type = LeafHolder<const value_type>;
        auto pconst_rhs      = dynamic_cast<const immutable_type*>(&rhs);
        if(pconst_rhs == nullptr) return false;
        return value() == pconst_rhs->value();
    }

    /// Implements contains_type by comparing template parameter to RTTI
    bool contains_type_(bool should_be_const,
                        const rtti_type& type) const noexcept override {
        return is_const == should_be_const && typeid(T) == type;
    }

    bool is_convertible_(bool should_be_const,
                         const rtti_type& type) const noexcept override {
        // If !should_be_const (i.e., wants mutable) then we must wrap mutable
        if(!should_be_const) return contains_type_(false, type);

        // otherwise they want read-only and all that matters is the type.
        return contains_type_(is_const, type);
    }

    /// Derived class should override to implement value
    ///@{
    virtual reference value_()             = 0;
    virtual const_reference value_() const = 0;
    ///@}
};

/** @brief Leaf holder accounting for aliasing vs. copying.
 *
 *  @tparam T The cv- and reference-qualified type of the object.
 *
 *  The DSL needs to ensure that temporary objects remain in scope until the DSL
 *  is fully evaluated. This requires tracking the type of the original leaf
 *  object, which is the purpose of this layer of the holder.
 */
template<typename T>
class QualifiedLeafHolder : public LeafHolder<std::remove_reference_t<T>> {
private:
    /// Type of *this
    using my_type = QualifiedLeafHolder<T>;

    /// Type of T w/o reference
    using no_reference_type = std::remove_reference_t<T>;

    /// Type *this derives from
    using base_type = LeafHolder<no_reference_type>;

    /// Is @p T an rvalue reference
    static constexpr bool is_rvalue = std::is_rvalue_reference_v<T>;

    /// Is @p T an lvalue reference
    static constexpr bool is_lvalue = std::is_lvalue_reference_v<T>;

public:
    /// Pull in types from base class
    ///@{
    using typename base_type::const_reference;
    using typename base_type::leaf_pointer;
    using typename base_type::pointer;
    using typename base_type::reference;
    using typename base_type::value_type;
    ///@}

    /// Type used to hold @p T
    using holder_type = std::conditional_t<is_lvalue, pointer, value_type>;

    /// Creates a holder which alias @p value if @p T is an lvalue type.
    explicit QualifiedLeafHolder(T value) :
      m_value_(make_holder_(std::forward<T>(value))) {}

    /// Determines if the value in *this compares equal to the value in @p rhs.
    bool operator==(const QualifiedLeafHolder& rhs) const noexcept {
        return m_value_ == rhs.m_value_;
    }

    /// Determines if the value in *this is different than the value in @p rhs.
    bool operator!=(const QualifiedLeafHolder& rhs) const noexcept {
        return !((*this) == rhs);
    }

protected:
    /// Deleted to help avoid having empty holders
    QualifiedLeafHolder() = delete;

    /// Implements clone by calling object's copy ctor
    leaf_pointer clone_() const override {
        return std::make_unique<my_type>(*this);
    }

    /// Implements value by appropriately unwrapping the holder
    reference value_() override { return unwrap_holder_(m_value_); }

    /// Implements value by appropriately unwrapping the holder
    const_reference value_() const override { return unwrap_holder_(m_value_); }

private:
    /// Wraps the process of putting @p object into a holder object.
    static holder_type make_holder_(T object) {
        if constexpr(is_lvalue) {
            return pointer(&object);
        } else {
            return value_type(std::move(object));
        }
    }

    /// Wraps the process of getting an object out of holder @p value
    static reference unwrap_holder_(holder_type& value) {
        if constexpr(is_lvalue) {
            return *value;
        } else {
            return value;
        }
    }

    /// Wraps the process of getting an object out of holder @p value
    static const_reference unwrap_holder_(const holder_type& value) {
        if constexpr(is_lvalue) {
            return *value;
        } else {
            return value;
        }
    }

    /// The actual object
    holder_type m_value_;
};
} // namespace utilities::dsl::detail_