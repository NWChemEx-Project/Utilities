#include "utilities/type_traits/variant/cat.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities::type_traits::variant;

TEST_CASE("variant::cat_t") {
    SECTION("No variant") {
        STATIC_REQUIRE(std::is_same_v<cat_t<>, std::variant<>>);
    }

    SECTION("One Variant") {
        using variant_type = std::variant<int, double, char>;
        STATIC_REQUIRE(std::is_same_v<cat_t<variant_type>, variant_type>);
    }

    SECTION("Two Variants") {
        using variant0_t = std::variant<int, double>;
        using variant1_t = std::variant<char, bool>;
        using corr_t     = std::variant<int, double, char, bool>;
        STATIC_REQUIRE(std::is_same_v<cat_t<variant0_t, variant1_t>, corr_t>);
    }
}