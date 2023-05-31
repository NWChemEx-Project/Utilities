/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "utilities/containers/indexable_container_base.hpp"
#include "utilities/mathematician/combinatorics.hpp"
#include <algorithm> //For is_permutation, next/prev permutation

namespace utilities {

/** @brief Facilitates looping over all unique permutations of a set.
 *
 *  An instance of the Permutations class simulates a container filled with all
 *  unique permutations of a sequence. The container only actually holds two
 *  copies of the sequence (the original and the current sequence) and updates
 *  the current sequence as appropriate. This class is primarily designed to be
 *  used like:
 *
 *  @code
 *  std::vector<int> s{1, 2, 3};
 *  for(auto perm : Permutations(s)){
 *      // Do something with the permutation.
 *  }
 *  @endcode
 *
 *  @tparam SequenceType The type of the container holding the input sequence.
 *                       The type must support copy construction.
 */
template<typename SequenceType>
class Permutations : public IndexableContainerBase<Permutations<SequenceType>> {
private:
    /// Type of this instance
    using my_type = Permutations<SequenceType>;
    /// Type of the base class
    using base_type = IndexableContainerBase<my_type>;

public:
    /// Type of the original sequence and each permutation of it
    using value_type = SequenceType;
    /// Type of an index/offset
    using size_type = typename base_type::size_type;

    /** @brief Creates a container that "holds" all permutations of the input
     *         sequence.
     *
     *  This ctor will create a new container that simulates a container filled
     *  with all unique Permutations of @p seq. The resulting container does
     *  not actually hold all such permutations, but generates them on the fly.
     *
     *  @param[in] seq The sequence we want all unique permutations of.
     *
     *  @throw std::bad_alloc if there is insufficient memory to store two
     *         copies of the sequence. Strong throw guarantee.
     */
    explicit Permutations(SequenceType seq);

private:
    friend base_type;
    /// Implements size() for base class by returning the pre-computed size
    size_type size_() const noexcept { return m_nperms_; }
    /// Implements operator[] for base class
    const SequenceType& at_(size_type i) const;

    /// Lexicographically speaking, this is the 0-th permutation
    SequenceType m_sorted_orig_;
    /// The last permutation we returned
    mutable SequenceType m_buffer_;
    /// The index of the last permutation we returned
    mutable size_type m_counter_;
    /// Offset of the original permutation from the 0 permutation
    size_type m_dx_;
    /// The total number of permutations in this container
    size_type m_nperms_;
}; // class Permutations

// ------------------------------Implementation--------------------------------

template<typename SequenceType>
Permutations<SequenceType>::Permutations(SequenceType seq) :
  m_sorted_orig_(seq),
  m_buffer_(seq),
  m_counter_(0),
  m_dx_(0),
  m_nperms_(n_permutations(seq)) {
    std::sort(m_sorted_orig_.begin(), m_sorted_orig_.end());
    m_dx_ = permutation_to_decimal(seq, m_sorted_orig_);
}

template<typename SequenceType>
const SequenceType& Permutations<SequenceType>::at_(size_type i) const {
    // We can quickly generate the current, next, and previous permutations, so
    // pull those scenarios out of the general case.
    if(i == m_counter_ + 1) {
        std::next_permutation(m_buffer_.begin(), m_buffer_.end());
    } else if(m_counter_ != 0 && i == m_counter_ - 1) {
        std::prev_permutation(m_buffer_.begin(), m_buffer_.end());
    } else if(i != m_counter_) {
        m_buffer_ =
          decimal_to_permutation((m_dx_ + i) % m_nperms_, m_sorted_orig_);
    }
    m_counter_ = i;
    return m_buffer_;
}

} // namespace utilities
