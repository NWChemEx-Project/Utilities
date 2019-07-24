#pragma once
#include "utilities/containers/detail_/math_set_view.hpp"

namespace utilities::detail_ {

/** @brief Implements a MathSet that is made-up of MathSets
 *
 *  For efficiency this class flattens out nested MathSet instances. It will be
 *  the PIMPL for a MathSet class whose full type looks something like
 *  `MathSet<MathSet<T>>`. Under the hood that contents of the non_flattened set
 *  would look something like std::vector<std::vector<T>>, but with this PIMPL
 *  they instead look like `std::vector<T>`. To achieve the flattening, this
 *  PIMPL stores the offsets of each inner MathSet inside MathSetView instances.
 *
 *  @note Because the elements are stored in a flattened manner it is assumed
 *        that insertion of new elements will primarily occur onto the end of
 *        this set. Insertion elsewhere into this set is, at the moment, not
 *        supported.
 *
 * @tparam ElementType This is presumed to be something like MathSet<T>
 */
template<typename ElementType>
class NestedSetPIMPL {
    using view_type = MathSetView<ElementType>;
    using view_vec  = std::vector<view_type>;

public:
    using value_type      = view_type;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using iterator        = typename view_vec::iterator;
    using const_iterator  = typename view_vec::const_iterator;
    using size_type       = typename ElementType::size_type;

    NestedSetPIMPL() : m_data_(std::make_shared<ElementType>()) {}

    template<typename Itr1, typename Itr2>
    NestedSetPIMPL(Itr1&& itr1, Itr2&& itr2);

    auto begin() { return m_views_.begin(); }
    auto begin() const { return m_views_.begin(); }
    auto end() { return m_views_.end(); }
    auto end() const { return m_views_.end(); }

    auto size() const noexcept { return m_views_.size(); }

    void insert(ElementType new_elem);

private:
    view_vec m_views_;
    std::shared_ptr<ElementType> m_data_;
};

//-------------------------Implementations--------------------------------------
#define NESTED_SET_PIMPL_TYPE NestedSetPIMPL<ElementType>

template<typename ElementType>
template<typename Itr1, typename Itr2>
NESTED_SET_PIMPL_TYPE::NestedSetPIMPL(Itr1&& itr1, Itr2&& itr2) :
  NestedSetPIMPL() {
    while(itr1 != itr2) {
        insert(*itr1);
        ++itr1;
    }
}

template<typename ElementType>
void NESTED_SET_PIMPL_TYPE::insert(ElementType new_elem) {
    const auto ebegin = m_data_->size();
    const auto eend   = ebegin + new_elem.size();
    m_views_.push_back(view_type(ebegin, eend, m_data_));
    m_data_->insert(new_elem.begin(), new_elem.end());
}

#undef NESTED_SET_PIMPL_TYPE

} // namespace utilities::detail_
