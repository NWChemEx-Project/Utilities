#pragma once
#include "utilities/containers/math_set/detail_/math_set_view_pimpl.hpp"
#include "utilities/containers/math_set/math_set_class.hpp"
namespace utilities {

/** @brief Class which holds a slice of a MathSet instance.
 *
 *  Many set operations result in subsets of one or more sets. This class is
 *  designed to model the resulting subsets with reference/pointer semantics.
 *  This means that MathSetView instances do not copy the elements from the
 *  parent set, but rather hold pointers to them. Consequentially, a MathSetView
 *  is only valid as long as the original elements are valid.
 *
 * @tparam ElementType The type of the element in the aliased MathSet. Must
 *                     satisfy the same concepts as the template parameter type
 *                     of MathSet.
 */
template<typename ElementType>
class MathSetView {
private:
    /// Type of this class
    using my_type = MathSetView<ElementType>;

    /// Type of the PIMPL holding the aliases
    using pimpl_type = detail_::MathSetViewPIMPL<ElementType>;

    /// Type of the corresponding MathSet
    using math_set_type = MathSet<ElementType>;

    /// Type used to store a read-only reference
    using const_view_type = std::reference_wrapper<const ElementType>;

public:
    /// Type of an element stored in this set
    using value_type = typename math_set_type::value_type;
    /// Type of a reference to a read/write element in the parent set
    using reference = typename math_set_type::reference;
    /// Type of a reference to a read-only element in the parent set
    using const_reference = typename math_set_type::const_reference;
    /// Type used for indexing and counting
    using size_type = typename math_set_type::size_type;

    /** @brief Makes an empty MathSetView.
     *
     *  The default ctor makes a MathSetView with no elements. It is possible
     *  to add aliased elements to the set by assigning a different MathSetView
     *  to the instance resulting from this ctor.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *         PIMPL. Strong throw guarantee.
     */
    MathSetView() : m_pimpl_(std::make_unique<pimpl_type>()) {}

    /** @brief Makes an instance that aliases the same elements as @p rhs.
     *
     *  This ctor is used to make a MathSetView which aliases the same set of
     *  elements as another MathSetView. The two MathSetView instances do not
     *  alias each other (for example assigning to @p rhs will not change this
     *  instance).
     *
     * @param[in] rhs The MathSetView from which to get the initial set of
     *            aliased elements.
     * @throw std::bad_alloc if there is insufficient memory to allocate the
     *        PIMPL. Strong throw guarantee.
     */
    MathSetView(const my_type& rhs);

    /** @brief Transfers aliasing responsibilities to this instance
     *
     *  This ctor is used to create a new MathSetView that gets its aliases from
     *  another MathSetView. After this ctor is called all references and
     *  iterators to the aliased elements in @p rhs remain valid.
     *
     *  @param[in] rhs The MathSetView whose aliases will be taken. After this
     *             call @p rhs is in a valid, but otherwise undefined state.
     *  @throw none No throw guarantee.
     */
    MathSetView(my_type&& rhs) noexcept = default;

    /** @brief Makes the current instance alias the same elements as @p rhs
     *
     *  This function will make the current MathSetView instance alias the same
     *  elements as @p rhs. The internal state is not an alias of @p rhs, but
     *  rather aliases the same parent container as @p rhs. In other words after
     *  calling this function further modifications to @p rhs will not affect
     *  this instance.
     *
     *  @param[in] rhs The MathSetView to copy the aliases of.
     *  @return The current MathSetView instance aliasing the same elements as
     *          @p rhs.
     *  @throw std::bad_alloc if there is insufficient memory to copy the
     *         aliases strong throw guarantee.
     */
    my_type& operator=(const my_type& rhs);

    /** @brief Transfers aliasing responsibilities from another MathSetView
     *
     *  This function is used to transfer the aliases from one MathSetView to
     *  another. After calling this function all references and iterators to
     *  the elements previously aliased by @p rhs remain valid.
     *
     *  @param[in] rhs The MathSetView instance to take the state of. After this
     *             call @p rhs is in a valid, but otherwise undefined state.
     *  @return The current instance containing @p rhs's aliases.
     *  @throw none No throw guarantee.
     */
    my_type& operator=(my_type&& rhs) noexcept = default;

    /** @brief Constructs a MathSetView given a range of references to hold.
     *
     *  This ctor will construct a MathSetView whose state is initialized to the
     *  references in the range [begin, end).
     *
     * @tparam Itr1 Assumed to be the type of an iterator that points to
     *         std::reference_wrapper elements.
     * @tparam Itr2 Assumed to be @p Itr1 up to cv-qualifications
     * @param[in] begin An iterator pointing at the first element to include in
     *            the view.
     * @param[in] end An iterator pointing to just past the last element which
     *            should be included in the view.
     * @throw std::bad_alloc if there is insufficient memory to store the
     *        aliases. Strong throw guarantee.
     */
    template<typename Itr1, typename Itr2>
    explicit MathSetView(Itr1&& begin, Itr2&& end);

    /** @brief Returns an iterator pointing at the first element in this set.
     *
     *  This function can be used to retrieve an iterator pointing to the first
     *  element in the set. If the set is empty the returned iterator will
     *  compare equal to the iterator returned by `end()`. The resulting
     *  iterator will return read-only references to the internal elements and
     *  can not be used to modify the elements.
     *
     *  @return A random-access iterator pointing to the first element in the
     *          set.
     *  @throw none No throw guarantee.
     */
    auto begin() const noexcept { return m_pimpl_.begin(); }

