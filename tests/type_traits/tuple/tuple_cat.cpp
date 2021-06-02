#include "utilities/type_traits/tuple/tuple_cat.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities::type_traits::tuple;

TEST_CASE("tuple_cat_t") {
    SECTION("No tuples") {
        STATIC_REQUIRE(std::is_same_v<tuple_cat_t<>, std::tuple<>>);
    }

    SECTION("One tuple") {
        using tuple_type = std::tuple<int, double, char>;
        STATIC_REQUIRE(std::is_same_v<tuple_cat_t<tuple_type>, tuple_type>);
    }

    SECTION("Two tuples") {
        using tuple0_t = std::tuple<int, double>;
        using tuple1_t = std::tuple<char, bool>;
        using corr_t   = std::tuple<int, double, char, bool>;
        STATIC_REQUIRE(std::is_same_v<tuple_cat_t<tuple0_t, tuple1_t>, corr_t>);
    }
}