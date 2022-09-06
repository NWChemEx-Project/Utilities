/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "utilities/mathematician/combinatorics.hpp"
#include <algorithm> //For accumulate
#include <stdexcept>

namespace utilities {
namespace detail_ {

template<>
std::size_t BCImpl<std::size_t>::eval(std::size_t n, std::size_t k) {
    if(k == 0) return 1;
    if(k > n) return 0;
    // Guaranteed to fit in size_t,VER use pascal's rule
    if(n < sizeof(std::size_t) * 8)
        return BCImpl<std::size_t>::eval(n - 1, k - 1) +
               BCImpl<std::size_t>::eval(n - 1, k);
    throw std::overflow_error("I haven't coded up n>64");
}

template<>
std::size_t MCImpl<std::size_t>::eval(const std::vector<std::size_t>& ks) {
    std::size_t result    = 1;
    std::size_t numerator = std::accumulate(ks.begin(), ks.end(), 0);
    for(auto k : ks) {
        for(size_t i = 1; i <= k; ++i) {
            result *= numerator;
            result = (result - result % i) / i;
            numerator -= 1;
        }
    }
    return result;
}

template class BCImpl<std::size_t>;
template class MCImpl<std::size_t>;

} // namespace detail_
} // namespace utilities
