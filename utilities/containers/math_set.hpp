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
     *  If this set is stable
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
bool MATH_SET_TYPE::operator==(const MathSet<ElementType>& rhs) const noexcept {
    return (*m_pimpl_) == (*rhs.m_pimpl_);
}

template<typename ElementType>
bool MATH_SET_TYPE::operator!=(const MathSet<ElementType>& rhs) const noexcept {
    return (*m_pimpl_) != (*rhs.m_pimpl_);
}

#undef MATH_SET_TYPE

} // namespace utilities
