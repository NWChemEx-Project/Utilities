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

#include "utilities/type_traits/variant/cat.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities::type_traits::variant;

TEST_CASE("variant::cat_t") {
    SECTION("No variant") {
        STATIC_REQUIRE(std::is_same_v<cat_t<>, std::variant<>>);
    }

    SECTION("One Variant") {
        using variant_type = std::variant<int, double, char>;
        STATIC_REQUIRE(std::is_same_v<cat_t<variant_type>, variant_type>);
    }

    SECTION("Two Variants") {
        using variant0_t = std::variant<int, double>;
        using variant1_t = std::variant<char, bool>;
        using corr_t     = std::variant<int, double, char, bool>;
        STATIC_REQUIRE(std::is_same_v<cat_t<variant0_t, variant1_t>, corr_t>);
    }
}