#include "utilities/type_traits/variant/tuple.hpp"
#include <catch2/catch.hpp>
#include <type_traits>

using namespace utilities;
using namespace utilities::detail_;

TEST_CASE("variant_to_tuple") {
    using variant_type = std::variant<int, double, float>;
    using type         = typename variant_to_tuple<variant_type>::type;
    using corr         = std::tuple<int, double, float>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}

TEST_CASE("variant_to_tuple_t") {
    using variant_type = std::variant<int, double, float>;
    using type         = variant_to_tuple_t<variant_type>;
    using corr         = std::tuple<int, double, float>;
    STATIC_REQUIRE(std::is_same_v<type, corr>);
}