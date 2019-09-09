#pragma once
#include "utilities/iterators/input_iterator_base.hpp"

namespace utilities::iterators {
namespace detail_ {

/** @brief Calls `begin()` on the container nested at a depth of `target_depth`
 *
 *  This function
 * @tparam depth
 * @tparam target_depth
 * @tparam T
 * @param container
 * @return
 */
template<std::size_t depth, std::size_t target_depth, typename T>
auto call_begin(T&& container) {
    if constexpr(depth == target_depth) {
        return container.begin();
    } else {
        return call_begin<depth + 1, target_depth>(*container.begin());
    }
}

template<std::size_t depth, std::size_t target_depth, typename T>
auto call_end(T&& container) {
    if constexpr(depth == target_depth) {
        auto rv           = container.end();
        using return_type = decltype(rv);
        return std::forward<return_type>(rv);
    } else {
        auto rv           = call_end<depth + 1, target_depth>(*container.end());
        using return_type = decltype(rv);
        return std::forward<return_type>(rv);
    }
}

} // namespace detail_

template<typename ContainerType, std::size_t depth, std::size_t curr_depth = 0>
class FlattenedContainerIterator {
    FlattenedContainerIterator(ContainerType* parent) : m_parent_(parent) {}

    auto&& get() {
        if constexpr(depth == curr_depth) {}
    }

    /// The container we are iterating over
    ContainerType* m_parent_;
};

} // namespace utilities::iterators
