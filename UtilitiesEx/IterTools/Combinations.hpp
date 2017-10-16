#pragma once
#include "UtilitiesEx/IterTools/Permutations.hpp"

namespace UtilitiesEx {

/** @brief A container for holding all combinations of a sequence of numbers.
 *
 *  Given a sequence of @f$N@f$ elements, there are @f{N \choose m}@f$ possible
 *  ways to choose @f$m@f$ elements if repetitions are not allowed.  Like its
 *  Permutations container brethern, this class doesn't actually store the
 *  combinations, but rather computes them on the fly.
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
 *  @tparm SequenceType The sequence for which we are generating combinations.
 */
template<typename SequenceType>
class Combinations {
private:
    class CombinationItr;
public:
    ///Type of this class
    using my_type=Combinations<SequenceType>;

    ///Type of an element of this container
    using value_type=SequenceType;

    ///Combinations are non-mutable so same as const_reference
    using reference=const value_type&;

    ///Type of a const reference to an element of this container
    using const_reference=const value_type&;

    ///Non-mutable container so same as const_iterator
    using iterator=CombinationItr;

    ///Type of a non-mutating iterator to this container
    using const_iterator=CombinationItr;

    ///Type of the difference between two iterators
    using difference_type=long int;

    ///The type of an offset
    using size_type=std::size_t;

    /** @brief Constructs a container filled with all Combinations of the empty
     *         set, of which there is one, @em{i.e.} the empty set.
     *
     *  @throws None No throw guarantee.
     */
    Combinations()noexcept=default;

    /** @brief Fills container with all Combinations of \p input_set
     *
     *  @param[in] input_set The set whose Combinations will grace this
     *             container.
     *  @param[in] k The number of objects chosen at a time
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
    Combinations(const my_type& /*rhs*/)=default;

    /** @copybrief Combinations(const Combinations&)
     *
     *  @param[in] rhs the instance to copy.
     *  @throws std::bad_alloc if there is not enough memory to copy \p rhs.
     *          Strong throw guarantee.
     */
    Combinations& operator=(const my_type& /*rhs*/)=default;

    /** @brief Takes ownership of another container
     *
     *  @param[in] rhs The set of Combinations to take ownership of
     *  @throws None No throw guarantee.
     */
    Combinations(my_type&& /*rhs*/)noexcept=default;

    /** @brief Takes ownership of another container
     *
     *  @param[in] rhs The set of Combinations to take ownership of
     *  @returns This instance after taking ownership
     *  @throws None No throw guarantee.
     */
    my_type& operator=(my_type&& /*rhs*/)noexcept=default;

    /** @brief Frees memory associated with container.
     *
     *  @throws None No throw guarantee.
     */
    ~Combinations()=default;

    /** @brief Creates an iterator for this container that points to the first
     *         element of the container.
     *
     *  The returned iterator's state is decoupled from the container.
     *  This is done because next_Combination and prev_Combination actually
     *  modify the contents of the container they operate on.
     *
     *  @return An iterator pointing to the first element of the container.
     *  @throws std::bad_alloc if there is not enough memory to copy the set to
     *          the iterator.  Strong throw guarantee.
     */
    iterator begin()const
    {
        return CombinationItr(original_set_,k_,false);
    }

    /** @brief Creates an iterator just past the last element of this container.
     *
     *  @copydetails Combinations::begin()
     *
     *  @return An iterator set to just past the last element of this container.
     *  @throws std::bad_alloc if there is not enough memory to copy the set to
     *          the iterator.  Strong throw guarantee.
     */
    iterator end()const
    {
        return CombinationItr(original_set_,k_,true);
    }

    /** @copydoc Combinations::begin()
     *
     */
    const_iterator cbegin()const
    {
        return begin();
    }

    /** @copydoc Combinations::end()
     *
     */
    const_iterator cend()const
    {
        return end();
    }

    /** @brief True if this contains the same Combinations as @p other.
     *
     *
     *  @param[in] other The container to compare to.
     *  @return True if this container and @p other contain the same
     *               Combinations.
     *  @throws None No throw guarantee.
     */
    bool operator==(const my_type& other)const noexcept;

    /** @brief True if this contains different Combinations from @p other.
     *
     *  @copydetails Combinations::operator==(Combinations)
     *  @return True if this container holds different Combinations.
     *  @throws None No throw guarantee.
     *
     */
    bool operator!=(const my_type& other)const noexcept
    {
        return !((*this)==other);
    }

    /** @brief Swaps the contents of this container with another
     *
     *  @param[i] other The container to swap contents with
     *  @throws None No throw guarantee.
     */
    void swap(my_type& other)noexcept
    {
        std::swap(original_set_,other.original_set_);
        std::swap(k_, other.k_);
        std::swap(size_,other.size_);
    }

    /** @brief Returns the number of elements in this container.
     *
     *  @return The number of Combinations in this container
     *  @throws None No throw guarantee.
     */
    size_type size()const noexcept
    {
        return size_;
    }

