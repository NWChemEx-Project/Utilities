#pragma once
#include "utilities/containers/math_set/detail_/math_set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Implements MathSetView
 *
 *  MathSetView instances hold references to the elements they alias. This class
 *  stores those references. All references held by this class are read-only
 *  regardless of whether or not the aliased MathSet is read-only.
 *
 *  @tparam T The cv-qualified type of elements in the MathSet that is being
 *            aliased.
 */
template<typename ElementType>
class MathSetViewPIMPL : public MathSetPIMPL<ElementType> {
private:
    /// Type of the base class
    using base_type = MathSetPIMPL<ElementType>;

    /// Type used to alias a read/write element of another set
    using view_type = std::reference_wrapper<ElementType>;

    /// Type used to alias a read-only element of another set
    using const_view_type = std::reference_wrapper<const ElementType>;

    /// Type of vector holding const views
    using vector_of_views_type = std::vector<const_view_type>;

public:
    /// Type of an element stored in the aliased set
    using value_type = typename base_type::value_type;

    /// Type of a read-only reference to an element of the aliased set
    using const_reference = typename base_type::const_reference;

    /// The type used indexing and offsets, integral like
    using size_type = typename base_type::size_type;

    /** @brief Creates an empty set
     *
     *  The set resulting from this ctor is empty and not technically tied to
     *  any parent set because this instance will hold no elements.
     *
     *  @throw none No throw guarantee.
     */
    MathSetViewPIMPL() = default;

    /** @brief Initializes the PIMPL with aliases to the provided elements.
     *
     *  This ctor creates a MathSetView that aliases the provided elements. The
     *  elements will be aliased as read-only inside the resulting instance even
     *  though they are taken as read/write.
     *
     *  @param[in] data References to the elements that this set will alias.
     *  @throw std::bad_alloc if there is insufficient memory to copy the
     *         read-write references to read-only references. Strong throw
     *         guarantee.
     */
    explicit MathSetViewPIMPL(std::vector<view_type> data);

    /** @brief Initializes the PIMPL with aliases to the provided elements.
     *
     *  This ctor creates a MathSetView that aliases the provided elements. The
     *  elements will be aliased as read-only inside the resulting instance.
     *
     *  @param[in] data References to the elements that this set will alias.
     *  @throw none No throw guarantee.
     */
    explicit MathSetViewPIMPL(vector_of_views_type data) noexcept;

private:
    /// Implements operator[] for MathSetPIMPL
    const_reference get_(size_type i) const override;

    /// Implements count() for MathSetPIMPL
    size_type count_(const_reference elem) const noexcept override;

    /// Implements push_back() for MathSetPIMPL
    void push_back_(value_type elem) override;

    /// Implements size() for MathSetPIMPL
    size_type size_() const noexcept override { return m_data_.size(); }

    /// The actual data references stored in this view
    vector_of_views_type m_data_;
}; // class MathSetViewPIMPL

// -----------------------------------Implementations---------------------------
#define MATH_SET_VIEW_PIMPL MathSetViewPIMPL<ElementType>

template<typename T>
MathSetViewPIMPL(std::vector<std::reference_wrapper<T>>)->MathSetViewPIMPL<T>;

template<typename ElementType>
MATH_SET_VIEW_PIMPL::MathSetViewPIMPL(std::vector<view_type> data) :
  MathSetViewPIMPL([=]() {
      std::vector<const_view_type> rv;
      for(auto x : data) rv.push_back(std::cref(x.get()));
      return rv;
  }()) {}

template<typename ElementType>
MATH_SET_VIEW_PIMPL::MathSetViewPIMPL(vector_of_views_type data) noexcept :
  m_data_(std::move(data)) {}

template<typename ElementType>
typename MATH_SET_VIEW_PIMPL::const_reference MATH_SET_VIEW_PIMPL::get_(
  size_type i) const {
    return m_data_[i].get();
}

template<typename ElementType>
typename MATH_SET_VIEW_PIMPL::size_type MATH_SET_VIEW_PIMPL::count_(
  const_reference elem) const noexcept {
    auto l = [=](auto x) { return x.get() == elem; };
    return std::find_if(m_data_.begin(), m_data_.end(), l) != m_data_.end();
}

template<typename ElementType>
void MATH_SET_VIEW_PIMPL::push_back_(value_type elem) {
    throw std::runtime_error("Shouldn't be able to get here from public API");
}

#undef MATH_SET_VIEW_PIMPL
} // namespace utilities::detail_
