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

#include <catch2/catch.hpp>
#include <utilities/iter_tools/combinations.hpp>

/* Testing strategy
 *
 * We piggy-back off of Permutations to implement Combinations. Consequentially
 * all we need to test is:
 *
 * - ctor initializes the class correctly
 * - size works correctly (actually done as part of ctor)
 * - at_ increments the combinations correctly
 *
 */

using namespace utilities;
using vector_t = std::vector<int>;

TEST_CASE("Combinations<vector<int>> : ctor") {
    SECTION("No repeat") {
        SECTION("Empty set") {
            Combinations c(vector_t{}, 0);
            SECTION("Size") { REQUIRE(c.size() == 1); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{}); }
        }
        SECTION("Full set, choose 0") {
            Combinations c(vector_t{1, 2, 3}, 0);
            SECTION("Size") { REQUIRE(c.size() == 1); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{}); }
        }
        SECTION("Full set, choose 1") {
            Combinations c(vector_t{1, 2, 3}, 1);
            SECTION("Size") { REQUIRE(c.size() == 3); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{1}); }
        }
        SECTION("Full set, choose 2") {
            Combinations c(vector_t{1, 2, 3}, 2);
            SECTION("Size") { REQUIRE(c.size() == 3); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{1, 2}); }
        }
        SECTION("Full set, choose 3") {
            Combinations c(vector_t{1, 2, 3}, 3);
            SECTION("Size") { REQUIRE(c.size() == 1); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{1, 2, 3}); }
        }
    }
    SECTION("Repeat") {
        SECTION("Empty set") {
            Combinations c(vector_t{}, 0, true);
            SECTION("Size") { REQUIRE(c.size() == 1); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{}); }
        }
        SECTION("Full set, choose 0") {
            Combinations c(vector_t{1, 2, 3}, 0, true);
            SECTION("Size") { REQUIRE(c.size() == 1); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{}); }
        }
        SECTION("Full set, choose 1") {
            Combinations c(vector_t{1, 2, 3}, 1, true);
            SECTION("Size") { REQUIRE(c.size() == 3); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{1}); }
        }
        SECTION("Full set, choose 2") {
            Combinations c(vector_t{1, 2, 3}, 2, true);
            SECTION("Size") { REQUIRE(c.size() == 6); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{1, 1}); }
        }
        SECTION("Full set, choose 3") {
            Combinations c(vector_t{1, 2, 3}, 3, true);
            SECTION("Size") { REQUIRE(c.size() == 10); }
            SECTION("Value") { REQUIRE(*c.begin() == vector_t{1, 1, 1}); }
        }
    }
}

TEST_CASE("Combinations<vector<int>> : at") {
    SECTION("No repeat") {
        SECTION("Empty set") {
            Combinations c(vector_t{}, 0);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{}); }
        }
        SECTION("Full set, choose 0") {
            Combinations c(vector_t{1, 2, 3}, 0);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{}); }
        }
        SECTION("Full set, choose 1") {
            Combinations c(vector_t{1, 2, 3}, 1);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{1}); }
            SECTION("Element 1") { REQUIRE(c[1] == vector_t{2}); }
            SECTION("Element 2") { REQUIRE(c[2] == vector_t{3}); }
        }
        SECTION("Full set, choose 2") {
            Combinations c(vector_t{1, 2, 3}, 2);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{1, 2}); }
            SECTION("Element 1") { REQUIRE(c[1] == vector_t{1, 3}); }
            SECTION("Element 2") { REQUIRE(c[2] == vector_t{2, 3}); }
        }
        SECTION("Full set, choose 3") {
            Combinations c(vector_t{1, 2, 3}, 3);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{1, 2, 3}); }
        }
    }
    SECTION("Repeats") {
        SECTION("Empty set") {
            Combinations c(vector_t{}, 0, true);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{}); }
        }
        SECTION("Full set, choose 0") {
            Combinations c(vector_t{1, 2, 3}, 0, true);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{}); }
        }
        SECTION("Full set, choose 1") {
            Combinations c(vector_t{1, 2, 3}, 1, true);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{1}); }
            SECTION("Element 1") { REQUIRE(c[1] == vector_t{2}); }
            SECTION("Element 2") { REQUIRE(c[2] == vector_t{3}); }
        }
        SECTION("Full set, choose 2") {
            Combinations c(vector_t{1, 2, 3}, 2, true);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{1, 1}); }
            SECTION("Element 1") { REQUIRE(c[1] == vector_t{1, 2}); }
            SECTION("Element 2") { REQUIRE(c[2] == vector_t{1, 3}); }
            SECTION("Element 3") { REQUIRE(c[3] == vector_t{2, 2}); }
            SECTION("Element 4") { REQUIRE(c[4] == vector_t{2, 3}); }
            SECTION("Element 5") { REQUIRE(c[5] == vector_t{3, 3}); }
        }
        SECTION("Full set, choose 3") {
            Combinations c(vector_t{1, 2, 3}, 3, true);
            SECTION("Element 0") { REQUIRE(c[0] == vector_t{1, 1, 1}); }
            SECTION("Element 1") { REQUIRE(c[1] == vector_t{1, 1, 2}); }
            SECTION("Element 2") { REQUIRE(c[2] == vector_t{1, 1, 3}); }
            SECTION("Element 3") { REQUIRE(c[3] == vector_t{1, 2, 2}); }
            SECTION("Element 4") { REQUIRE(c[4] == vector_t{1, 2, 3}); }
            SECTION("Element 5") { REQUIRE(c[5] == vector_t{1, 3, 3}); }
            SECTION("Element 6") { REQUIRE(c[6] == vector_t{2, 2, 2}); }
            SECTION("Element 7") { REQUIRE(c[7] == vector_t{2, 2, 3}); }
            SECTION("Element 8") { REQUIRE(c[8] == vector_t{2, 3, 3}); }
            SECTION("Element 9") { REQUIRE(c[9] == vector_t{3, 3, 3}); }
        }
    }
}

TEST_CASE("Combinations<vector<int>> : can be used in a foreach loop") {
    std::vector corr{vector_t{1, 2}, vector_t{1, 3}, vector_t{2, 3}};
    std::size_t counter = 0;
    for(auto comb : Combinations(vector_t{1, 2, 3}, 2)) {
        REQUIRE(comb == corr[counter++]);
    }
}
