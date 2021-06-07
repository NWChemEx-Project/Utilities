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