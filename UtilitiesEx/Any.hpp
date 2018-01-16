#pragma once
#include <memory>
#include <type_traits>

namespace UtilitiesEx {

/** @brief The Any class is capable of holding an instance of any type in a
 *  type-safe manner.
 *
 *  The primary motivation for this class is sometimes you just want to store a
 *  bunch of objects, of different types, in the same container.  Since in C++
 *  types do not all inherit from some common type (like Python's Object class
 *  for example) the Any class provides an mock common base class.  Ultimately
 *  this class works by type-erasure.
 *
 *  To use this class:
 *  @code
 *  //To put value into the Any instance
 *  double value = 2.0;
 *  Any wrapped_value(value);
 *
 *  //To get the value back
 *  double retrieved_value = AnyCast<double>(wrapped_value);
 *  @endcode
 *
 *  @note C++17 provides an Any class and should be used instead when C++17 is
 *  allowed by the project.
 *
 *  @note Interestingly the universal reference constructor takes precedence
 *  over the copy constructor, hence the meta-template programming.
 *
 */
class Any {
private:
    /** @brief Class for determining whether or not a type is derived from Any.
     *
     *  If @p T is derived from Any (or is an Any), then the resulting typedef,
     *  is_related, will contain a bool member variable called "value" set to
     *  true, otherwise "value" is false.
     *
     *
     *  @tparam T The type to check for inheritance.
     */
    template<typename T>
    using is_related=
        std::is_base_of<Any, typename std::remove_reference<T>::type>;

    /** @brief Class for disabling a function via SFINAE if the input is derived
     *  from Any.
     *
     *  If @p T is derived from Any (or is an Any), then the resulting typedef,
     *  disable_if_related, will contain a member typedef "type"
     *  set that is an alias for void, otherwise "type" will not exist.
     *
     *
     *  @tparam T The type to check for inheritance.
     */
    template<typename T>
    using disable_if_related =
        typename std::enable_if<!is_related<T>::value>::type;
public:
    /** @brief Makes an empty Any instance.
     *
     *  The resulting Any instance wraps no object.  An object can be added to
     *  it by calling emplace.
     *
     *  @throw None. No throw guarantee.
     */
    Any()noexcept=default;

    /**
     * @brief Deep copies an already existing Any instance.
     *
     * @note If the type of the wrapped object is pointer-like the resulting Any
     * instance will contain a different pointer containing the same memory
     * address.
     *
     * @param rhs The Any instance to copy
     *
     * @throw std::bad_alloc if there is insufficient memory to copy the
     * instance stored in @p rhs.  Strong throw guarantee.
     */
    Any(const Any& rhs):
            ptr_(std::move(rhs.ptr_->clone()))
    {}

    /**
     * @brief Sets the current instance to a deep copy of another instance.
     *
     * @note If the type of the wrapped object is pointer-like the resulting Any
     * instance will contain a different pointer containing the same memory
     * address.
     * @param rhs The instance to deep copy.
     * @return The current instance containing a deep copy of @p rhs's state.
     * @throws std::bad_alloc if there is insufficient memory to copy the
     * instance stored in @p rhs.  Strong throw guarantee.
     */
    Any& operator=(const Any& rhs)
    {
        if(this != &rhs)
            ptr_ = std::move(rhs.ptr_->clone());
        return *this;
    }

    /**
     * @brief Causes the current Any instance to take ownership of another
     * instance.
     *
     * @param rhs The Any instance to take ownership of. After calling this
     * constructor @p rhs is in a valid, but undefined state.
     *
     * @throws None. No throw guarantee.
     */
    Any(Any&& rhs)noexcept=default;

    /**
     * @brief Sets the current instance to the state of another Any instance.
     *
     * @param rhs The Any instance we are taking control of.  After this call
     * @p rhs is in a valid, but otherwise undefined state.
     *
     * @return The current instance possessing the state of @p rhs.
     * @throw None. No throw guarantee.
     */
    Any& operator=(Any&& rhs)noexcept=default;

    /**
     * @brief Used to construct an Any instance holding a particular value.
     *
     * This is the constructor meant to be primarily used.
     *
     * @tparam T The type of instance to wrap.  Must meet the concept of
     * copyable.
     * @tparam X A dummy type used to prevent this function from
     * participating in resolution if T is related to Any.
     * @param value An instance to wrap.
     * @throws std::bad_alloc if memory allocation fails.  Strong throw
     * guarantee.
     * @throws ??? If the copy constructor of @p T throws given @p value.
     * Same guarantee as T's constructor.
     */
    template<typename T, typename X = disable_if_related<T>>
    explicit Any(T&& value):
            ptr_(std::move(wrap_ptr<T>(std::forward<T>(value))))
    {}

    /**
     * @brief Releases the wrapped memory associated with the present Any
     * instance.
     *
     * Any instances own the memory they are wrapping.  This member function can
     * be used to release the memory before the Any instance goes out of scope.
     *
     * @throw None. No throw guarantee.
     */
    void reset()noexcept
    {
        ptr_.reset();
    }

