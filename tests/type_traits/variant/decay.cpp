#include "utilities/type_traits/variant/decay.hpp"
#include <catch2/catch.hpp>

using namespace utilities::type_traits;

TEST_CASE("Decay"){
    SECTION("Empty"){
        using input  = std::variant<>;
        using corr   = std::variant<>;
        using result = typename variant::detail_::Decay<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("One type"){
        using input  = std::variant<int&>;
        using corr   = std::variant<int>;
        using result = typename variant::detail_::Decay<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("Two types"){
        using input  = std::variant<int&, const float>;
        using corr   = std::variant<int, float>;
        using result = typename variant::detail_::Decay<input>::type;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
}

TEST_CASE("decay_t"){
    SECTION("Empty"){
        using input  = std::variant<>;
        using corr   = std::variant<>;
        using result = variant::decay_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("One type"){
        using input  = std::variant<int&>;
        using corr   = std::variant<int>;
        using result = variant::decay_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
    SECTION("Two types"){
        using input  = std::variant<int&, const float>;
        using corr   = std::variant<int, float>;
        using result = variant::decay_t<input>;
        STATIC_REQUIRE(std::is_same_v<corr, result>);
    }
}
