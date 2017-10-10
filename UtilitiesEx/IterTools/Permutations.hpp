#pragma once
#include "UtilitiesEx/Mathematician/Combinatorics.hpp"
#include <algorithm>
#include <iterator> //For iterator tags
#include <limits> //For maximum value of size_t
#include <tuple> //For std::tie

namespace UtilitiesEx {

/** @brief Simulates a container filled with all unique permutations of a
 *         sequence.
 *
 *  This class simulates a container filled with all unique  permutations of a
 *  sequence.  The STL provides next_permutation and prev_permutation for this
 *  purpose; however, these are generators and not containers.  Much of the STL
 *  (as well as foreach loops) assumes a container like syntax, hence the need
 *  for this class.
 *
 *  Note for a sequence of @f$N@f$ elements there are @f$N!@f$ permutations. If
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
 *  @note This container contains @b{all} the unique permutations.  The STL
 *  variants, on the other hand, stop at the lexicographically largest
 *  (next_permutation) or lexicographically smallest (prev_permutation)
 *  permutation.
 *
 *  @tparam sequence_type The type of the container used to hold the sequence.
 *          The type must satisfy sequence container.
 */
template<typename SequenceType>
class Permutations {
private:
    class PermutationItr;
public:
    ///Type of this class
    using my_type=Permutations<SequenceType>;

    ///Type of an element of this container
    using value_type=SequenceType;

    ///Permutations are non-mutable so same as const_reference
    using reference=const value_type&;

    ///Type of a const reference to an element of this container
    using const_reference=const value_type&;

    ///Non-mutable container so same as const_iterator
    using iterator=PermutationItr;

    ///Type of a non-mutating iterator to this container
    using const_iterator=PermutationItr;

    ///Type of the difference between two iterators
    using difference_type=long int;

    ///The type of an offset
    using size_type=std::size_t;

    /** @brief Constructs a container filled with all permutations of the empty
     *         set, of which there is one, @em{i.e.} the empty set.
     *
     *  @throws None No throw guarantee.
     */
    Permutations()noexcept=default;

    /** @brief Fills container with all permutations of \p input_set
     *
     *  @param[in] input_set The set whose permutations will grace this container.
     *  @throws std::bad_alloc If there is not enough memory to copy the input.
     *          Strong throw guarantee.
     */
    Permutations(const_reference input_set);

    /** @brief Deep copies another Permutations instance.
     *
     *  @param[in] rhs The instance to copy.
     *  @throws std::bad_alloc if there is not enough memory to copy \p rhs.
     *          Strong throw guarantee.
     */
    Permutations(const my_type& /*rhs*/)=default;

    /** @copybrief Permutations(const Permutations&)
     *
     *  @param[in] rhs the instance to copy.
     *  @throws std::bad_alloc if there is not enough memory to copy \p rhs.
     *          Strong throw guarantee.
     */
    Permutations& operator=(const my_type& /*rhs*/)=default;

    /** @brief Takes ownership of another container
     *
     *  @param[in] rhs The set of permutations to take ownership of
     *  @throws None No throw guarantee.
     */
    Permutations(my_type&& /*rhs*/)noexcept=default;

    /** @brief Takes ownership of another container
     *
     *  @param[in] rhs The set of permutations to take ownership of
     *  @returns This instance after taking ownership
     *  @throws None No throw guarantee.
     */
    my_type& operator=(my_type&& /*rhs*/)noexcept=default;

    /** @brief Frees memory associated with container.
     *
     *  @throws None No throw guarantee.
     */
    ~Permutations()=default;

    /** @brief Creates an iterator for this container that points to the first
     *         element of the container.
     *
     *  The returned iterator's state is decoupled from the container.
     *  This is done because next_permutation and prev_permutation actually
     *  modify the contents of the container they operate on.
     *
     *  @return An iterator pointing to the first element of the container.
     *  @throws std::bad_alloc if there is not enough memory to copy the set to
     *          the iterator.  Strong throw guarantee.
     */
    iterator begin()const
    {
        return PermutationItr(original_set_,0);
    }

