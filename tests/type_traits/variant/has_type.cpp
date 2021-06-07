#include "utilities/type_traits/variant/has_type.hpp"
#include <catch2/catch.hpp>

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