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

    auto begin() const noexcept { return m_pimpl_.begin(); }
    auto cbegin() const noexcept { return m_pimpl_.cbegin(); }
    auto end() const noexcept { return m_pimpl_.end(); }
    auto cend() const noexcept { return m_pimpl_.cend(); }

    const_reference operator[](size_type i) const { return m_pimpl_[i]; }

    auto count(const_reference e) const noexcept { return m_pimpl_.count(e); }
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

    /** @brief Allows implicit conversion to read/write MathSet by copying
     *
     *  This conversion allows users to use MathSetView instances as read/write
     *  MathSet instances. This is done by deep copying the elements aliased
     *  by this MathSetView instance. The use of a deep copy avoids issues
     *  related to aliasing. Modification of the elements can be done through
     *  the parent set.
     *
     *  @return A MathSet instance which contains a deep copy of the elements
     *          aliased by this set.
     *  @throw std::bad_alloc if there is insufficient memory to make the copy.
     *         Strong throw guarantee.
     */
    operator MathSet<ElementType>() const { m_pimpl_; }

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
