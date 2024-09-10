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

#include "../../catch.hpp"
#include "utilities/type_traits/tuple/tuple_product.hpp"
#include <type_traits>

using namespace utilities::type_traits::tuple;
using namespace utilities::type_traits::tuple::detail_;

TEST_CASE("base_prod_t") {
    using type = base_prod_t<std::pair, int, double>;
    using corr = std::pair<int, double>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("base_prod_expand_r_t") {
    using rtuple = std::tuple<int, double, float>;
    using type   = base_prod_expand_r_t<std::pair, char, rtuple>;
    using pair0  = std::pair<char, int>;
    using pair1  = std::pair<char, double>;
    using pair2  = std::pair<char, float>;
    using corr   = std::tuple<pair0, pair1, pair2>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("tuple_prod_t") {
    using ltuple = std::tuple<char, long>;
    using rtuple = std::tuple<int, double, float>;
    using type   = product_t<std::pair, ltuple, rtuple>;
    using pair0  = std::pair<char, int>;
    using pair1  = std::pair<char, double>;
    using pair2  = std::pair<char, float>;
    using pair3  = std::pair<long, int>;
    using pair4  = std::pair<long, double>;
    using pair5  = std::pair<long, float>;
    using corr   = std::tuple<pair0, pair1, pair2, pair3, pair4, pair5>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}