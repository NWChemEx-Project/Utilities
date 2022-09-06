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
#include <utilities/containers/own_or_borrow.hpp>

using namespace utilities;

TEST_CASE("OwnOrBorrow: Default ctor") {
    OwnOrBorrow<int> o;
    REQUIRE(o.value() == 0);
}

TEST_CASE("OwnOrBorrow: Value ctor") {
    OwnOrBorrow<int> o(2);
    REQUIRE(o.value() == 2);
}

TEST_CASE("OwnOrBorrow: Alias ctor") {
    int two{2};
    OwnOrBorrow<int> o(&two);
    REQUIRE(&o.value() == &two);
}

TEST_CASE("OwnOrBorrow : Move ctor") {
    SECTION("move from non-aliasing") {
        OwnOrBorrow<int> o(2);
        OwnOrBorrow<int> o2(std::move(o));
        SECTION("Value") { REQUIRE(o2.value() == 2); }
    }
    SECTION("Move from aliasing") {
        int two{2};
        OwnOrBorrow<int> o(&two);
        OwnOrBorrow<int> o2(std::move(o));
        SECTION("Value") { REQUIRE(o2.value() == 2); }
        SECTION("Alias") { REQUIRE(&o2.value() == &two); }
    }
}

TEST_CASE("OwnOrBorrow : Move assignment") {
    SECTION("move from non-aliasing") {
        OwnOrBorrow<int> o(2);
        OwnOrBorrow<int> o2;
        auto po2 = &(o2 = std::move(o));
        SECTION("Value") { REQUIRE(o2.value() == 2); }
        SECTION("Returns this") { REQUIRE(po2 == &o2); }
    }
    SECTION("Move from aliasing") {
        int two{2};
        OwnOrBorrow<int> o(&two);
        OwnOrBorrow<int> o2;
        auto po2 = &(o2 = std::move(o));
        SECTION("Value") { REQUIRE(o2.value() == 2); }
        SECTION("Alias") { REQUIRE(&o2.value() == &two); }
        SECTION("Returns this") { REQUIRE(po2 == &o2); }
    }
}

TEST_CASE("OwnOrBorrow: Assign value") {
    SECTION("Storing value") {
        OwnOrBorrow<int> o;
        o = 2;
        REQUIRE(o.value() == 2);
    }
    SECTION("Storing alias") {
        int two{2};
        OwnOrBorrow<int> o(&two);
        o = 3;
        REQUIRE(o.value() == 3);
    }
}

TEST_CASE("OwnOrBorrow: Assign alias") {
    int three{3};
    SECTION("Storing value") {
        OwnOrBorrow<int> o;
        o = &three;
        REQUIRE(&o.value() == &three);
    }
    SECTION("Storing alias") {
        int two{2};
        OwnOrBorrow<int> o(&two);
        o = &three;
        REQUIRE(&o.value() == &three);
    }
}

TEST_CASE("OwnOrBorrow: value()") {
    SECTION("Storing value") {
        OwnOrBorrow<int> o(3);
        REQUIRE(o.value() == 3);
    }
    SECTION("Storing alias") {
        int two{2};
        OwnOrBorrow<int> o(&two);
        REQUIRE(&o.value() == &two);
    }
}

TEST_CASE("OwnOrBorrow: value() const") {
    SECTION("Storing value") {
        const OwnOrBorrow<int> o(3);
        using rv_t = std::remove_reference_t<decltype(o.value())>;
        STATIC_REQUIRE(std::is_same_v<rv_t, const int>);
        REQUIRE(o.value() == 3);
    }
    SECTION("Storing alias") {
        int two{2};
        const OwnOrBorrow<int> o(&two);
        using rv_t = std::remove_reference_t<decltype(o.value())>;
        STATIC_REQUIRE(std::is_same_v<rv_t, const int>);
        REQUIRE(&o.value() == &two);
    }
}

TEST_CASE("Comparisons") {
    int two{2}, three{3};
    OwnOrBorrow<int> zero;
    OwnOrBorrow<int> two_own(2);
    OwnOrBorrow<int> two_alias(&two);
    OwnOrBorrow<int> three_alias(&three);

    REQUIRE(two_own == two_own);
    REQUIRE(two_alias == two_alias);
    REQUIRE(two_own == OwnOrBorrow<int>(2));
    REQUIRE(two_own != zero);
    REQUIRE(two_own == two_alias);
    REQUIRE(two_own != three_alias);
}
