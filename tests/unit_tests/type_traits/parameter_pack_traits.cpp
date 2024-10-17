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

#include "../catch.hpp"
#include "utilities/type_traits/parameter_pack_traits.hpp"
#include <iostream>

using namespace utilities::type_traits;

using search_types = std::tuple<int, double, char>;

struct Base {};
struct Derived : public Base {};

// Contains Type
TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type: empty parameter pack",
                        "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(parameter_pack_contains_type<search_type>::value);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type: parameter pack w/o type",
                        "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(parameter_pack_contains_type<search_type, float, bool,
                                                      unsigned long>::value);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type: parameter pack w type",
                        "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(parameter_pack_contains_type<search_type, float, int, double,
                                                char>::value);
}

TEMPLATE_LIST_TEST_CASE(
  "parameter_pack_contains_type_v: parameter pack w/o type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(
      parameter_pack_contains_type_v<search_type, float, bool, unsigned long>);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type_v: parameter pack w type",
                        "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(
      parameter_pack_contains_type_v<search_type, float, int, double, char>);
}

// Contains derived type
TEST_CASE("parameter_pack_contains_derived_type: empty parameter pack", "") {
    STATIC_REQUIRE_FALSE(parameter_pack_contains_derived_type<Base>::value);
}
TEST_CASE(
  "parameter_pack_contains_derived_type: parameter pack w/o Derived instance",
  "") {
    STATIC_REQUIRE_FALSE(
      parameter_pack_contains_derived_type<Base, int, double>::value);
}
TEST_CASE("parameter_pack_contains_derived_type: Base instance parameter pack",
          "") {
    STATIC_REQUIRE(
      parameter_pack_contains_derived_type<Base, Base, int>::value);
}
TEST_CASE(
  "parameter_pack_contains_derived_type: Derived instance parameter pack", "") {
    STATIC_REQUIRE(
      parameter_pack_contains_derived_type<Base, Derived, int>::value);
}

TEST_CASE("parameter_pack_contains_derived_type_v: empty parameter pack", "") {
    STATIC_REQUIRE_FALSE(parameter_pack_contains_derived_type_v<Base>);
}
TEST_CASE(
  "parameter_pack_contains_derived_type_v: parameter pack w/o Derived instance",
  "") {
    STATIC_REQUIRE_FALSE(
      parameter_pack_contains_derived_type_v<Base, int, double>);
}
TEST_CASE(
  "parameter_pack_contains_derived_type_v: Base instance parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_contains_derived_type_v<Base, Base, int>);
}
TEST_CASE(
  "parameter_pack_contains_derived_type_v: Derived instance parameter pack",
  "") {
    STATIC_REQUIRE(parameter_pack_contains_derived_type_v<Base, Derived, int>);
}

// Count Type
TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type: empty parameter pack", "",
                        search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(parameter_pack_count_type<search_type>::value == 0ul);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type: parameter pack w/o type",
                        "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(parameter_pack_count_type<search_type, float, bool,
                                             unsigned long>::value == 0ul);
}

TEMPLATE_LIST_TEST_CASE(
  "parameter_pack_count_type: parameter pack w single instance of type", "",
  search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(
      parameter_pack_count_type<search_type, float, int, double, char>::value ==
      1ul);
}

TEMPLATE_LIST_TEST_CASE(
  "parameter_pack_count_type: parameter pack w 2 instances of type", "",
  search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(
      parameter_pack_count_type<search_type, float, int, int, double, char,
                                unsigned long, char, double>::value == 2ul);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type_v: parameter pack w/o type",
                        "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(
      parameter_pack_count_type_v<search_type, float, bool, unsigned long> ==
      0ul);
}

TEMPLATE_LIST_TEST_CASE(
  "parameter_pack_count_type_v: parameter pack w single instance of type", "",
  search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(
      parameter_pack_count_type_v<search_type, float, int, double, char> ==
      1ul);
}

