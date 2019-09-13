#include <catch2/catch.hpp>
#include <utilities/containers/math_set/detail_/element_type_traits.hpp>

TEST_CASE("ElementTypeTraits<int>") {
    using type = utilities::detail_::ElementTypeTraits<int>;

    STATIC_REQUIRE(std::is_same_v<typename type::value_type, int>);
    STATIC_REQUIRE(std::is_same_v<typename type::reference, int&>);
    STATIC_REQUIRE(std::is_same_v<typename type::const_reference, const int&>);
}
