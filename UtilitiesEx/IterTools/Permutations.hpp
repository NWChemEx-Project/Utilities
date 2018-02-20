#pragma once
#include "UtilitiesEx/IterTools/RangeContainer.hpp"
#include "UtilitiesEx/Mathematician/Combinatorics.hpp"
#include "UtilitiesEx/TypeTraits/IteratorTypes.hpp"
#include <algorithm> //For is_permutation, next/prev permutation
#include <limits>    //For maximum value of size_t
#include <tuple>     //For std::tie

namespace UtilitiesEx {
namespace detail_ {

/** @brief The class actually implementing the guts of generating permutations.
 *
 *  This class is written in terms of next/prev permutation which wrap
 *  around on themselves.  When they wrap they return the first element.
 *  This class satisfies the concept of a random access iterator.
 *
 *  @tparam SequenceType the type of the input sequence and the resulting
 *  permutations.  Should satisfy the concept of sequence.
 *  @todo C++17 allows for inheriting default ctors.
 */
template<typename SequenceType>
class PermutationItr
  : public detail_::RandomAccessIteratorBase<PermutationItr<SequenceType>,
                                             SequenceType> {
    using my_type   = PermutationItr<SequenceType>;
    using base_type = detail_::RandomAccessIteratorBase<my_type, SequenceType>;

    public:
    using value_type      = typename base_type::value_type;
    using const_reference = typename base_type::const_reference;
    using difference_type = typename base_type::difference_type;
    using size_type       = typename base_type::size_type;

    /** @brief Makes a place-holder PermutationItr
     *
     *  @throws ??? if SequenceType's default ctor throws.  Same guarantee as
     *          SequenceType's default ctor.
     */
    PermutationItr() = default;

    /** @brief Makes an instance that points to a (possibly) non-empty container
     *  filled with permutations.
     *
     *  Note that all state is stored in this iterator.
     *
     * @param input_set The set to iterate over.
     * @param offset  Which permutation to start with.
     * @throws ??? If SequenceType's copy constructor throws.  Strong throw
     *         guarantee.
     */
    PermutationItr(const_reference input_set, size_type offset) :
      orig_set_(input_set),
      set_(input_set),
      offset_(offset) {}

    /** @brief Returns the element of the parent container currently pointed
     *         to by this iterator.
     *
     *  @return The element being pointed to.
     *  @throws None. No throw guarantee.
     */
    const_reference dereference() const override { return set_; }

    /** @brief Makes the iterator point to the next permutation.
     *
     *  PermutationsImpl are ordered lexicographically and "next" follows from
     *  this convention.
     *
     *  @warning Incrementing beyond the end of the container is allowed;
     *           however, dereferencing the corresponding iterator is
     *           undefined behavior.
     *
     *  @return The iterator after incrementing
     *  @throws ??? if SequenceType's begin() or end() function throws or if
     *          std::next_permutation throws given the resulting iterators.
     *          Same throw guarantee.
     */
    PermutationItr& increment() {
        std::next_permutation(set_.begin(), set_.end());
        ++offset_;
        return *this;
    }

    /** Compares two PermutationItrs for exact equality
     *
     *  Exact equality is defined as pointing to the same permutation,
     *  having the same starting permutation, and having both wrapped (or
     *  not wrapped).
     *
     *  @param[in] rhs The iterator to compare to.
     *  @return True if this iterator is exactly the same as @p rhs
     *  @throws None No throw guarantee.
     */
    bool are_equal(const PermutationItr& rhs) const noexcept {
        return std::tie(orig_set_, set_, offset_) ==
               std::tie(rhs.orig_set_, rhs.set_, rhs.offset_);
    }

    /** @brief Makes the iterator point to the previous permutation.
     *
     *  PermutationsImpl are ordered lexicographically and "previous" follows
     *  from this convention.
     *
     *  @warning Decrementing beyond the beginning of the container is
     *           allowed; however, dereferencing the corresponding iterator
     *           is undefined behavior.
     *
     *  @return The iterator after decrementing
     *  @throws ??? if SequenceType's begin() or end() member functions throw or
     *          if prev_permutation throws with the resulting iterators.  Same
     *          guarantee as throwing function.
     */
    PermutationItr& decrement() {
        std::prev_permutation(set_.begin(), set_.end());
        --offset_;
        return *this;
    }

    /** @brief Moves the current iterator @p n iterations
     *
     *  @param[in] n The number of iterations to move the iterator.  Can
     *             be either forward or backward.
     *  @returns The current iterator pointing at the element @p n
     *           iterations away.
     *  @throws std::bad_alloc if decimal_to_permutation has insufficient
     *          memory to complete
     *
     */
    PermutationItr& advance(difference_type n) {
        offset_ += n;
        set_ = decimal_to_permutation(offset_, orig_set_);
        return *this;
    }

    /** @brief Returns the number of permutations between this and @p other
     *
     * @param other The iterator pointing to the other permutation to
     * compare to.
     * @return The number of permutations between this and other
     * @throws None. No throw guarantee
     * */
    difference_type distance_to(const PermutationItr& other) const noexcept {
        const bool is_greater = (offset_ >= other.offset_);
        const difference_type abs_val =
          (is_greater ? offset_ - other.offset_ : other.offset_ - offset_);
        return (is_greater ? -abs_val : abs_val);
    }

    /**
     * @brief Swaps the state of the current instance with that of another.
     *
     *
     * @param rhs the instance to swap with.  After the operation it will
     * contain the state of the current instance.
     * @throw ??? if SequenceType's swap function throws.  Guarantee is no throw
     *        if SequenceType's swap is also no throw.  Otherwise it is weak at
     *        best.
     */
    void swap(PermutationItr& rhs) {
        std::swap(orig_set_, rhs.orig_set_);
        std::swap(set_, rhs.set_);
        std::swap(offset_, rhs.offset_);
    }

    private:
    /// A copy of the parent's set, doesn't get modified
    value_type orig_set_;

    /// A copy of the parent's set, modified by next/prev permutation
    value_type set_;

    /// The number of increments from the first call
    size_type offset_ = 0;

}; // End class PermutationItr

/**
 * @brief A container that simulates being filled with all unique permutations
 * of a sequence.
 * @tparam SequenceType The type of the original sequence as well as the
 * resulting permuations.
 */
template<typename SequenceType>
class PermutationsImpl : public RangeContainer<PermutationItr<SequenceType>> {
    using base_type = RangeContainer<PermutationItr<SequenceType>>;