    /** @brief Returns the maximum possible size of this container.
     *
     *  Since this container does not actually contain its elements, so long as
     *  the number of unique Combinations is expressible as a size_type integer
     */
    constexpr size_type max_size()const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

    /** @brief Returns true if the container is empty.
     *
     *  @returns true if the container is empty.
     *  @throws None No throw guarantee.
     */
    bool empty()const noexcept
    {
        return !original_set_.size();
    }

private:
    ///This is the set we are generating all unique Combinations of
    value_type original_set_;

    ///The number of objects taken at a time
    size_type k_=0;

    ///This is the number of Combinations in the container
    size_type size_=1;

    /** @brief The iterator actually returned by the Combinations class
     *
     *  This class is written in terms of the PermutationItr class.  As a
     *  technical note we invert the usual 0=false, 1=true mapping to get the
     *  unique permutations back in lexicographical order.
     */
    class CombinationItr : public
        detail_::RandomAccessIteratorBase<CombinationItr,SequenceType> {
    public:

        /** @brief Makes a new Combination iterator over a given set.
         *
         *  @param[in] input_set The set to iterate over.
         *  @param[in] k The number of objects chosen at a time.
         *  @param[in] at_end True if this is the end iterator
         *  @throws std::bad_alloc If the copy fails because of lack of memory.
         *          Strong throw guarantee.
         */
        CombinationItr(const_reference input_set, size_type offset, bool at_end);

        /** @brief Makes a deep copy of the current instance.
         *
         *  @param[in] rhs The Combination to copy.
         */
        CombinationItr(const CombinationItr& /*rhs*/)=default;

        /** @brief Assigns a deep copy to the current instance.
         *
         *  @param[in] rhs The Combination to copy.
         *  @returns The current iterator after the copy
         */
        CombinationItr& operator=(const CombinationItr& /*rhs*/)=default;

        /** @brief Takes ownership of another iterator
         *
         *  @param[in] rhs The iterator we are going to own
         *  @throws None No throw guarantee.
         */
        CombinationItr(CombinationItr&& /*rhs*/)noexcept=default;

        /** @copybrief
         *  @param[in] rhs The iterator to take ownership of.
         *  @throws None No throw guarantee.
         */
        CombinationItr& operator=(CombinationItr&& /*rhs*/)noexcept=default;

        ~CombinationItr()=default;

        /** @brief Returns the element of the parent container currently pointed
         *         to by this iterator.
         *
         *  @return The element being pointed to.
         *  @throws None No throw guarantee.
         */
        const_reference dereference()const noexcept
        {
            return comb_;
        }


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
        CombinationItr& increment()noexcept
        {
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
        CombinationItr& decrement()noexcept
        {
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
        CombinationItr& advance(difference_type n)
        {
            current_perm_+=n;
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
        bool are_equal(const CombinationItr& other)const noexcept
        {
            return std::tie(set_,current_perm_) ==
                   std::tie(other.set_,other.current_perm_);
        }

        /** @brief Returns the distance between this iterator and another
         *
         * @param[in] rhs The iterator we want the distance to.
         * @returns the distance between the two iterators
         */
        difference_type distance_to(const CombinationItr& rhs)const noexcept
        {
            return current_perm_ - rhs.current_perm_;
        }

    private:
        ///A copy of the parent's set
        value_type set_;

        ///The current combination
        value_type comb_;

        ///The current permutation
        typename Permutations<std::vector<bool>>::const_iterator current_perm_;

        ///Updates @p comb_ to be consistent with current_perm_
        void update_comb();

    };//End class CombinationItr
}; //End class Combinations

/******************************************************************************
 * Definitions.
 ******************************************************************************/

/////////////////////////////////  Combinations  ///////////////////////////////
//Combination range constructor
template<typename container_type>
Combinations<container_type>::Combinations(const container_type& input_set,
                                           std::size_t k):
        original_set_(input_set),k_(k),
        size_(binomial_coefficient<size_type>(input_set.size(),k))
{
}

//Combination equality
template<typename container_type>
bool Combinations<container_type>::operator==(
        const Combinations<container_type>& other)const noexcept
{
    if(size_ != other.size_) return false;
    return std::is_permutation(original_set_.begin(),original_set_.end(),
                               other.original_set_.begin());
}

///////////////////////////////// CombinationItr ///////////////////////////////

template<typename container_type>
void Combinations<container_type>::CombinationItr::update_comb()
{
    const auto& p=*current_perm_;
    for(size_type i=0,counter=0; i<p.size(); ++i)
    {
        if(!p[i])
            comb_[counter++]=set_[i];
        if(counter==comb_.size())
            break;
    }
}

template<typename container_type>
Combinations<container_type>::CombinationItr::CombinationItr(
        const container_type& input_set,
        std::size_t k,
        bool at_end):
    set_(input_set),comb_(k)
{
    std::vector<bool> temp(input_set.size(),true);
    for(size_t i=0;i<k;++i)
        temp[i]=false;
    Permutations<std::vector<bool>> perms(temp);
    current_perm_=(at_end ? perms.end() : perms.begin());
    update_comb();
}



} //End namespace

