#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set/nested_set_pimpl.hpp>
#include <utilities/containers/math_set.hpp>

using namespace utilities;
using namespace utilities::detail_;
using set_t = MathSet<int>;

TEST_CASE("NestedSetPIMPL default ctor") {
    NestedSetPIMPL<set_t> s;
    REQUIRE(s.size() == 0);
}

TEST_CASE("NestedSetPIMPL initializer list ctor") {
    SECTION("Empty initializer list") {
        NestedSetPIMPL s(std::initializer_list<set_t>{});
        REQUIRE(s.size() == 0);
    }
    SECTION("Non-empty initializer list") {
        std::vector corr{1, 2, 2, 3, 3, 4};
        NestedSetPIMPL s{MathSet{1, 2}, MathSet{2, 3}, MathSet{3, 4}};

        using deduced_type = decltype(s);
        STATIC_REQUIRE(std::is_same_v<deduced_type, NestedSetPIMPL<set_t>>);

        REQUIRE(s.size() == 3);

        auto* pelem = &s[0][0]; // Set to the address of the first element
        std::size_t counter = 0;
        for(const MathSet<int>& x : s) {
            REQUIRE(x.size() == 2);
            for(const int& y : x) {
                REQUIRE(y == corr[counter]);
                // Check that the addresses are contiguous
                REQUIRE(&y == pelem);
                ++pelem;
                ++counter;
            }
        }
    }
}

TEST_CASE("NestedSetPIMPL range ctor") {
    SECTION("Empty range") {
        std::vector<set_t> v;
        NestedSetPIMPL p(v.begin(), v.end());
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty range") {
        std::vector v{set_t{1, 2}, set_t{3}};
        NestedSetPIMPL p(v.begin(), v.end());
        REQUIRE(p.size() == 2);
        auto pp = &(p[0][0]);
        REQUIRE(p[0][0] == 1);
        REQUIRE(p[0][1] == 2);
        ++pp;
        REQUIRE(pp == &(p[0][1]));
        REQUIRE(p[1][0] == 3);
        ++pp;
        REQUIRE(pp == &(p[1][0]));
    }
}

TEST_CASE("NestedSetPIMPL insert") {
    SECTION("Empty set") {
        NestedSetPIMPL<set_t> s;

        s.insert(set_t{1, 2});
        REQUIRE(s.size() == 1);
        REQUIRE(s[0] == set_t{1, 2});
    }
    SECTION("Non-empty set") {
        NestedSetPIMPL s{set_t{1, 2}};

        SECTION("Add before element") {
            s.insert(s.begin(), set_t{3, 4});
            REQUIRE(s.size() == 2);
            REQUIRE(s[0] == set_t{3, 4});
            REQUIRE(s[1] == set_t{1, 2});
        }

        SECTION("Add after element") {
            s.insert(s.end(), set_t{3, 4});
            REQUIRE(s.size() == 2);
            REQUIRE(s[0] == set_t{1, 2});
            REQUIRE(s[1] == set_t{3, 4});
        }

        SECTION("Add to element beginning") {
            s[0].insert(s[0].begin(), 0);
            REQUIRE(s.size() == 1);
            REQUIRE(s[0] == set_t{0, 1, 2});
        }
        SECTION("Add to element middle") {
            s[0].insert(s[0].begin() + 1, 0);
            REQUIRE(s.size() == 1);
            REQUIRE(s[0] == set_t{1, 0, 2});
        }
        SECTION("Add to element end") {
            s[0].insert(s[0].end(), 0);
            REQUIRE(s.size() == 1);
            REQUIRE(s[0] == set_t{1, 2, 0});
        }
    }
}

TEST_CASE("NestedSetPIMPL get") {
    NestedSetPIMPL s{set_t{1, 2}, set_t{3, 4}, set_t{}};
    REQUIRE(s[0] == set_t{1, 2});
    REQUIRE(s[1] == set_t{3, 4});
    REQUIRE(s[2] == set_t{});
}

TEST_CASE("NestedSetPIMPL size") {
    SECTION("empty set") {
        NestedSetPIMPL<set_t> p;
        REQUIRE(p.size() == 0);
    }
    SECTION("non-empty set") {
        NestedSetPIMPL p{set_t{1}, set_t{2}, set_t{3}};
        REQUIRE(p.size() == 3);
    }
}

TEST_CASE("NestedSetPIMPL clear") {
    NestedSetPIMPL s{set_t{}};
    REQUIRE(s.size() == 1);
    s.clear();
    REQUIRE(s.size() == 0);
}

TEST_CASE("Nested Nested MathSet") {
    std::vector v{set_t{1, 2}, set_t{3, 4}, set_t{4, 5}};
    MathSet<set_t> s(v.begin(), v.end());
    std::vector v2{set_t{1, 3}, set_t{4, 1}, set_t{}, set_t{1}};
    MathSet<set_t> s2(v2.begin(), v2.end());
    NestedSetPIMPL p{s, s2};

    REQUIRE(p.size() == 2);
    auto ptr = &(p[0][0][0]);

    REQUIRE(p[0] == s);

    REQUIRE(p[0][0] == set_t{1, 2});
    REQUIRE(p[0][0][0] == 1);
    ++ptr;
    REQUIRE(p[0][0][1] == 2);
    REQUIRE(&(p[0][0][1]) == ptr);
    ++ptr;

    REQUIRE(p[0][1] == set_t{3, 4});
    REQUIRE(p[0][1][0] == 3);
    REQUIRE(&(p[0][1][0]) == ptr);
    ++ptr;
    REQUIRE(p[0][1][1] == 4);
    REQUIRE(&(p[0][1][1]) == ptr);
    ++ptr;

    REQUIRE(p[0][2] == set_t{4, 5});
    REQUIRE(p[0][2][0] == 4);
    REQUIRE(&(p[0][2][0]) == ptr);
    ++ptr;
    REQUIRE(p[0][2][1] == 5);
    REQUIRE(&(p[0][2][1]) == ptr);
    ++ptr;

    REQUIRE(p[1] == s2);

    REQUIRE(p[1][0] == set_t{1, 3});
    REQUIRE(p[1][0][0] == 1);
    REQUIRE(&(p[1][0][0]) == ptr);
    ++ptr;
    REQUIRE(p[1][0][1] == 3);
    REQUIRE(&(p[1][0][1]) == ptr);
    ++ptr;

    REQUIRE(p[1][1] == set_t{4, 1});
    REQUIRE(p[1][1][0] == 4);
    REQUIRE(&(p[1][1][0]) == ptr);
    ++ptr;
    REQUIRE(p[1][1][1] == 1);
    REQUIRE(&(p[1][1][1]) == ptr);
    ++ptr;

    REQUIRE(p[1][2] == set_t{});

    REQUIRE(p[1][3] == set_t{1});
    REQUIRE(p[1][3][0] == 1);
    REQUIRE(&(p[1][3][0]) == ptr);
}
