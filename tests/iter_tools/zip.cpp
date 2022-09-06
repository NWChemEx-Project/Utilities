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
#include <utilities/iter_tools/zip.hpp>

using namespace utilities;

TEST_CASE("Zip<vector<int>, vector<string> > : ctor") {
    std::vector v1{1, 2, 3};
    std::vector<std::string> v2{"one", "two", "three"};
    Zip z(std::vector<int>{1, 2, 3},
          std::vector<std::string>{"one", "two", "three"});
    SECTION("Zip type") {
        STATIC_REQUIRE(
          std::is_same_v<Zip<std::vector<int>, std::vector<std::string>>,
                         decltype(z)>);
    }
    SECTION("Size") { REQUIRE(z.size() == 3); }
    SECTION("Initial Value") {
        SECTION("Correct") {
            REQUIRE(z[0] == std::tuple<int, std::string>{1, "one"});
        }
        SECTION("Are read-/write- references") {
            STATIC_REQUIRE(std::is_same_v<int&, decltype(std::get<0>(z[0]))>);
            STATIC_REQUIRE(
              std::is_same_v<std::string&, decltype(std::get<1>(z[0]))>);
        }
    }
}

TEST_CASE("Zip<vector<int>&, vector<string>& > : ctor") {
    std::vector v1{1, 2, 3};
    std::vector<std::string> v2{"one", "two", "three"};
    Zip z(v1, v2);
    SECTION("Zip type") {
        STATIC_REQUIRE(
          std::is_same_v<Zip<std::vector<int>&, std::vector<std::string>&>,
                         decltype(z)>);
    }
    SECTION("Size") { REQUIRE(z.size() == 3); }
    SECTION("Initial Value") {
        SECTION("Correct") {
            REQUIRE(z[0] == std::tuple<int, std::string>{1, "one"});
        }
        SECTION("Are references") {
            REQUIRE(&std::get<0>(z[0]) == &v1[0]);
            REQUIRE(&std::get<1>(z[0]) == &v2[0]);
        }
        SECTION("Are read-/write- references") {
            STATIC_REQUIRE(std::is_same_v<int&, decltype(std::get<0>(z[0]))>);
            STATIC_REQUIRE(
              std::is_same_v<std::string&, decltype(std::get<1>(z[0]))>);
        }
    }
}

TEST_CASE("Zip<const vector<int>&, const vector<string>& > : ctor") {
    const std::vector v1{1, 2, 3};
    const std::vector<std::string> v2{"one", "two", "three"};
    Zip z(v1, v2);
    SECTION("Zip type") {
        STATIC_REQUIRE(
          std::is_same_v<
            Zip<const std::vector<int>&, const std::vector<std::string>&>,
            decltype(z)>);
    }
    SECTION("Size") { REQUIRE(z.size() == 3); }
    SECTION("Initial Value") {
        SECTION("Correct") {
            REQUIRE(z[0] == std::tuple<int, std::string>{1, "one"});
        }
        SECTION("Are references") {
            REQUIRE(&std::get<0>(z[0]) == &v1[0]);
            REQUIRE(&std::get<1>(z[0]) == &v2[0]);
        }
        SECTION("Are read-only references") {
            STATIC_REQUIRE(
              std::is_same_v<const int&, decltype(std::get<0>(z[0]))>);
            STATIC_REQUIRE(
              std::is_same_v<const std::string&, decltype(std::get<1>(z[0]))>);
        }
    }
}

TEST_CASE("Zip<vector<int>&, vector<string>& > : size") {
    std::vector v1{1, 2, 3};
    SECTION("Same size") {
        std::vector<std::string> v2{"one", "two", "three"};
        Zip z(v1, v2);
        REQUIRE(z.size() == 3);
    }
    SECTION("Different sizes") {
        std::vector<std::string> v2{"one", "two", "three", "four"};
        Zip z(v1, v2);
        REQUIRE(z.size() == 3);
    }
}

TEST_CASE("Zip<vector<int>&, vector<string>& > : at_") {
    std::vector v1{1, 2, 3};
    SECTION("Same size") {
        std::vector<std::string> v2{"one", "two", "three"};
        Zip z(v1, v2);
        SECTION("Element 0") {
            REQUIRE(z[0] == std::tuple<int, std::string>{1, "one"});
        }
        SECTION("Element 1") {
            REQUIRE(z[1] == std::tuple<int, std::string>{2, "two"});
        }
        SECTION("Element 2") {
            REQUIRE(z[2] == std::tuple<int, std::string>{3, "three"});
        }
    }
    SECTION("Different sizes") {
        std::vector<std::string> v2{"one", "two", "three", "four"};
        Zip z(v1, v2);
        SECTION("Element 0") {
            REQUIRE(z[0] == std::tuple<int, std::string>{1, "one"});
        }
        SECTION("Element 1") {
            REQUIRE(z[1] == std::tuple<int, std::string>{2, "two"});
        }
        SECTION("Element 2") {
            REQUIRE(z[2] == std::tuple<int, std::string>{3, "three"});
        }
    }
}

TEST_CASE("Zip<vector<int>&, vector<string>&> : works in foreach loop") {
    std::vector v1{1, 2, 3};
    std::vector<std::string> v2{"one", "two", "three"};
    std::size_t counter = 0;
    for(auto&& [x, y] : Zip(v1, v2)) {
        REQUIRE(&x == &v1[counter]);
        REQUIRE(&y == &v2[counter++]);
    }
}
