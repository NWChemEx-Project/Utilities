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

#include "../../test_helpers.hpp"
#include <utilities/dsl/detail_/leaf_holder.hpp>

using namespace utilities::dsl;
using std::is_same_v;

/* Testing notes:
 *
 * We can only actually make instances of QualifiedLeafHolder as the base
 * classes are abstract.
 */

using types_to_test = std::tuple<std::string, std::vector<int>>;

TEMPLATE_LIST_TEST_CASE("QualifiedLeafHolder", "", types_to_test) {
    using type     = TestType;
    using ctype    = const type;
    using ref      = type&;
    using cref     = const type&;
    using pointer  = type*;
    using cpointer = const type*;

    using value_holder   = detail_::QualifiedLeafHolder<type>;
    using cvalue_holder  = detail_::QualifiedLeafHolder<ctype>;
    using ref_holder     = detail_::QualifiedLeafHolder<ref>;
    using cref_holder    = detail_::QualifiedLeafHolder<cref>;
    using rvalue_holder  = detail_::QualifiedLeafHolder<type&&>;
    using const_rvalue_h = detail_::QualifiedLeafHolder<const type&&>;

    types_to_test values{"Hello world", std::vector<int>{1, 2, 3}};
    types_to_test other_values{"Hello sky", std::vector<int>{3, 2, 1}};

    // Grab the values to copy/alias and make copies to move
    ref value       = std::get<type>(values);
    ref other_value = std::get<type>(other_values);
    cref cvalue     = std::get<type>(values);
    type value_copy(value), value_copy2(value);
    auto* p_value_copy        = value_copy.data();
    const auto* p_value_copy2 = value_copy2.data();

    value_holder wrap_value(value);
    cvalue_holder wrap_cvalue(value);
    ref_holder wrap_ref(value);
    cref_holder wrap_cref(value);
    rvalue_holder wrap_rref(std::move(value_copy));
    const_rvalue_h wrap_crref(std::move(value_copy2));

    SECTION("Template meta-programming") {
        SECTION("is_const") {
            STATIC_REQUIRE_FALSE(value_holder::is_const);
            STATIC_REQUIRE(cvalue_holder::is_const);
            STATIC_REQUIRE_FALSE(ref_holder::is_const);
            STATIC_REQUIRE(cref_holder::is_const);
            STATIC_REQUIRE_FALSE(rvalue_holder::is_const);
            STATIC_REQUIRE(const_rvalue_h::is_const);
        }
        SECTION("value_type") {
            STATIC_REQUIRE(is_same_v<typename value_holder::value_type, type>);
            STATIC_REQUIRE(is_same_v<typename cvalue_holder::value_type, type>);
            STATIC_REQUIRE(is_same_v<typename ref_holder::value_type, type>);
            STATIC_REQUIRE(is_same_v<typename cref_holder::value_type, type>);
            STATIC_REQUIRE(is_same_v<typename rvalue_holder::value_type, type>);
            STATIC_REQUIRE(
              is_same_v<typename const_rvalue_h::value_type, type>);
        }
        SECTION("const_reference") {
            STATIC_REQUIRE(
              is_same_v<typename value_holder::const_reference, cref>);
            STATIC_REQUIRE(
              is_same_v<typename cvalue_holder::const_reference, cref>);
            STATIC_REQUIRE(
              is_same_v<typename ref_holder::const_reference, cref>);
            STATIC_REQUIRE(
              is_same_v<typename cref_holder::const_reference, cref>);
            STATIC_REQUIRE(
              is_same_v<typename rvalue_holder::const_reference, cref>);
            STATIC_REQUIRE(
              is_same_v<typename const_rvalue_h::const_reference, cref>);
        }
        SECTION("reference") {
            STATIC_REQUIRE(is_same_v<typename value_holder::reference, ref>);
            STATIC_REQUIRE(is_same_v<typename cvalue_holder::reference, cref>);
            STATIC_REQUIRE(is_same_v<typename ref_holder::reference, ref>);
            STATIC_REQUIRE(is_same_v<typename cref_holder::reference, cref>);
            STATIC_REQUIRE(is_same_v<typename rvalue_holder::reference, ref>);
            STATIC_REQUIRE(is_same_v<typename const_rvalue_h::reference, cref>);
        }
        SECTION("const_pointer") {
            STATIC_REQUIRE(
              is_same_v<typename value_holder::const_pointer, cpointer>);
            STATIC_REQUIRE(
              is_same_v<typename cvalue_holder::const_pointer, cpointer>);
            STATIC_REQUIRE(
              is_same_v<typename ref_holder::const_pointer, cpointer>);
            STATIC_REQUIRE(
              is_same_v<typename cref_holder::const_pointer, cpointer>);
            STATIC_REQUIRE(
              is_same_v<typename rvalue_holder::const_pointer, cpointer>);
            STATIC_REQUIRE(
              is_same_v<typename const_rvalue_h::const_pointer, cpointer>);
        }
        SECTION("pointer") {
            STATIC_REQUIRE(is_same_v<typename value_holder::pointer, pointer>);
            STATIC_REQUIRE(
              is_same_v<typename cvalue_holder::pointer, cpointer>);
            STATIC_REQUIRE(is_same_v<typename ref_holder::pointer, pointer>);
            STATIC_REQUIRE(is_same_v<typename cref_holder::pointer, cpointer>);
            STATIC_REQUIRE(is_same_v<typename rvalue_holder::pointer, pointer>);
            STATIC_REQUIRE(
              is_same_v<typename const_rvalue_h::pointer, cpointer>);
        }
        SECTION("holder_type") {
            STATIC_REQUIRE(is_same_v<typename value_holder::holder_type, type>);
            STATIC_REQUIRE(
              is_same_v<typename cvalue_holder::holder_type, type>);
            STATIC_REQUIRE(
              is_same_v<typename ref_holder::holder_type, pointer>);
            STATIC_REQUIRE(
              is_same_v<typename cref_holder::holder_type, cpointer>);
            STATIC_REQUIRE(
              is_same_v<typename rvalue_holder::holder_type, type>);
            STATIC_REQUIRE(
              is_same_v<typename const_rvalue_h::holder_type, type>);
        }
    }

    SECTION("Ctors") {
        SECTION("Value") {
            // By type / const type should copy
            REQUIRE(&value != &wrap_value.value());
            REQUIRE(value == wrap_value.value());

            REQUIRE(&cvalue != &wrap_cvalue.value());
            REQUIRE(cvalue == wrap_cvalue.value());

            // By ref/const ref should alias
            REQUIRE(&value == &wrap_ref.value());
            REQUIRE(&cvalue == &wrap_cref.value());

            // By rref should move
            REQUIRE(value == wrap_rref.value());
            // Apparently std::string copies for small strings
            if constexpr(std::is_same_v<type, std::vector<int>>) {
                REQUIRE(p_value_copy == wrap_rref.value().data());
            }

            // By crref should copy b.c. taking state changes the object
            REQUIRE(cvalue == wrap_crref.value());
            REQUIRE(p_value_copy2 != wrap_crref.value().data());
        }
        SECTION("Copy ctor") {
            test_utilities::test_copy_ctor(wrap_value);
            test_utilities::test_copy_ctor(wrap_cvalue);

            // Should be shallow if we are aliasing
            test_utilities::test_copy_ctor(wrap_ref);
            ref_holder wrap_ref_copy(wrap_ref);
            REQUIRE(&wrap_ref_copy.value() == &wrap_ref.value());

            test_utilities::test_copy_ctor(wrap_cref);
            cref_holder wrap_cref_copy(wrap_cref);
            REQUIRE(&wrap_cref_copy.value() == &wrap_cref.value());

            test_utilities::test_copy_ctor(wrap_rref);
            test_utilities::test_copy_ctor(wrap_crref);
        }
    }

    SECTION("clone") {
        auto pwrap_value = wrap_value.clone();
        REQUIRE(pwrap_value->are_equal(wrap_value));

        auto pwrap_cvalue = wrap_cvalue.clone();
        REQUIRE(pwrap_cvalue->are_equal(wrap_cvalue));

        auto pwrap_ref = wrap_ref.clone();
        REQUIRE(pwrap_ref->are_equal(wrap_ref));

        auto pwrap_cref = wrap_cref.clone();
        REQUIRE(pwrap_cref->are_equal(wrap_cref));

        auto pwrap_rref = wrap_rref.clone();
        REQUIRE(pwrap_rref->are_equal(wrap_rref));

        auto pwrap_crref = wrap_crref.clone();
        REQUIRE(pwrap_crref->are_equal(wrap_crref));
    }

    SECTION("are_equal") {
        // How the value is held shouldn't matter (including cv-qualifiers)
        REQUIRE(wrap_value.are_equal(wrap_value));
        REQUIRE(wrap_value.are_equal(wrap_cvalue));
        REQUIRE(wrap_value.are_equal(wrap_ref));
        REQUIRE(wrap_value.are_equal(wrap_cref));
        REQUIRE(wrap_value.are_equal(wrap_rref));
        REQUIRE(wrap_value.are_equal(wrap_crref));

        // Compare to a different value
        REQUIRE_FALSE(wrap_value.are_equal(value_holder(other_value)));

        // Compare to a different type
        double x = 3.14;
        detail_::QualifiedLeafHolder<double> wrap_x(x);
        REQUIRE_FALSE(wrap_value.are_equal(wrap_x));
        REQUIRE_FALSE(wrap_x.are_equal(wrap_value));
    }

    SECTION("contains_type") {
        REQUIRE(wrap_value.template contains_type<type>());
        REQUIRE_FALSE(wrap_value.template contains_type<const type>());

        REQUIRE_FALSE(wrap_cvalue.template contains_type<type>());
        REQUIRE(wrap_cvalue.template contains_type<const type>());

        REQUIRE(wrap_ref.template contains_type<type>());
        REQUIRE_FALSE(wrap_ref.template contains_type<const type>());

        REQUIRE_FALSE(wrap_cref.template contains_type<type>());
        REQUIRE(wrap_cref.template contains_type<const type>());

        REQUIRE(wrap_rref.template contains_type<type>());
        REQUIRE_FALSE(wrap_rref.template contains_type<const type>());

        REQUIRE_FALSE(wrap_crref.template contains_type<type>());
        REQUIRE(wrap_crref.template contains_type<const type>());
    }

    SECTION("is_convertible") {
        REQUIRE(wrap_value.template is_convertible<type>());
        REQUIRE(wrap_value.template is_convertible<const type>());

        REQUIRE_FALSE(wrap_cvalue.template is_convertible<type>());
        REQUIRE(wrap_cvalue.template is_convertible<const type>());

        REQUIRE(wrap_ref.template is_convertible<type>());
        REQUIRE(wrap_ref.template is_convertible<const type>());

        REQUIRE_FALSE(wrap_cref.template is_convertible<type>());
        REQUIRE(wrap_cref.template is_convertible<const type>());

        REQUIRE(wrap_rref.template is_convertible<type>());
        REQUIRE(wrap_rref.template is_convertible<const type>());

        REQUIRE_FALSE(wrap_crref.template is_convertible<type>());
        REQUIRE(wrap_crref.template is_convertible<const type>());
    }

    SECTION("value()") {
        REQUIRE(wrap_value.value() == value);
        REQUIRE(wrap_cvalue.value() == cvalue);
        REQUIRE(wrap_ref.value() == value);
        REQUIRE(wrap_cref.value() == cvalue);
        REQUIRE(wrap_rref.value() == value);
        REQUIRE(wrap_crref.value() == cvalue);
    }

    SECTION("value() const") {
        REQUIRE(std::as_const(wrap_value).value() == cvalue);
        REQUIRE(std::as_const(wrap_cvalue).value() == cvalue);
        REQUIRE(std::as_const(wrap_ref).value() == value);
        REQUIRE(std::as_const(wrap_cref).value() == cvalue);
        REQUIRE(std::as_const(wrap_rref).value() == value);
        REQUIRE(std::as_const(wrap_crref).value() == cvalue);
    }

    SECTION("operator==") {
        REQUIRE(wrap_value == value_holder(value));
        REQUIRE_FALSE(wrap_value == value_holder(other_value));

        REQUIRE(wrap_cvalue == cvalue_holder(value));
        REQUIRE_FALSE(wrap_cvalue == cvalue_holder(other_value));

        type value2(value);

        REQUIRE(wrap_ref == ref_holder(value));
        REQUIRE_FALSE(wrap_ref == ref_holder(other_value));
        // Different addresses
        REQUIRE_FALSE(wrap_ref == ref_holder(value2));

        REQUIRE(wrap_cref == cref_holder(cvalue));
        REQUIRE_FALSE(wrap_cref == cref_holder(other_value));
        // Different addresses
        REQUIRE_FALSE(wrap_cref == cref_holder(value2));

        type value_copy2(value), value_copy3(value);
        type other_copy(other_value), other_copy2(other_value);
        REQUIRE(wrap_rref == rvalue_holder(std::move(value_copy2)));
        REQUIRE_FALSE(wrap_rref == rvalue_holder(std::move(other_copy)));

        REQUIRE(wrap_crref == const_rvalue_h(std::move(value_copy3)));
        REQUIRE_FALSE(wrap_crref == const_rvalue_h(std::move(other_copy2)));
    }

    SECTION("operator!=") {
        // Just negates operator== so spot checking is fine
        REQUIRE_FALSE(wrap_value != value_holder(value));
        REQUIRE(wrap_value != value_holder(other_value));
    }
}