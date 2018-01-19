#pragma once
#include <unordered_set>

namespace UtilitiesEx {

/**
 * @brief Implements an std::unordered_set-like object except that it knows
 * about union, etc.
 *
 * @tparam element_type The type of the elements in the set
 * @tparam container_type The type of the container that is holding the
 *         elements.  Must satisfy the C++ concept of container.
 */
template<typename element_type,
         typename container_type=std::unordered_set<element_type>>
class MathSet {
public:
    using value_type = typename container_type::value_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using difference_type = typename container_type::difference_type;
    using size_type = typename container_type::size_type;

    MathSet()noexcept=default;
    MathSet(const MathSet&)=default;
    MathSet(MathSet&&)noexcept=default;
    ~MathSet()noexcept=default;
    MathSet& operator=(const MathSet&)=default;
    MathSet& operator=(MathSet&&)noexcept=default;

    iterator begin(){ return elements_.begin();}
    const_iterator cbegin()const{ return elements_.cbegin();}
    iterator end(){ return elements_.end(); }
    const_iterator cend()const{return elements_.cend();}

    template<typename input_element>
    auto insert(input_element&& ei)
    {
        return elements_.insert(std::forward<input_element>(ei));
    }

    size_type size()const noexcept{return elements_.size();}
    size_type max_size()const noexcept{return elements_.max_size();}
    bool empty()const noexcept{return elements_.empty();}

    bool operator==(const MathSet& rhs)const noexcept
    {
        return elements_ == rhs.elements_;
    }

    bool operator!=(const MathSet& rhs)const noexcept{return !((*this)==rhs);}


private:
    container_type elements_;
};

}//End namespace
