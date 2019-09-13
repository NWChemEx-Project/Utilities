#pragma once
#include "utilities/containers/math_set/detail_/math_set_traits.hpp"
#include "utilities/containers/math_set/detail_/nested_set_pimpl.hpp"
#include "utilities/iter_tools/enumerate.hpp"
#include <map>
#include <set>

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
 *  `MathSet` class avoids these problems by transparently using views and by
 *  providing built-in support for standard set operations.
 *
 *  Conceptually MathSet models the power set of an ordered set. Unlike std::set
 *  the order is stable with respect to insertion, namely, the elements are
 *  stored in insertion order and are not sorted like std:;set. The power set
 *  part of the description means that this container acts as if its elements
 *  include the power set (all possible subsets) of the set. In other words, it
 *  is possible to get read/write or read-only references to any subset of the
 *  set contained in a MathSet instance. These references behave just like
 *  references to the elements of any other container. Modifying the references
 *  modifies the value in the container. This aliasing behavior is avoided if
 *  instead of taking the element by reference you take it by value. In this
 *  case a deep copy will be made and the result will be independent of the
 *  original container.
 *
 *  @warning At the moment it is your responsability to ensure that you do not
 *           invalidate the uniqueness of the elements through direct
 *           modification. For example:
 *           ```
 *           MathSet s{1, 2, 3};
 *           auto& s0 = s[0];
 *           // Don't do this next line
 *           //s0 = 2; Would cause s to be {2, 2, 3}
 *           ```
 *           This warning only applies to working with references to elements.
 *           If you go through the public API of the MathSet class, the MathSet
 *           class will enforce uniqueness automatically.
 *
 * @tparam element_type The type of the elements in this set. Should be
 *                      copyable, movable, comparable via  operator<, and
 *                      comparable via operator==.
 */
template<typename ElementType>
class MathSet {
private:
    /// Type of this instance
    using my_type = MathSet<ElementType>;
    /// Type of the class providing the type traits for this instance
    using traits_t = detail_::MathSetTraits<ElementType>;
    /// Type of the base class common to all MathSetPIMPL instances
    using pimpl_base = detail_::MathSetPIMPL<ElementType>;
    /// Type of owning PIMPL that should be used as a default PIMPL
    using default_pimpl = typename traits_t::default_pimpl;
    /// Type of a pointer to a PIMPL
    using pimpl_ptr = std::unique_ptr<pimpl_base>;

public:
    /// Non-cv type of the elements stored in this container
    using value_type = typename pimpl_base::value_type;
    /// Type of an index or offset into this container
    using size_type = typename pimpl_base::size_type;
    /// Type of a read/write reference to an element of this container
    using reference = typename pimpl_base::reference;
    /// Type of a read-only reference to an element of this container
    using const_reference = typename pimpl_base::const_reference;
    /// Type of an iterator over this set that allows modifying the elements
    using iterator = typename pimpl_base::iterator;
    /// Type of a read-only iterator over this set
    using const_iterator = typename pimpl_base::const_iterator;

    /** @brief Creates an empty set.
     *
     *  This constructor creates an empty set. Elements can be added to the
     *  resulting set by calling insert. Any elements added to the resulting
     *  instance will be owned by the instance.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL. Strong throw guarantee.
     */
    MathSet() : MathSet(std::make_unique<default_pimpl>()) {}

    /** @brief Makes a deep copy of another MathSet
     *
     *  This ctor will initialize a new MathSet instance with a deep copy of
     *  another MathSet's state. Note that in particular this means that if you
     *  are copying from an alias the new instance will be decoupled from the
     *  alias's parent container.
     *
     *  @param[in] rhs The MathSet instance to make a deep copy of.
     *
     *  @throw std::bad_alloc if there is insufficient memory to perform the
     *                        copy. Strong throw guarantee.
     */
    MathSet(const my_type& rhs);

    /** @brief Transfers ownership of another instance's state to a new MathSet
     *         instance.
     *
     *  This ctor can be used to transfer ownership of a MathSet's state to a
     *  new instance. The resulting instance maintains any aliasing relations
     *  that @p rhs possessed.
     *
     *  @param[in] rhs The instance which originally owns the state. After this
     *                  function is called @p rhs will be in a valid, but
     *                  otherwise undefined state.
     *
     *  @throw none No throw guarantee.
     */
    MathSet(my_type&& rhs) noexcept = default;

