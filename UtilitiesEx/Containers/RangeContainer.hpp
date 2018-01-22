#pragma once

#include <limits> // For std::numeric_limits<size_type>
#include <tuple> //  For std::tie
#include <vector>

namespace UtilitiesEx {
namespace detail_ {

/** @brief Simulates a container filled all elements in a range.
 *
 *  This class is largely code factorization from the various IterTools
 *  containers. It contains no actual state aside from copies of the starting
 *  and ending iterators (i.e. it does not actually store every element in the
 *  range like it pretends to).  Consequentially all iterators to this container
 *  are constant iterators as they can not mutate the container.
 *
 *  This class implements most of the container API for the derived class.  The
 *  derived class is only responsible for the constructors.  That said instances
 *  of RangeContainers are never meant to be instantiated by the user.
 *
 *  @todo It's possible (maybe with a bit of tweaking) to remove some of the
 *  boilerplate in IterTools classes using this class as a base class.
 *
 *  @tparam IteratorType The type of iterator used to generate the elements.  We
 *  presently assume it at least meets the concept of bidirectional iterator.
 */
template <typename IteratorType>
class RangeContainer {
public:
    ///The type of the elements contained within this container
    using value_type      = typename IteratorType::value_type;

    ///The type of a reference to an element in this container
    using reference       = value_type&;

    ///The type of a reference to a read-only element in this container
    using const_reference = const value_type&;

    ///The type of an iterator over this container.
    using iterator        = IteratorType;

    ///The type of an iterator that only reads the elements of this container.
    using const_iterator  = IteratorType;

    ///The type of the difference between two elements of this container
    using difference_type = long int;

    ///The type of an offset or index to an element in this container
    using size_type       = std::size_t;

    /** @brief Constructs an empty container.
     *
     *  @throw ??? Throws if the default constructor for the iterator throws.
     *  Strong throw guarantee.
     */
    RangeContainer() = default;

    /**
     * @brief Makes a copy of another RangeContainer by deep copying its stored
     * iterators.
     *
     * The exact copy semantics depend on the iterator, but generally speaking
     * the result is a container that is disjoint from the copied instance.
     *
     * @param[in] rhs The RangeContainer instance to copy.
     *
     * @throw ??? Throws if the iterator's copy constructor throws.  Strong
     * throw guarantee.
     */
    RangeContainer(const RangeContainer& /*rhs*/) = default;

    /** @brief Makes a new RangeContainer instance by taking over the state of
     *  another instance.
     *
     *  @param rhs The container whose state is being taken.  After this call
     *  it is in a valid, but otherwise undefined state.
     *
     *  @throw ??? Throws if iterator's move constructor throws.  Strong throw
     *  guarantee.
     */
    RangeContainer(RangeContainer&& /*rhs*/) = default;

    /** @brief The primarily useful constructor capable of making a container
     *         holding the specified range.
     *
     * @param start_itr An iterator pointing to the first element in the
     *        container.
     * @param end_itr An iterator pointing to just past the last element in the
     *        container.
     * @param size_in The number of elements between end_itr and start_itr.
     */
    RangeContainer(iterator start_itr, iterator end_itr, size_type size_in) :
            start_(start_itr), end_(end_itr), size_(size_in)
    { }

    /**
     * @brief Assigns a deep copy of another RangeContainer to the current
     * instance.
     *
     * @param[in] rhs The RangeContainer instance to copy.
     * @return The current instance set to a copy of @p rhs 's state.
     * @throw ??? Throws if the iterator's copy constructors throw.  Strong
     * throw guarantee.
     */
    RangeContainer& operator=(const RangeContainer& /*rhs*/)= default;

    /** @brief Causes the current container to take ownership of another
     *  instance's state.
     *
     *  @param[in] rhs The instance whose state to steal.  After this call @p
     *  rhs is in a valid, but otherwise undefined state.
     *
     *  @return The current instance containing @p rhs's state.
     *  @throw ??? Throws if the iterator's move constructor throws.  Strong
     *  throw guarantee.
     */
    RangeContainer& operator=(RangeContainer&& /*rhs*/)= default;

    /** @brief Frees up memory associated with the current container.
     *
     *  @throw None. No throw guarantee.
     *
     */
    ~RangeContainer()noexcept = default;

    /** @brief Returns an iterator to first element of the container.
     *
     *  All the state of the container is contained within the iterator and thus
     *  the iterator remains valid even if the container goes out of scope.
     *
     *  @return An iterator pointing to the first element of this container.
     *  @throw ??? Throws if the iterator's copy constructor throws.
     */
    iterator begin(){return start_;}

    ///@copydoc begin()
    const_iterator begin() const
    {
        return start_;
    }

    ///@copydoc begin()
    const_iterator cbegin() const
    {
        return start_;
    }

    /**
     * @brief Returns an iterator that points to just past the last element of
     * the container.
     *
     * The entire state of the container is kept in the iterators, hence the
     * resulting iterator is valid even if the container goes out of scope.
     *
     * @return An iterator set to the element just past the last element of the
     * array.
     *
     * @throw ??? Throws if the iterator's copy constructor throws.  Strong
     * throw guarantee.
     */
    iterator end(){return end_;}

    ///@copydoc end()
    const_iterator end() const
    {
        return end_;
    }

    ///@copydoc end()
    const_iterator cend() const
    {
        return end_;
    }

    /** @brief Returns the number of elements in this container.
     *
     *
     *  @return The number of elements in this container.
     *  @throw None. No throw guarantee.
     */
    size_type size() const noexcept
    {
        return size_;
    }

    /**
     * @brief The theoretical maximum size of the container regardless of
     * hardware limitations.
     *
     * For now the maximum size of the container is limited only by the maximum
     * value @p size_type can hold.
     *
     * @return The theoretical maximum number of elements this container can
     * hold.
     *
     * @throw None. No throw guarantee.
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

    /**
     * @brief Used to check if a container "contains" elements.
     *
     * Since RangeContainer instances do not actually contain their elements
     * this check is really ensuring that the range of elements traversed by the
     * iterators is non-empty.
     *
     * @return True if the container is empty and false otherwise.
     * @throw None. No throw guarantee.
     */
    bool empty() const noexcept
    {
        return begin() == end();
    }

    /**
     * @brief Compares two RangeContainer instances for equality.
     *
     * Equality of two RangeContainer instances is defined as having internal
     * iterators that compare equal.
     *
     * @param rhs The RangeContainer instance to compare to.
     * @return True if the two instances are equivalent and false otherwise.
     * @throw ??? Throws if the equality comparison operator of the iterator
     * throws.  Strong throw guarantee.
     */
    bool operator==(const RangeContainer& rhs) const
    {
        return std::tie(start_, end_) == std::tie(rhs.start_, rhs.end_);
    }

    /**
     * @brief Determines if two RangeContainer instances are different.
     *
     * Equality of RangeContainer instances is defined as wrapping iterators
     * that compare equal.
     *
     * @param rhs The container to compare to.
     * @return True if either iterator in this class differs from the
     * corresponding iterator in @p rhs.
     * @throw ??? Throws if operator==() of the iterators throws.  Strong throw
     * guarantee.
     */
    bool operator!=(const RangeContainer& rhs) const
    {
        return !((*this) == rhs);
    }

private:
    ///An iterator pointing to the first element
    iterator start_;

    ///An iterator to just past the last element
    iterator end_;

    ///The number of elements between @p start_ and @p end_
    size_type size_;
};

}
}


