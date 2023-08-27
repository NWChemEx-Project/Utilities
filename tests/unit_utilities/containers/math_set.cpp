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

#include <array>
#include <catch2/catch.hpp>
#include <utilities/containers/math_set.hpp>
#include <utilities/iter_tools/zip.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>

using namespace utilities;

template<std::size_t N, typename element_type>
void check_state(MathSet<element_type>& set,
                 std::array<element_type, N> values) {
    REQUIRE(set.size() == N);
    REQUIRE(set.empty() == (N == 0));
    if(N)
        REQUIRE(set.begin() != set.end());
    else
        REQUIRE(set.begin() == set.end());
    for(auto x : Zip(set, values)) {
        REQUIRE(std::get<0>(x) == std::get<1>(x));
    }
}

TEST_CASE("MathSet") {
    SECTION("Is a container") { REQUIRE(is_container<MathSet<double>>::value); }

    SECTION("Empty and trivial sets") {
        MathSet<double> empty_set;

        SECTION("Default constructor") { check_state<0>(empty_set, {}); }

        MathSet<double> set_11;
        SECTION("Empty equality") { REQUIRE(set_11 == empty_set); }

        set_11.insert(1.1);

        SECTION("Adding an element") { check_state<1>(set_11, {1.1}); }

        SECTION("Comparisons empty and trivial") {
            REQUIRE(set_11 != empty_set);
            REQUIRE(empty_set < set_11);
            REQUIRE(empty_set <= set_11);
            REQUIRE(set_11 > empty_set);
            REQUIRE(set_11 >= empty_set);
            REQUIRE(!(empty_set > set_11));
            REQUIRE(!(empty_set >= set_11));
            REQUIRE(!(set_11 < empty_set));
            REQUIRE(!(set_11 <= empty_set));
        }

        SECTION("Copy constructor") {
            MathSet<double> set2_11(set_11);
            check_state<1>(set2_11, {1.1});
        }

        SECTION("Assignment operator") {
            empty_set = set_11;
            check_state<1>(empty_set, {1.1});
        }

        SECTION("Move constructor") {
            MathSet<double> set2_11(std::move(set_11));
            check_state<1>(set2_11, {1.1});
        }

        SECTION("Move assignment") {
            empty_set = std::move(set_11);
            check_state<1>(empty_set, {1.1});
        }
    }

    SECTION("Non-trivial sets") {
        MathSet<double> set1({1.1, 2.2, 3.3});
        MathSet<double> set2({2.2, 4.4, 5.5, 6.6});

        SECTION("Initializer list constructor") {
            check_state<3>(set1, {1.1, 2.2, 3.3});
            check_state<4>(set2, {2.2, 4.4, 5.5, 6.6});
        }

        SECTION("Comparisons w/ empty_set") {
            MathSet<double> empty_set;
            REQUIRE(empty_set != set1);
            REQUIRE(empty_set < set1);
            REQUIRE(empty_set <= set1);
            REQUIRE(set1 > empty_set);
            REQUIRE(set1 >= empty_set);
        }

        SECTION("Union assignment") {
            auto& pset1 = (set1.union_assign(set2));
            REQUIRE(&pset1 == &set1);
            check_state<6>(set1, {1.1, 2.2, 3.3, 4.4, 5.5, 6.6});

            SECTION("Non-trivial comparisons") {
                REQUIRE(set2 < set1);
                REQUIRE(set2 <= set1);
                REQUIRE(set1 > set2);
                REQUIRE(set1 >= set2);
                REQUIRE(set2 != set1);
                REQUIRE(set2 >= set2);
                REQUIRE(set2 <= set2);
            }
        }

        SECTION("Take union") {
            auto rv = set2.take_union(set1);
            check_state<4>(set2, {2.2, 4.4, 5.5, 6.6});
            check_state<6>(rv, {1.1, 2.2, 3.3, 4.4, 5.5, 6.6});
        }

        SECTION("Intersection assign") {
            auto& pset1 = (set1.intersection_assign(set2));
            REQUIRE(&pset1 == &set1);
            check_state<1>(set1, {2.2});
        }

        SECTION("Intersection") {
            auto rv = set1.intersection(set2);
            check_state<3>(set1, {1.1, 2.2, 3.3});
            check_state<1>(rv, {2.2});
        }

        SECTION("Difference assign") {
            auto& pset1 = (set1.difference_assign(set2));
            REQUIRE(&pset1 == &set1);
            check_state<2>(set1, {1.1, 3.3});
        }

        SECTION("Difference") {
            auto rv = set1.difference(set2);
            check_state<3>(set1, {1.1, 2.2, 3.3});
            check_state<2>(rv, {1.1, 3.3});
        }

        SECTION("Symmetric difference assign") {
            auto& pset2 = (set2.symmetric_difference_assign(set1));
            REQUIRE(&pset2 == &set2);
            check_state<5>(set2, {1.1, 3.3, 4.4, 5.5, 6.6});
        }

        SECTION("Symmetric difference") {
            auto rv = set2.symmetric_difference(set1);
            check_state<4>(set2, {2.2, 4.4, 5.5, 6.6});
            check_state<5>(rv, {1.1, 3.3, 4.4, 5.5, 6.6});
        }
    }
}
