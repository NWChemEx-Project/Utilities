#include "utilities/type_traits/variant/variant_product.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities;
using namespace utilities::detail_;

TEST_CASE("variant_prod_t") {
    using ltuple = std::variant<char, long>;
    using rtuple = std::variant<int, double, float>;
    using type   = variant_prod_t<std::pair, ltuple, rtuple>;
    using pair0  = std::pair<char, int>;
    using pair1  = std::pair<char, double>;
    using pair2  = std::pair<char, float>;
    using pair3  = std::pair<long, int>;
    using pair4  = std::pair<long, double>;
    using pair5  = std::pair<long, float>;
    using corr   = std::variant<pair0, pair1, pair2, pair3, pair4, pair5>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}