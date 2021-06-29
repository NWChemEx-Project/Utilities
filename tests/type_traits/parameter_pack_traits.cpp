#include "utilities/type_traits/parameter_pack_traits.hpp"
#include <catch2/catch.hpp>
#include <iostream>

using namespace utilities::type_traits;

using search_types = std::tuple<int, double, char>;

struct Base {};
struct Derived : public Base {};


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



// Contains derived type
TEST_CASE("parameter_pack_contains_derived_type: empty parameter pack", "") {
    STATIC_REQUIRE_FALSE(parameter_pack_contains_derived_type<Base>::value);
}
TEST_CASE("parameter_pack_contains_derived_type: parameter pack w/o Derived instance", "") {
    STATIC_REQUIRE_FALSE(parameter_pack_contains_derived_type<Base,int,double>::value);
} 
TEST_CASE("parameter_pack_contains_derived_type: Base instance parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_contains_derived_type<Base,Base,int>::value);
}
TEST_CASE("parameter_pack_contains_derived_type: Derived instance parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_contains_derived_type<Base,Derived,int>::value);
}

TEST_CASE("parameter_pack_contains_derived_type_v: empty parameter pack", "") {
    STATIC_REQUIRE_FALSE(parameter_pack_contains_derived_type_v<Base>);
}
TEST_CASE("parameter_pack_contains_derived_type_v: parameter pack w/o Derived instance", "") {
    STATIC_REQUIRE_FALSE(parameter_pack_contains_derived_type_v<Base,int,double>);
}
TEST_CASE("parameter_pack_contains_derived_type_v: Base instance parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_contains_derived_type_v<Base,Base,int>);
}
TEST_CASE("parameter_pack_contains_derived_type_v: Derived instance parameter pack", "") {
    STATIC_REQUIRE(parameter_pack_contains_derived_type_v<Base,Derived,int>);
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


// Count derived type
TEST_CASE("parameter_pack_count_derived_type: empty parameter pack", "" ) {
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base>::value == 0ul);
}
TEST_CASE("parameter_pack_count_derived_type: parameter pack w/o derived instance","") {
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base,int,double>::value==0ul);
}
TEST_CASE("parameter_pack_count_derived_type: parameter pack w/ base instance",""){
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base,int,Base>::value==1ul);
}
TEST_CASE("parameter_pack_count_derived_type: parameter pack w/ derived instance",""){
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base,int,Derived>::value==1ul);
}
TEST_CASE("parameter_pack_count_derived_type: parameter pack w/ base+derived instance",""){
    STATIC_REQUIRE(parameter_pack_count_derived_type<Base,int,Derived,double,Base>::value==2ul);
}

TEST_CASE("parameter_pack_count_derived_type_v: empty parameter pack", "" ) {
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base> == 0ul);
}
TEST_CASE("parameter_pack_count_derived_type_v: parameter pack w/o derived instance","") {
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base,int,double> == 0ul);
}
TEST_CASE("parameter_pack_count_derived_type_v: parameter pack w/ base instance",""){
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base,int,Base> == 1ul);
}
TEST_CASE("parameter_pack_count_derived_type_v: parameter pack w/ derived instance",""){
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base,int,Derived> == 1ul);
}
TEST_CASE("parameter_pack_count_derived_type_v: parameter pack w/ base+derived instance",""){
    STATIC_REQUIRE(parameter_pack_count_derived_type_v<Base,int,Derived,double,Base> == 2ul);
}
