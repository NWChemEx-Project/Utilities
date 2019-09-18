#include <catch2/catch.hpp>
#include <utilities/containers/math_set/math_set.hpp>

using namespace utilities;

TEST_CASE("MathSet<int>: operator==") {
    MathSet s0{1, 2, 3};

    SECTION("Same instance") { REQUIRE(s0 == s0); }
    SECTION("Different instance, same content") {
        MathSet s1{1, 2, 3};
        REQUIRE(s0 == s1);
    }
    SECTION("Different instance, different content") {
        MathSet s1{2, 3, 4};
        REQUIRE_FALSE(s0 == s1);
    }
    SECTION("MathSetView<int>, same content") {
        MathSet s1{1, 2, 3};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView s2(v.begin(), v.end());
        REQUIRE(s0 == s2);
    }
    SECTION("MathSetView<int>, different content") {
        MathSet s1{2, 3, 4};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView s2(v.begin(), v.end());
        REQUIRE_FALSE(s0 == s2);
    }
}

TEST_CASE("MathSet<int>: operator!=") {
    MathSet s0{1, 2, 3};

    SECTION("Same instance") { REQUIRE_FALSE(s0 != s0); }
    SECTION("Different instance, same content") {
        MathSet s1{1, 2, 3};
        REQUIRE_FALSE(s0 != s1);
    }
    SECTION("Different instance, different content") {
        MathSet s1{2, 3, 4};
        REQUIRE(s0 != s1);
    }
    SECTION("MathSetView<int>, same content") {
        MathSet s1{1, 2, 3};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView s2(v.begin(), v.end());
        REQUIRE_FALSE(s0 != s2);
    }
    SECTION("MathSetView<int>, different content") {
        MathSet s1{2, 3, 4};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView s2(v.begin(), v.end());
        REQUIRE(s0 != s2);
    }
}

TEST_CASE("MathSetView<int>: operator==") {
    MathSet s0{1, 2, 3};
    std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
    MathSetView s1{v.begin(), v.end()};

    SECTION("Same instance") { REQUIRE(s1 == s1); }
    SECTION("Different instance, same content") {
        MathSet s2{1, 2, 3};
        REQUIRE(s1 == s2);
    }
    SECTION("Different instance, different content") {
        MathSet s2{2, 3, 4};
        REQUIRE_FALSE(s1 == s2);
    }
    SECTION("MathSetView<int>, same content") {
        MathSet s2{1, 2, 3};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView s3(v2.begin(), v2.end());
        REQUIRE(s1 == s3);
    }
    SECTION("MathSetView<int>, different content") {
        MathSet s2{2, 3, 4};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView s3(v2.begin(), v2.end());
        REQUIRE_FALSE(s1 == s3);
    }
}

TEST_CASE("MathSetView<int>: operator!=") {
    MathSet s0{1, 2, 3};
    std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
    MathSetView s1{v.begin(), v.end()};

    SECTION("Same instance") { REQUIRE_FALSE(s1 != s1); }
    SECTION("Different instance, same content") {
        MathSet s2{1, 2, 3};
        REQUIRE_FALSE(s1 != s2);
    }
    SECTION("Different instance, different content") {
        MathSet s2{2, 3, 4};
        REQUIRE(s1 != s2);
    }
    SECTION("MathSetView<int>, same content") {
        MathSet s2{1, 2, 3};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView s3(v2.begin(), v2.end());
        REQUIRE_FALSE(s1 != s3);
    }
    SECTION("MathSetView<int>, different content") {
        MathSet s2{2, 3, 4};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView s3(v2.begin(), v2.end());
        REQUIRE(s1 != s3);
    }
}

