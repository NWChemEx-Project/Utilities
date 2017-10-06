#pragma once

#include<vector>
#include<algorithm>

namespace IterTools {

/** @brief Generates combinations for an arbitrary set.
 *
 *
 *
 *  @tparam const_iterator The type of the iterator to your container.  Should
 *          satisfy the concept of a random access iterator.
 */
template<typename container_type>
class CombinationGenerator {
public:
    using size_type=size_t;

    /** @brief Makes iterator for empty set.
     *
     *
     *  @throws None No throw guarantee.
     */
    CombinationGenerator()noexcept;

    static begin(const container_type)

private:
    std::vector<bool> current_comb_;

    const container_type* container_;

    CombinationGenerator(const container_type& input_set,
                         size_type
                         bool at_end) noexcept;

    void next_();
};

/*******************************************************************************
 * Implementations for CombinationGenerator
 ******************************************************************************/

template<typename container_type>
CombinationGenerator<container_type>::CombinationGenerator()noexcept :
    current_comb_(),container_(nullptr)
{}

template<typename container_type>
CombinationGenerator<container_type>::CombinationGenerator(
        const container_type& input_set, bool at_end) noexcept :
    current_comb_(size), container_(&input_set)
{
    if
}

} //End namespace IterTools
