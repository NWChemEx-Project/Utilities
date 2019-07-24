#include <catch2/catch.hpp>
#include <utilities/containers/detail_/nested_set_pimpl.hpp>
#include <utilities/containers/math_set.hpp>

using namespace utilities;
using namespace utilities::detail_;

TEST_CASE("NestedSetPIMPL default ctor") {
    NestedSetPIMPL<MathSet<int>> s;

    REQUIRE(s.size() == 0);
}

TEST_CASE("NestedSetPIMPL container ctor") {
    std::vector corr{MathSet{1, 2}, MathSet{3, 4}, MathSet{5, 6}};

    NestedSetPIMPL<MathSet<int>> s(corr.begin(), corr.end());
    REQUIRE(s.size() == 3);
    std::size_t counter = 1;
    for(const auto& x : s) {
        REQUIRE(x.size() == 2);
        for(const auto& y : x) REQUIRE(y == counter++);
    }
}
