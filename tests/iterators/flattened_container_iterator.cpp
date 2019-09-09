#include <catch2/catch.hpp>
#include <utilities/iterators/flattened_container_iterator.hpp>

using vector_t = std::vector<int>;
using vec2     = std::vector<vector_t>;
using vec3     = std::vector<vec2>;
using namespace utilities::iterators;

TEST_CASE("call_begin") {
    vector_t v{1, 2, 3, 4, 5};
    vec2 v2{v, v, v};
    vec3 v3{v2, v2, v2};
    SECTION("vector") { REQUIRE(v.begin() == detail_::call_begin<0, 0>(v)); }
    SECTION("vector of vector") {
        SECTION("<0, 0>") {
            REQUIRE(v2.begin() == detail_::call_begin<0, 0>(v2));
        }
        SECTION("<0, 1>") {
            REQUIRE(v2.begin()->begin() == detail_::call_begin<0, 1>(v2));
        }
    }
    SECTION("vector of vector of vector") {
        SECTION("<0, 0>") {
            auto corr_result = v3.begin();
            REQUIRE(corr_result == detail_::call_begin<0, 0>(v3));
        }
        SECTION("<0, 1>") {
            auto corr_result = v3.begin()->begin();
            REQUIRE(corr_result == detail_::call_begin<0, 1>(v3));
        }
        SECTION("<0, 2>") {
            auto corr_result = v3.begin()->begin()->begin();
            REQUIRE(corr_result == detail_::call_begin<0, 2>(v3));
        }
    }
}

TEST_CASE("call_end") {
    vector_t v{1, 2, 3, 4, 5};
    vec2 v2{v, v, v};
    vec3 v3{v2, v2, v2};
    SECTION("vector") {
        auto corr_result = v.end();
        using corr_type  = decltype(corr_result);
        auto result      = detail_::call_end<0, 0>(v);
        STATIC_REQUIRE(std::is_same_v<corr_type, decltype(result)>);
        REQUIRE(result == corr_result);
    }
    SECTION("vector of vector") {
        SECTION("<0, 0>") {
            auto corr_result = v2.end();
            using corr_type  = decltype(corr_result);
            auto result      = detail_::call_end<0, 0>(v2);
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(result)>);
            REQUIRE(result == corr_result);
        }
        SECTION("<0, 1>") {
            auto corr_result = v2.end()->end();
            using corr_type  = decltype(corr_result);
            auto result      = detail_::call_end<0, 1>(v2);
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(result)>);
            REQUIRE(result == corr_result);
        }
    }
    SECTION("vector of vector of vector") {
        SECTION("<0, 0>") {
            auto corr_result = v3.end();
            using corr_type  = decltype(corr_result);
            auto result      = detail_::call_end<0, 0>(v3);
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(result)>);
            REQUIRE(result == corr_result);
        }
        SECTION("<0, 1>") {
            auto corr_result = v3.end()->end();
            using corr_type  = decltype(corr_result);
            auto result      = detail_::call_end<0, 1>(v3);
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(result)>);
            REQUIRE(result == corr_result);
        }
        SECTION("<0, 2>") {
            auto corr_result = v3.end()->end()->end();
            using corr_type  = decltype(corr_result);
            auto result      = detail_::call_end<0, 2>(v3);
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(result)>);
            REQUIRE(result == corr_result);
        }
    }
}