TEST_CASE("MathSet<int>: operator^") {
    MathSet<int> s0{1, 2, 3};
    SECTION("Same instance") { REQUIRE((s0 ^ s0) == s0); }
    SECTION("Empty MathSet") {
        MathSet<int> s1;
        REQUIRE((s0 ^ s1) == s1);
    }
    SECTION("Empty MathSetView") {
        MathSetView<int> s1;
        REQUIRE((s0 ^ s1) == s1);
    }
    SECTION("MathSet, same content") {
        MathSet<int> s1{1, 2, 3};
        REQUIRE((s0 ^ s1) == s1);
    }
    SECTION("MathSetView, same content") {
        std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 ^ s1) == s1);
    }
    SECTION("MathSet, subset") {
        MathSet<int> s1{1, 3};
        REQUIRE((s0 ^ s1) == s1);
    }
    SECTION("MathSetView, subset") {
        std::vector v{std::ref(s0[0]), std::ref(s0[2])};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 ^ s1) == s1);
    }
    SECTION("MathSet, superset") {
        MathSet<int> s1{1, 2, 3, 4};
        REQUIRE((s0 ^ s1) == s0);
    }
    SECTION("MathSetView, superset") {
        int four = 4;
        std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2]),
                      std::ref(four)};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 ^ s1) == s0);
    }
    SECTION("MathSet, no common elements") {
        MathSet<int> s1{4, 5, 6};
        MathSet<int> empty;
        REQUIRE((s0 ^ s1) == empty);
    }
    SECTION("MathSetView, no common elements") {
        MathSet<int> s1{4, 5, 6};
        MathSetView<int> empty;
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView<int> s2{v.begin(), v.end()};
        REQUIRE((s0 ^ s2) == empty);
    }
    SECTION("MathSet, some overlap") {
        MathSet<int> s1{4, 2, 6};
        MathSet<int> s2{2};
        REQUIRE((s0 ^ s1) == s2);
    }
    SECTION("MathSetView, some overlap") {
        MathSet<int> s1{4, 2, 6};
        MathSet<int> s2{2};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView<int> s3{v.begin(), v.end()};
        REQUIRE((s0 ^ s3) == s2);
    }
    SECTION("References are to the LHS") {
        MathSet<int> s1{1, 2, 3};
        const auto& intersection = s0 ^ s1;
        for(std::size_t i = 0; i < 3; ++i) REQUIRE(&intersection[i] == &s0[i]);
    }
}

TEST_CASE("MathSetView<int>: operator^") {
    MathSet<int> s0{1, 2, 3};
    std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
    MathSetView<int> s1{v.begin(), v.end()};
    SECTION("Same instance") { REQUIRE((s1 ^ s1) == s1); }
    SECTION("Empty MathSet") {
        MathSet<int> s2;
        REQUIRE((s1 ^ s2) == s2);
    }
    SECTION("Empty MathSetView") {
        MathSetView<int> s2;
        REQUIRE((s1 ^ s2) == s2);
    }
    SECTION("MathSet, same content") {
        MathSet<int> s2{1, 2, 3};
        REQUIRE((s1 ^ s2) == s1);
    }
    SECTION("MathSetView, same content") {
        std::vector v2{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 ^ s2) == s1);
    }
    SECTION("MathSet, subset") {
        MathSet<int> s2{1, 3};
        REQUIRE((s1 ^ s2) == s2);
    }
    SECTION("MathSetView, subset") {
        std::vector v2{std::ref(s0[0]), std::ref(s0[2])};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 ^ s2) == s2);
    }
    SECTION("MathSet, superset") {
        MathSet<int> s2{1, 2, 3, 4};
        REQUIRE((s1 ^ s2) == s1);
    }
    SECTION("MathSetView, superset") {
        int four = 4;
        std::vector v2{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2]),
                       std::ref(four)};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 ^ s2) == s1);
    }
    SECTION("MathSet, no common elements") {
        MathSet<int> s2{4, 5, 6};
        MathSet<int> empty;
        REQUIRE((s1 ^ s2) == empty);
    }
    SECTION("MathSetView, no common elements") {
        MathSet<int> s2{4, 5, 6};
        MathSetView<int> empty;
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView<int> s3{v2.begin(), v2.end()};
        REQUIRE((s1 ^ s3) == empty);
    }
    SECTION("MathSet, some overlap") {
        MathSet<int> s2{4, 2, 6};
        MathSet<int> s3{2};
        REQUIRE((s1 ^ s2) == s3);
    }
    SECTION("MathSetView, some overlap") {
        MathSet<int> s2{4, 2, 6};
        MathSet<int> s3{2};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView<int> s4{v2.begin(), v2.end()};
        REQUIRE((s1 ^ s4) == s3);
    }
    SECTION("References are to the LHS") {
        MathSet<int> s2{1, 2, 3};
        const auto& intersection = s1 ^ s2;
        for(std::size_t i = 0; i < 3; ++i) REQUIRE(&intersection[i] == &s1[i]);
    }
}

