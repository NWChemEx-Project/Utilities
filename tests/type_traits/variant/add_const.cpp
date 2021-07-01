#include "utilities/type_traits/variant/add_const.hpp"
#include <catch2/catch.hpp>

using namespace utilities::type_traits;

TEST_CASE("AddConst") {
    SECTION("No types") {
        using input  = std::variant<>;
        using corr   = std::variant<>;
        using result = typename variant::detail_::AddConst<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }

    SECTION("One type") {
        using input  = std::variant<int>;
        using corr   = std::variant<const int>;
        using result = typename variant::detail_::AddConst<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }

    SECTION("Two types") {
        using input  = std::variant<int, float>;
        using corr   = std::variant<const int, const float>;
        using result = typename variant::detail_::AddConst<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
}

TEST_CASE("add_const_t") {
    SECTION("No types") {
        using input  = std::variant<>;
        using corr   = std::variant<>;
        using result = variant::add_const_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }

    SECTION("One type") {
        using input  = std::variant<int>;
        using corr   = std::variant<const int>;
        using result = variant::add_const_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }

    SECTION("Two types") {
        using input  = std::variant<int, float>;
        using corr   = std::variant<const int, const float>;
        using result = variant::add_const_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
}
