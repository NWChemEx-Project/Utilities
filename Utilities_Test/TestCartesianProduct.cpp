#include <Utilities/IterTools/CartesianProduct.hpp>
#include <Utilities/IterTools/Enumerate.hpp>
#include <catch/catch.hpp>

using namespace Utilities;

template<typename CP_t, typename vector_type>
void check_state(CP_t& c, vector_type&& corr) {
    REQUIRE(c.size() == corr.size());
    if(c.size())
        REQUIRE(c.begin() != c.end());
    else
        REQUIRE(c.begin() == c.end());
    for(auto x : Enumerate(c)) {
        auto i   = std::get<0>(x);
        auto val = std::get<1>(x);
        REQUIRE(corr[i] == val);
    }
}

TEST_CASE("Empty") {
    auto c = CartesianProduct();
    check_state(c, std::vector<std::tuple<>>{});
}

TEST_CASE("Single container") {
    std::vector<int> l{1, 2, 3};
    std::vector<std::tuple<int>> corr{{1}, {2}, {3}};
    auto c = CartesianProduct(l);
    check_state(c, corr);
}

TEST_CASE("Two containers") {
    std::vector<int> l{1, 2, 3};
    std::vector<std::tuple<int, int>> corr{
      {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}};
    auto c = CartesianProduct(l, l);
    check_state(c, corr);
}

TEST_CASE("Two different containers") {
    std::vector<int> l{1, 2, 3};
    std::vector<int> l2{1, 3};
    std::vector<std::tuple<int, int>> corr{{1, 1}, {1, 3}, {2, 1},
                                           {2, 3}, {3, 1}, {3, 3}};
    auto c = CartesianProduct(l, l2);
    check_state(c, corr);
}
