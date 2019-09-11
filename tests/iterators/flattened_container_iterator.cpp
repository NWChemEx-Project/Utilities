#include <catch2/catch.hpp>
#include <utilities/iterators/flattened_container_iterator.hpp>

using vec1     = std::vector<int>;
using vec1_itr = typename vec1::iterator;
using vec2     = std::vector<vec1>;
using vec2_itr = typename vec2::iterator;
using vec3     = std::vector<vec2>;
using vec3_itr = typename vec3::iterator;

using namespace utilities::iterators;

TEST_CASE("get_begin_tuple") {
    vec1 v{1, 2, 3};
    vec2 v2{v, v, v};
    SECTION("vector") {
        auto rv         = detail_::get_begin_tuple<0, 0>(v);
        using corr_type = std::tuple<vec1_itr>;
        STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
        REQUIRE(rv == std::make_tuple(v.begin()));
    }
    SECTION("vector of vector") {
        SECTION("0, 0") {
            auto rv         = detail_::get_begin_tuple<0, 0>(v2);
            using corr_type = std::tuple<vec2_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v2.begin()));
        }
        SECTION("0, 1") {
            auto rv         = detail_::get_begin_tuple<0, 1>(v2);
            using corr_type = std::tuple<vec2_itr, vec1_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v2.begin(), v2.begin()->begin()));
        }
    }
    SECTION("vector of vector of vector") {
        vec3 v3{v2, v2, v2};
        SECTION("0, 0") {
            auto rv         = detail_::get_begin_tuple<0, 0>(v3);
            using corr_type = std::tuple<vec3_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v3.begin()));
        }
        SECTION("0, 1") {
            auto rv         = detail_::get_begin_tuple<0, 1>(v3);
            using corr_type = std::tuple<vec3_itr, vec2_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v3.begin(), v3.begin()->begin()));
        }
        SECTION("0, 2") {
            auto rv         = detail_::get_begin_tuple<0, 2>(v3);
            using corr_type = std::tuple<vec3_itr, vec2_itr, vec1_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v3.begin(), v3.begin()->begin(),
                                          v3.begin()->begin()->begin()));
        }
    }
    SECTION("Throws if container is not appropriately nested") {
        vec3 v{vec2{}};
        // Lambda b/c macro can't handle the comma in the template parameters
        auto l = [=]() { detail_::get_begin_tuple<0, 2>(v); };
        REQUIRE_THROWS_AS(l(), std::runtime_error);
    }
}

TEST_CASE("get_end_tuple") {
    vec1 v{1, 2, 3};
    vec2 v2{v, v, v};
    SECTION("vector") {
        auto rv         = detail_::get_end_tuple<0, 0>(v);
        using corr_type = std::tuple<vec1_itr>;
        STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
        REQUIRE(rv == std::make_tuple(v.end()));
    }
    SECTION("vector of vector") {
        SECTION("0, 0") {
            auto rv         = detail_::get_end_tuple<0, 0>(v2);
            using corr_type = std::tuple<vec2_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v2.end()));
        }
        SECTION("0, 1") {
            auto rv         = detail_::get_end_tuple<0, 1>(v2);
            using corr_type = std::tuple<vec2_itr, vec1_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v2.end(), v2[2].end()));
        }
    }
    SECTION("vector of vector of vector") {
        vec3 v3{v2, v2, v2};
        SECTION("0, 0") {
            auto rv         = detail_::get_end_tuple<0, 0>(v3);
            using corr_type = std::tuple<vec3_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v3.end()));
        }
        SECTION("0, 1") {
            auto rv         = detail_::get_end_tuple<0, 1>(v3);
            using corr_type = std::tuple<vec3_itr, vec2_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv == std::make_tuple(v3.end(), v3[2].end()));
        }
        SECTION("0, 2") {
            auto rv         = detail_::get_end_tuple<0, 2>(v3);
            using corr_type = std::tuple<vec3_itr, vec2_itr, vec1_itr>;
            STATIC_REQUIRE(std::is_same_v<corr_type, decltype(rv)>);
            REQUIRE(rv ==
                    std::make_tuple(v3.end(), v3[2].end(), v3[2][2].end()));
        }
    }
}

