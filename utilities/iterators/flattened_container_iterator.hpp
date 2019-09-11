#pragma once
#include "utilities/iterators/input_iterator_base.hpp"

namespace utilities::iterators {

/// Forward declare iterator to use it in TMP
template<std::size_t depth, typename ContainerType>
class FlattenedContainerIterator;

namespace detail_ {

/** @brief Returns a tuple of the begin iterators for a nested container.
 *
 *  @relates FlattenedContainerIterator
 *
 *  Given a nested container with @f$N@f$ nestings. This function will
 *  recursively call `begin()` on the first element of the container until
 *  `begin()` has been called on @p tdepth nestings. The resulting iterators are
 *  collected into a tuple such that the @f$i@f$-th element is the iterator
 *  returned at depth @f$i@f$. This is probably easier to understand with an
 *  example:
 *
 *  @code
 *  // Make "v" our nested container of type
 *  // std::vector<std::vector<std::vector<int>>>
 *  std::vector v{
 *    std::vector{std::vector{1, 2}, std::vector{2, 3}},
 *    std::vector{std::vector{3, 4}. std::vector{4, 5}}
 *  };
 *
 *  // Only want to iteratre over the outermost container
 *  auto itr1 = get_begin_tuple<0, 0>(v);
 *  assert(std::get<0>(itr1) == v.begin());
 *
 *  // Iterate over the middle container
 *  auto itr2 = get_begin_tuple<0, 1>(v);
 *  assert(std::get<0>(itr2) == v.begin());
 *  assert(std::get<1>(itr2) == v.begin()->begin());
 *
 *  // Iterate over the most nested container
 *  auto itr3 = get_begin_tuple<0, 2>(v);
 *  assert(std::get<0>(itr3) == v.begin());
 *  assert(std::get<1>(itr3) == v.begin()->begin());
 *  assert(std::get<2>(itr3) == v.begin()->begin()->begin());
 *  @endcode
 *
 *
 * @tparam depth The current depth. Users should always set this to 0 it will be
 *               incremented as recursion occurs.
 * @tparam tdepth How many nestings down should we go? This value must be less
 *                than equal to the number of nestings.
 * @tparam ContainerType The type of the nested container.
 * @param[in] container The nested container we are calling begin on
 * recursively.
 * @return A tuple of length @p tdepth + 1 such that element i is the iterator
 *         resulting from calling `begin` on the first element at a depth of i.
 * @throw std::runtime_error if the container is not actually as nested as it
 *        should be. Strong throw guarantee. Basically the type tells us the
 *        maximum nesting. At runtime you actually have to fill it to that level
 *        of nesting. For example, if you do:
 *
 *        @code
 *        std::vector v{std::vector<std::vector<int>>{}};
 *        auto itr = get_begin_tuple<0, 2>(v);
 *        @endcode
 *
 *        you'll trip this error because we can't actually call `begin` on an
 *        element of the middle container (you could call `get_begin_tuple<0,1>`
 *        though).
 */
template<std::size_t depth, std::size_t tdepth, typename ContainerType>
auto get_begin_tuple(ContainerType&& container);

/** @brief Returns a tuple of the end iterators for a nested container.
 *
 *  @relates FlattenedContainerIterator
 *
 *  Given a nested container with @f$N@f$ nestings. This function will
 *  recursively call `end()` on the last element of the container until
 *  `end()` has been called on @p tdepth nestings. The resulting iterators are
 *  collected into a tuple such that the @f$i@f$-th element is the iterator
 *  returned at depth @f$i@f$. This is probably easier to understand with an
 *  example:
 *
 *  @code
 *  // Make "v" our nested container of type
 *  // std::vector<std::vector<std::vector<int>>>
 *  std::vector v{
 *    std::vector{std::vector{1, 2}, std::vector{2, 3}},
 *    std::vector{std::vector{3, 4}. std::vector{4, 5}}
 *  };
 *
 *  // Only want to iteratre over the outermost container
 *  auto itr1 = get_end_tuple<0, 0>(v);
 *  assert(std::get<0>(itr1) == v.end());
 *
 *  // Iterate over the middle container
 *  auto itr2 = get_end_tuple<0, 1>(v);
 *  assert(std::get<0>(itr2) == v.end());
 *  assert(std::get<1>(itr2) == v.back().end());
 *
 *  // Iterate over the most nested container
 *  auto itr3 = get_end_tuple<0, 2>(v);
 *  assert(std::get<0>(itr3) == v.end());
 *  assert(std::get<1>(itr3) == v.back().end());
 *  assert(std::get<2>(itr3) == v.back().back()->end());
 *  @endcode
 *
 *
 * @tparam depth The current depth. Users should always set this to 0 it will be
 *               incremented as recursion occurs.
 * @tparam tdepth How many nestings down should we go? This value must be less
 *                than equal to the number of nestings.
 * @tparam ContainerType The type of the nested container.
 * @param[in] container The nested container we are calling end on recursively.
 * @return A tuple of length @p tdepth + 1 such that element i is the iterator
 *         resulting from calling `end` on the last element at a depth of i.
 * @throw std::runtime_error if the container is not actually as nested as it
 *        should be. Strong throw guarantee. Basically the type tells us the
 *        maximum nesting. At runtime you actually have to fill it to that level
 *        of nesting. For example, if you do:
 *
 *        @code
 *        std::vector v{std::vector<std::vector<int>>{}};
 *        auto itr = get_end_tuple<0, 2>(v);
 *        @endcode
 *
 *        you'll trip this error because we can't actually call `end` on an
 *        element of the middle container (you could call `get_end_tuple<0,1>`
 *        though).
 */
template<std::size_t depth, std::size_t tdepth, typename ContainerType>
auto get_end_tuple(ContainerType&& container);

/** @brief Resets all iterators in a tuple
 *
 *  This function assumes you are looping over an @f$N@f$-tuple of iterators in
 *  lexicographical order. In such case when you increase iterator i, you must
 *  reset all iterators in the range @f$[i+1, N)@f$. This function will reset
 *  iterator @f$m@f$ by calling `begin` on the object the @f$m-1@f$-th iterator
 *  is pointing to.
 *
 * @tparam tdepth The index of the first iterator to reset.
 * @tparam ContainerType The type of the nested container that the iterator is
 *         for.
 * @tparam IteratorType The type of the tuple holding all the iterators.
 * @param[in] container The container the iterator is for.
 * @param[in] itr The tuple of iterators we are resetting.
 * @return A tuple of iterators such that starting with the @p tdepth -th
 *         iterator all iterators have been reset by calling `begin` on the
 *         element pointed to by the proceeding iterator.
 * @throw ??? If calling `begin` on any iterator throws. Same throw guarantee.
 */
template<std::size_t tdepth, typename ContainerType, typename IteratorType>
auto reset_tuple_of_itr(ContainerType&& container, IteratorType itr);

/** @brief Increments a tuple of iterators assuming lexicographical ordering.
 *
 *  This function will start with the last iterator in the tuple and try to
 *  increment it. If the iterator can be incremented it will be and the
 *  resulting tuple is returned. If the iterator can not be incremented then
 *  this function will try to increment the second to last iterator. This cycle
 *  repeats until an iterator can be incremented or all iterators are exhausted,
 *  in the latter case the tuple is returned unchanged. In the former case this
 *  function will also call `reset_tuple_of_itr` on the tuple.
 *
 * @tparam depth Should always be specified as 0 by the user. Used internally to
 *               do recursion.
 * @tparam ContainerType The type of the container the iterators are for.
 * @tparam IteratorType  The type of the tuple holding the iterators
 * @param[in] container The container the iterators are for.
 * @param[in] itr The tuple of iterators we are incrementing.
 * @return The tuple of iterators advanced to the next lexicographical element.
 * @throw ??? if `end` throws for any of the nested containers or if
 *            reset_tuple_of_itr throws. Same throw guarantee.
 */
template<std::size_t depth, typename ContainerType, typename IteratorType>
auto increment_tuple_of_itr(ContainerType&& container, IteratorType itr);

/// Type of a tuple holding iterators
template<std::size_t depth, typename T>
using iterator_tuple_t =
  decltype(detail_::get_begin_tuple<0, depth>(std::declval<T>()));

/** @brief The type of the elements returned byFlattenedContainerIterator
 *
 *  This typedef is a mess and factoring it into smaller typedefs isn't going to
 *  do much. Anyways, it works by (from inside out):
 *
 *  - Get the type of the tuple holding all the begin iterators
 *  - Get the last iterator in the tuple
 *  - Get the type the iterator points to and remove any references it may have
 *
 */
template<std::size_t depth, typename T>
using tuple_itr_value_type = std::remove_reference_t<decltype(
  *std::get<depth>(std::declval<iterator_tuple_t<depth, T>>()))>;

/// Typedef of the base iterator for the FlattenedContainerIterator
template<std::size_t depth, typename T>
using fci_base = InputIteratorBase<FlattenedContainerIterator<depth, T>,
                                   tuple_itr_value_type<depth, T>>;

} // namespace detail_

template<std::size_t depth, typename ContainerType>
class FlattenedContainerIterator
  : public detail_::fci_base<depth, ContainerType> {
    /// The type of this iterator
    using my_type = FlattenedContainerIterator<depth, ContainerType>;
    /// The type of the base class
    using base_type = detail_::fci_base<depth, ContainerType>;

public:
    /// Type of a read-only reference to an element in this container
    using const_reference = typename base_type::const_reference;

    /** @brief Creates an iterator that flattens the specified container out to
     *         a depth @p depth.
     *
     * @param[in] container The container we are iterating over.
     * @param[in] at_end Should this iterator be just past the last element. If
     *            false then the iterator points to the first element.
     * @throws ??? if get_end_tuple or get_begin_tuple throw. Strong throw
     *             guarantee.
     */
    FlattenedContainerIterator(ContainerType* container, bool at_end);

    /// Implements operator*() for the base class
    const_reference dereference() const override;

    /// Implements operator++() for the base class
    my_type& increment() override;

    /// Implements operator== for the base class
    bool are_equal(const my_type& rhs) const noexcept override;

private:
    /// The parent container we are iterating over
    ContainerType* m_container_;
    /// The collection of iterators to iterate over
    detail_::iterator_tuple_t<depth, ContainerType> m_itr_;
}; // class FlattenedContainerIterator

// -------------------------Implementations ------------------------------------

namespace detail_ {

template<std::size_t depth, std::size_t tdepth, typename ContainerType>
auto get_begin_tuple(ContainerType&& container) {
    auto my_tuple = std::make_tuple(container.begin());
    if constexpr(depth == tdepth) {
        return my_tuple;
    } else {
        if(container.begin() == container.end())
            throw std::runtime_error("Container is not sufficiently nested");
        auto& my_elem = *std::get<0>(my_tuple);
        return std::tuple_cat(my_tuple,
                              get_begin_tuple<depth + 1, tdepth>(my_elem));
    }
}

template<std::size_t depth, std::size_t tdepth, typename ContainerType>
auto get_end_tuple(ContainerType&& container) {
    auto my_tuple = std::make_tuple(container.end());
    if constexpr(depth == tdepth) {
        return my_tuple;
    } else {
        if(container.begin() == container.end())
            throw std::runtime_error("Container is not sufficiently nested");
        auto itr = container.begin();
        std::advance(itr, container.size() - 1);
        return std::tuple_cat(my_tuple, get_end_tuple<depth + 1, tdepth>(*itr));
    }
}

template<std::size_t tdepth, typename ContainerType, typename IteratorType>
auto reset_tuple_of_itr(ContainerType&& container, IteratorType itr) {
    constexpr auto tuple_length = std::tuple_size_v<IteratorType>;
    if constexpr(tdepth == tuple_length) {
        return itr;
    } else {
        if constexpr(tdepth == 0) {
            std::get<0>(itr) = container.begin();
        } else {
            if constexpr(tdepth == 1) { // Care is needed if itr 0 is done
                if(std::get<0>(itr) == container.end()) return itr;
            }
            std::get<tdepth>(itr) = std::get<tdepth - 1>(itr)->begin();
        }
        return reset_tuple_of_itr<tdepth + 1>(container, std::move(itr));
    }
}

template<std::size_t depth, typename ContainerType, typename IteratorType>
auto increment_tuple_of_itr(ContainerType&& container, IteratorType itr) {
    constexpr auto tuple_length = std::tuple_size_v<IteratorType>;
    static_assert(tuple_length != 0, "Can not increment empty tuple");
    static_assert(depth != tuple_length, "Ran off the end of the tuple");

    // Start from back and try to increment
    constexpr auto curr_idx = tuple_length - 1 - depth;
    ++std::get<curr_idx>(itr);
    if constexpr(curr_idx != 0) {
        if(std::get<curr_idx>(itr) == std::get<curr_idx - 1>(itr)->end())
            return increment_tuple_of_itr<depth + 1>(
              std::forward<ContainerType>(container), std::move(itr));
    } else {
        if(std::get<curr_idx>(itr) == container.end()) return itr;
    }
    return reset_tuple_of_itr<curr_idx + 1>(
      std::forward<ContainerType>(container), std::move(itr));
}

} // namespace detail_

#define FCI FlattenedContainerIterator<depth, ContainerType>

template<std::size_t depth, typename ContainerType>
FCI::FlattenedContainerIterator(ContainerType* container, bool at_end) :
  m_container_(container),
  m_itr_(at_end ? detail_::get_end_tuple<0, depth>(*container) :
                  detail_::get_begin_tuple<0, depth>(*container)) {}

template<std::size_t depth, typename ContainerType>
typename FCI::const_reference FCI::dereference() const {
    return *std::get<depth>(m_itr_);
}

template<std::size_t depth, typename ContainerType>
FCI& FCI::increment() {
    m_itr_ =
      detail_::increment_tuple_of_itr<0>(*m_container_, std::move(m_itr_));
    return *this;
}

template<std::size_t depth, typename ContainerType>
bool FCI::are_equal(const my_type& rhs) const noexcept {
    return std::tie(m_container_, m_itr_) ==
           std::tie(rhs.m_container_, rhs.m_itr_);
}

#undef FCI
} // namespace utilities::iterators
