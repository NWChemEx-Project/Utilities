#include <catch2/catch.hpp>
#include <utilities/containers/math_set.hpp>
#include <vector>

using namespace utilities;

TEST_CASE("MathSet<int> default ctor") {
    MathSet<int> s;

    REQUIRE(s.size() == 0);
    REQUIRE(s.empty());
}

TEST_CASE("MathSet<int> initializer list") {
    //    SECTION("Single entry") {
    //        SECTION("Container type") {
    //            std::vector<int> v{1, 2, 3, 4, 5};
    //            MathSet s { v };
    //            using v_type = typename decltype(s)::value_type;
    //            STATIC_REQUIRE(std::is_same_v<v_type, std::vector<int>>);
    //        }
    //        SECTION("Other MathSet") {
    //            MathSet<int> s;
    //            MathSet s1{{s}};
    //            using v_type = typename decltype(s1)::value_type;
    //            STATIC_REQUIRE(std::is_same_v<v_type,
    //            detail_::MathSetView<int>>);
    //        }
    //    }

    SECTION("Multiple entries") {
        MathSet s{int{1}, int{2}, int{3}};
        using v_type = typename decltype(s)::value_type;
        STATIC_REQUIRE(std::is_same_v<v_type, int>);
    }
}
