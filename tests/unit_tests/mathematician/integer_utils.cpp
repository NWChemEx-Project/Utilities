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

#include "../catch.hpp"
#include <utilities/mathematician/integer_utils.hpp>

using namespace utilities;

TEST_CASE("UnsignedSubtract") {
    SECTION("Default types") {
        std::size_t n1{3};
        std::size_t n2{2};
        long n1mn2{1};
        long n2mn1{-1};
        REQUIRE(UnsignedSubtract(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract(n2, n1) == n2mn1);
    }
    SECTION("Shorten return") {
        std::size_t n1{3};
        std::size_t n2{2};
        int n1mn2{1};
        int n2mn1{-1};
        REQUIRE(UnsignedSubtract<int>(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract<int>(n2, n1) == n2mn1);
    }
    SECTION("Shorten input") {
        unsigned n1{3};
        unsigned n2{2};
        long n1mn2{1};
        long n2mn1{-1};
        REQUIRE(UnsignedSubtract(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract(n2, n1) == n2mn1);
    }
    SECTION("Shorten both") {
        unsigned n1{3};
        unsigned n2{2};
        int n1mn2{1};
        int n2mn1{-1};
        REQUIRE(UnsignedSubtract<int>(n1, n2) == n1mn2);
        REQUIRE(UnsignedSubtract<int>(n2, n1) == n2mn1);
    }
}