TEST_CASE("MathSet<int>: operator+") {
    MathSet<int> s0{1, 2, 3};
    SECTION("Same instance") { REQUIRE((s0 + s0) == s0); }
    SECTION("Empty MathSet") {
        MathSet<int> s1;
        REQUIRE((s0 + s1) == s0);
    }
    SECTION("Empty MathSetView") {
        MathSetView<int> s1;
        REQUIRE((s0 + s1) == s0);
    }
    SECTION("MathSet, same content") {
        MathSet<int> s1{1, 2, 3};
        REQUIRE((s0 + s1) == s1);
    }
    SECTION("MathSetView, same content") {
        std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 + s1) == s1);
    }
    SECTION("MathSet, subset") {
        MathSet<int> s1{1, 3};
        REQUIRE((s0 + s1) == s0);
    }
    SECTION("MathSetView, subset") {
        std::vector v{std::ref(s0[0]), std::ref(s0[2])};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 + s1) == s0);
    }
    SECTION("MathSet, superset") {
        MathSet<int> s1{1, 2, 3, 4};
        REQUIRE((s0 + s1) == s1);
    }
    SECTION("MathSetView, superset") {
        int four = 4;
        std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2]),
                      std::ref(four)};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 + s1) == s1);
    }
    SECTION("MathSet, no common elements") {
        MathSet<int> s1{4, 5, 6};
        MathSet<int> s2{1, 2, 3, 4, 5, 6};
        REQUIRE((s0 + s1) == s2);
    }
    SECTION("MathSetView, no common elements") {
        MathSet<int> s1{4, 5, 6};
        MathSet<int> s2{1, 2, 3, 4, 5, 6};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView<int> s3{v.begin(), v.end()};
        REQUIRE((s0 + s3) == s2);
    }
    SECTION("MathSet, some overlap") {
        MathSet<int> s1{4, 2, 6};
        MathSet<int> s2{1, 2, 3, 4, 6};
        REQUIRE((s0 + s1) == s2);
    }
    SECTION("MathSetView, some overlap") {
        MathSet<int> s1{4, 2, 6};
        MathSet<int> s2{1, 2, 3, 4, 6};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView<int> s3{v.begin(), v.end()};
        REQUIRE((s0 + s3) == s2);
    }
    SECTION("References are to the LHS and then RHS") {
        MathSet<int> s1{4, 2, 6};
        const auto& the_union = s0 + s1;
        for(std::size_t i = 0; i < 3; ++i) REQUIRE(&the_union[i] == &s0[i]);
        REQUIRE(&the_union[3] == &s1[0]);
        REQUIRE(&the_union[4] == &s1[2]);
    }
}

