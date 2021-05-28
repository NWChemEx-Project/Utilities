#include "utilities/type_traits/tuple/tuple_product.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities;
using namespace utilities::detail_;

TEST_CASE("base_prod_t") {
    using type = base_prod_t<std::pair, int, double>;
    using corr = std::pair<int, double>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("base_prod_expand_r_t") {
    using rtuple = std::tuple<int, double, float>;
    using type   = base_prod_expand_r_t<std::pair, char, rtuple>;
    using pair0  = std::pair<char, int>;
    using pair1  = std::pair<char, double>;
    using pair2  = std::pair<char, float>;
    using corr   = std::tuple<pair0, pair1, pair2>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("tuple_prod_t") {
    using ltuple = std::tuple<char, long>;
    using rtuple = std::tuple<int, double, float>;
    using type   = tuple_prod_t<std::pair, ltuple, rtuple>;
    using pair0  = std::pair<char, int>;
    using pair1  = std::pair<char, double>;
    using pair2  = std::pair<char, float>;
    using pair3  = std::pair<long, int>;
    using pair4  = std::pair<long, double>;
    using pair5  = std::pair<long, float>;
    using corr   = std::tuple<pair0, pair1, pair2, pair3, pair4, pair5>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}