    /** @brief Returns an iterator pointing at the first element in this set.
     *
     *  This function can be used to retrieve an iterator pointing to the first
     *  element in the set. If the set is empty the returned iterator will
     *  compare equal to the iterator returned by `end()`. The resulting
     *  iterator will return read-only references to the internal elements and
     *  can not be used to modify the elements.
     *
     *  @return A random-access iterator pointing to the first element in the
     *          set.
     *  @throw none No throw guarantee.
     */
    auto cbegin() const noexcept { return m_pimpl_.cbegin(); }

    /** @brief Returns an iterator pointing to just past the last element in
     *         this set.
     *
     *  This function can be used to retrieve an iterator pointing to just past
     *  the last element in the set. The resulting iterator is meant for use as
     *  a semaphore and should not be dereferenced.
     *
     *  @return A random-access iterator pointing to just past the last element
     *          in the set.
     *  @throw none No throw guarantee.
     */
    auto end() const noexcept { return m_pimpl_.end(); }

    /** @brief Returns an iterator pointing to just past the last element in
     *         this set.
     *
     *  This function can be used to retrieve an iterator pointing to just past
     *  the last element in the set. The resulting iterator is meant for use as
     *  a semaphore and should not be dereferenced.
     *
     *  @return A random-access iterator pointing to just past the last element
     *          in the set.
     *  @throw none No throw guarantee.
     */
    auto cend() const noexcept { return m_pimpl_.cend(); }

    /** @brief Returns the @p i -th element in the set by read-only reference
     *
     *  This function is used to access the @p i-th element in this set. The
     *  order of the elements is determined by the order in which they were
     *  inserted into this set.
     *
     *  @param[in] i The index of the element to retrieve. Must be in the range
     *            [0, size()).
     *
     *  @return A read-only reference to the @p i-th element in the set.
     *
     *  @throw std::out_of_range if @p i is not in the range [0, size()). Strong
     *         throw guarantee.
     */
    const_reference operator[](size_type i) const { return m_pimpl_[i]; }

    /** @brief Returns the number of times an element appears in the set.
     *
     *  This function will determine if @p e appears in the set or not. @p e
     *  will be compared to elements in the set using operator==. Since
     *  MathSet instances enforce uniqueness the return will either be 0 or 1.
     *
     *  @param[in] e The element whose inclusion in the set is in question.
     *
     *  @return The number of times @p e appears in this set. Will be either 0
     *          or 1.
     *
     *  @throw none No throw guarantee.
     */
    auto count(const_reference e) const noexcept { return m_pimpl_.count(e); }

    /** @brief Determines if the current set contains elements or not.
     *
     *  This function can be used to determine if the set modeled by the current
     *  view is empty or not. This function will not tell you whether or not
     *  the parent MathSet is empty.
     *
     *  @return true if this set currently holds no aliases and false otherwise.
     *
     *  @throw none No throw guarantee.
     */
    auto empty() const noexcept { return m_pimpl_.empty(); }

    /** @brief The number of elements being aliased by this view.
     *
     *  This function is used to determine how many elements are in this view
     *  of the parent set. In general this is **not** the same as the number of
     *  elements in the parent set.
     *
     *  @return The number of elements being aliased by this view.
     *  @throw none No throw guarantee
     */
    auto size() const noexcept { return m_pimpl_.size(); }

    /** @brief Allows implicit conversion to a read-only MathSet without copying
     *
     *  This conversion allows users to use this MathSetView instance as if it
     *  were a read-only MathSet without copying. This is allowed because the
     *  resulting MathSet cannot modify the elements.
     *
     *  @return The aliased elements in a MathSet API.
     *
     *  @throw none No throw guarantee.
     */
    operator const MathSet<ElementType>&() const noexcept { return m_pimpl_; }

private:
    /// The PIMPL (in a MathSet for ease of conversion) holding the aliases
    MathSet<ElementType> m_pimpl_;
}; // class MathSetView

// -----------------------------Implementations--------------------------------
#define MATH_SET_VIEW MathSetView<ElementType>

template<typename Itr1, typename Itr2>
MathSetView(Itr1&& itr1, Itr2 &&)
  ->MathSetView<std::remove_reference_t<decltype(itr1->get())>>;

template<typename ElementType>
MATH_SET_VIEW::MathSetView(const MATH_SET_VIEW& rhs) :
  m_pimpl_(std::make_unique<pimpl_type>([&]() {
      std::vector<const_view_type> rv;
      for(auto& x : rhs) rv.push_back(std::cref(x));
      return rv;
  }())) {}

template<typename ElementType>
MATH_SET_VIEW& MATH_SET_VIEW::operator=(const my_type& rhs) {
    return *this = std::move(my_type(rhs));
}

template<typename ElementType>
template<typename Itr1, typename Itr2>
MATH_SET_VIEW::MathSetView(Itr1&& begin, Itr2&& end) :
  m_pimpl_(std::make_unique<pimpl_type>(
    std::vector(std::forward<Itr1>(begin), std::forward<Itr2>(end)))) {}

#undef MATH_SET_VIEW
} // namespace utilities
