#include <UtilitiesEx/IterTools/CartesianProduct.hpp>
#include <UtilitiesEx/IterTools/Enumerate.hpp>
#include <catch/catch.hpp>

using namespace UtilitiesEx;

TEST_CASE("Empty") {
    auto c = CartesianProduct();
    REQUIRE(c.size() == 0);
    REQUIRE(c.begin() == c.end());
}

TEST_CASE("Single container") {
    std::vector<int> l{1, 2, 3};
    auto c = CartesianProduct(l);
    REQUIRE(c.size() == 3);
    for(auto x : Enumerate(c))
        REQUIRE(l[std::get<0>(x)] == std::get<0>(std::get<1>(x)));
}

TEST_CASE("Two containers") {
    std::vector<int> l{1, 2, 3};
    std::vector<std::tuple<int,int>> corr{{1,1}, {1,2}, {1,3},
                                          {2,1}, {2,2}, {2,3},
                                          {3,1}, {3,2}, {3,3} };
    auto c = CartesianProduct(l, l);
    REQUIRE(c.size() == 9);
    for(auto x : Enumerate(c)){
        const auto itr = std::get<0>(x);
        const auto val = std::get<1>(x);
        REQUIRE(std::get<0>(corr[itr]) == std::get<0>(val));
        REQUIRE(std::get<1>(corr[itr]) == std::get<1>(val));
    }

}