    public:
    /**
     * @brief Makes a container with no permutations in it.
     * @throw ??? if SequenceType's default ctor throws.  Strong throw
     * guarantee if SequenceType's ctor can throw otherwise no throw guarantee.
     */
    PermutationsImpl() = default;

    /** @brief Fills container with all permutations of \p input_set
     *
     *  @param[in] input_set The sequence whose permutations will grace this
     *             container.
     *  @throws std::bad_alloc If there is not enough memory to copy the input.
     *          Strong throw guarantee.
     *  @throws ??? If the copy constructor of SequenceType throws.  Strong
     *  throw guarantee.
     *
     */
    PermutationsImpl(typename base_type::const_reference input_set) :
      base_type(
        PermutationItr<SequenceType>{input_set, 0},
        PermutationItr<SequenceType>{input_set, n_permutations(input_set)},
        n_permutations(input_set)) {}
};
} // namespace detail_

/**
 * @brief Makes a container "filled" with all unique permutations of a given
 * sequence.
 *
 * This is a convenience function for making a container filled with all
 * unique permutations of a given sequence such that the types of the container
 * (and consequentially the returned permutations) are inferred automatically.
 * It should be noted that the returned container does not actually contain all
 * unique permutations of the sequence, rather permutations are generated on
 * the fly.  Ultimately it relies on std::next_permutation/std::prev_permutation
 * and thus can only generate unique permutations.
 *
 * @note If for some reason you want all permutations of a sequence (and not
 * just the unique ones) it suffices to put the numbers 0 to the length of your
 * sequence minus 1 in a container and treat the resulting permutations as the
 * indices.
 *
 * @tparam container_type The type of the original sequence and also the type of
 *         the generated permutations.
 * @param container The container to form all unique permutations of.
 * @return A container filled with all unique permutations of @p container.
 * @throw ??? If @p container_type 's copy ctor throws.
 */
template<typename container_type>
auto Permutations(container_type&& container) {
    using raw_container_t = std::decay_t<container_type>;
    return detail_::PermutationsImpl<raw_container_t>(
      std::forward<container_type>(container));
}

} // namespace UtilitiesEx
