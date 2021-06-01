#include "utilities/type_traits/tuple/variant.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities;
using namespace utilities::detail_;

TEST_CASE("tuple_to_variant") {
    using tuple_type = std::tuple<int, double, float>;
    using type       = typename tuple_to_variant<tuple_type>::type;
    using corr       = std::variant<int, double, float>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("tuple_to_variant_t") {
    using tuple_type = std::tuple<int, double, float>;
    using type       = tuple_to_variant_t<tuple_type>;
    using corr       = std::variant<int, double, float>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}