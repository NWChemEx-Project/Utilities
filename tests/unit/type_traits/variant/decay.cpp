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

#include "utilities/type_traits/variant/decay.hpp"
#include <catch2/catch.hpp>

using namespace utilities::type_traits;

TEST_CASE("Decay") {
    SECTION("Empty") {
        using input  = std::variant<>;
        using corr   = std::variant<>;
        using result = typename variant::detail_::Decay<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("One type") {
        using input  = std::variant<int&>;
        using corr   = std::variant<int>;
        using result = typename variant::detail_::Decay<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("Two types") {
        using input  = std::variant<int&, const float>;
        using corr   = std::variant<int, float>;
        using result = typename variant::detail_::Decay<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
}

TEST_CASE("decay_t") {
    SECTION("Empty") {
        using input  = std::variant<>;
        using corr   = std::variant<>;
        using result = variant::decay_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("One type") {
        using input  = std::variant<int&>;
        using corr   = std::variant<int>;
        using result = variant::decay_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("Two types") {
        using input  = std::variant<int&, const float>;
        using corr   = std::variant<int, float>;
        using result = variant::decay_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
}
