#include <catch2/catch.hpp>
#include <utilities/containers/detail_/range_view_pimpl.hpp>
#include <utilities/containers/math_set_view.hpp>
#include <vector>

using namespace utilities;

TEST_CASE("MathSetView<int> pimpl ctor") {
    //    auto ptr  = std::make_shared<std::vector<int>>(std::vector<int>{1, 2,
    //    3}); auto ptr2 = std::make_shared<detail_::RangeViewPIMPL<int>>(1, 2,
    //    ptr); MathSetView<int> s(ptr2);
    //
    //    REQUIRE(s.size() == 1);
    //    REQUIRE_FALSE(s.empty());
}
