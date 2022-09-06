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

#include "utilities/type_traits/tuple/has_type.hpp"
#include <catch2/catch.hpp>

using namespace utilities::type_traits::tuple;

using search_tuple = std::tuple<int, double, char>;

TEMPLATE_LIST_TEST_CASE("has_type: empty tuple", "", search_tuple) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(detail_::has_type<search_type, std::tuple<>>::value);
}

TEMPLATE_LIST_TEST_CASE("has_type: tuple w/o type", "", search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, bool, unsigned long>;
    STATIC_REQUIRE_FALSE(detail_::has_type<search_type, tuple_type>::value);
}

TEMPLATE_LIST_TEST_CASE("has_type: tuple w type", "", search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, int, double, char>;
    STATIC_REQUIRE(detail_::has_type<search_type, tuple_type>::value);
}

TEMPLATE_LIST_TEST_CASE("has_type_v: tuple w/o type", "", search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, bool, unsigned long>;
    STATIC_REQUIRE_FALSE(has_type_v<search_type, tuple_type>);
}

TEMPLATE_LIST_TEST_CASE("has_type_v: tuple w type", "", search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, int, double, char>;
    STATIC_REQUIRE(has_type_v<search_type, tuple_type>);
}