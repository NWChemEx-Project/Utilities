#include <catch2/catch.hpp>
#include <utilities/containers/math_set.hpp>
#include <utilities/iter_tools/zip.hpp>
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

TEST_CASE("MathSet copy ctor") {}

TEST_CASE("MathSet<MathSet<int>>") {
    std::vector corr{1, 2, 2, 3, 4, 5};
    MathSet s{MathSet{1, 2}, MathSet{2, 3}, MathSet{4, 5}};

    REQUIRE(s.size() == 3);
    REQUIRE_FALSE(s.empty());

    SECTION("Check values") {
        auto corri = corr.begin();
        for(const auto& x : s) {
            for(const auto& y : x) {
                REQUIRE(y == *corri);
                ++corri;
            }
        }
    }

    SECTION("Check contiguous-ness ") {
        auto pelem = &s[0][0];
        for(const MathSet<int>& x : s) {
            for(const int& y : x) {
                REQUIRE(&y == pelem);
                ++pelem;
            }
        }
    }

    SECTION("Grabbing element by value makes a deep copy") {
        MathSet<int> elem = s[0];
        REQUIRE_FALSE(&elem[0] == &s[0][0]);
    }
}
