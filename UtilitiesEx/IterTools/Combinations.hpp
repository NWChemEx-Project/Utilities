#pragma once
#include "UtilitiesEx/IterTools/Permutations.hpp"
#include <algorithm> //is_permutation
#include <limits>    //max size of size_t
#include <tuple>     //for tie

namespace UtilitiesEx {

/** @brief A container for holding all combinations of a sequence of numbers.
 *
 *  Given a sequence of @f$N@f$ elements, there are @f${N \choose m}@f$ possible
 *  ways to choose @f$m@f$ elements if repetitions are not allowed and
 *  @f${N+m-1\choose m}@f$ if they are.  Like its Permutations container
 *  brethern, this class doesn't actually store the combinations, but rather
 *  computes them on the fly.
 *
 *  As implemented this class will contain all combinations, all of which are
 *  not necessarily unique.  More specifically, if the input sequence contains
 *  only unique elements then the result will be all unique combinations;
 *  however, if there are repeats in the input set then there will be repeats
 *  in the output.  The order of combinations is lexicographic in the positions
 *  of the elements NOT the values of the elements (if the elements are sorted
 *  in lexicographic order prior to being passed to this container then the
 *  order will also be lexicographic).
 *
 *  @tparam SequenceType The sequence for which we are generating combinations.
 *         Should satisfy the concept of container.
 *  @tparam Repeats True if repeats are allowed.  Default false.
 */
template<typename SequenceType, bool Repeats = false>
class Combinations {
    private:
    /// Forward declare for type stating reasons
    class CombinationItr;

    public:
    /// Type of this class (only declared for convenience)
    using my_type = Combinations<SequenceType, Repeats>;

    /// Type of an element inside this container
    using value_type = SequenceType;

    /// Combinations are non-mutable so same as const_reference
    using reference = const value_type&;

    /// Type of a const reference to an element of this container
    using const_reference = const value_type&;

    /// Non-mutable container so same as const_iterator
    using iterator = CombinationItr;

    /// Type of a non-mutating iterator to this container
    using const_iterator = CombinationItr;

    /// Type of the difference between two iterators
    using difference_type = long int;

    /// The type of an offset
    using size_type = std::size_t;

    /** @brief Constructs a container filled no combinations.
     *
     *  The resulting instance is suitable only as a placeholder in the sense
     *  that it can only contain combinations by assigning to it.
     *
     *  @throws None No throw guarantee.
     */
    Combinations() noexcept = default;

    /** @brief Fills container with all Combinations of \p input_set
     *
     *  @param[in] input_set The set whose Combinations will grace this
     *             container.
     *  @param[in] k The number of objects chosen at a time.  Assumed to be in
     *             the range [0,len(input_set)] if repeats are not allowed and
     *             undefined behavior (likely a segfault) if @p k is not.
     *             For repeats any @p k is possible so long as the set contains
     *             at least one element (if its empty only k=0 is allowed).
     *
     *  @throws std::bad_alloc If there is not enough memory to copy the input.
     *          Strong throw guarantee.
     */
    Combinations(const_reference input_set, size_type k);

    /** @brief Deep copies another Combinations instance.
     *
     *  @param[in] rhs The instance to copy.
     *  @throws std::bad_alloc if there is not enough memory to copy \p rhs.
     *          Strong throw guarantee.
     */
    Combinations(const my_type& /*rhs*/) = default;

    /** @copybrief Combinations(const Combinations&)
     *
     *  @param[in] rhs the instance to copy.
     *  @return The current instance containing a deep copy of @p rhs
     *  @throws std::bad_alloc if there is not enough memory to copy @p rhs.
     *          Strong throw guarantee.
     */
    Combinations& operator=(const my_type& /*rhs*/) = default;

    /** @brief Takes ownership of another container.
     *
     *  After this operation @p rhs is in a valid, but otherwise unspecified
     *  state.
     *
     *  @param[in] rhs The set of Combinations to take ownership of
     *  @throws None No throw guarantee.
     */
    Combinations(my_type&& /*rhs*/) noexcept = default;

    /** @brief Takes ownership of another container
     *
     *  After this operation @p rhs is in a valid, but otherwise undefined
     *  state.
     *
     *  @param[in] rhs The set of Combinations to take ownership of
     *  @returns This instance after taking ownership of @p rhs
     *  @throws None No throw guarantee.
     */
    my_type& operator=(my_type&& /*rhs*/) noexcept = default;

    /** @brief Frees memory associated with this container.
     *
     *  Note that the iterators for this class contain deep copies of its state.
     *  So technically they are still valid after this classes destruction.  It
     *  is recommended to not use them in that manner as it is contrary to usual
     *  practices.
     *
     *  @throws None No throw guarantee.
     */
    ~Combinations() noexcept = default;

    /** @brief Creates an iterator for this container that points to the first
     *         combination in the container.
     *
     *  The returned iterator's state is decoupled from the container.
     *  This is done purely for convenience.  The result is that changes to this
     *  container will not affect any references to outstanding iterators in any
     *  way shape or form.
     *
     *  @return An iterator pointing to the first element of the container.
     *  @throws std::bad_alloc if there is not enough memory to copy the set to
     *          the iterator.  Strong throw guarantee.
     */
    iterator begin() const {
        return CombinationItr(original_set_, k_, defaulted_);
    }

    /** @brief Creates an iterator just past the last element of this container.
     *
     *  @copydetails Combinations::begin()
     *
     *  @return An iterator set to just past the last element of this container.
     *  @throws std::bad_alloc if there is not enough memory to copy the set to
     *          the iterator.  Strong throw guarantee.
     */
    iterator end() const { return CombinationItr(original_set_, k_, true); }

