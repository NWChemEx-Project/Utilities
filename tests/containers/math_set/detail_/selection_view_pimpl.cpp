#include <catch2/catch.hpp>
#include <utilities/containers/math_set/detail_/selection_view_pimpl.hpp>
#include <vector>

using namespace utilities::detail_;
using vector_t = std::vector<int>;

TEST_CASE("SelectionViewPIMPL range ctor") {
    SECTION("Empty set") {
        vector_t v;
        SelectionViewPIMPL p(v.begin(), v.end(), &v);
        REQUIRE(v.size() == 0);
        REQUIRE_THROWS_AS(p[0], std::out_of_range);
    }
    SECTION("Non-empty set") {
        vector_t v{1, 2, 3};
        vector_t idx{0, 2};
        SelectionViewPIMPL p(idx.begin(), idx.end(), &v);
        REQUIRE(p.size() == 2);
        REQUIRE(p[0] == 1);
        REQUIRE(&p[0] == &v[0]);
        REQUIRE(p[1] == 3);
        REQUIRE(&p[1] == &v[2]);
    }
}

TEST_CASE("SelectionViewPIMPL get") {
    SECTION("Empty set") {
        vector_t v;
        SelectionViewPIMPL p(v.begin(), v.end(), &v);
        p.insert(0);
        REQUIRE(v == vector_t{0});
        REQUIRE(&p[0] == &v[0]);
    }
    SECTION("Non-empty set") {
        vector_t v{1, 2, 3, 4};
        vector_t idx{0, 2};
        SelectionViewPIMPL p(idx.begin(), idx.end(), &v);
        REQUIRE(p[0] == 1);
        REQUIRE(&p[0] == &v[0]);
        REQUIRE(p[1] == 3);
        REQUIRE(&p[1] == &v[2]);
    }
}

TEST_CASE("SelectionViewPIMPL insert") {
    SECTION("empty") {
        vector_t vec;
        vector_t indices;
        SelectionViewPIMPL p(indices.begin(), indices.end(), &vec);

        p.insert(0);
        REQUIRE(p.size() == 1);
        REQUIRE(&p[0] == &vec[0]);
        REQUIRE(vec == vector_t{0});

        SECTION("At the beginning") {
            p.insert(p.begin(), 4);
            REQUIRE(p.size() == 2);
            REQUIRE(&p[0] == &vec[1]);
            REQUIRE(&p[1] == &vec[0]);
            REQUIRE(vec == vector_t{0, 4});
        }

        SECTION("At the end") {
            p.insert(p.end(), 4);
            REQUIRE(p.size() == 2);
            REQUIRE(&p[0] == &vec[0]);
            REQUIRE(&p[1] == &vec[1]);
            REQUIRE(vec == vector_t{0, 4});
        }
    }
    SECTION("Non-empty") {
        vector_t vec{1, 2, 3};
        std::vector indices{0, 2};
        SelectionViewPIMPL p(indices.begin(), indices.end(), &vec);

        SECTION("At the beginning") {
            p.insert(p.begin(), 4);
            REQUIRE(p.size() == 3);
            std::vector<int> corr{4, 1, 3};
            auto itr = p.begin();
            for(std::size_t i = 0; i < 3; ++i, ++itr) {
                REQUIRE(*itr == corr[i]);
            }
            REQUIRE(vec == std::vector<int>{1, 2, 3, 4});
        }

        SECTION("At the end") {
            p.insert(p.end(), 4);
            REQUIRE(p.size() == 3);
            std::vector<int> corr{1, 3, 4};
            auto itr = p.begin();
            for(std::size_t i = 0; i < 3; ++i, ++itr) REQUIRE(*itr == corr[i]);
            REQUIRE(vec == std::vector<int>{1, 2, 3, 4});
        }
    }
}

TEST_CASE("SelectionViewPIMPL size") {
    SECTION("Empty set") {
        vector_t v;
        SelectionViewPIMPL p(v.begin(), v.end(), &v);
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty set") {
        vector_t v{1, 2, 3, 4, 5};
        vector_t idx{0, 3};
        SelectionViewPIMPL p(idx.begin(), idx.end(), &v);
        REQUIRE(p.size() == 2);
    }
}
