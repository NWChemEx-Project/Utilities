#include <catch2/catch.hpp>
#include <utilities/containers/math_set/detail_/range_view_pimpl.hpp>
#include <vector>

using namespace utilities::detail_;
using vector_t = std::vector<int>;

TEST_CASE("RangeViewPIMPL range ctor") {
    SECTION("Empty range") {
        vector_t v;
        RangeViewPIMPL p(0, 0, &v);
        REQUIRE(p.size() == 0);
        REQUIRE_THROWS_AS(p[0], std::out_of_range);
    }
    SECTION("Actual range") {
        vector_t vec{1, 2, 3};
        RangeViewPIMPL p(1, 2, &vec);
        STATIC_REQUIRE(std::is_same_v<typename decltype(p)::value_type, int>);

        REQUIRE(p.size() == 1);
        auto b = p.begin();
        REQUIRE(&(*b) == &(vec[1]));
        REQUIRE(++b == p.end());
    }
}

TEST_CASE("RangeViewPIMPL get") {
    vector_t v{1, 2, 3};
    RangeViewPIMPL p(1, 3, &v);
    REQUIRE(&p[0] == &v[1]);
    REQUIRE(p[0] == 2);
    REQUIRE(&p[1] == &v[2]);
    REQUIRE(p[1] == 3);
}

TEST_CASE("RangeViewPIMPL insert") {
    SECTION("Empty set") {
        vector_t v;
        RangeViewPIMPL p(0, 0, &v);
        p.insert(0);
        REQUIRE(p.size() == 1);
        REQUIRE(p[0] == 0);
        REQUIRE(v == vector_t{0});

        SECTION("At beginning") {
            p.insert(p.begin(), 1);
            REQUIRE(p.size() == 2);
            REQUIRE(p[0] == 1);
            REQUIRE(p[1] == 0);
            REQUIRE(v == vector_t{1, 0});
        }

        SECTION("At end") {
            p.insert(p.end(), 1);
            REQUIRE(p.size() == 2);
            REQUIRE(p[0] == 0);
            REQUIRE(p[1] == 1);
            REQUIRE(v == vector_t{0, 1});
        }
    }
    SECTION("Non-empty") {
        vector_t v{1, 2, 3};
        RangeViewPIMPL p(0, 2, &v);

        SECTION("At the beginning") {
            p.insert(p.begin(), 3);
            REQUIRE(p.size() == 3);
            std::vector<int> corr{3, 1, 2};
            auto itr = p.begin();
            for(std::size_t i = 0; i < 3; ++i, ++itr) {
                REQUIRE(*itr == corr[i]);
            }
            REQUIRE(v == std::vector<int>{3, 1, 2, 3});
        }

        SECTION("At the end") {
            p.insert(p.end(), 4);
            REQUIRE(p.size() == 3);
            std::vector<int> corr{1, 2, 4};
            auto itr = p.begin();
            for(std::size_t i = 0; i < 3; ++i, ++itr) REQUIRE(*itr == corr[i]);
            REQUIRE(v == std::vector<int>{1, 2, 4, 3});
        }
    }
}

TEST_CASE("RangeViewPIMPL size") {
    SECTION("Empty set") {
        vector_t v;
        RangeViewPIMPL p(0, 0, &v);
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty set") {
        vector_t v{1, 2, 3, 4, 5};
        RangeViewPIMPL p(1, 3, &v);
        REQUIRE(p.size() == 2);
    }
}
