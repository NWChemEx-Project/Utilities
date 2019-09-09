#include <catch2/catch.hpp>
#include <utilities/type_traits/tuple_cat_type.hpp>

using namespace utilities;

TEST_CASE("tuple_cat_t") {
    using tuple1 = std::tuple<int, double>;
    using tuple2 = std::tuple<char, std::tuple<int>>;
    SECTION("No tuples") {
        STATIC_REQUIRE(std::is_same_v<std::tuple<>, tuple_cat_t<>>);
    }
    SECTION("one tuple") {
        STATIC_REQUIRE(std::is_same_v<tuple1, tuple_cat_t<tuple1>>);
    }
    SECTION("two tuples") {
        using test_tuple = tuple_cat_t<tuple1, tuple2>;
        using corr_tuple = std::tuple<int, double, char, std::tuple<int>>;
        STATIC_REQUIRE(std::is_same_v<test_tuple, corr_tuple>);
    }
}
