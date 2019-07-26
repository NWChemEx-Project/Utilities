#pragma once
#include "utilities/containers/detail_/range_view_pimpl.hpp"
#include "utilities/containers/math_set_view.hpp"

namespace utilities::detail_ {

/** @brief Implements a MathSet that is made-up of MathSets
 *
 *  For efficiency this class flattens out nested MathSet instances. It will be
 *  the PIMPL for a MathSet class whose full type looks something like
 *  `MathSet<MathSet<T>>`. Under the hood we store one MathSet<T> instance and
 *
 *  @note Because the elements are stored in a flattened manner it is assumed
 *        that insertion of new elements will primarily occur onto the end of
 *        this set. Insertion elsewhere into this set is, at the moment, not
 *        supported.
 *
 * @tparam ElementType This is presumed to be something like MathSet<T>
 */
template<typename ElementType>
class NestedSetPIMPL : public MathSetPIMPL<ElementType> {
    using nested_type     = typename ElementType::value_type;
    using view_pimpl_type = RangeViewPIMPL<nested_type>;
    using base_type       = MathSetPIMPL<ElementType>;

public:
    using const_reference = typename base_type::const_reference;
    using size_type       = typename base_type::size_type;
    using iterator        = typename base_type::iterator;

    NestedSetPIMPL() : m_data_(std::make_shared<ElementType>()) {}
    NestedSetPIMPL(std::initializer_list<ElementType> il);

    template<typename Itr1, typename Itr2>
    NestedSetPIMPL(Itr1&& itr1, Itr2&& itr2);

private:
    void add_element_(ElementType elem);
    const_reference get_(size_type i) const override { return m_views_[i]; }
    void insert_(iterator offset, ElementType new_elem) override;
    size_type size_() const noexcept override { return m_views_.size(); }

    std::vector<ElementType> m_views_;
    std::shared_ptr<ElementType> m_data_;
};

//-------------------------Implementations--------------------------------------
#define NESTED_SET_PIMPL_TYPE NestedSetPIMPL<ElementType>

template<typename ElementType>
NESTED_SET_PIMPL_TYPE::NestedSetPIMPL(std::initializer_list<ElementType> il) :
  NestedSetPIMPL(il.begin(), il.end()) {}

template<typename ElementType>
template<typename Itr1, typename Itr2>
NESTED_SET_PIMPL_TYPE::NestedSetPIMPL(Itr1&& itr1, Itr2&& itr2) :
  NestedSetPIMPL() {
    while(itr1 != itr2) {
        insert_(this->end(), *itr1);
        ++itr1;
    }
}

template<typename ElementType>
void NESTED_SET_PIMPL_TYPE::add_element_(ElementType elem) {
    const auto ebegin = m_data_->size();
    const auto eend   = ebegin + elem.size();
    auto view_pimpl = std::make_shared<view_pimpl_type>(ebegin, eend, m_data_);
    m_views_.emplace_back(std::move(ElementType(view_pimpl)));
    for(auto&& x : elem) m_data_->insert(m_data_->end(), std::move(x));
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

    // Reset containers, will move old values over
    std::vector<ElementType> old_views;
    old_views.swap(m_views_);
    ElementType{}.swap(*m_data_);

    // Add values up to offset back in
    for(size_type i = 0; i < diff; ++i) add_element_(std::move(old_views[i]));

    // Add the new value at offset
    add_element_(std::move(new_elem));

    // Add remaining values, starting with what originally was offset
    for(size_type i = diff; i < old_views.size(); ++i)
        add_element_(std::move(old_views[i]));
}

#undef NESTED_SET_PIMPL_TYPE

} // namespace utilities::detail_