TEMPLATE_LIST_TEST_CASE(
  "parameter_pack_count_type_v: parameter pack w 2 instances of type", "",
  search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(
      parameter_pack_count_type_v<search_type, float, int, int, double, char,
                                  unsigned long, char, double> == 2ul);
}

// Count derived type
TEST_CASE("parameter_pack_count_derived_type: empty parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base>::value == 0ul);
}
TEST_CASE(
  "parameter_pack_count_derived_type: parameter pack w/o derived instance",
  "") {
    STATIC_REQUIRE(
      parameter_pack_count_derived_type<Base, int, double>::value == 0ul);
}
TEST_CASE("parameter_pack_count_derived_type: parameter pack w/ base instance",
          "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base, int, Base>::value ==
                   1ul);
}
TEST_CASE(
  "parameter_pack_count_derived_type: parameter pack w/ derived instance", "") {
    STATIC_REQUIRE(
      parameter_pack_count_derived_type<Base, int, Derived>::value == 1ul);
}
TEST_CASE(
  "parameter_pack_count_derived_type: parameter pack w/ base+derived instance",
  "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base, int, Derived, double,
                                                     Base>::value == 2ul);
}

TEST_CASE("parameter_pack_count_derived_type_v: empty parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base> == 0ul);
}
TEST_CASE(
  "parameter_pack_count_derived_type_v: parameter pack w/o derived instance",
  "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base, int, double> ==
                   0ul);
}
TEST_CASE(
  "parameter_pack_count_derived_type_v: parameter pack w/ base instance", "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base, int, Base> == 1ul);
}
TEST_CASE(
  "parameter_pack_count_derived_type_v: parameter pack w/ derived instance",
  "") {
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base, int, Derived> ==
                   1ul);
}
TEST_CASE("parameter_pack_count_derived_type_v: parameter pack w/ base+derived "
          "instance",
          "") {
    STATIC_REQUIRE(
      parameter_pack_count_derived_type_v<Base, int, Derived, double, Base> ==
      2ul);
}

TEST_CASE("remove_duplicate_types") {
    SECTION("No duplicate types") {
        using corr_type = std::tuple<int, double, char>;
        using type2test =
          remove_duplicate_types<std::tuple<>, int, double, char>;
        STATIC_REQUIRE(std::is_same_v<corr_type, typename type2test::type>);
    }
    SECTION("One duplicate types") {
        using corr_type = std::tuple<int, char>;
        using type2test = remove_duplicate_types<std::tuple<>, int, int, char>;
        STATIC_REQUIRE(std::is_same_v<corr_type, typename type2test::type>);
    }
    SECTION("One duplicate type (2x)") {
        using corr_type = std::tuple<int>;
        using type2test = remove_duplicate_types<std::tuple<>, int, int, int>;
        STATIC_REQUIRE(std::is_same_v<corr_type, typename type2test::type>);
    }
    SECTION("Two duplicate types") {
        using corr_type = std::tuple<int, double>;
        using type2test =
          remove_duplicate_types<std::tuple<>, int, double, int, double>;
        STATIC_REQUIRE(std::is_same_v<corr_type, typename type2test::type>);
    }
    SECTION("No types") {
        using corr_type = std::tuple<>;
        using type2test = remove_duplicate_types<std::tuple<>>;
        STATIC_REQUIRE(std::is_same_v<corr_type, typename type2test::type>);
    }
}

TEST_CASE("unique_types_t") {
    // This is just an alias for remove_duplicates_types::types, which we know
    // works so just spot check it
    SECTION("No duplicate types") {
        using corr_type = std::tuple<int, double>;
        STATIC_REQUIRE(std::is_same_v<corr_type, unique_types_t<int, double>>);
    }
    SECTION("Duplicate types") {
        using corr_type = std::tuple<int>;
        STATIC_REQUIRE(std::is_same_v<corr_type, unique_types_t<int, int>>);
    }
    SECTION("No types") {
        using corr_type = std::tuple<>;
        STATIC_REQUIRE(std::is_same_v<corr_type, unique_types_t<>>);
    }
}