    /* Creating a new MathSet by copy/move of another MathSet is well defined;
     * however, when you try to copy/move assign ambiguities arise.
     *
     *  Let A be an alias of a subset of B. Then for some other set C what does
     *  A = C do?
     *
     *  If C is completely disjoint from B (and therefore A as well) one can
     *  argue it makes sense to remove A's elements from B, add C's elements to
     *  B, and then make A alias the new elements. But where do we put C's
     *  elements in B? Do we replace A's elements? If so, what do we do if A and
     *  C have different numbers of elements? If we do not replace A's elements
     *  do we then just append C's elements onto B's elements?
     *
     *  If C contains one or more elements from B (and or A) it makes sense to
     *  remove the elements of A not in C from B, for each element in C also in
     *  B make A point to the existing element, and then follow the above
     *  protocol for new elements. If the elements common to C and B are not in
     *  the same order in both C and B then what order do we store them in, C's
     *  or B's?
     */
    my_type& operator=(const my_type& rhs) = delete;
    my_type& operator=(my_type&& rhs) = delete;

    /** @brief Creates a new MathSet initialized with the provided contents
     *
     *  This ctor can be used to create a MathSet instance whose state is
     *  initialized to the contents of the provided initializer list. This class
     *  does **NOT** ensure that the elements in the provided list are unique.
     *
     *  @note To initialize a MathSet<MathSet<T>> instance with an initializer
     *        list containing a single element it is necessary to explicitly
     *        specify the template parameter, i.e., you can not use automatic
     *        template type deduction or it will trigger the copy ctor.
     *
     * @param[in] il The list of elements the set should initially contain.
     *
     * @throw std::bad_alloc if their is insufficient memory to hold the initial
     *                       elements. Strong throw guarantee.
     */
    MathSet(std::initializer_list<ElementType> il);

    /** @brief Creates a MathSet instance that contains the value in the
     *         provided range.
     *
     *  This ctor will populate the resulting instance with the elements in the
     *  provided range.
     *
     * @tparam Itr1 The type of the iterator pointing to the first element in
     *              the range.
     * @tparam Itr2 The type of the iterator pointing to just past the last
     *              element in the range.
     *
     * @param[in] itr1 An iterator pointing to the first element that should be
     *                 included in the newly created set.
     * @param[in] itr2 An iterator pointing to just past the last element in the
     *                 range.
     *
     * @throw std::bad_alloc if there is insufficient memory to store the range
     *                       of elements. Strong throw guarantee.
     */
    template<typename Itr1, typename Itr2>
    MathSet(Itr1&& itr1, Itr2&& itr2);

    /** @brief Constructs a new MathSet instance with the provided PIMPL
     *
     *  This ctor is primarily for use by PIMPLs and MathSet instances that
     *  create views of MathSets. The created MathSet instance will use the
     *  provided PIMPL no questions asked.
     *
     * @param[in] pimpl The PIMPL that the resulting instance should use.
     *
     * @throw none No throw guarantee.
     */
    explicit MathSet(pimpl_ptr pimpl) noexcept : m_pimpl_(std::move(pimpl)) {}

    virtual ~MathSet() = default;

    iterator begin() noexcept { return m_pimpl_->begin(); }

    /** @brief Returns a read-only iterator pointing at the first element in the
     *         set.
     *
     *  This function returns an iterator that points to the first element in
     *  the set. The iterator can be used to loop over the elements in the set
     *  by incrementing it until it equals the return of the end() function.
     *  Elements pointed at by the resulting iterator are read-only and cannot
     *  be modified through the iterator.
     *
     *  @return A read-only, random-access iterator that points to the first
     *          element in the set.
     *
     *  @throw none No throw guarantee.
     */
    const_iterator begin() const noexcept { return cptr_()->begin(); }

