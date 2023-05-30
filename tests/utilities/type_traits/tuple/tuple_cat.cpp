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

#include "utilities/type_traits/tuple/tuple_cat.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities::type_traits::tuple;

TEST_CASE("tuple_cat_t") {
    SECTION("No tuples") {
        STATIC_REQUIRE(std::is_same_v<tuple_cat_t<>, std::tuple<>>);
    }

    SECTION("One tuple") {
        using tuple_type = std::tuple<int, double, char>;
        STATIC_REQUIRE(std::is_same_v<tuple_cat_t<tuple_type>, tuple_type>);
    }

    SECTION("Two tuples") {
        using tuple0_t = std::tuple<int, double>;
        using tuple1_t = std::tuple<char, bool>;
        using corr_t   = std::tuple<int, double, char, bool>;
        STATIC_REQUIRE(std::is_same_v<tuple_cat_t<tuple0_t, tuple1_t>, corr_t>);
    }
}