    /**
     * @brief Swaps the state of two Any instances.
     *
     * @param other The Any instance to swap contents with.
     * @throw None. No throw guarantee.
     */
    void swap(Any& other)noexcept
    {
        ptr_.swap(other.ptr_);
    }

    /**
     * @brief Returns true if the current Any instance is presently wrapping a
     * value.
     *
     * @return True if the current instance has allocated memory and False
     * otherwise.
     * @throw None. No throw guarantee.
     */
    bool has_value()const noexcept
    {
        return static_cast<bool>(ptr_);
    }

    /**
     * @brief Initializes the wrapped value by forwarding the provided arguments
     * to the wrapped value's constructor.
     *
     * @tparam T The type of the object to wrap.
     * @tparam Args The types of the arguments to forward to the @p T's
     * constructor.
     * @param args The arguments to forward to @p T's constructor.
     * @return The wrapped instance by reference.
     * @throw std::bad_alloc if there is insufficient memory.  Strong throw
     * guarantee.
     * @throw ??? If @p T's constructor throws given @p args.  Same guarantee
     * as T's constructor.
     */
    template<typename T, typename...Args>
    std::decay_t<T>& emplace(Args&&...args)
    {
        using no_cv = std::decay_t<T>;
        ptr_ = wrap_ptr<no_cv>(std::forward<Args>(args)...);
        return cast<no_cv>();
    };

private:
    ///Allows AnyCast to return the wrapped value
    template<typename T>
    friend T& AnyCast(Any&);

    ///Defines API for handling the data
    struct AnyBase_
    {
        ///Ensures the data gets deleted correctly
        virtual ~AnyBase_()=default;
        ///Ensures we can copy without slicing
        virtual std::unique_ptr<AnyBase_> clone()=0;
    };

    ///Implements AnyBase_ for type T
    template<typename T>
    struct AnyWrapper_ : public AnyBase_
    {
        ///Constructor copies the value
        AnyWrapper_(const T& value_in):
                value(value_in)
        {}

        ///Constructor simply moves the value
        AnyWrapper_(T&& value_in):
                value(std::move(value_in))
        {}

        ///The actual wrapped value
        T value;

        /** @brief Polymorphic copy function that returns a new instance of the
         *  wrapped object.
         *
         *  @throw std::bad_alloc if there is insufficient memory to copy.
         *  Strong throw guarantee.
         *  @throw ??? If @param T's copy constructor throws.  Same guarantee as
         *  T's copy constructor.
         */
        std::unique_ptr<AnyBase_> clone() override
        {
            return std::move(std::make_unique<AnyWrapper_<T>>(value));
        }
    };

    ///Code factorization for the internal process of wrapping a value
    template<typename T, typename...Args>
    std::unique_ptr<AnyBase_> wrap_ptr(Args&&...args)
    {
        using no_cv =std::decay_t<T>;
        static_assert(std::is_copy_constructible<no_cv>::value,
                      "Only copy constructable objects may be assigned to Any"
                      " instances");
        using result_t = AnyWrapper_<no_cv>;
        return std::move(
                std::make_unique<result_t>(std::forward<Args>(args)...)
        );
    }

    ///Actually implements the cast, private to match STL API
    template<typename T>
    T& cast()
    {
        return dynamic_cast<Any::AnyWrapper_<T>&>(*ptr_).value;
    }

    ///The actual type-erased value
    std::unique_ptr<AnyBase_> ptr_;
};

/**
 * @brief Provides access to the value wrapped in an Any instance.
 * @relates Any
 * @tparam T The type to cast the Any instance to.
 * @param wrapped_value An any instance containing a value.
 * @return The value wrapped by @p wrapped_value.
 * @throw std::bad_cast if the value wrapped by @p wrapped_value is not
 * convertible to type @p T.  Strong throw guarantee.
 */
template<typename T>
T& AnyCast(Any& wrapped_value)
{
    return wrapped_value.cast<T>();
}

///@copydoc T& AnyCast(Any&)
template<typename T>
const T& AnyCast(const Any& wrapped_type)
{
    return const_cast<Any&>(wrapped_type).cast<T>();
}

/** @brief Makes an Any instance by forwarding the arguments to the wrapped
 *  instance's constructor.
 *
 *  This is a convenience function for directly populating an Any instance so
 *  that the user does not have to first construct a temporary and then forward
 *  that temporary to an Any instance.
 *
 *  @relates Any
 *
 *  @param args The arguments to forward to @p T's constructor.
 *  @param T The type of the object we are going to wrap.
 *  @param Args The types of the arguments that are being forwarded.
 *  @return An any instance constructed with the current arguments.
 *
 *  @throw std::bad_alloc if there is insufficient memory for the resulting
 *  instance.  Strong throw guarantee.
 *  @throw ??? If @p T's constructor throws.  Same guarantee as T's constructor.
 *
 */
template<typename T, typename...Args>
Any MakeAny(Args&&...args)
{
    return Any(std::move(T(std::forward<Args>(args)...)));
};

}//End namespace
