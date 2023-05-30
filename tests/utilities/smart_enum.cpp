/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <catch2/catch.hpp>
#include <sstream>
#include <utilities/smart_enum.hpp>

using namespace utilities;

DECLARE_SmartEnum(Fruit, apple, pear, banana, grape);

/* I'm not sure if it's a compiler bug with Intel, but it won't let me use
 * the enums as constexpr despite them being initialized as such...
 */

TEST_CASE("SmartEnum") {
    /* Intel no like-y
    constexpr Fruit fuji = Fruit::apple;
    constexpr Fruit gala = Fruit::apple;
    constexpr Fruit bosc = Fruit::pear;
    static_assert(fuji == gala);
    static_assert(fuji != bosc);
    static_assert(fuji < bosc);
    static_assert(fuji <= gala);
    static_assert(bosc > fuji);
    static_assert(fuji >= gala);
    */

    Fruit fuji = Fruit::apple;
    Fruit gala = Fruit::apple;
    Fruit bosc = Fruit::pear;

    REQUIRE(fuji == gala);
    REQUIRE(fuji != bosc);

    // This uses the fact that enums are sorted alphabetically by the instance

    REQUIRE(fuji < bosc);
    REQUIRE(fuji <= gala);
    REQUIRE(bosc > fuji);
    REQUIRE(fuji >= gala);

    std::stringstream ss;
    ss << fuji;

    std::stringstream corr_ss;
    corr_ss << "apple";

    REQUIRE(ss.str() == corr_ss.str());
}
