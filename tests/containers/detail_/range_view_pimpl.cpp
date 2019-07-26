#include <catch2/catch.hpp>
#include <utilities/containers/detail_/range_view_pimpl.hpp>
#include <vector>
using namespace utilities::detail_;

TEST_CASE("RangeViewPIMPL range ctor") {
    auto ptr = std::make_shared<std::vector<int>>(std::vector<int>{1, 2, 3});

    RangeViewPIMPL p(1, 2, ptr);
    STATIC_REQUIRE(std::is_same_v<typename decltype(p)::value_type, int>);

    REQUIRE(p.size() == 1);
    auto b = p.begin();
    REQUIRE(&(*b) == &((*ptr)[1]));
    REQUIRE(++b == p.end());
}
