#pragma once
#include "utilities/iter_tools/permutations.hpp"

namespace utilities {

/** @brief A container that holds all combinations of a sequence.
 *
 *  The Combinations class simulates a container that is filled will all
 *  combinations of a sequence (optionally with repeating elements allowed). The
 *  container does not actually store all of these combinations (it only stores
 *  two: the original and an internal buffer holding the last returned
 *  combination), but rather generates them on-the-fly.
 *
 *  @tparam SequenceType The type of the input sequence and subsequently all
 *                       returned combinations.
 */
template<typename SequenceType>
class Combinations : public IndexableContainerBase<Combinations<SequenceType>> {
private:
    /// The type of this instance
    using my_type = Combinations<SequenceType>;
    /// The type of the base class
    using base_type = IndexableContainerBase<my_type>;

public:
    /// Non-cv-qualified type stored in this container
    using value_type = SequenceType;
    /// Type used for indexing and offsets
    using size_type = typename base_type::size_type;

    /** @brief Makes a new Combinations instance that holds all combinations
     *         involving @p k elements that can be formed from @p seq.
     *
     * This ctor creates a new Combinations instance that can be used to loop
     * over all unique `k`-tuples that can be formed from the input sequence.
     * Optionally, the stored `k`-tuples can be "multichoose" combinations where
     * elements in the input sequence can appear multiple times.
     *
     * @param[in] seq The set of elements from which combinations will be taken.
     *                The elements in @p seq must be unique in order to only
     *                generate unique combinations. If an element is repeated
     *                combinations involving that element will appear multiple
     *                times.
     * @param[in] k The number of elements to take from @p seq at a time.
     * @param[in] repeat If true, then elements of @p seq may appear more than
     *                   once in any particular combination. Default is false.
     */
    Combinations(SequenceType seq, size_type k, bool repeat = false);

private:
    /// Gives base class access to the implementations needed for CRTP
    friend base_type;
    /// Implements size() for base class
    size_type size_() const noexcept { return m_perm_.size(); }
    /// Implements operator[] for base class
    const SequenceType& at_(size_type index) const;

    /// The original set of elements
    SequenceType m_set_;
    /// The buffer used to store the current combination
    mutable SequenceType m_comb_;
    /// The number of elements to take at a time (the "k" in "n choose k")
    size_type m_k_;
    /// Stores a list of all possible combinations in terms of membership
    Permutations<std::vector<bool>> m_perm_;
    /// Are repeats allowed
    bool m_repeat_;
}; // class Combinations

// ------------------------------Implementations--------------------------------

template<typename SequenceType>
Combinations<SequenceType>::Combinations(SequenceType seq, size_type k,
                                         bool repeat) :
  m_set_(seq),
  m_comb_(),
  m_k_(k),
  m_perm_([=]() {
      const auto n        = seq.size();
      const auto eff_size = (n || k ? n + k - 1 : 0);
      std::vector<bool> temp(!repeat ? n : eff_size, true);
      for(std::size_t i = 0; i < k; ++i) temp[i] = false;
      return Permutations(temp);
  }()),
  m_repeat_(repeat) {}

template<typename SequenceType>
const SequenceType& Combinations<SequenceType>::at_(size_type index) const {
    m_comb_   = SequenceType(m_k_);
    auto perm = m_perm_[index];
    for(size_type i = 0, counter = 0, bar_count = 0; i < perm.size(); ++i) {
        if(!perm[i])
            m_comb_[counter++] = m_set_[!m_repeat_ ? i : bar_count];
        else
            ++bar_count;
        if(counter == m_comb_.size()) // Early termination, rest are false
            break;
    }
    return m_comb_;
}
} // End namespace utilities
