#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set_view.hpp>
#include <vector>
using namespace utilities::detail_;

TEST_CASE("MathSetViewPIMPL") {
    auto ptr = std::make_shared<std::vector<int>>(std::vector<int>{1, 2, 3});

    MathSetView p(1, 2, ptr);

    REQUIRE(p.size() == 1);
    auto b = p.begin();
    REQUIRE(&(*b) == &((*ptr)[1]));
    REQUIRE(++b == p.end());
}