TEST_CASE("MathSetView<int>: operator+") {
    MathSet<int> s0{1, 2, 3};
    std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
    MathSetView<int> s1{v.begin(), v.end()};
    SECTION("Same instance") { REQUIRE((s1 + s1) == s1); }
    SECTION("Empty MathSet") {
        MathSet<int> s2;
        REQUIRE((s1 + s2) == s1);
    }
    SECTION("Empty MathSetView") {
        MathSetView<int> s2;
        REQUIRE((s1 + s2) == s1);
    }
    SECTION("MathSet, same content") {
        MathSet<int> s2{1, 2, 3};
        REQUIRE((s1 + s2) == s1);
    }
    SECTION("MathSetView, same content") {
        std::vector v2{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 + s2) == s1);
    }
    SECTION("MathSet, subset") {
        MathSet<int> s2{1, 3};
        REQUIRE((s1 + s2) == s1);
    }
    SECTION("MathSetView, subset") {
        std::vector v2{std::ref(s0[0]), std::ref(s0[2])};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 + s2) == s1);
    }
    SECTION("MathSet, superset") {
        MathSet<int> s2{1, 2, 3, 4};
        REQUIRE((s1 + s2) == s2);
    }
    SECTION("MathSetView, superset") {
        int four = 4;
        std::vector v2{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2]),
                       std::ref(four)};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 + s2) == s2);
    }
    SECTION("MathSet, no common elements") {
        MathSet<int> s2{4, 5, 6};
        MathSet<int> s3{1, 2, 3, 4, 5, 6};
        REQUIRE((s1 + s2) == s3);
    }
    SECTION("MathSetView, no common elements") {
        MathSet<int> s2{4, 5, 6};
        MathSet<int> s3{1, 2, 3, 4, 5, 6};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView<int> s4{v2.begin(), v2.end()};
        REQUIRE((s1 + s4) == s3);
    }
    SECTION("MathSet, some overlap") {
        MathSet<int> s2{4, 2, 6};
        MathSet<int> s3{1, 2, 3, 4, 6};
        REQUIRE((s1 + s2) == s3);
    }
    SECTION("MathSetView, some overlap") {
        MathSet<int> s2{4, 2, 6};
        MathSet<int> s3{1, 2, 3, 4, 6};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView<int> s4{v2.begin(), v2.end()};
        REQUIRE((s1 + s4) == s3);
    }
    SECTION("References are to the LHS and then RHS") {
        MathSet<int> s2{4, 2, 6};
        const auto& the_union = s1 + s2;
        for(std::size_t i = 0; i < 3; ++i) REQUIRE(&the_union[i] == &s1[i]);
        REQUIRE(&the_union[3] == &s2[0]);
        REQUIRE(&the_union[4] == &s2[2]);
    }
}

TEST_CASE("MathSet<int>: operator-") {
    MathSet<int> s0{1, 2, 3};
    MathSet<int> empty;
    SECTION("Same instance") { REQUIRE((s0 - s0) == empty); }
    SECTION("Empty MathSet") {
        MathSet<int> s1;
        REQUIRE((s0 - s1) == s0);
    }
    SECTION("Empty MathSetView") {
        MathSetView<int> s1;
        REQUIRE((s0 - s1) == s0);
    }
    SECTION("MathSet, same content") {
        MathSet<int> s1{1, 2, 3};
        REQUIRE((s0 - s1) == empty);
    }
    SECTION("MathSetView, same content") {
        std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 - s1) == empty);
    }
    SECTION("MathSet, subset") {
        MathSet<int> s1{1, 3};
        REQUIRE((s0 - s1) == MathSet<int>{2});
    }
    SECTION("MathSetView, subset") {
        std::vector v{std::ref(s0[0]), std::ref(s0[2])};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 - s1) == MathSet<int>{2});
    }
    SECTION("MathSet, superset") {
        MathSet<int> s1{1, 2, 3, 4};
        REQUIRE((s0 - s1) == empty);
    }
    SECTION("MathSetView, superset") {
        int four = 4;
        std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2]),
                      std::ref(four)};
        MathSetView<int> s1{v.begin(), v.end()};
        REQUIRE((s0 - s1) == empty);
    }
    SECTION("MathSet, no common elements") {
        MathSet<int> s1{4, 5, 6};
        REQUIRE((s0 - s1) == s0);
    }
    SECTION("MathSetView, no common elements") {
        MathSet<int> s1{4, 5, 6};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView<int> s3{v.begin(), v.end()};
        REQUIRE((s0 - s3) == s0);
    }
    SECTION("MathSet, some overlap") {
        MathSet<int> s1{4, 2, 6};
        MathSet<int> s2{1, 3};
        REQUIRE((s0 - s1) == s2);
    }
    SECTION("MathSetView, some overlap") {
        MathSet<int> s1{4, 2, 6};
        MathSet<int> s2{1, 3};
        std::vector v{std::ref(s1[0]), std::ref(s1[1]), std::ref(s1[2])};
        MathSetView<int> s3{v.begin(), v.end()};
        REQUIRE((s0 - s3) == s2);
    }
    SECTION("References are to the LHS") {
        MathSet<int> s1{3};
        const auto& the_difference = s0 - s1;
        for(std::size_t i = 0; i < 2; ++i)
            REQUIRE(&the_difference[i] == &s0[i]);
    }
}

