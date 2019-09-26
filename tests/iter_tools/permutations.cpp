#include <catch2/catch.hpp>
#include <utilities/iter_tools/permutations.hpp>

using namespace utilities;
using set_type = std::vector<int>;

/* Testing Strategy
 *
 * We know that the majority of the container API works because we have tested
 * the IndexableContainerBase class. What we don't know is:
 *
 * - Does Permutations's ctor initialize it correctly in all cases?
 * - Is size() hooked up correctly?
 *   - (It calls n_permutations, which we know works)
 * - Is at_ implemented correctly?
 *   - Does it increment correctly
 *
 */

TEST_CASE("Permutations<vector<int>> : ctor") {
    SECTION("Empty set") {
        Permutations p(set_type{});
        SECTION("Size") { REQUIRE(p.size() == 1); }
        SECTION("Value") { REQUIRE(*p.begin() == set_type{}); }
    }
    SECTION("Lexicographical 0") {
        set_type s{1, 2, 3};
        Permutations p(s);
        SECTION("Size") { REQUIRE(p.size() == 6); }
        SECTION("Value") { REQUIRE(*p.begin() == s); }
    }
    SECTION("Lexicographical 2") {
        set_type s{2, 1, 3};
        Permutations p(s);
        SECTION("Size") { REQUIRE(p.size() == 6); }
        SECTION("Value") { REQUIRE(*p.begin() == s); }
    }
    SECTION("Repeat elements") {
        set_type s{1, 2, 2};
        Permutations p(s);
        SECTION("size") { REQUIRE(p.size() == 3); }
        SECTION("value") { REQUIRE(*p.begin() == s); }
    }
}

TEST_CASE("Permutations<vector<int>> : size_") {
    SECTION("Empty") {
        Permutations p(set_type{});
        REQUIRE(p.size() == 1);
    }
    SECTION("Two elements") {
        Permutations p(set_type{2, 1});
        REQUIRE(p.size() == 2);
    }
}

TEST_CASE("Permutations<vector<int>> : at_") {
    SECTION("Empty") {
        set_type s{};
        Permutations p(s);
        SECTION("Element 0") { REQUIRE(p[0] == s); }
    }
    SECTION("Three elements, no repeats, lexicographical 0") {
        Permutations p(set_type{1, 2, 3});
        SECTION("Element 0") { REQUIRE(p[0] == set_type{1, 2, 3}); }
        SECTION("Element 1") { REQUIRE(p[1] == set_type{1, 3, 2}); }
        SECTION("Element 2") { REQUIRE(p[2] == set_type{2, 1, 3}); }
        SECTION("Element 3") { REQUIRE(p[3] == set_type{2, 3, 1}); }
        SECTION("Element 4") { REQUIRE(p[4] == set_type{3, 1, 2}); }
        SECTION("Element 5") { REQUIRE(p[5] == set_type{3, 2, 1}); }
    }
    SECTION("Three elements, no repeats, not lexicographical 0") {
        Permutations p(set_type{2, 1, 3});
        SECTION("Element 0") { REQUIRE(p[0] == set_type{2, 1, 3}); }
        SECTION("Element 1") { REQUIRE(p[1] == set_type{2, 3, 1}); }
        SECTION("Element 2") { REQUIRE(p[2] == set_type{3, 1, 2}); }
        SECTION("Element 3") { REQUIRE(p[3] == set_type{3, 2, 1}); }
        SECTION("Element 4") { REQUIRE(p[4] == set_type{1, 2, 3}); }
        SECTION("Element 5") { REQUIRE(p[5] == set_type{1, 3, 2}); }
    }
    SECTION("Three elements, repeats, lexicographical 0") {
        Permutations p(set_type{1, 2, 2});
        SECTION("Element 0") { REQUIRE(p[0] == set_type{1, 2, 2}); }
        SECTION("Element 1") { REQUIRE(p[1] == set_type{2, 1, 2}); }
        SECTION("Element 2") { REQUIRE(p[2] == set_type{2, 2, 1}); }
    }
    SECTION("Three elements, repeats, not lexicographical 0") {
        Permutations p(set_type{2, 1, 2});
        SECTION("Element 0") { REQUIRE(p[0] == set_type{2, 1, 2}); }
        SECTION("Element 1") { REQUIRE(p[1] == set_type{2, 2, 1}); }
        SECTION("Element 2") { REQUIRE(p[2] == set_type{1, 2, 2}); }
    }
}

TEST_CASE("Permutations<vector<int>> : Works in a foreach loop") {
    set_type corr{1, 2, 3};
    for(auto p : Permutations(set_type{1, 2, 3})) {
        REQURIE(p == corr);
        std::next_permutation(corr.begin(), corr.end());
    }
}