    /** @brief Returns a read-only iterator pointing at the first element in the
     *         set.
     *
     *  This function returns an iterator that points to the first element in
     *  the set. The iterator can be used to loop over the elements in the set
     *  by incrementing it until it equals the return of the end() function.
     *  Elements pointed at by the resulting iterator are read-only and cannot
     *  be modified through the iterator.
     *
     *  @return A read-only, random-access iterator that points to the first
     *          element in the set.
     *
     *  @throw none No throw guarantee.
     */
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return m_pimpl_->end(); }

    /** @brief Returns an iterator to just past the end of this set.
     *
     *  The iterator returned by this function is just past the end of the set.
     *  Conceptually you can think of this as being an iterator to the first
     *  element **NOT** in the set; however, the iterator returned by this
     *  function should **NOT** be dereferenced or used for iteration.
     *
     *  @return An iterator just past the end of this set. The iterator is
     *          suitable for signaling end of iteration.
     *
     *  @throw none No throw guarantee.
     */
    const_iterator end() const noexcept { return cptr_()->end(); }

    /** @brief Returns an iterator to just past the end of this set.
     *
     *  The iterator returned by this function is just past the end of the set.
     *  Conceptually you can think of this as being an iterator to the first
     *  element **NOT** in the set; however, the iterator returned by this
     *  function should **NOT** be dereferenced or used for iteration.
     *
     *  @return An iterator just past the end of this set. The iterator is
     *          suitable for signaling end of iteration.
     *
     *  @throw none No throw guarantee.
     */
    const_iterator cend() const noexcept { return end(); }

    /** @brief Returns the i-th element in this set.
     *
     *  Elements in this set are ordered. This function will return the @p i -th
     *  element in the set at the time of the call. The element will be returned
     *  as a read-only reference.
     *
     *  @param[in] i The index of the element to retrieve. Must be in the range
     *               [0, size()).
     *
     *  @return The i-th element in the set by read-only reference
     *
     *  @throw std::out_of_range if @p i is not in the range [0, size()). Strong
     *                           throw guarantee.
     */
    const_reference operator[](size_type i) const { return (*m_pimpl_)[i]; }

    reference operator[](size_type i) { return (*m_pimpl_)[i]; }

    /** @brief Returns the number of times @p elem appears in the set
     *
     *  Mathematically sets may only contain unique elements; however, this
     *  class does not enforce that restriction, hence the number of times an
     *  element can appear in the set is in the range [0, size()). This function
     *  will return the number of times that @p elem actually appears in the
     *  set. operator== will be used to locate @p elem.
     *
     *  @param[in] elem The element whose membership in this set is in question.
     *
     *  @return A integer in the range [0, size()) indicating the number of
     *         times @p elem appears in the set.
     *
     *  @throw none No throw guarantee.
     */
    size_type count(const_reference elem) const noexcept;

    /** @brief Returns the number of elements in the current set.
     *
     *  This function is used to determine how many elements are in the set
     *  modeled by the current instance. If the current instance is a view of
     *  another set, the returned value refers to only the elements in the view,
     *  not the elements in the parent set.
     *
     *  @return The number of elements in the current set.
     *
     *  @throw none No throw guarantee.
     */
    size_type size() const noexcept { return m_pimpl_->size(); }

    /** @brief Returns whether the current set is empty or not.
     *
     *  This function returns whether the current set is the empty set. The set
     *  is not the empty set if it contains at least one element.
     *
     *  @return True if the current set is empty and false otherwise.
     *
     *  @throw none No throw guarantee.
     */
    bool empty() const noexcept { return cbegin() == cend(); }

    /** @brief Adds an element to the set.
     *
     *  This function will add an element onto the end of the set if the element
     *  is not already present in the set. If the element is not already present
     *  then it is inserted onto the set at the index resulting from calling
     *  `size()` prior to insertion. If this set is an alias, insertion will
     *  also insert the element into the parent set. After calling this
     *  function all references, pointers, and iterators to this set are
     *  invalidated.
     *
     *  @param[in] elem The element to add to the container.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the element
     *                        weak throw guarantee.
     */
    void push_back(ElementType elem) { m_pimpl_->push_back(std::move(elem)); }

private:
    /// Type used to allow this class to hold instances of itself
    using ptr_to_my_type = std::unique_ptr<my_type>;

    /// Code factorization for getting a const-PIMPL on demand
    const auto cptr_() const noexcept;

    /// The object actually implementing the state and fundamental algorithms
    std::unique_ptr<pimpl_base> m_pimpl_;
};

//-----------------------------Implementations----------------------------------
/// Improve readability of the MathSet type in the definitions
#define MATH_SET_TYPE MathSet<ElementType>

template<typename Itr1, typename Itr2>
MathSet(Itr1&&, Itr2 &&)->MathSet<typename Itr1::value_type>;

template<typename ElementType>
MATH_SET_TYPE::MathSet(const MATH_SET_TYPE& rhs) :
  MathSet(std::make_unique<default_pimpl>(rhs.begin(), rhs.end())) {}

template<typename ElementType>
MATH_SET_TYPE::MathSet(std::initializer_list<ElementType> il) :
  MathSet(il.begin(), il.end()) {}

template<typename ElementType>
template<typename Itr1, typename Itr2>
MATH_SET_TYPE::MathSet(Itr1&& itr1, Itr2&& itr2) : MathSet() {
    while(itr1 != itr2) {
        m_pimpl_->push_back(*itr1);
        ++itr1;
    }
}

template<typename ElementType>
typename MATH_SET_TYPE::size_type MATH_SET_TYPE::count(
  const_reference elem) const noexcept {
    return m_pimpl_->count(elem);
}

template<typename ElementType>
const auto MATH_SET_TYPE::cptr_() const noexcept {
    return const_cast<const pimpl_base*>(m_pimpl_.get());
}

#undef MATH_SET_TYPE

} // namespace utilities
