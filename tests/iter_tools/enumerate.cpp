#include <catch2/catch.hpp>
#include <utilities/iter_tools/enumerate.hpp>
#include <vector>

using namespace utilities;

TEST_CASE("Enumerate<std::vector<int>>") {
    std::vector test{1, 2, 3};
    std::size_t counter = 0;
    for(auto&& [x, y] : Enumerate(test)) {
        REQUIRE(x == counter);
        REQUIRE(y == ++counter);
    }
}
