#pragma once
#include "UtilitiesEx/Mathematician/Combinatorics.hpp"
#include "UtilitiesEx/TypeTraits/IteratorTypes.hpp"
#include <algorithm> //For is_permutation, next/prev permutation
#include <limits>    //For maximum value of size_t
#include <tuple>     //For std::tie

namespace UtilitiesEx {

/** @brief Simulates a non-mutable container filled with all unique
           permutations of a sequence.
 *
 *  This class simulates a container filled with all unique  permutations of a
 *  sequence.  The STL provides next_permutation and prev_permutation for this
 *  purpose; however, these are generators and not containers.  Much of the STL
 *  (as well as foreach loops) assumes a container like syntax, hence the need
 *  for this class.
 *
 *  For a sequence of @f$N@f$ elements there are @f$N!@f$ permutations. If
 *  @f$M@f$ of these elements are unique the number of unique permutations is
 *  much less (but still typically a lot) and given by:
 *  @f[
 *  {{N}\choose{n_1,n_2,\ldots,n_i,\ldots,n_M}}=\frac{N!}{\prod_{i=1}^Mn_i!}
 *  @f]
 *  where @f$n_i@f$ is the number of times the @f$i@f$-th unique element appears
 *  in the sequence.  Hence there are in theory a lot of elements in this
 *  container.  This class doesn't actually store them, but generates them on
 *  the fly.
 *
 *  @note This container contains @b all the unique permutations.  The STL
 *  variants, on the other hand, stop at the lexicographically largest
 *  (next_permutation) or lexicographically smallest (prev_permutation)
 *  permutation.  For numbering purposes whatever sequence you provide is the
 *  0th element of this container, even if it's not the lexicographically
 *  smallest permutation.
 *
 *  @tparam sequence_type The type of the container used to hold the sequence.
 *          The type must satisfy sequence container concept.
 */
template <typename SequenceType> class Permutations {
private:
  /// Forward declaration of the iterator type
  class PermutationItr;

public:
  /// Type of this class
  using my_type = Permutations<SequenceType>;

  /// Type of an element of this container
  using value_type = SequenceType;

  /// Permutations are non-mutable so same as const_reference
  using reference = const value_type &;

  /// Type of a const reference to an element of this container
  using const_reference = const value_type &;

  /// Non-mutable container so same as const_iterator
  using iterator = PermutationItr;

  /// Type of a non-mutating iterator to this container
  using const_iterator = PermutationItr;

  /// Type of the difference between two iterators
  using difference_type = long int;

  /// The type of an offset
  using size_type = std::size_t;

  /** @brief Constructs an empty container.
   *
   *  This container is truly empty, that is to say its not even the set of
   *  permutations of the empty set.
   *
   *  @throws None No throw guarantee.
   */
  Permutations() noexcept = default;

  /** @brief Fills container with all permutations of \p input_set
   *
   *  @param[in] input_set The sequence whose permutations will grace this
   *             container.
   *  @throws std::bad_alloc If there is not enough memory to copy the input.
   *          Strong throw guarantee.
   */
  Permutations(const_reference input_set)
      : original_set_(input_set), size_(n_permutations(input_set)) {}

  /** @brief Deep copies another Permutations instance.
   *
   *  @param[in] rhs The instance to copy.
   *  @throws std::bad_alloc if there is not enough memory to copy \p rhs.
   *          Strong throw guarantee.
   */
  Permutations(const my_type & /*rhs*/) = default;

  /** @copybrief Permutations(const Permutations&)
   *
   *  @param[in] rhs the instance to copy.
   *  @return The current instance containing a deep copy of @p rhs
   *  @throws std::bad_alloc if there is not enough memory to copy @p rhs.
   *          Strong throw guarantee.
   */
  Permutations &operator=(const my_type & /*rhs*/) = default;

  /** @brief Takes ownership of another container
   *
   *  @note After this operation @p rhs is in a valid, but undefined state
   *
   *  @param[in] rhs The set of permutations to take ownership of
   *  @throws None No throw guarantee.
   */
  Permutations(my_type && /*rhs*/) noexcept = default;

  /** @brief Assigns ownership of another container to this container
   *
   *  @note After this operation @p rhs is in a valid, but undefined state.
   *
   *  @param[in] rhs The set of permutations to take ownership of
   *  @returns This instance after taking ownership
   *  @throws None No throw guarantee.
   */
  my_type &operator=(my_type && /*rhs*/) noexcept = default;

  /** @brief Frees memory associated with container.
   *
   *  Although it is not recommended to do this (for clarity purposes),
   *  iterators remain valid even after the container's destruction.
   *
   *  @throws None No throw guarantee.
   */
  ~Permutations() noexcept = default;

  /** @brief Creates an iterator for this container that points to the first
   *         element of the container.
   *
   *  The returned iterator's state is decoupled from the container.
   *  This is done because next_permutation and prev_permutation actually
   *  modify the contents of the container they operate on.  This in turn
   *  actually makes the iterators to this container thread-safe.
   *
   *  @return An iterator pointing to the first element of the container.
   *  @throws std::bad_alloc if there is not enough memory to copy the set to
   *          the iterator.  Strong throw guarantee.
   */
  iterator begin() const { return PermutationItr(original_set_, 0); }

  /** @brief Creates an iterator just past the last element of this container.
   *
   *  The returned iterator is decoupled from the current container.  This is
   *  done because next/prev permutation actually modify the contents of the
   *  container they operate on.
   *
   *  @return An iterator set to just past the last element of this container.
   *  @throws std::bad_alloc if there is not enough memory to copy the set to
   *          the iterator.  Strong throw guarantee.
   */
  iterator end() const { return PermutationItr(original_set_, size_); }

  /** @brief Creates an iterator for this container that points to the first
   *         element of the container.
   *
   *  The returned iterator's state is decoupled from the container.
   *  This is done because next_permutation and prev_permutation actually
   *  modify the contents of the container they operate on.  This in turn
   *  actually makes the iterators to this container thread-safe.
   *
   *  @return An iterator pointing to the first element of the container.
   *  @throws std::bad_alloc if there is not enough memory to copy the set to
   *          the iterator.  Strong throw guarantee.
   */
  const_iterator cbegin() const { return begin(); }

  /** @brief Creates an iterator just past the last element of this container.
   *
   *  The returned iterator is decoupled from the current container.  This is
   *  done because next/prev permutation actually modify the contents of the
   *  container they operate on.
   *
   *  @return An iterator set to just past the last element of this container.
   *  @throws std::bad_alloc if there is not enough memory to copy the set to
   *          the iterator.  Strong throw guarantee.
   */
  const_iterator cend() const { return end(); }

  /** @brief True if this contains the same permutations as @p other.
   *
   *  Since we don't actually store all the permutations this function
   *  actually works by determining if this and \p other contain sets that
   *  are permutations of one another.
   *
   *  @param[in] other The container to compare to.
   *  @return True if this container and @p other contain the same
   *               permutations.
   *  @throws None No throw guarantee.
   */
  bool operator==(const my_type &other) const noexcept {
    if (size_ != other.size_)
      return false;
    return std::is_permutation(original_set_.begin(), original_set_.end(),
                               other.original_set_.begin());
  }

  /** @brief True if this contains different permutations from @p other.
   *
   *  Syntactic sugar that simply negates the result of operator==.
   *
   *  @return True if this container holds different permutations.
   *  @throws None No throw guarantee.
   *
   */
  bool operator!=(const my_type &other) const noexcept {
    return !((*this) == other);
  }

  /** @brief Returns a copy of the requested permutation
   *
   *  @section Memory
   *  This call ultimately amounts to calls to decimal_to_fns and
   *  fns_to_permutation and thus has the same memory footprints.
   *
   *  @param[in] perm The value of the requested permutation.  Must be in the
   *                  range [0,size())
   *  @return The requested permutation
   *  @throws std::bad_alloc if there is insufficient memory to compute the
   *          operation.
   *
   */
  value_type operator[](size_type perm) const {
    return decimal_to_permutation(perm, original_set_);
  }

  /** @brief Swaps the contents of this container with another
   *
   *  @param[in] other The container to swap contents with
   *  @throws None No throw guarantee.
   */
  void swap(my_type &other) noexcept {
    std::swap(original_set_, other.original_set_);
    std::swap(size_, other.size_);
  }

  /** @brief Returns the number of elements in this container.
   *
   *  @return The number of permutations in this container
   *  @throws None No throw guarantee.
   */
  size_type size() const noexcept { return size_; }

  /** @brief Returns the maximum possible size of this container.
   *
   *  Since this container does not actually contain its elements, so long as
   *  the number of unique permutations is expressible as a size_type integer
   */
  constexpr size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
  }

