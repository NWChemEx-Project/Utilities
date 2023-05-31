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
#include <utilities/iterators/offset_iterator.hpp>

using namespace utilities::iterators;
using vector_t = std::vector<int>;

/* Testing strategy.
 *
 * Thanks to unit testing of the base iterator classes, we already know that the
 * vast majority of the public API. This is contingent on use implementing the
 * various hooks correctly. To that end we need to check:
 *
 * - the ctors
 *   - This includes const-correctness (copy/move can't be used to go from
 *     read-only to read/write access)
 * - dereference is implemented correctly
 *   - This includes that read-/write-able or read-only elements are returned
 * - increment, decrement work and return *this after increment/decrement
 * - are_equal correctly ensures we are indexing the same container and point to
 *   the same element
 * - advance correctly advances
 * - distance_to correctly advances and throws if not reachable
 */

TEST_CASE("OffsetIterator<vector<int>> : default ctor") {
    OffsetIterator<vector_t> itr; // Basically just makes sure it can compile.
}

TEST_CASE("OffsetIterator<vector<int>> : wrapping ctor") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    SECTION("Points to zero-th element") { REQUIRE(&(*itr) == &corr[0]); }
}

TEST_CASE("OffsetIterator<vector<int>> : copy ctor") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator itr2(itr);
    SECTION("Are same") { REQUIRE(itr == itr2); }
    SECTION("Elements are read-/write-able") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<const vector<int>> : copy ctor") {
    const vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator itr2(itr);
    SECTION("Are same") { REQUIRE(itr == itr2); }
    SECTION("Elements are read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : move ctor") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator itr3(itr);
    OffsetIterator itr2(std::move(itr));
    SECTION("Are same") { REQUIRE(itr3 == itr2); }
    SECTION("Elements are read-/write-able") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<const vector<int>> : move ctor") {
    const vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator itr3(itr);
    OffsetIterator itr2(std::move(itr));
    SECTION("Are same") { REQUIRE(itr3 == itr2); }
    SECTION("Elements are read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : copy assignment") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator<vector_t> itr2;
    auto pitr2 = &(itr2 = itr);
    SECTION("Returns *this") { REQUIRE(pitr2 == &itr2); }
    SECTION("Are same") { REQUIRE(itr == itr2); }
    SECTION("Elements are read-/write-able") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<const vector<int>> : copy assignment") {
    const vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator<const vector_t> itr2;
    auto pitr2 = &(itr2 = itr);
    SECTION("Returns *this") { REQUIRE(pitr2 == &itr2); }
    SECTION("Are same") { REQUIRE(itr == itr2); }
    SECTION("Elements are read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : move assignment") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator itr3(itr);
    OffsetIterator<vector_t> itr2;
    auto pitr2 = &(itr2 = std::move(itr));
    SECTION("Returns *this") { REQUIRE(pitr2 == &itr2); }
    SECTION("Are same") { REQUIRE(itr3 == itr2); }
    SECTION("Elements are read-/write-able") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<const vector<int>> : move assignment") {
    const vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    OffsetIterator itr3(itr);
    OffsetIterator<const vector_t> itr2;
    auto pitr2 = &(itr2 = std::move(itr));
    SECTION("Returns *this") { REQUIRE(pitr2 == &itr2); }
    SECTION("Are same") { REQUIRE(itr3 == itr2); }
    SECTION("Elements are read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(*itr2)>);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : dereference") {
    vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);
    SECTION("Correct value") { REQUIRE(*itr == 1); }
    SECTION("Aliases") { REQUIRE(&(*itr) == &corr[0]); }
    SECTION("Elements are read-/write-able") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(*itr)>);
    }
}

TEST_CASE("OffsetIterator<const vector<int>> : dereference") {
    const vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);
    SECTION("Correct Value") { REQUIRE(*itr == 1); }
    SECTION("Aliases") { REQUIRE(&(*itr) == &corr[0]); }
    SECTION("Elements are read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(*itr)>);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : increment") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    auto pitr = &(++itr);
    SECTION("Correct value") { REQUIRE(*itr == 2); }
    SECTION("Returns this") { REQUIRE(pitr == &itr); }
}

TEST_CASE("OffsetIterator<vector<int>> : decrement") {
    vector_t corr{1, 2, 3};
    OffsetIterator itr(0, &corr);
    ++itr;
    auto pitr = &(--itr);
    SECTION("Correct value") { REQUIRE(*itr == 1); }
    SECTION("Returns this") { REQUIRE(pitr == &itr); }
}

TEST_CASE("OffsetIterator<vector<int>> : are_equal") {
    vector_t corr{1, 2, 3};
    OffsetIterator s{0, &corr};
    SECTION("Same container") {
        OffsetIterator s1{0, &corr};
        SECTION("Same element") { REQUIRE(s == s1); }
        SECTION("Different element") {
            ++s1;
            REQUIRE(s != s1);
        }
    }
    SECTION("Different container") {
        vector_t corr2{1, 2, 3};
        OffsetIterator s1{0, &corr2};
        REQUIRE(s != s1);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : advance") {
    vector_t corr{1, 2, 3};
    OffsetIterator s(0, &corr);
    auto ps = &(s += 2);
    SECTION("Returns *this") { REQUIRE(ps == &s); }
    SECTION("Correct value") { REQUIRE(*s == 3); }
    SECTION("Reverse advance") {
        s -= 2;
        REQUIRE(*s == 1);
    }
}

TEST_CASE("OffsetIterator<vector<int>> : distance_to") {
    vector_t corr{1, 2, 3};
    OffsetIterator s(0, &corr);
    OffsetIterator s1(0, &corr);
    SECTION("Same element") { REQUIRE((s1 - s) == 0); }
    SECTION("Ahead by 1") {
        ++s1;
        REQUIRE(s1 - s == 1);
        REQUIRE(s - s1 == -1);
    }
    SECTION("Ahead by 2") {
        s1 += 2;
        REQUIRE(s1 - s == 2);
        REQUIRE(s - s1 == -2);
    }
    SECTION("Throws if unreachable") {
        vector_t corr2;
        OffsetIterator s2(0, &corr2);
        REQUIRE_THROWS_AS(s - s2, std::out_of_range);
    }
}
