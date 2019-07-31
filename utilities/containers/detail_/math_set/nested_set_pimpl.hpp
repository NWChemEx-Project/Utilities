#pragma once
#include "math_set_traits.hpp"
#include "utilities/containers/detail_/math_set/math_set_traits.hpp"
#include "utilities/containers/detail_/math_set/range_view_pimpl.hpp"
#include "utilities/containers/detail_/math_set/set_pimpl.hpp"

namespace utilities::detail_ {

/** @brief Implements a MathSet that is made-up of MathSets
 *
 *  For efficiency this class flattens out nested MathSet instances. In general
 *  when this PIMPL is used it is used inside a MathSet with a type like
 *  MathSet<MathSet<T>>. In this case, under the hood we store one dense
 *  MathSet<T> instance and keep appending each additional MathSet<T>'s `T`
 *  elements into that single instance. We maintain a separate container with
 *  MathSet<T> instances that use RangeViewPIPML<T> PIMPLs to alias the offsets
 *  into the dense `MathSet<T>`. When users request elements of the
 *  MathSet<MathSet<T>> class they actually get the aliases.
 *
 *  @note Because the elements are stored in a flattened manner it is assumed
 *        that insertion of new elements will primarily occur onto the end of
 *        this set. Insertion elsewhere into this set is going to be
 *        inefficient.
 *
 * @tparam ElementType Expected to be a cv-qualified MathSet type. Of note we
 *                     assume that it can be constructed given a MathSetPIMPL
 *                     instance.
 */
template<typename ElementType>
class NestedSetPIMPL : public MathSetPIMPL<ElementType> {
    /// The type of the elements in this PIMPL's elements
    using nested_type = typename ElementType::value_type;
    /// Each element in this PIMPL uses a PIMPL of this type
    using view_pimpl_type = RangeViewPIMPL<nested_type>;
    /// The default PIMPL for the nested MathSet
    using default_pimpl = typename MathSetTraits<nested_type>::default_pimpl;
    /// My base class type
    using base_type = MathSetPIMPL<ElementType>;

public:
    /// The type of a const-reference to an element of this set
    using const_reference = typename base_type::const_reference;
    /// The type of an offset or index
    using size_type = typename base_type::size_type;
    /// The type of an iterator over the MathSet containing this PIMPL
    using iterator = typename base_type::iterator;

    /** @brief Creates an empty NestedSetPIMPL instance.
     *
     *  The instance resulting from this ctor has no elements in it, but
     *  elements can be added by calling insert.
     *
     *  @throw none No throw guarantee.
     *
     */
    NestedSetPIMPL();

    /** @brief Fills the Set with provided initial values.
     *
     *  This ctor can be used to initialize a set with a series of values.
     *
     *  @param[in] il The initial values for the set.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the
     *                        elements. Strong throw guarantee.
     */
    NestedSetPIMPL(std::initializer_list<ElementType> il);

    /** @brief Fills the Set from a range.
     *
     *  This ctor will iterate over the range [itr1, itr2) and add the elements
     *  in the range to this set.
     *
     *  @tparam Itr1 The type of the first iterator. Assumed to be at least a
     *               forward iterator.
     *  @tparam Itr2 The type of the second iterator. Assumed to be comparable
     *               to an iterator of type @p Itr1.
     *
     *  @param[in] itr1 The iterator pointing to the first element in the range.
     *  @param[in] itr2 The iterator pointing to just past the last element in
     *                  the range.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the
     *                        elements. Strong throw guarantee.
     */
    template<typename Itr1, typename Itr2>
    NestedSetPIMPL(Itr1&& itr1, Itr2&& itr2);

private:
    /// Code factorization for adding an element
    void add_element_(ElementType elem);

    const_reference get_(size_type i) const override { return m_views_[i]; }
    void insert_(iterator offset, ElementType new_elem) override;
    size_type size_() const noexcept override { return m_views_.size(); }
    void erase_(const_reference elem) noexcept override;
    void clear_() noexcept override;

    /// The views of the subsets
    std::vector<ElementType> m_views_;

    /// The flattened sets
    std::unique_ptr<MathSetPIMPL<nested_type>> m_data_;
};

//-------------------------Implementations--------------------------------------
#define NESTED_SET_PIMPL_TYPE NestedSetPIMPL<ElementType>

template<typename Itr1, typename Itr2>
NestedSetPIMPL(Itr1&&, Itr2 &&)->NestedSetPIMPL<typename Itr1::value_type>;

template<typename ElementType>
NESTED_SET_PIMPL_TYPE::NestedSetPIMPL() :
  m_data_(std::make_unique<default_pimpl>()) {}

template<typename ElementType>
NESTED_SET_PIMPL_TYPE::NestedSetPIMPL(std::initializer_list<ElementType> il) :
  NestedSetPIMPL(il.begin(), il.end()) {}

template<typename ElementType>
template<typename Itr1, typename Itr2>
NESTED_SET_PIMPL_TYPE::NestedSetPIMPL(Itr1&& itr1, Itr2&& itr2) :
  NestedSetPIMPL() {
    while(itr1 != itr2) {
        if(this->count(*itr1) == 0) insert_(this->end(), *itr1);
        ++itr1;
    }
}

template<typename ElementType>
void NESTED_SET_PIMPL_TYPE::add_element_(ElementType elem) {
    const auto ebegin = m_data_->size();
    const auto eend   = ebegin + elem.size();

    auto view_pimpl =
      std::make_unique<view_pimpl_type>(ebegin, eend, m_data_.get());

    m_views_.emplace_back(std::move(ElementType(std::move(view_pimpl))));

    // Need to allow duplicates because partitioning can make unique, e.g.
    // {{1, 2}, {1}} is okay even though it looks like {1, 2, 1} flattened
    for(auto&& x : elem) m_data_->no_check_insert(std::move(x));
}

template<typename ElementType>
void NESTED_SET_PIMPL_TYPE::insert_(iterator offset, ElementType new_elem) {
    // If it's at the end life is easy
    if(offset == this->end()) {
        add_element_(std::move(new_elem));
        return;
    }

    // It's not though...
    auto diff = offset - this->begin(); // Index where new element goes

    // Reset containers, need to copy old views to preserve the element values
    std::vector<ElementType> old_views(m_views_);
    this->clear();

    // Add values up to offset back in
    for(size_type i = 0; i < diff; ++i) add_element_(std::move(old_views[i]));

    // Add the new value at offset
    add_element_(std::move(new_elem));

    // Add remaining values, starting with what originally was offset
    for(size_type i = diff; i < old_views.size(); ++i)
        add_element_(std::move(old_views[i]));
}

template<typename ElementType>
void NESTED_SET_PIMPL_TYPE::erase_(const_reference elem) noexcept {
    std::vector<ElementType> old_elems(m_views_);
    this->clear();
    for(auto&& x : old_elems)
        if(x != elem) this->insert(x);
}

template<typename ElementType>
void NESTED_SET_PIMPL_TYPE::clear_() noexcept {
    m_views_.clear();
    m_data_->clear();
}

#undef NESTED_SET_PIMPL_TYPE

} // namespace utilities::detail_