TEST_CASE("MathSetView<int>: operator-") {
    MathSet<int> s0{1, 2, 3};
    MathSet<int> empty;
    std::vector v{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
    MathSetView<int> s1{v.begin(), v.end()};
    SECTION("Same instance") { REQUIRE((s1 - s1) == empty); }
    SECTION("Empty MathSet") {
        MathSet<int> s2;
        REQUIRE((s1 - s2) == s1);
    }
    SECTION("Empty MathSetView") {
        MathSetView<int> s2;
        REQUIRE((s1 - s2) == s1);
    }
    SECTION("MathSet, same content") {
        MathSet<int> s2{1, 2, 3};
        REQUIRE((s1 - s2) == empty);
    }
    SECTION("MathSetView, same content") {
        std::vector v2{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2])};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 - s2) == empty);
    }
    SECTION("MathSet, subset") {
        MathSet<int> s2{1, 3};
        REQUIRE((s1 - s2) == MathSet<int>{2});
    }
    SECTION("MathSetView, subset") {
        std::vector v2{std::ref(s0[0]), std::ref(s0[2])};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 - s2) == MathSet<int>{2});
    }
    SECTION("MathSet, superset") {
        MathSet<int> s2{1, 2, 3, 4};
        REQUIRE((s1 - s2) == empty);
    }
    SECTION("MathSetView, superset") {
        int four = 4;
        std::vector v2{std::ref(s0[0]), std::ref(s0[1]), std::ref(s0[2]),
                       std::ref(four)};
        MathSetView<int> s2{v2.begin(), v2.end()};
        REQUIRE((s1 - s2) == empty);
    }
    SECTION("MathSet, no common elements") {
        MathSet<int> s2{4, 5, 6};
        REQUIRE((s1 - s2) == s1);
    }
    SECTION("MathSetView, no common elements") {
        MathSet<int> s2{4, 5, 6};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView<int> s4{v2.begin(), v2.end()};
        REQUIRE((s1 - s4) == s1);
    }
    SECTION("MathSet, some overlap") {
        MathSet<int> s2{4, 2, 6};
        MathSet<int> s3{1, 3};
        REQUIRE((s1 - s2) == s3);
    }
    SECTION("MathSetView, some overlap") {
        MathSet<int> s2{4, 2, 6};
        MathSet<int> s3{1, 3};
        std::vector v2{std::ref(s2[0]), std::ref(s2[1]), std::ref(s2[2])};
        MathSetView<int> s4{v2.begin(), v2.end()};
        REQUIRE((s1 - s4) == s3);
    }
    SECTION("References are to the LHS") {
        MathSet<int> s2{3};
        const auto& the_difference = s1 - s2;
        for(std::size_t i = 0; i < 2; ++i)
            REQUIRE(&the_difference[i] == &s1[i]);
    }
}
