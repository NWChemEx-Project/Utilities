#include <catch2/catch.hpp>
#include <utilities/containers/detail_/nested_set_pimpl.hpp>
#include <utilities/containers/math_set.hpp>

using namespace utilities;
using namespace utilities::detail_;

TEST_CASE("NestedSetPIMPL default ctor") {
    NestedSetPIMPL<MathSet<int>> s;

    REQUIRE(s.size() == 0);
}

TEST_CASE("NestedSetPIMPL intializer list") {
    std::vector corr{1, 2, 2, 3, 3, 4};
    NestedSetPIMPL s{MathSet{1, 2}, MathSet{2, 3}, MathSet{3, 4}};

    using deduced_type = decltype(s);
    STATIC_REQUIRE(std::is_same_v<deduced_type, NestedSetPIMPL<MathSet<int>>>);
    STATIC_REQUIRE(
      std::is_same_v<typename deduced_type::value_type, MathSet<int>>);

    REQUIRE(s.size() == 3);

    auto* pelem         = &s[0][0]; // Set to the address of the first element
    std::size_t counter = 0;
    for(const MathSet<int>& x : s) {
        REQUIRE(x.size() == 2);
        for(const int& y : x) {
            REQUIRE(y == corr[counter]);
            // Check that the sets are flattened (i.e. addresses are contiguous
            REQUIRE(&y == pelem);
            ++pelem;
            ++counter;
        }
    }
}

TEST_CASE("Nested Nested MathSet") { std::vector corr{1, 2, 3, 4, 5, 6, 7}; }
