#pragma once
#include "UtilitiesEx/IterTools/Permutations.hpp"
#include <algorithm> //is_permutation
#include <limits>    //max size of size_t
#include <tuple>     //for tie

namespace UtilitiesEx {
namespace detail_ {

/** @brief The main implementation of the CombinationImpl class.  This iterator
 *  does all the work.
 *
 *  This class is written in terms of the PermutationItr class.  As a
 *  technical note we invert the usual 0=false, 1=true mapping to get the
 *  unique permutations back in lexicographical order.  This class can be used
 *  alone as a generator, but is intended to be used with a RangeContainer
 *  instance so that it can be used in a range-based for loop.
 *
 *  @tparam SequenceType The type of a generated combination.
 *  @tparam repeat Should we allow repeated elements while forming combinations?
 */
template<typename SequenceType, bool repeat>
class CombinationItr : public detail_::RandomAccessIteratorBase<
                         CombinationItr<SequenceType, repeat>, SequenceType> {
    /// The type of an instance of this class, defined for sanity
    using my_type = CombinationItr<SequenceType, repeat>;
    /// The type of the base class, again for sanity.
    using base_type = detail_::RandomAccessIteratorBase<my_type, SequenceType>;

    public:
    /// Typedefs forwarded from the base class
    ///@{
    using value_type      = typename base_type::value_type;
    using const_reference = typename base_type::const_reference;
    using size_type       = typename base_type::size_type;
    using difference_type = typename base_type::difference_type;
    ///@}

    /** @brief Makes a new combination iterator over a given sequence.
     *
     *  @param[in] input_set The set to iterate over.
     *  @param[in] k The number of objects chosen at a time.  @p k is
     *             assumed in the range [0,len(input_set)] if repeats are
     *             not allowed, otherwise it may be any value.
     *  @param[in] at_end True if this is the end iterator
     *  @throws std::bad_alloc If the copy fails because of lack of memory.
     *          Strong throw guarantee.
     */
    CombinationItr(const_reference input_set, size_type k, bool at_end) :
      set_(input_set),
      comb_(k),
      current_perm_() {
        const size_type n = input_set.size();
        // k==n==0 is possible and leads to -1 (technically ok, -1 choose
        // 0=1...)
        const size_type eff_size = (n || k ? n + k - 1 : 0);
        std::vector<bool> temp(!repeat ? n : eff_size, true);
        for(size_t i = 0; i < k; ++i) temp[i] = false;
        auto perms    = Permutations(temp);
        current_perm_ = (at_end ? perms.end() : perms.begin());
        update_comb();
    }

    CombinationItr() = default;

    /** @brief Makes a deep copy of the current instance.
     *
     *  Since the original instance is not tied to its parent container
     *  neither is the resulting iterator.
     *
     *  @param[in] rhs The Combination to copy.
     *  @throws std:bad_alloc if copying fails.  Strong throw guarantee.
     */
    CombinationItr(const CombinationItr& /*rhs*/) = default;

    /** @brief Assigns a deep copy to the current instance.
     *
     *  @param[in] rhs The Combination to copy.
     *  @returns The current iterator after the copy
     *  @throws std::bad_alloc if the underlying copy fails.  Strong throw
     *  guarantee.
     */
    CombinationItr& operator=(const CombinationItr& /*rhs*/) = default;

    /** @brief Takes ownership of another iterator
     *
     *  After the operation @p rhs is in a valid, but otherwise unspecified
     *  state.
     *
     *  @param[in] rhs The iterator we are going to own
     *  @throws None No throw guarantee.
     */
    CombinationItr(CombinationItr&& /*rhs*/) = default;

    /** @brief Takes ownership of another CombinationItr instance
     *
     *  @param[in] rhs The iterator to take ownership of.
     *  @return The current instance containing @p rhs 's state.
     *  @throws None No throw guarantee.
     */
    CombinationItr& operator=(CombinationItr&& /*rhs*/) = default;

    /// Trivial destructor
    ~CombinationItr() noexcept = default;

    /** @brief Returns the element of the parent container currently pointed
     *         to by this iterator.
     *
     *  @return The element being pointed to.
     *  @throws None No throw guarantee.
     */
    const_reference dereference() const noexcept { return comb_; }

    /** @brief Makes the iterator point to the next Combination.
     *
     *  Combinations are ordered lexicographically and "next" follows from
     *  this convention.
     *
     *  @warning Incrementing beyond the end of the container is allowed;
     *           however, dereferencing the corresponding iterator is
     *           undefined behavior.
     *
     *  @return The iterator after incrementing
     *  @throws None No throw guarantee.
     */
    CombinationItr& increment() noexcept {
        ++current_perm_;
        update_comb();
        return *this;
    }

    /** @brief Makes the iterator point to the previous Combination.
     *
     *  Combinations are ordered lexicographically and "previous" follows
     *  from this convention.
     *
     *  @warning Decrementing beyond the beginning of the container is
     *           allowed; however, dereferencing the corresponding iterator
     *           is undefined behavior.
     *
     *  @return The iterator after decrementing
     *  @throws None No throw guarantee.
     */
    CombinationItr& decrement() noexcept {
        --current_perm_;
        update_comb();
        return *this;
    }

    /** @brief Moves the current iterator @p n iterations
     *
     *  @param[in] n The number of iterations to move the iterator.  Can
     *             be either forward or backward.
     *  @returns The current iterator pointing at the element @p n
     *           iterations away.
     *  @throws std::bad_alloc if the underlying PermutationItr has
     *  insufficient memory to complete the request
     *
     */
    CombinationItr& advance(difference_type n) {
        current_perm_ += n;
        update_comb();
        return *this;
    }

    /** Compares two CombinationItrs for exact equality
     *
     *  Exact equality is defined as pointing to the same Combination,
     *  having the same starting Combination, and having both wrapped (or
     *  not wrapped).
     *
     *  @param[in] other The iterator to compare to.
     *  @return True if this iterator is exactly the same as @p other
     *  @throws None No throw guarantee.
     */
    bool are_equal(const CombinationItr& other) const noexcept {
        return std::tie(set_, current_perm_) ==
               std::tie(other.set_, other.current_perm_);
    }

    /** @brief Returns the distance between this iterator and another
     *
     * @param[in] rhs The iterator we want the distance to.
     * @returns the distance between the two iterators
     */
    difference_type distance_to(const CombinationItr& rhs) const noexcept {
        return current_perm_ - rhs.current_perm_;
    }

    void swap(CombinationItr& rhs) {
        std::swap(set_, rhs.set_);
        std::swap(comb_, rhs.comb_);
        std::swap(current_perm_, rhs.current_perm_);
    }

    private:
    /// A copy of the parent's set
    value_type set_;

    /// The current combination
    value_type comb_;

    /// The current permutation
    typename PermutationsImpl<std::vector<bool>>::const_iterator current_perm_;

    /// Updates @p comb_ to be consistent with current_perm_
    void update_comb() {
        const auto& p = *current_perm_;
        for(size_type i = 0, counter = 0, bar_count = 0; i < p.size(); ++i) {
            if(!p[i])
                comb_[counter++] = set_[!repeat ? i : bar_count];
            else
                ++bar_count;
            if(counter == comb_.size()) // Early termination, rest are false
                break;
        }
    }

}; // End class CombinationItr

template<typename SequenceType, bool repeat>
class CombinationsImpl
  : public RangeContainer<CombinationItr<SequenceType, repeat>> {
    using base_type = RangeContainer<CombinationItr<SequenceType, repeat>>;

    public:
    using size_type       = typename base_type::size_type;
    using const_reference = typename base_type::const_reference;

    CombinationsImpl() = default;

    CombinationsImpl(const CombinationsImpl&) = default;

    /**
     *  @brief Takes ownership of another CombinationsImpl instance.
     *
     *  @param rhs The instance to take ownership of.  After this call it will
     *  be in a valid, but otherwise undefined state.
     *  @throw ??? throws if SequenceType's move constructor throws.  Same
     *  guarantee as SequenceType's move constructor.
     */
    CombinationsImpl(CombinationsImpl&& /*rhs*/) = default;

    /** @brief Fills container with all combinations of @p input_set
     *
     *  @param[in] input_set The sequence whose combinations will grace this
     *             container.
     *  @param[in] k The number of elements in each combination.  @p k should
     *             be in the range [0, input_set.size()).
     *  @throws std::bad_alloc If there is not enough memory to copy the input.
     *          Strong throw guarantee.
     *  @throws ??? If copying input_set throws.  Strong throw guarantee.
     */
    CombinationsImpl(const_reference input_set, size_type k) :
      base_type(CombinationItr<SequenceType, repeat>{input_set, k, false},
                CombinationItr<SequenceType, repeat>{input_set, k, true},
                !repeat ? binomial_coefficient<size_type>(input_set.size(), k) :
                          binomial_coefficient<size_type>(
                            (input_set.size() + k) - 1, k)) {}

    /**
     * @brief Frees
     */
    ~CombinationsImpl() = default;

    CombinationsImpl& operator=(const CombinationsImpl&) = default;

    CombinationsImpl& operator=(CombinationsImpl&&) = default;
};

} // End namespace detail_

/** @brief Makes a container for holding all combinations of a sequence with
 *  repeats.
 *
 *  Given a sequence of @f$N@f$ elements, there are @f${N+m-1\choose m}@f$
 *  combinations of @f$m@f$ elements if repeats are allowed.  The class
 *  resulting from this function will simulate a container filled with all of
 *  them.  It should be noted that the resulting class does not actually contain
 *  all the elements, but rather the elements are generated on the fly.
 *  Owing to the underlying algorithm, the order of combinations is
 *  lexicographic in the positions of the elements NOT the values of the
 *  elements (if the elements in the input sequence are sorted in lexicographic
 *  order then the order will also be lexicographic in the elements).
 *
 *  @tparam container_type The sequence for which we are generating
 *  combinations.  Should satisfy the concept of container.
 *  @param container The sequence of elements for which combinations should be
 *  formed.
 *  @param k The number of elements per combination.  @p k should be in the
 *  range [0, container.size()).
 *  @throws std::bad_alloc if the container can not be copied.  Strong throw
 *  guarantee.
 *  @throws std::overflow_error if computing the size causes the binomial
 *  coefficient algorithm to overflow.  Strong throw guarantee.
 *  @throws ??? if the copy constructor of SequenceType throws for any reason.
 *  Strong throw guarantee.
 */
template<typename container_type>
auto CombinationsWithRepeat(container_type&& container, std::size_t k) {
    return detail_::CombinationsImpl<std::decay_t<container_type>, true>(
      std::forward<container_type>(container), k);
}

/** @brief Makes a container for holding all combinations of a sequence of
 * numbers without repeats.
 *
 *  Given a sequence of @f$N@f$ elements, there are @f${N \choose m}@f$ possible
 *  ways to choose @f$m@f$ elements.  The class
 *  resulting from this function will simulate a container filled with all of
 *  them.  It should be noted that the resulting class does not actually contain
 *  all the elements, but rather the elements are generated on the fly.
 *  Owing to the underlying algorithm, the order of combinations is
 *  lexicographic in the positions of the elements NOT the values of the
 *  elements (if the elements in the input sequence are sorted in lexicographic
 *  order then the order will also be lexicographic in the elements).
 *
 *  @tparam container_type The sequence for which we are generating
 * combinations. Should satisfy the concept of container.
 *  @param container The sequence of elements for which we are generating all
 *  combinations of length @p k.
 *  @param k The number of elements in any generated combination.  @p k should
 *  be in the range [0, container.size()).
 *  @return A container instance suitable for inclusion in a range-based for
 *  loop.
 *  @throws std::bad_alloc if the container can not be copied.  Strong throw
 *  guarantee.
 *  @throws std::overflow_error if computing the size causes the binomial
 *  coefficient algorithm to overflow.  Strong throw guarantee.
 *  @throws ??? if the copy constructor of SequenceType throws for any reason.
 *  Strong throw guarantee.
 */
template<typename container_type>
auto Combinations(container_type&& container, std::size_t k) {
    return detail_::CombinationsImpl<std::decay_t<container_type>, false>(
      std::forward<container_type>(container), k);
}

} // End namespace UtilitiesEx
