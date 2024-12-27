#pragma once
#include <memory>
#include <type_traits>

namespace utilities::dsl::detail_ {

class LeafHolderBase {
public:
    using leaf_pointer = std::unique_ptr<LeafHolderBase>;

    virtual ~LeafHolderBase() noexcept = default;

    leaf_pointer clone() const { return clone_(); }

protected:
    LeafHolderBase()                      = default;
    LeafHolderBase(const LeafHolderBase&) = default;
    LeafHolderBase(LeafHolderBase&&)      = default;

    virtual leaf_pointer clone_() const = 0;
};

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

protected:
    LeafHolder() {
        // This class shouldn't have references
        static_assert(std::is_same_v<std::remove_reference_t<T>, T>);
    }
    virtual reference value_()             = 0;
    virtual const_reference value_() const = 0;
};

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

    explicit QualifiedLeafHolder(T value) : m_value_(make_holder_(value)) {}

protected:
    QualifiedLeafHolder() = delete;

    leaf_pointer clone_() const override {
        return std::make_unique<my_type>(*this);
    }

    reference value_() override { return unwrap_holder_(m_value_); }

    const_reference value_() const override { return unwrap_holder_(m_value_); }

private:
    static holder_type make_holder_(T object) {
        if constexpr(is_lvalue) {
            return pointer(&object);
        } else {
            return value_type(std::move(object));
        }
    }

    static reference unwrap_holder_(holder_type& value) {
        if constexpr(is_lvalue) {
            return *value;
        } else {
            return value;
        }
    }

    static const_reference unwrap_holder_(const holder_type& value) {
        if constexpr(is_lvalue) {
            return *value;
        } else {
            return value;
        }
    }

    holder_type m_value_;
};
} // namespace utilities::dsl::detail_