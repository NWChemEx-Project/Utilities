#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set/range_view_pimpl.hpp>
#include <utilities/containers/detail_/math_set/set_pimpl.hpp>
#include <vector>

using namespace utilities::detail_;
using vector_t = std::vector<int>;

TEST_CASE("RangeViewPIMPL range ctor") {
    SECTION("Empty range") {
        SetPIMPL<int> s;
        RangeViewPIMPL p(0, 0, &s);
        REQUIRE(p.size() == 0);
        REQUIRE_THROWS_AS(p[0], std::out_of_range);
    }
    SECTION("Actual range") {
        SetPIMPL vec{1, 2, 3};
        RangeViewPIMPL p(1, 2, &vec);
        STATIC_REQUIRE(std::is_same_v<typename decltype(p)::value_type, int>);

        REQUIRE(p.size() == 1);
        REQUIRE(&p[0] == &(vec[1]));
    }
}

TEST_CASE("RangeViewPIMPL get") {
    SetPIMPL s{1, 2, 3, 4, 5};
    RangeViewPIMPL p(1, 3, &s);
    REQUIRE(&p[0] == &s[1]);
    REQUIRE(p[0] == 2);
    REQUIRE(&p[1] == &s[2]);
    REQUIRE(p[1] == 3);
}

TEST_CASE("RangeViewPIMPL push_back") {
    SetPIMPL s{1, 2, 3, 4, 5};
    RangeViewPIMPL p(1, 3, &s);
    REQUIRE_THROWS_AS(p.push_back(9), std::runtime_error);
}

TEST_CASE("RangeViewPIMPL size") {
    SECTION("Empty set") {
        SetPIMPL<int> v;
        RangeViewPIMPL p(0, 0, &v);
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty set") {
        SetPIMPL v{1, 2, 3, 4, 5};
        RangeViewPIMPL p(1, 3, &v);
        REQUIRE(p.size() == 2);
    }
}
