#include <catch2/catch.hpp>
#include <utilities/type_traits/tuple/is_tuple.hpp>

using namespace utilities::type_traits::tuple;

TEST_CASE("IsTuple") {
    STATIC_REQUIRE(is_tuple_v<std::tuple<int, int>>);
    STATIC_REQUIRE(is_tuple_v<std::pair<int, int>>);
    STATIC_REQUIRE_FALSE(is_tuple_v<int>);
}
