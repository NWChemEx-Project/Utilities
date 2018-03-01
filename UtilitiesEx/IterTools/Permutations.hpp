#pragma once
#include "UtilitiesEx/IterTools/RangeContainer.hpp"
#include "UtilitiesEx/Mathematician/Combinatorics.hpp"
#include "UtilitiesEx/TypeTraits/IteratorTypes.hpp"
#include <algorithm> //For is_permutation, next/prev permutation
#include <limits>    //For maximum value of size_t
#include <tuple>     //For std::tie

namespace UtilitiesEx {
namespace detail_ {

/** @brief The class that actually implements the guts of generating
 *  permutations.
 *
 *  This class is actually written in terms of the next/prev permutation
 *  generators included in the STL.
 *
 *  @section Technical notes
 *  - Since next/prev permutation mutate the container they operate on, this
 *    class actually stores a deep copy of the container it is forming
 *    permutations of so as to avoid altering the original container.
 *  - next/prev permutation wrap around on themselves.  For this reason we
 *    need to maintain how many times we've iterated so as to know when we have
 *    finished.
 *
 *  @tparam SequenceType the type of the input sequence and the resulting
 *  permutations.  Should satisfy the concept of sequence.
 *  @todo C++17 allows for inheriting default ctors.
 */
template<typename SequenceType>
class PermutationItr
  : public detail_::RandomAccessIteratorBase<PermutationItr<SequenceType>,
                                             SequenceType> {
    /// Type of this class (to simplify defining base_type)
    using my_type = PermutationItr<SequenceType>;

    /// Type of the base class (to simplify scoping types)
    using base_type = detail_::RandomAccessIteratorBase<my_type, SequenceType>;

public:
    /// Brings some of base class's typedefs into scope
    ///@{
    using value_type      = typename base_type::value_type;
    using const_reference = typename base_type::const_reference;
    using difference_type = typename base_type::difference_type;
    using size_type       = typename base_type::size_type;
    ///@}

    /** @brief Makes an iterator that iterates over nothing.
     *
     *  This iterator is capable of iterating over the empty range.
     *
     *  @note A default iterator is not the same as an iterator over the
     *  empty set.  The former iterates over an empty range, whereas the latter
     *  iterates over permutations of the empty set (for which there is one
     *  element, the empty set).
     *
     *  @throws ??? if SequenceType's default ctor throws.  Same guarantee as
     *          SequenceType's default ctor.
     */
    PermutationItr() = default;

    /** @brief Makes an instance that will generate permutations of an input
     *  sequence.
     *
     *  This ctor will make a deep copy of the input sequence so as to avoid
     *  mutating it.  Consequentially, all state is stored in this
     *  iterator and the iterator is valid even if the input container goes out
     *  of scope.
     *
     * @param[in] input_set The set to iterate over.
     * @param[in] offset  Which permutation to start with.  Offset should be
     *        in the range [0, size) where permutations are number
     *        lexicographically starting from the input sequence and ending when
     *        the original sequence is regenerated.
     * @throws ??? If SequenceType's copy constructor throws.  Strong throw
     *         guarantee.
     */
    PermutationItr(const_reference input_set, size_type offset) :
      orig_set_(input_set),
      set_(input_set),
      offset_(offset) {}

    /** @brief Allows access to the current permutation.
     *
     *  In accordance with usual C++ practice the element is returned by
     *  reference.  However, any changes made to the element will be overridden
     *  when the iterator is incremented or decremented.
     *
     *  @note The base class will use this function to implement both the
     *  read-only and the read/write dereference operation via const_cast.
     *
     *  @return The element being pointed to.
     *  @throws None. No throw guarantee.
     */
    const_reference dereference() const override { return set_; }

    /** @brief Makes the iterator point to the next permutation.
     *
     *  Permutations are ordered lexicographically and "next" follows from
     *  this convention.  If the current permutation is the lexicographically
     *  greatest permutation the next permutation is the lexicographically
     *  lowest permutation (such behavior is possible if the input sequence is
     *  not sorted in lexicographical order to begin with).
     *
     *  @warning Incrementing beyond the last permutation is allowed;
     *           however, dereferencing the corresponding iterator is
     *           undefined behavior.
     *
     *  @return The iterator by reference after incrementing the permutation.
     *  @throws ??? if SequenceType's begin() or end() function throws or if
     *          std::next_permutation throws given the resulting iterators.
     *          Same throw guarantee as the throwing function.
     */
    PermutationItr& increment() {
        std::next_permutation(set_.begin(), set_.end());
        ++offset_;
        return *this;
    }

    /** @biref Compares two PermutationItrs for exact equality.
     *
     *  Exact equality is defined as:
     *  1. Containing the same permutation.
     *  2. Having the same starting permutation.
     *  3. Having both wrapped or not wrapped.
     *     - Relevant for comparing the first permutation to the one just past
     *       the end (which is the same permutation)
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
     *  from this convention.  If the current permutation is the
     *  lexicographically lowest permutation, decrementing will generate the
     *  lexicographically highest permutation.  This class is such that
     *  decrementing an iterator that is just past the end, will generate the
     *  last permutation.
     *
     *  @warning Decrementing beyond the input sequence is allowed, but
     *  dereferencing the resulting state is undefined behavior.
     *
     *  @return The iterator after decrementing
     *  @throws ??? if SequenceType's begin() or end() member functions throw or
     *          if prev_permutation throws with the resulting iterators.  Same
     *          guarantee as the throwing function.
     */
    PermutationItr& decrement() {
        std::prev_permutation(set_.begin(), set_.end());
        --offset_;
        return *this;
    }

    /** @brief Advances the current iterator @p n iterations.
     *
     *  This function can be used to skip permutations either in the forward
     *  (positive @p n values) or backwards (@p negative n values) directions.
     *
     *  @param[in] n The number of iterations to move the iterator.  Positive
     *             @p n produce lexicographically larger permutations whereas
     *             negative @p n produces lexicographically smaller
     *             permutations.
     *  @returns The current iterator pointing at the element @p n
     *           iterations away.
     *  @throws std::bad_alloc if decimal_to_permutation has insufficient
     *          memory to complete.  Strong throw guarantee.
     *
     */
    PermutationItr& advance(difference_type n) {
        set_ = decimal_to_permutation(offset_ + n, orig_set_);
        offset_ += n; //After above call for strong throw guarantee
        return *this;
    }

    /** @brief Returns the number of permutations between this and @p rhs
     *
     *  If both this iterator and @p rhs started from the same sequence then
     *  @f$\Delta X = rhs.offset_ - offset_ @f$ is the distance between them.
     *  However, if they did not start from the same sequence we additionally
     *  have to account for this.  Specifically if we let @f$\Delta Y@f$ be the
     *  distance from this iterator's original sequence to @p rhs's original
     *  sequence the distance to other is @f$\Delta Y + \Delta X@f$.
     *
     *  @param[in] rhs The iterator to compare against.  It is assumed that
     *             @p rhs's state is contained with this instance's range.
     *  @return The number of permutations between this and other
     *  @throws std::invalid_argument if @p rhs did not start from the same
     *          sequence.  Strong throw guarantee.
     *
     *
     */
    difference_type distance_to(const PermutationItr& rhs) const {
        difference_type dy = 0, dx = 0;
        const bool base_is_less =
          std::lexicographical_compare(orig_set_.begin(), orig_set_.end(),
                                 rhs.orig_set_.begin(), rhs.orig_set_.end());
        if(base_is_less)
            dy = permutation_to_decimal(rhs.orig_set_, orig_set_);
        else
            dy -=permutation_to_decimal(orig_set_, rhs.orig_set_);
        const bool off_is_less = offset_ < rhs.offset_;
        if(off_is_less)
            dx = rhs.offset_ - offset_;
        else
            dx -= offset_ - rhs.offset_;
        return dx + dy;
    }

    /**
     * @brief Swaps the state of the current instance with that of another.
     *
     *
     * @param rhs the instance to swap with.  After the operation it will
     *        contain the state of the current instance.
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
    using iterator_type = detail_::PermutationItr<raw_container_t>;
    const auto nperms = n_permutations(std::forward<container_type>(container));
    return detail_::RangeContainer<iterator_type>{
      iterator_type{std::forward<container_type>(container), 0},
      iterator_type{std::forward<container_type>(container), nperms}, nperms};
}

} // namespace UtilitiesEx