  /** @brief Returns true if the container is empty.
   *
   *  This function will only return true if this container was default
   *  constructed.  If it was provided any set, even the empty set, this class
   *  will contain at least one member.
   *
   *  @returns true if the container is empty.
   *  @throws None No throw guarantee.
   */
  bool empty() const noexcept { return cbegin() == cend(); }

private:
  /// This is the set we are generating all unique permutations of
  value_type original_set_;

  /// This is the number of permutations in the container
  size_type size_ = 0;

  /** @brief The iterator actually returned by the Permutations class
   *
   *  This class is written in terms of next/prev permutation which wrap
   *  around on themselves.  When they wrap they return the first element.
   *  This class satisfies the concept of a random access iterator.
   */
  class PermutationItr
      : public detail_::RandomAccessIteratorBase<PermutationItr, SequenceType> {
  public:
    /** @brief Makes a place-holder PermutationItr
     *
     *  @throws None No throw guarantee.
     */
    PermutationItr() noexcept = default;

    PermutationItr(const PermutationItr & /*rhs*/) = default;
    PermutationItr(PermutationItr && /*rhs*/) noexcept = default;
    PermutationItr &operator=(const PermutationItr & /*rhs*/) = default;
    PermutationItr &operator=(PermutationItr && /*rhs*/) noexcept = default;

    /** @brief Makes a usable PermutationItr.
     *
     *
     * @param input_set The set to iterate over.
     * @param offset  Which permutation to start with.
     * @throws std::bad_alloc if the any of the copies fail.
     */
    PermutationItr(const value_type &input_set, size_type offset)
        : orig_set_(input_set), set_(input_set), offset_(offset) {}

    /// Trivial destructor
    ~PermutationItr() = default;

    /** @brief Returns the element of the parent container currently pointed
     *         to by this iterator.
     *
     *  @return The element being pointed to.
     *  @throws None No throw guarantee.
     */
    const_reference dereference() const { return set_; }

    /** @brief Makes the iterator point to the next permutation.
     *
     *  Permutations are ordered lexicographically and "next" follows from
     *  this convention.
     *
     *  @warning Incrementing beyond the end of the container is allowed;
     *           however, dereferencing the corresponding iterator is
     *           undefined behavior.
     *
     *  @return The iterator after incrementing
     *  @throws None No throw guarantee.
     */
    PermutationItr &increment() noexcept {
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
    bool are_equal(const PermutationItr &rhs) const noexcept {
      return std::tie(orig_set_, set_, offset_) ==
             std::tie(rhs.orig_set_, rhs.set_, rhs.offset_);
    }

    /** @brief Makes the iterator point to the previous permutation.
     *
     *  Permutations are ordered lexicographically and "previous" follows
     *  from this convention.
     *
     *  @warning Decrementing beyond the beginning of the container is
     *           allowed; however, dereferencing the corresponding iterator
     *           is undefined behavior.
     *
     *  @return The iterator after decrementing
     *  @throws None No throw guarantee.
     */
    PermutationItr &decrement() noexcept {
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
    PermutationItr &advance(difference_type n) {
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
    difference_type distance_to(const PermutationItr &other) const noexcept {
      const bool is_greater = (offset_ >= other.offset_);
      const difference_type abs_val =
          (is_greater ? offset_ - other.offset_ : other.offset_ - offset_);
      return (is_greater ? -abs_val : abs_val);
    }

  private:
    /// A copy of the parent's set, doesn't get modified
    value_type orig_set_;

    /// A copy of the parent's set, modified by next/prev permutation
    value_type set_;

    /// The number of increments from the first call
    size_type offset_ = 0;

  }; // End class PermutationItr
};   // End class Permutations

} // namespace UtilitiesEx