    /** @brief Creates an iterator just past the last element of this container.
     *
     *  @copydetails Permutations::begin()
     *
     *  @return An iterator set to just past the last element of this container.
     *  @throws std::bad_alloc if there is not enough memory to copy the set to
     *          the iterator.  Strong throw guarantee.
     */
    iterator end()const
    {
        return PermutationItr(original_set_,size_);
    }

    /** @copydoc Permutations::begin()
     *
     */
    const_iterator cbegin()const
    {
        return begin();
    }

    /** @copydoc Permuations::end()
     *
     */
    const_iterator cend()const
    {
        return end();
    }

    /** @brief True if this contains the same permutations as @p other.
     *
     *  Since we don't actually store all the permutations this function
     *  actually works by determining if this and \p other contain sets that
     *  permutations of one another.
     *
     *  @param[in] other The container to compare to.
     *  @return True if this container and @p other contain the same
     *               permutations.
     *  @throws None No throw guarantee.
     */
    bool operator==(const my_type& other)const noexcept;

    /** @brief True if this contains different permutations from @p other.
     *
     *  @copydetails Permutations::operator==(Permutations)
     *  @return True if this container holds different permutations.
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
        std::swap(size_,other.size_);
    }

    /** @brief Returns the number of elements in this container.
     *
     *  @return The number of permutations in this container
     *  @throws None No throw guarantee.
     */
    size_type size()const noexcept
    {
        return size_;
    }

    /** @brief Returns the maximum possible size of this container.
     *
     *  Since this container does not actually contain its elements, so long as
     *  the number of unique permutations is expressible as a size_type integer
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
    ///This is the set we are generating all unique permutations of
    value_type original_set_;

    ///This is the number of permutations in the container
    size_type size_=1;

    /** @brief The iterator actually returned by the Permutations class
     *
     *  This class is written in terms of next/prev permutation which wrap
     *  around on themselves.  When they wrap they return the first element
     */
    class PermutationItr {
    public:
        ///The type of a permutation returned by this iterator
        using value_type=SequenceType;

        ///The type of the difference between two iterators
        using difference_type=long int;

        ///The type of a permutation
        using reference=const value_type&;

        ///The type of a const permutation
        using const_reference=const value_type&;

        ///The type of a pointer to a permutation
        using pointer=const value_type*;

        ///The type of a const pointer to a permutation
        using const_pointer=const value_type*;

        ///The iterator tag
        using iterator_category=std::bidirectional_iterator_tag;

        ///The type of the internal offset maintained
        using size_type=std::size_t;

        /** @brief Makes a place-holder PermutationItr
         *
         *  @thros None No throw guarantee.
         */
        PermutationItr()noexcept=default;

        /** @brief Makes a new permutation iterator over a given set.
         *
         *  @param[in] input_set The set to iterate over.
         *  @param[in] offset The value this iterator starts at.
         *  @throws std::bad_alloc If the copy fails because of lack of memory.
         *          Strong throw guarantee.
         */
        PermutationItr(const_reference input_set, size_type offset):
            set_(input_set),offset_(offset)
        {}

        /** @brief Makes a deep copy of the current instance.
         *
         *  @param[in] rhs The Permutation to copy.
         */
        PermutationItr(const PermutationItr& /*rhs*/)=default;

        /** @brief Assigns a deep copy to the current instance.
         *
         *  @param[in] rhs The Permutation to copy.
         *  @returns The current iterator after the copy
         */
        PermutationItr& operator=(const PermutationItr& /*rhs*/)=default;

        /** @brief Takes ownership of another iterator
         *
         *  @param[in] rhs The iterator we are going to own
         *  @throws None No throw guarantee.
         */
        PermutationItr(PermutationItr&& /*rhs*/)noexcept=default;

        /** @copybrief
         *  @param[in] rhs The iterator to take ownership of.
         *  @throws None No throw guarantee.
         */
        PermutationItr& operator=(PermutationItr&& /*rhs*/)noexcept=default;

        ~PermutationItr()=default;

        /** @brief Returns the element of the parent container currently pointed
         *         to by this iterator.
         *
         *  @return The element being pointed to.
         *  @throws None No throw guarantee.
         */
        const_reference operator*()const noexcept
        {
            return set_;
        }

