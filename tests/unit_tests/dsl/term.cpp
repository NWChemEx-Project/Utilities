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
#include <utilities/dsl/dsl.hpp>

/* Testing Strategy.
 *
 * The Term class ensures that it is possible to combine DSL objects without
 * having to overload operator+, operator*, etc. for every object type in the
 * DSL. Thus to test it, it suffices to have a DSL object that derives from
 * Term and then to operate on that object. For this purpose we define an object
 * "a" which represents adding 4 and 2. We then add, multiply, etc. to "a" the
 * integer 42.
 */

TEST_CASE("Term") {
    int four(4), two(2), forty_two(42);
    using lhs_type = utilities::dsl::Add<int, int>;
    lhs_type a(four, two);

    SECTION("operator+") {
        utilities::dsl::Add<lhs_type, int> corr(a, forty_two);
        REQUIRE((a + forty_two) == corr);
    }

    SECTION("operator-") {
        utilities::dsl::Subtract<lhs_type, int> corr(a, forty_two);
        REQUIRE((a - forty_two) == corr);
    }

    SECTION("operator*") {
        utilities::dsl::Multiply<lhs_type, int> corr(a, forty_two);
        REQUIRE((a * forty_two) == corr);
    }

    SECTION("operator/") {
        utilities::dsl::Divide<lhs_type, int> corr(a, forty_two);
        REQUIRE((a / forty_two) == corr);
    }
}