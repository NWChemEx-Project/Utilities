#include "catch/catch.hpp"
#include <UtilitiesEx/IterTools/Permutations.hpp>
#include <UtilitiesEx/TypeTraits/type_traitsExtensions.hpp>

using namespace UtilitiesEx;
using set_type = std::vector<int>;
using perm_itr = detail_::PermutationItr<set_type>;

void check_state(perm_itr& b, const perm_itr& end,
                 const std::vector<set_type>& corr, size_t off = 0) {
    if(corr.empty())
        REQUIRE(b == end);
    else
        REQUIRE(b != end);
    long counter = 0;
    while(b != end) {
        // Check that from each permutation the distance to every other perm
        // is good (subtract off cause logic won't work if temp starts less
        // than b)
        for(size_t pstart = 0; pstart < corr.size() - off; ++pstart) {
            perm_itr temp{corr[pstart], 0};
            long dx = static_cast<long>(pstart) - counter;

            REQUIRE(b.distance_to(temp) == dx);
            perm_itr copyb{b};
            REQUIRE(*(copyb.advance(dx)) == *temp);
        }
        REQUIRE(*b++ == corr[counter++]);
    }
    // b is at end, this gets it to point at the last permutation
    --b;
    for(size_t i = corr.size(); i > 0; --i) { REQUIRE(*b-- == corr[i - 1]); }
}

TEST_CASE("Empty Permutation") {
    perm_itr p0;
    check_state(p0, perm_itr{}, {});
}

TEST_CASE("Permutations of empty set") {
    set_type empty{};
    perm_itr p0{empty, 0};
    perm_itr p1{empty, 1};
    check_state(p0, p1, {empty});
}

TEST_CASE("Permutations of set of unique elements") {
    set_type s0{1, 2, 3};
    perm_itr p0{s0, 0};
    perm_itr p1{s0, 6};
    SECTION("Correctness") {
        check_state(p0, p1,
                    {set_type{1, 2, 3}, set_type{1, 3, 2}, set_type{2, 1, 3},
                     set_type{2, 3, 1}, set_type{3, 1, 2}, set_type{3, 2, 1}});
    }
    SECTION("Copy Ctor") {
        perm_itr p2{p0};
        REQUIRE(p2 == p0);
    }
    SECTION("Copy Assignment") {
        perm_itr p2;
        REQUIRE(p2 != p0);
        p2 = p0;
        REQUIRE(p2 == p0);
    }
    SECTION("Move Ctor") {
        perm_itr corr{p0};
        perm_itr p2{std::move(p0)};
        REQUIRE(p2 == corr);
    }
    SECTION("Move Assignment") {
        perm_itr p2;
        perm_itr corr{p0};
        REQUIRE(p2 != p0);
        p2 = std::move(p0);
        REQUIRE(p2 == corr);
    }
}

TEST_CASE("Permutations of set of non-unique elements") {
    set_type s0{1, 2, 2};
    perm_itr p0{s0, 0};
    perm_itr p1{s0, 3};
    check_state(p0, p1,
                {set_type{1, 2, 2}, set_type{2, 1, 2}, set_type{2, 2, 1}});
}

TEST_CASE("Permutations from not lexicographically smallest") {
    set_type s0{1, 3, 2};
    perm_itr p0{s0, 0};
    perm_itr p1{s0, 6};
    check_state(p0, p1,
                {set_type{1, 3, 2}, set_type{2, 1, 3}, set_type{2, 3, 1},
                 set_type{3, 1, 2}, set_type{3, 2, 1}, set_type{1, 2, 3}},
                1);
}

TEST_CASE("Permutations of set of non-unique elements from not "
          "lexicographically smallest") {
    set_type s0{2, 1, 2};
    perm_itr p0{s0, 0};
    perm_itr p1{s0, 3};
    check_state(p0, p1,
                {set_type{2, 1, 2}, set_type{2, 2, 1}, set_type{1, 2, 2}}, 1);
}
