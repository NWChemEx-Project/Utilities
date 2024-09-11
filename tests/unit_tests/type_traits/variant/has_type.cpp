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
#include "utilities/type_traits/variant/has_type.hpp"

using namespace utilities::type_traits::variant;

using search_variant = std::variant<int, double, char>;

TEMPLATE_LIST_TEST_CASE("has_type_v: variant w/o type", "", search_variant) {
    using search_type  = TestType;
    using variant_type = std::variant<float, bool, unsigned long>;
    STATIC_REQUIRE_FALSE(has_type_v<search_type, variant_type>);
}

TEMPLATE_LIST_TEST_CASE("has_type_v: variant w type", "", search_variant) {
    using search_type  = TestType;
    using variant_type = std::variant<float, int, double, char>;
    STATIC_REQUIRE(has_type_v<search_type, variant_type>);
}