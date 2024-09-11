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
#include "utilities/type_traits/variant/to_tuple.hpp"
#include <type_traits>

using namespace utilities::type_traits::variant;

TEST_CASE("to_tuple") {
    using variant_type = std::variant<int, double, float>;
    using type         = typename detail_::to_tuple<variant_type>::type;
    using corr         = std::tuple<int, double, float>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("to_tuple_t") {
    using variant_type = std::variant<int, double, float>;
    using type         = to_tuple_t<variant_type>;
    using corr         = std::tuple<int, double, float>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}