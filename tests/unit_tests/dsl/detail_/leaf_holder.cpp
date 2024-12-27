#include "../../test_helpers.hpp"
#include <utilities/dsl/detail_/leaf_holder.hpp>

using namespace utilities::dsl;
using std::is_same_v;

/* Testing notes:
 *
 * We can only actually make instances of QualifiedLeafHolder as the base
 * classes are abstract.
 */

using types_to_test = std::tuple<std::string>;

TEMPLATE_LIST_TEST_CASE("QualifiedLeafHolder<std::string>", "", types_to_test) {
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

    types_to_test values{"Hello world"};

    ref value = std::get<type>(values);

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

    SECTION("Ctors") {}
}