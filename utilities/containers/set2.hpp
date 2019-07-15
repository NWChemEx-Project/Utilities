#pragma once

namespace utilities {

/** @brief A set-like class designed to be nestable.
 *
 *  Nested sets are a common occurrence in programming. This class is designed
 *  to model the most derived layer of the nested set. This class has two
 *  specializations:
 *
 *  - NestedSet<std::tuple<Args...>>
 *    - For performance implementation is std::tuple<NestedSet<Args>...>
 *  - NestedSet<NestedSet<element_type>>
 *    - Stores elements contiguously in the nested set and holds offsets in the
 *      outer set.
 *
 *
 *
 * @tparam element_type
 * @tparam container_type The type of container used to hold the elements. The
 *                        properties of the container determine the properties
 *                        of the set. For example using std::set results in an
 *                        ordered, sorted set with no duplicate elements,
 *                        whereas using std::vector results in an ordered set,
 *                        which remembers insertion order, and supports
 *                        duplicate elements.
 */
template<typename element_type,
         typename container_type = std::vector<element_type>>
class NestedSet {
public:
    using value_type      = typename container_type::value_type;
    using size_type       = typename container_type::size_type;
    using reference       = typename container_type::reference;
    using const_reference = typename container_type::const_reference;

    iterator begin() { return m_data_.begin(); }
    const_iterator begin() { return m_data_.begin(); }

private:
    container_type m_data_;
};

// template<typename...args>
// class NestedSet<std::tuple<args...>> {
// public:
// using value_type = std::tuple<args...>;
// using reference = std::tuple<std::reference_wrapper<args>...>;
// using const_reference = std::tuple<std::reference_wrapper<const args>...>;
//
// reference operator[](size_type i) { return index_guts_(i, m_idxs_); }
//
// const_reference operator[](size_type i) const { return index_guts_(i,
// m_idxs_); }
//
// private:
// template<std::size_t...Idxs>
// auto index_guts_(size_type i, std::index_sequence<Idxs...>) {
//    return std::make_tuple(std::ref(std::get<Idx>(m_data_)[i])...);
//}
//
// template<std::size_t...Idxs>
// auto index_guts_(size_type i, std::index_sequence<Idxs...>) const {
//    return std::make_tuple(std::cref(std::get<Idx>(m_data_)[i])...);
//}
//
// static constexpr auto nargs = sizeof...(args);
//
// static constexpr auto m_idxs_ = std::make_index_sequence<nargs>();
//
// std::tuple<NestedSet<args>...> m_data_;
//};
//
// template<typename element_type>
// class NestedSet<NestedSet<element_type>> {
// public:
//    using value_type = NestedSet<element_type>;
//
//    void push_back()
//    reference operator[](size_type i) { return m_data_[i]; }
//
// private:
//    size_type m_offset_;
//    NestSet<element_type> m_data_;
//};
//
// template<typename element_type>
// class NestedSetView : public NestedSet<element_type> {
// public:
//
//};

//------------------------------Implementations---------------------------------

} // namespace utilities
