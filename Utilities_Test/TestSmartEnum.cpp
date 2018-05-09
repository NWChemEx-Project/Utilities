#include <Utilities/SmartEnum.hpp>
#include <catch/catch.hpp>
#include <sstream>

using namespace Utilities;

DECLARE_SmartEnum(Fruit, apple, pear);

TEST_CASE("SmartEnum") {
    constexpr Fruit fuji = Fruit::apple;
    constexpr Fruit gala = Fruit::apple;
    constexpr Fruit bosc = Fruit::pear;

    static_assert(fuji == gala);
    REQUIRE(fuji == gala);
    static_assert(fuji != bosc);
    REQUIRE(fuji != bosc);

    // This uses the fact that enums are sorted alphabetically by the instance
    static_assert(fuji < bosc);
    REQUIRE(fuji < bosc);
    static_assert(fuji <= gala);
    REQUIRE(fuji <= gala);
    static_assert(bosc > fuji);
    REQUIRE(bosc > fuji);
    static_assert(fuji >= gala);
    REQUIRE(fuji >= gala);

    std::stringstream ss;
    ss << fuji;

    std::stringstream corr_ss;
    corr_ss << "apple";

    REQUIRE(ss.str() == corr_ss.str());
}
