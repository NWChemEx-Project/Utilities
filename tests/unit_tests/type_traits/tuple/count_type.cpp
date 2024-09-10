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

#include "../../catch.hpp"
#include "utilities/type_traits/tuple/count_type.hpp"

using namespace utilities::type_traits::tuple;

using search_tuple = std::tuple<int, double, char>;

TEMPLATE_LIST_TEST_CASE("count_type: empty tuple", "", search_tuple) {
    using search_type = TestType;
    STATIC_REQUIRE(detail_::count_type<search_type, std::tuple<>>::value ==
                   0ul);
}

TEMPLATE_LIST_TEST_CASE("count_type: tuple w/o type", "", search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, bool, unsigned long>;
    STATIC_REQUIRE(detail_::count_type<search_type, tuple_type>::value == 0ul);
}

TEMPLATE_LIST_TEST_CASE("count_type: tuple w single instance of type", "",
                        search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, int, double, char>;
    STATIC_REQUIRE(detail_::count_type<search_type, tuple_type>::value == 1ul);
}

TEMPLATE_LIST_TEST_CASE("count_type: tuple w 2 instances of type", "",
                        search_tuple) {
    using search_type = TestType;
    using tuple_type =
      std::tuple<float, int, int, double, char, unsigned long, char, double>;
    STATIC_REQUIRE(detail_::count_type<search_type, tuple_type>::value == 2ul);
}

TEMPLATE_LIST_TEST_CASE("count_type_v: tuple w/o type", "", search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, bool, unsigned long>;
    STATIC_REQUIRE(count_type_v<search_type, tuple_type> == 0ul);
}

TEMPLATE_LIST_TEST_CASE("count_type_v: tuple w single instance of type", "",
                        search_tuple) {
    using search_type = TestType;
    using tuple_type  = std::tuple<float, int, double, char>;
    STATIC_REQUIRE(count_type_v<search_type, tuple_type> == 1ul);
}

TEMPLATE_LIST_TEST_CASE("count_type_v: tuple w 2 instances of type", "",
                        search_tuple) {
    using search_type = TestType;
    using tuple_type =
      std::tuple<float, int, int, double, char, unsigned long, char, double>;
    STATIC_REQUIRE(count_type_v<search_type, tuple_type> == 2ul);
}
