#include "utilities/type_traits/parameter_pack_traits.hpp"
#include <catch2/catch.hpp>
#include <iostream>

using namespace utilities::type_traits;

using search_types = std::tuple<int, double, char>;



// Contains Type
TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type: empty parameter pack", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(parameter_pack_contains_type<search_type>::value);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type: parameter pack w/o type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(parameter_pack_contains_type<search_type, float, bool, unsigned long>::value);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type: parameter pack w type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(parameter_pack_contains_type<search_type, float, int, double, char>::value);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type_v: parameter pack w/o type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE_FALSE(parameter_pack_contains_type_v<search_type, float, bool, unsigned long>);
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_contains_type_v: parameter pack w type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE(parameter_pack_contains_type_v<search_type, float, int, double, char>);
}







// Count Type
TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type: empty parameter pack", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type<search_type>::value == 0ul );
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type: parameter pack w/o type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type<search_type,float,bool,unsigned long>::value == 0ul );
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type: parameter pack w single instance of type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type<search_type, float, int, double, char>::value == 1ul );
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type: parameter pack w 2 instances of type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type<search_type, float, int, int, double, char, unsigned long, char, double>::value == 2ul );
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type_v: parameter pack w/o type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type_v<search_type,float,bool,unsigned long> == 0ul );
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type_v: parameter pack w single instance of type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type_v<search_type, float, int, double, char> == 1ul );
}

TEMPLATE_LIST_TEST_CASE("parameter_pack_count_type_v: parameter pack w 2 instances of type", "", search_types) {
    using search_type = TestType;
    STATIC_REQUIRE( parameter_pack_count_type_v<search_type, float, int, int, double, char, unsigned long, char, double> == 2ul );
}