TEST_CASE("reset_tuple_of_itr") {
    vec1 v1{1, 2, 3};
    vec2 v2{v1, v1, v1};
    SECTION("vector") {
        auto itr = detail_::get_begin_tuple<0, 0>(v1);
        ++std::get<0>(itr);
        auto new_itr = detail_::reset_tuple_of_itr<1>(v1, itr);
        REQUIRE(itr == new_itr);
        ++std::get<0>(itr);
        new_itr = detail_::reset_tuple_of_itr<1>(v1, itr);
        REQUIRE(itr == new_itr);
    }
    SECTION("Vector of Vector") {
        auto itr = detail_::get_begin_tuple<0, 1>(v2);
        for(std::size_t i = 0; i < v2.size(); ++i) {
            REQUIRE(std::get<1>(itr) == v2[i].begin());
            ++std::get<0>(itr);
            itr = detail_::reset_tuple_of_itr<1>(v2, itr);
        }
        itr = detail_::reset_tuple_of_itr<0>(v2, itr);
        REQUIRE(std::get<0>(itr) == v2.begin());
    }
    SECTION("Vector of Vector of Vector") {
        vec3 v3{v2, v2, v2};
        SECTION("0, 1") {
            auto itr = detail_::get_begin_tuple<0, 1>(v3);
            for(std::size_t i = 0; i < v3.size(); ++i) {
                REQUIRE(std::get<1>(itr) == v3[i].begin());
                ++std::get<0>(itr);
                itr = detail_::reset_tuple_of_itr<1>(v3, itr);
            }
        }
        SECTION("0, 2") {
            auto itr = detail_::get_begin_tuple<0, 2>(v3);
            for(std::size_t i = 0; i < v3.size(); ++i) {
                REQUIRE(std::get<1>(itr) == v3[i].begin());
                for(std::size_t j = 0; j < v3[i].size(); ++j) {
                    REQUIRE(std::get<2>(itr) == v3[i][j].begin());
                    ++std::get<1>(itr);
                    itr = detail_::reset_tuple_of_itr<2>(v3, itr);
                }
                ++std::get<0>(itr);
                if(std::get<0>(itr) == v3.end()) break;
                itr = detail_::reset_tuple_of_itr<1>(v3, itr);
            }
        }
    }
}

TEST_CASE("increment_tuple_of_itr") {
    vec1 v1{1, 2, 3};
    vec2 v2{vec1{1, 2, 3}, vec1{4, 5, 6}, vec1{7, 8, 9}};
    SECTION("vector") {
        auto itr = detail_::get_begin_tuple<0, 0>(v1);
        for(auto i = 0; i < v1.size(); ++i) {
            REQUIRE(&(*std::get<0>(itr)) == &v1[i]);
            itr = detail_::increment_tuple_of_itr<0>(v1, itr);
        }
    }
    SECTION("vector of vector") {
        SECTION("0 0") {
            auto itr = detail_::get_begin_tuple<0, 0>(v2);
            for(auto i = 0; i < v2.size(); ++i) {
                REQUIRE(&(*std::get<0>(itr)) == &v2[i]);
                itr = detail_::increment_tuple_of_itr<0>(v2, itr);
            }
        }
        SECTION("0 1") {
            auto itr = detail_::get_begin_tuple<0, 1>(v2);
            for(auto i = 0; i < v1.size(); ++i) {
                REQUIRE(&(*std::get<0>(itr)) == &v2[i]);
                for(auto j = 0; j < v2[i].size(); ++j) {
                    REQUIRE(&(*std::get<1>(itr)) == &v2[i][j]);
                    itr = detail_::increment_tuple_of_itr<0>(v2, itr);
                }
            }
        }
    }
    SECTION("vector of vector of vector") {
        vec3 v3{v2, v2, v2};
        SECTION("0 0") {
            auto itr = detail_::get_begin_tuple<0, 0>(v3);
            for(auto i = 0; i < v3.size(); ++i) {
                REQUIRE(&(*std::get<0>(itr)) == &v3[i]);
                itr = detail_::increment_tuple_of_itr<0>(v3, itr);
            }
        }
        SECTION("0 1") {
            auto itr = detail_::get_begin_tuple<0, 1>(v3);
            for(auto i = 0; i < v3.size(); ++i) {
                REQUIRE(&(*std::get<0>(itr)) == &v3[i]);
                for(auto j = 0; j < v3[i].size(); ++j) {
                    REQUIRE(&(*std::get<1>(itr)) == &v3[i][j]);
                    itr = detail_::increment_tuple_of_itr<0>(v3, itr);
                }
            }
        }
        SECTION("0 2") {
            auto itr = detail_::get_begin_tuple<0, 2>(v3);
            for(auto i = 0; i < v3.size(); ++i) {
                REQUIRE(&(*std::get<0>(itr)) == &v3[i]);
                for(auto j = 0; j < v3[i].size(); ++j) {
                    REQUIRE(&(*std::get<1>(itr)) == &v3[i][j]);
                    for(auto k = 0; k < v3[i][j].size(); ++k) {
                        REQUIRE(&(*std::get<2>(itr)) == &v3[i][j][k]);
                        itr = detail_::increment_tuple_of_itr<0>(v3, itr);
                    }
                }
            }
        }
    }
}