        /** @brief Allows access to the current permutations members.
         *
         *
         *  @return The current permutation for use with the -> operator
         *  @throws None No throw guarantee.
         */
        const_pointer operator->()const noexcept
        {
            return &set_;
        }

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
        PermutationItr& operator++()noexcept;

        /** @copydoc PermutationItr::operator++()
         *
         *  @return A copy of the iterator before incrementing
         *  @throws None No throw guarantee.
         */
        PermutationItr operator++(int)noexcept;

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
        PermutationItr& operator--()noexcept;

        /** @copydoc PermutationItr::operator--()
         *
         *  @return A copy of the iterator before decrementing
         *  @throws None No throw guarantee.
         */
        PermutationItr operator--(int)noexcept;

        /** Compares two PermutationItrs for exact equality
         *
         *  Exact equality is defined as pointing to the same permutation,
         *  having the same starting permutation, and having both wrapped (or
         *  not wrapped).
         *
         *  @param[in] other The iterator to compare to.
         *  @return True if this iterator is exactly the same as @p other
         *  @throws None No throw guarantee.
         */
        bool operator==(const PermutationItr& other)const noexcept;

        /** Compares two PermutationItrs for any difference
         *
         *  @copydetails PermutationItr::operator==(const PermutationItr&)
         *  @param[in] other The iterator to compare to.
         *  @return True if this iterator is not exactly the same as @p other
         *  @throws None No throw guarantee.
         */
        bool operator!=(const PermutationItr& other)const noexcept
        {
            return !((*this)==other);
        }

    private:
        ///A copy of the parent's set
        value_type set_;

        ///The number of increments from the first call
        size_type offset_;

    };//End class PermutationItr
}; //End class Permutations

/******************************************************************************
 * Definitions.
 ******************************************************************************/

/////////////////////////////////  Permutations  ///////////////////////////////
//Permutation range constructor
template<typename container_type>
Permutations<container_type>::Permutations(const container_type& input_set):
    original_set_(input_set),size_(0)
{
    container_type temp(input_set);
    std::vector<size_type> counts;
    std::sort(temp.begin(),temp.end());
    auto ei=temp.begin();
    while(ei!=temp.end())
    {
        const size_type n_ei=std::count(ei,temp.end(),*ei);
        counts.push_back(n_ei);
        ei+=n_ei;
    }
    size_=multinomial_coefficient<size_type>(counts);
}

//Permutation equality
template<typename container_type>
bool Permutations<container_type>::operator==(
        const Permutations<container_type>& other)const noexcept
{
    if(size_ != other.size_) return false;
    return std::is_permutation(original_set_.begin(),original_set_.end(),
                               other.original_set_.begin());
}

///////////////////////////////// PermutationItr ///////////////////////////////


//PermutationItr prefix increment operator
template<typename container_type>
typename Permutations<container_type>::PermutationItr&
Permutations<container_type>::PermutationItr::operator++()noexcept
{
    std::next_permutation(set_.begin(), set_.end());
    ++offset_;
    return *this;
}

//PermutationItr postfix increment operator
template<typename container_type>
typename Permutations<container_type>::PermutationItr
Permutations<container_type>::PermutationItr::operator++(int)noexcept
{
    PermutationItr copy(*this);
    ++(*this);
    return copy;
}

//PermutationItr prefix decrement operator
template<typename container_type>
typename Permutations<container_type>::PermutationItr&
Permutations<container_type>::PermutationItr::operator--()noexcept
{
    std::prev_permutation(set_.begin(), set_.end());
    --offset_;
    return *this;
}

//PermutationItr postfix decrement operator
template<typename container_type>
typename Permutations<container_type>::PermutationItr
Permutations<container_type>::PermutationItr::operator--(int)noexcept
{
    PermutationItr copy(*this);
    --(*this);
    return copy;
}

//PermutationItr equality operator
template<typename container_type>
bool
Permutations<container_type>::PermutationItr::operator==(
        const Permutations<container_type>::PermutationItr& other)const noexcept
{
    return std::tie(set_,offset_) ==
           std::tie(other.set_,other.offset_);
}

} //End namespace
