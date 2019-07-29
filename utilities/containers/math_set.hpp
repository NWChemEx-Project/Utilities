#pragma once
#include "utilities/containers/math_set/detail_/math_set_traits.hpp"

namespace utilities {

/** @brief A more mathematical set-like class than std::set
 *
 *  When working with sets one typically needs the ability to do more than what
 *  std::set provides. In particular support is needed for:
 *
 *  - union
 *  - intersection
 *  - difference
 *  - slicing
 *
 *  While to some extent it is possible to do these operations with std::set
 *  most of them will result in copies or verbose call to free-functions. The
 *  `MathSet` class avoids these problems by providing views and built-in
 *  support for standard set operations.
 *
 *  Conceptually MathSet models the power set of an ordered set. What this means
 *  is that internally we store the elements in the order they were inserted.
 *  Unlike std::set the order is stable with respect to insertion. The power set
 *  part of this description means that this container acts as if it holds all
 *  possible subsets of the set. In other words, it is possible to get a
 *  reference to any subset of the input set. These references behave just like
 *  references to the elements of any other container (if you modify the
 *  reference you modify the values in the container).If you want the returned
 *  set to be independent of the parent container make a copy.
 *
 *  Things to note:
 *  - Generally speaking MathSet has characteristics similar to std::vector,
 *    such as storing elements contiguously and having fast insertion onto the
 *    end. MathSet supports arbitrary insertion, but general insertions will be
 *    slow.
 *
 *
 * @tparam element_type The type of the elements in this set. Should be
 *                      copyable, movable, comparable via  operator<, and
 *                      comparable via operator==.
 */
template<typename ElementType>
class MathSet {
private:
    using my_type       = MathSet<ElementType>;
    using traits_t      = detail_::MathSetTraits<ElementType>;
    using pimpl_base    = typename traits_t::pimpl_base;
    using default_pimpl = typename traits_t::default_pimpl;

public:
    using value_type      = typename traits_t::value_type;
    using size_type       = typename traits_t::size_type;
    using reference       = typename traits_t::reference;
    using const_reference = typename traits_t::const_reference;
    using iterator        = typename traits_t::iterator;
    using const_iterator  = typename traits_t::const_iterator;

    MathSet() : MathSet(std::make_shared<default_pimpl>()) {}
    MathSet(const my_type& rhs);
    my_type& operator               =(const my_type& rhs);
    MathSet(my_type&& rhs) noexcept = default;
    my_type& operator=(my_type&& rhs) noexcept = default;
    MathSet(std::initializer_list<ElementType> il);
    template<typename Itr1, typename Itr2>
    MathSet(Itr1&& itr1, Itr2&& itr2);

    explicit MathSet(std::shared_ptr<pimpl_base> pimpl) : m_pimpl_(pimpl) {}

    void swap(my_type& rhs) noexcept { m_pimpl_.swap(rhs.m_pimpl_); }

    iterator begin() { return m_pimpl_->begin(); }
    auto begin() const { return m_pimpl_->begin(); }
    auto cbegin() const { return begin(); }

    iterator end() { return m_pimpl_->end(); }
    auto end() const { return m_pimpl_->end(); }
    auto cend() const { return end(); }

    reference operator[](size_type i) { return (*m_pimpl_)[i]; }
    const_reference operator[](size_type i) const { return (*m_pimpl_)[i]; }

    size_type size() const noexcept { return m_pimpl_->size(); }
    bool empty() const { return cbegin() == cend(); }

    /** @brief Adds an element to the set.
     *
     *  This function will add an element onto the end of the set. No check is
     *  done to ensure that the element is unique.
     *
     */
    void insert(ElementType elem) { m_pimpl_->insert(std::move(elem)); }

    void insert(iterator offset, ElementType elem) {
        m_pimpl_->insert(offset, std::move(elem));
    }

    bool operator==(const MathSet<ElementType>& rhs) const noexcept;
    bool operator!=(const MathSet<ElementType>& rhs) const noexcept;

private:
    std::shared_ptr<pimpl_base> m_pimpl_;
};

//-----------------------------Implementations----------------------------------
/// Improve readability of the MathSet type in the definitions
#define MATH_SET_TYPE MathSet<ElementType>

template<typename ElementType>
MATH_SET_TYPE::MathSet(const MATH_SET_TYPE& rhs) :
  MathSet(std::make_shared<default_pimpl>(rhs.begin(), rhs.end())) {}

template<typename ElementType>
MATH_SET_TYPE& MATH_SET_TYPE::operator=(const MATH_SET_TYPE& rhs) {
    std::make_shared<default_pimpl>(rhs.begin(), rhs.end()).swap(m_pimpl_);
    return *this;
}

template<typename ElementType>
MATH_SET_TYPE::MathSet(std::initializer_list<ElementType> il) :
  MathSet(std::make_shared<default_pimpl>(il)) {}

template<typename ElementType>
template<typename Itr1, typename Itr2>
MATH_SET_TYPE::MathSet(Itr1&& itr1, Itr2&& itr2) : MathSet() {
    while(itr1 != itr2) {
        m_pimpl_->insert(*itr1);
        ++itr1;
    }
}

template<typename ElementType>
bool MATH_SET_TYPE::operator==(const MathSet<ElementType>& rhs) const noexcept {
    return (*m_pimpl_) == (*rhs.m_pimpl_);
}

template<typename ElementType>
bool MATH_SET_TYPE::operator!=(const MathSet<ElementType>& rhs) const noexcept {
    return (*m_pimpl_) != (*rhs.m_pimpl_);
}

    // template<typename ElementType>
    // MathS<const ElementType>
    // MATH_SET_TYPE::intersect_(const MATH_SET_PIMPL_TYPE& rhs) const {
    //    std::vector<size_type> idxs;
    //    for(std::size_t i = 0; i < size(); ++i)
    //        if(rhs.count((*this)[i]) > 0)
    //            idxs.insert(i);
    //    return SelectionViewPIMPL(idxs.begin(), idxs.end(), this);
    //}
    //
    // template<typename ElementType>
    // SelectionViewPIMPL<const ElementType>
    // MATH_SET_PIMPL_TYPE::difference_(const MATH_SET_PIMPL_TYPE& rhs) const {
    //    std::vector<size_type> idxs;
    //    for(std::size_t i = 0; i < size(); ++i)
    //        if(rhs.count((*this)[i]) == 0)
    //            idxs.insert(i);
    //    return SelectionViewPIMPL(idxs.begin(), idxs.end(), this);
    //}
    //
    // template<typename ElementType>
    // SelectionViewPIMPL<ElementType>
    // MATH_SET_PIMPL_TYPE::difference_(const MATH_SET_PIMPL_TYPE& rhs) {
    //    std::vector<size_type> idxs;
    //    for(std::size_t i = 0; i < size(); ++i)
    //        if(rhs.count((*this)[i]) == 0)
    //            idxs.insert(i);
    //    return SelectionViewPIMPL(idxs.begin(), idxs.end(), this);
    //}

#undef MATH_SET_TYPE

} // namespace utilities
