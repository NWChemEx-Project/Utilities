#include <Utilities/IterTools/Enumerate.hpp>
#include <catch/catch.hpp>
#include <vector>

using namespace Utilities;

TEST_CASE("Enumerate") {
    std::vector<double> test{1.1, 2.2, 3.3};
    std::size_t counter = 0;
    for(auto x : Enumerate(test)) {
        REQUIRE(std::get<0>(x) == counter);
        REQUIRE(std::get<1>(x) == test[counter]);
        ++counter;
    }
}