TEST_CASE("FlattenedContainerIterator") {
    vec1 v1{1, 2, 3};
    vec2 v2{v1, v1, v1};
    SECTION("vector") {
        FlattenedContainerIterator<0, decltype(v1)> itr1(&v1, false);
        FlattenedContainerIterator<0, decltype(v1)> itr2(&v1, true);

        REQUIRE(itr1 != itr2);
        std::size_t counter = 0;
        while(itr1 != itr2 && counter < v1.size()) {
            REQUIRE(&(*itr1) == &v1[counter]);
            ++itr1;
            ++counter;
        }
        REQUIRE(itr1 == itr2);
    }
    SECTION("vector of vector") {
        SECTION("Depth 0") {
            FlattenedContainerIterator<0, decltype(v2)> itr1(&v2, false);
            FlattenedContainerIterator<0, decltype(v2)> itr2(&v2, true);

            REQUIRE(itr1 != itr2);
            std::size_t counter = 0;
            while(itr1 != itr2 && counter < v2.size()) {
                REQUIRE(&(*itr1) == &v2[counter]);
                ++itr1;
                ++counter;
            }
            REQUIRE(itr1 == itr2);
        }
        SECTION("Depth 1") {
            FlattenedContainerIterator<1, decltype(v2)> itr1(&v2, false);
            FlattenedContainerIterator<1, decltype(v2)> itr2(&v2, true);

            REQUIRE(itr1 != itr2);
            std::size_t counter_i = 0;
            std::size_t counter_j = 0;
            while(itr1 != itr2 && counter_i < v2.size()) {
                REQUIRE(&(*itr1) == &v2[counter_i][counter_j]);
                ++itr1;
                ++counter_j;
                if(counter_j == v2[counter_i].size()) {
                    counter_j = 0;
                    ++counter_i;
                }
            }
            REQUIRE(itr1 == itr2);
        }
    }
    SECTION("vector of vector of vector") {
        vec3 v3{v2, v2, v2};
        SECTION("Depth 0") {
            FlattenedContainerIterator<0, decltype(v3)> itr1(&v3, false);
            FlattenedContainerIterator<0, decltype(v3)> itr2(&v3, true);

            REQUIRE(itr1 != itr2);
            std::size_t counter = 0;
            while(itr1 != itr2 && counter < v3.size()) {
                REQUIRE(&(*itr1) == &v3[counter]);
                ++itr1;
                ++counter;
            }
            REQUIRE(itr1 == itr2);
        }
        SECTION("Depth 1") {
            FlattenedContainerIterator<1, decltype(v3)> itr1(&v3, false);
            FlattenedContainerIterator<1, decltype(v3)> itr2(&v3, true);

            REQUIRE(itr1 != itr2);
            std::size_t counter_i = 0;
            std::size_t counter_j = 0;
            while(itr1 != itr2 && counter_i < v3.size()) {
                REQUIRE(&(*itr1) == &v3[counter_i][counter_j]);
                ++itr1;
                ++counter_j;
                if(counter_j == v3[counter_i].size()) {
                    counter_j = 0;
                    ++counter_i;
                }
            }
            REQUIRE(itr1 == itr2);
        }
        SECTION("Depth 2") {
            FlattenedContainerIterator<2, decltype(v3)> itr1(&v3, false);
            FlattenedContainerIterator<2, decltype(v3)> itr2(&v3, true);

            REQUIRE(itr1 != itr2);
            std::size_t counter_i = 0;
            std::size_t counter_j = 0;
            std::size_t counter_k = 0;
            while(itr1 != itr2 && counter_i < v3.size()) {
                REQUIRE(&(*itr1) == &v3[counter_i][counter_j][counter_k]);
                ++itr1;
                ++counter_k;
                if(counter_k == v3[counter_i][counter_j].size()) {
                    counter_k = 0;
                    ++counter_j;
                }
                if(counter_j == v3[counter_i].size()) {
                    counter_j = 0;
                    ++counter_i;
                }
            }
            REQUIRE(itr1 == itr2);
        }
    }
}
