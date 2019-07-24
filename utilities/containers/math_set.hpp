#pragma once
#include "utilities/containers/detail_/math_set_traits.hpp"

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
 *  At the moment this class is only capable of being used as a stable ordered
 *  set (elements will be stored in the insertion order).
 *
 * @tparam element_type The type of the elements in this set. Should be
 *                      copyable, movable, comparable via  operator<, and
 *                      comparable via operator==.
 *
 * @warning Be very careful using this class with automatic template type
 *          parameter deduction. In particular take note of the following:
 *          @code
 *          std::vector<int> v{1, 2, 3, 4, 5};
 *
 *          MathSet s1{v}; // Makes MathSet<std::vector<int>> not MathSet<int>
 *          MathSet s2(v); // Use parentheses to make MathSet<int>
 *
 *          MathSet s3{s2}; //Makes MathSet<int> not MathSet<MathSet<int>>
 *          MathSet s4{{s2}} //Makes MathSet<MathSet<int>>
 *          @endcode
 *          When in doubt specify the template type parameter explicitly.
 */
template<typename ElementType>
class MathSet {
    using pimpl_type = typename detail_::MathSetTraits<ElementType>::pimpl_type;

public:
    using value_type      = typename pimpl_type::value_type;
    using size_type       = typename pimpl_type::size_type;
    using reference       = typename pimpl_type::reference;
    using const_reference = typename pimpl_type::const_reference;
    using iterator        = typename pimpl_type::iterator;
    using const_iterator  = typename pimpl_type::const_iterator;

    MathSet() = default;
    MathSet(std::initializer_list<ElementType> il) : m_pimpl_(il) {}

    iterator begin() { return m_pimpl_.begin(); }
    const_iterator begin() const { return m_pimpl_.begin(); }
    const_iterator cbegin() const { return begin(); }

    iterator end() { return m_pimpl_.end(); }
    const_iterator end() const { return m_pimpl_.end(); }
    const_iterator cend() const { return end(); }

    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

    size_type size() const noexcept { return m_pimpl_.size(); }
    bool empty() const { return cbegin() == cend(); }

    /** @brief Adds an element to the set.
     *
     *  If this set is stable
     *
     */
    void insert(ElementType elem) { m_pimpl_.insert(std::move(elem)); }

    template<typename Itr1, typename Itr2>
    void insert(Itr1 itr1, Itr2&& itr2);

private:
    pimpl_type m_pimpl_;
};

//-----------------------------Implementations----------------------------------
/// Improve readability of the MathSet type in the definitions
#define MATH_SET_TYPE MathSet<ElementType>

template<typename ElementType>
typename MATH_SET_TYPE::reference MATH_SET_TYPE::operator[](size_type i) {
    auto itr = begin();
    std::advance(itr, i);
    return *itr;
}

template<typename ElementType>
typename MATH_SET_TYPE::const_reference MATH_SET_TYPE::operator[](
  size_type i) const {
    auto itr = begin();
    std::advance(itr, i);
    return *itr;
}

template<typename ElementType>
template<typename Itr1, typename Itr2>
void MATH_SET_TYPE::insert(Itr1 itr1, Itr2&& itr2) {
    while(itr1 != itr2) {
        m_pimpl_.insert(*itr1);
        ++itr1;
    }
}

#undef MATH_SET_TYPE

} // namespace utilities
