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

#include "../test_helpers.hpp"
#include <utilities/dsl/leaf.hpp>

using namespace utilities::dsl;

/* Testing notes:
 *
 * We can only actually make instances of QualifiedLeafHolder as the base
 * classes are abstract.
 */

using types_to_test = std::tuple<std::string, std::vector<int>>;

TEMPLATE_LIST_TEST_CASE("Leaf", "", types_to_test) {
    using type     = TestType;
    using ctype    = const type;
    using ref      = type&;
    using cref     = const type&;
    using pointer  = type*;
    using cpointer = const type*;

    types_to_test values{"Hello world", std::vector<int>{1, 2, 3}};
    types_to_test other_values{"Hello sky", std::vector<int>{3, 2, 1}};

    ref value0         = std::get<type>(values);
    cref cvalue0       = std::get<type>(values);
    ref value1         = std::get<type>(other_values);
    cref cvalue1       = std::get<type>(other_values);
    type copy_value1   = std::get<type>(other_values);
    ctype ccopy_value1 = std::get<type>(other_values);

    Leaf defaulted;
    auto wrap_ref    = make_leaf(value0);
    auto wrap_cref   = make_leaf(cvalue0);
    auto wrap_value  = make_leaf(type(value0));
    auto wrap_cvalue = make_leaf(ctype(value0));
    auto wrap_rref   = make_leaf(std::move(copy_value1));
    auto wrap_crref  = make_leaf(std::move(ccopy_value1));

    SECTION("Ctors") {
        SECTION("Default") { REQUIRE_FALSE(defaulted.has_value()); }
        SECTION("Value") {
            REQUIRE(wrap_ref.has_value());
            REQUIRE(wrap_cref.has_value());
            REQUIRE(wrap_value.has_value());
            REQUIRE(wrap_cvalue.has_value());
            REQUIRE(wrap_rref.has_value());
            REQUIRE(wrap_crref.has_value());
        }
        test_utilities::test_copy_and_move(defaulted, wrap_ref, wrap_cref,
                                           wrap_value, wrap_cvalue, wrap_rref,
                                           wrap_crref);
    }

    SECTION("has_value") {
        REQUIRE_FALSE(defaulted.has_value());
        REQUIRE(wrap_ref.has_value());
        REQUIRE(wrap_cref.has_value());
        REQUIRE(wrap_value.has_value());
        REQUIRE(wrap_cvalue.has_value());
        REQUIRE(wrap_rref.has_value());
        REQUIRE(wrap_crref.has_value());
    }

    SECTION("value()") {
        SECTION("Return type") {
            using return_type = decltype(wrap_ref.template value<type>());
            STATIC_REQUIRE(std::is_same_v<return_type, ref>);

            using const_return = decltype(wrap_ref.template value<ctype>());
            STATIC_REQUIRE(std::is_same_v<const_return, cref>);
        }

        ref ref0   = wrap_ref.template value<type>();
        cref cref0 = wrap_ref.template value<ctype>();
        REQUIRE(&ref0 == &cref0);
        REQUIRE(ref0 == value0);
        REQUIRE(&ref0 == &value0);

        cref ref1 = wrap_cref.template value<ctype>();
        REQUIRE(ref1 == cvalue0);
        REQUIRE(&ref1 == &cvalue0);

        ref ref2   = wrap_value.template value<type>();
        cref cref2 = wrap_value.template value<ctype>();
        REQUIRE(ref2 == value0);
        REQUIRE(cref2 == value0);

        cref ref3 = wrap_cvalue.template value<ctype>();
        REQUIRE(ref3 == cvalue0);

        ref ref4   = wrap_rref.template value<type>();
        cref cref4 = wrap_rref.template value<ctype>();
        REQUIRE(ref4 == value1);
        REQUIRE(cref4 == value1);

        cref ref5 = wrap_crref.template value<ctype>();
        REQUIRE(ref5 == cvalue1);

        using except_t = std::runtime_error;
        REQUIRE_THROWS_AS(defaulted.template value<type>(), except_t);
        REQUIRE_THROWS_AS(wrap_cref.template value<type>(), except_t);
    }

    SECTION("value() const") {
        const auto& cwrap_ref = std::as_const(wrap_ref);
        SECTION("Return type") {
            using return_type = decltype(cwrap_ref.template value<type>());
            STATIC_REQUIRE(std::is_same_v<return_type, cref>);

            using const_return = decltype(cwrap_ref.template value<ctype>());
            STATIC_REQUIRE(std::is_same_v<const_return, cref>);
        }

        cref ref0  = cwrap_ref.template value<type>();
        cref cref0 = cwrap_ref.template value<ctype>();
        REQUIRE(&ref0 == &cref0);
        REQUIRE(ref0 == value0);
        REQUIRE(&ref0 == &value0);

        cref ref1 = wrap_cref.template value<ctype>();
        REQUIRE(ref1 == cvalue0);
        REQUIRE(&ref1 == &cvalue0);

        cref ref2  = wrap_value.template value<type>();
        cref cref2 = wrap_value.template value<ctype>();
        REQUIRE(ref2 == value0);
        REQUIRE(cref2 == value0);

        cref ref3 = wrap_cvalue.template value<ctype>();
        REQUIRE(ref3 == cvalue0);

        cref ref4  = wrap_rref.template value<type>();
        cref cref4 = wrap_rref.template value<ctype>();
        REQUIRE(ref4 == value1);
        REQUIRE(cref4 == value1);

        cref ref5 = wrap_crref.template value<ctype>();
        REQUIRE(ref5 == cvalue1);

        using except_t = std::runtime_error;
        REQUIRE_THROWS_AS(defaulted.template value<type>(), except_t);
        REQUIRE_THROWS_AS(wrap_cref.template value<type>(), except_t);
    }

    SECTION("reset") {
        REQUIRE(wrap_ref.has_value());
        wrap_ref.reset();
        REQUIRE_FALSE(wrap_ref.has_value());
    }

    SECTION("swap") {
        Leaf copy_wrap_ref(wrap_ref);
        Leaf copy_wrap_rref(wrap_rref);
        wrap_ref.swap(wrap_rref);
        REQUIRE(wrap_ref == copy_wrap_rref);
        REQUIRE(wrap_rref == copy_wrap_ref);
    }

    SECTION("operator==") {
        // Default == default
        REQUIRE(defaulted == Leaf{});

        // Default == nullptr initialized
        REQUIRE(defaulted == Leaf(nullptr));

        // Default != non-null value
        REQUIRE_FALSE(defaulted == wrap_ref);

        // Wrapping references to the same value
        REQUIRE(wrap_ref == make_leaf(value0));

        // Wrapping references to the same value, but different const-ness
        REQUIRE(wrap_ref == wrap_cref);

        // Wrapping different objects, but same value
        REQUIRE(wrap_ref == wrap_value);

        // Wrapping references to different values
        REQUIRE_FALSE(wrap_ref == wrap_rref);
    }

    SECTION("operator!=") {
        // Just negates operator== so spot checking is fine.
        REQUIRE(defaulted != wrap_ref);
        REQUIRE_FALSE(wrap_ref != wrap_cref);
    }

    SECTION("unwrap_leaf") {
        ref ref0   = unwrap_leaf<type>(wrap_ref);
        cref cref0 = unwrap_leaf<ctype>(wrap_ref);
        REQUIRE(&ref0 == &cref0);
        REQUIRE(ref0 == value0);
        REQUIRE(&ref0 == &value0);

        cref ref1 = unwrap_leaf<ctype>(wrap_cref);
        REQUIRE(ref1 == cvalue0);
        REQUIRE(&ref1 == &cvalue0);

        ref ref2   = unwrap_leaf<type>(wrap_value);
        cref cref2 = unwrap_leaf<ctype>(wrap_value);
        REQUIRE(ref2 == value0);
        REQUIRE(cref2 == value0);

        cref ref3 = unwrap_leaf<ctype>(wrap_cvalue);
        REQUIRE(ref3 == cvalue0);

        ref ref4   = unwrap_leaf<type>(wrap_rref);
        cref cref4 = unwrap_leaf<ctype>(wrap_rref);
        REQUIRE(ref4 == value1);
        REQUIRE(cref4 == value1);

        cref ref5 = unwrap_leaf<ctype>(wrap_crref);
        REQUIRE(ref5 == cvalue1);

        using except_t = std::runtime_error;
        REQUIRE_THROWS_AS(unwrap_leaf<type>(defaulted), except_t);
        REQUIRE_THROWS_AS(unwrap_leaf<type>(wrap_cref), except_t);
    }
}