    ///@copydoc Combinations::begin()
    const_iterator cbegin() const { return begin(); }

    ///@copydoc Combinations::end()
    const_iterator cend() const { return end(); }

    /** @brief True if this contains the same combinations as @p rhs.
     *
     *  Two instances of this class contain the same combinations if the
     *  sequences they were given initially are the same and we are taking the
     *  same number of elements at a time.  Strictly speaking the underlying
     *  elements may not be in the same order depending on the semantics of the
     *  container type this class is wrapping.  If it is ordered they will be.
     *
     *  @param[in] rhs The container to compare to.
     *  @return True if this container and @p rhs contain the same
     *               Combinations.
     *  @throws None No throw guarantee.
     */
    bool operator==(const my_type& rhs) const noexcept {
        return std::tie(size_, original_set_) ==
               std::tie(rhs.size_, rhs.original_set_);
    }

    /** @brief True if this contains different combinations from @p rhs.
     *
     *  This function simply negates the result of
     *  Combinations::operator==(Combinations).  See its documentation for more
     *  details on the defintion of equality.
     *
     *  @param[in] rhs The container to compare to
     *  @return True if this container holds different combinations than @p rhs
     *  @throws None No throw guarantee.
     *
     */
    bool operator!=(const my_type& rhs) const noexcept {
        return !((*this) == rhs);
    }

    /** @brief Swaps the contents of this container with those of @p rhs
     *
     *  @param[in] rhs The container to swap contents with
     *  @throws None No throw guarantee.
     */
    void swap(my_type& rhs) noexcept {
        std::swap(original_set_, rhs.original_set_);
        std::swap(size_, rhs.size_);
        std::swap(k_, rhs.k_);
        std::swap(defaulted_, rhs.defaulted_);
    }

    /** @brief Returns the number of elements in this container.
     *
     *  This is simply the value of @f${N\choose k}@f$ if this container does
     *  not contain repeats, and the value @f${N+k-1 \choose k}@f$ otherwise.
     *  @f$N@f$ and @f$k@f$ respectively are the total number of elements to
     *  choose from and the number to choose at a time.  This function does not
     *  actually represent the number of literal types stored in memory like
     *  most other containers.
     *
     *  @return The number of Combinations in this container
     *  @throws None No throw guarantee.
     */
    size_type size() const noexcept { return size_; }

    /** @brief Returns the maximum possible size of this container.
     *
     *  Since this container does not actually contain its elements, so long as
     *  the number of unique combinations is expressible as a size_type integer
     *  we assume it will fit in this container.  As is usual for the container
     *  concept this is a theoretical maximum and owing to hardware limitations
     *  it may not actually be possible to instantiate an object with that many
     *  elements.
     */
    constexpr size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    /** @brief Returns true if the container is empty.
     *
     *  This container is only empty if it was default constructed or is a copy
     *  of one that was.  In all other cases this class was fed a sequence,
     *  even if that sequence was empty, there is then at least one member in
     *  this container, the empty set.
     *
     *  @returns true if the container is empty.
     *  @throws None No throw guarantee.
     */
    bool empty() const noexcept { return defaulted_; }

    value_type operator[](difference_type i) const { return *(cbegin() + i); }

    private:
    /// This is the sequence we are generating all unique Combinations of
    value_type original_set_;

    /// The number of objects taken at a time
    size_type k_ = 0;

    /// This is the number of Combinations in the container
    size_type size_ = 0;

    /// True if this container was defaulted
    bool defaulted_ = true;

    /** @brief The iterator actually returned by the Combinations class
     *
     *  This class is written in terms of the PermutationItr class.  As a
     *  technical note we invert the usual 0=false, 1=true mapping to get the
     *  unique permutations back in lexicographical order.
     */
    class CombinationItr
      : public detail_::RandomAccessIteratorBase<CombinationItr, SequenceType> {
        public:
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
        CombinationItr(const_reference input_set, size_type k, bool at_end);

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

        private:
        /// A copy of the parent's set
        value_type set_;

        /// The current combination
        value_type comb_;

        /// The current permutation
        typename Permutations<std::vector<bool>>::const_iterator current_perm_;

        /// Updates @p comb_ to be consistent with current_perm_
        void update_comb();

    }; // End class CombinationItr
};     // End class Combinations

/******************************************************************************
 * Definitions.
 ******************************************************************************/

template<typename container_type, bool repeat>
Combinations<container_type, repeat>::Combinations(
  const container_type& input_set, std::size_t k) :
  original_set_(input_set),
  k_(k),
  size_(!repeat ?
          binomial_coefficient<size_type>(input_set.size(), k) :
          binomial_coefficient<size_type>((input_set.size() + k) - 1, k)),
  defaulted_(false) {}

template<typename container_type, bool repeat>
void Combinations<container_type, repeat>::CombinationItr::update_comb() {
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

template<typename container_type, bool repeat>
Combinations<container_type, repeat>::CombinationItr::CombinationItr(
  const container_type& input_set, std::size_t k, bool at_end) :
  set_(input_set),
  comb_(k),
  current_perm_() {
    const size_type n = input_set.size();
    // k==n==0 is possible and leads to -1 (technically ok, -1 choose 0=1...)
    const size_type eff_size = (n || k ? n + k - 1 : 0);
    std::vector<bool> temp(!repeat ? n : eff_size, true);
    for(size_t i = 0; i < k; ++i) temp[i] = false;
    Permutations<std::vector<bool>> perms(temp);
    current_perm_ = (at_end ? perms.end() : perms.begin());
    update_comb();
}
} // namespace UtilitiesEx
