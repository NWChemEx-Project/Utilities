/*
 * Copyright 2024 NWChemEx-Project
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

#include "test_dsl.hpp"
#include <utilities/dsl/divide.hpp>

/* Testing Strategy.
 *
 * Divide is basically a strong type, we just test it can be constructed with
 * all of the possible const-variations.
 */

TEMPLATE_LIST_TEST_CASE("Divide", "", test_utilities::binary_types) {
    using lhs_type = std::tuple_element_t<0, TestType>;
    using rhs_type = std::tuple_element_t<1, TestType>;

    auto values     = test_utilities::binary_values();
    auto [lhs, rhs] = std::get<TestType>(values);

    utilities::dsl::Divide<lhs_type, rhs_type> a_xx(lhs, rhs);
    utilities::dsl::Divide<const lhs_type, rhs_type> a_cx(lhs, rhs);
    utilities::dsl::Divide<lhs_type, const rhs_type> a_xc(lhs, rhs);
    utilities::dsl::Divide<const lhs_type, const rhs_type> a_cc(lhs, rhs);

    SECTION("CTors") {
        REQUIRE(a_xx.lhs() == lhs);
        REQUIRE(a_xx.rhs() == rhs);

        REQUIRE(a_cx.lhs() == lhs);
        REQUIRE(a_cx.rhs() == rhs);

        REQUIRE(a_xc.lhs() == lhs);
        REQUIRE(a_xc.rhs() == rhs);

        REQUIRE(a_cc.lhs() == lhs);
        REQUIRE(a_cc.rhs() == rhs);
    }
}