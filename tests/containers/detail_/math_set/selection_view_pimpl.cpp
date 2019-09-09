#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set/selection_view_pimpl.hpp>
#include <utilities/containers/detail_/math_set/set_pimpl.hpp>
#include <vector>

using namespace utilities::detail_;
using vector_t = std::vector<std::size_t>;

TEST_CASE("SelectionViewPIMPL parent ctor") {
    SetPIMPL<int> s{1, 2, 3, 4, 5};
    SECTION("Empty set") {
        SelectionViewPIMPL p(&s, vector_t{});
        REQUIRE(p.size() == 0);
    }
    SECTION("Legit set") {
        vector_t v{2, 4};
        SelectionViewPIMPL p(&s, std::move(v));
        REQUIRE(p.size() == 2);
        REQUIRE(p[0] == 3);
        REQUIRE(p[1] == 5);
    }
}

TEST_CASE("SelectionViewPIMPL get") {
    SetPIMPL v{1, 2, 3, 4};
    vector_t idx{0, 2};
    SelectionViewPIMPL p(&v, idx);
    REQUIRE(p[0] == 1);
    REQUIRE(&p[0] == &v[0]);
    REQUIRE(p[1] == 3);
    REQUIRE(&p[1] == &v[2]);
}

TEST_CASE("SelectionViewPIMPL size") {
    SECTION("Empty set") {
        SetPIMPL<int> v;
        SelectionViewPIMPL p(&v, vector_t{});
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty set") {
        SetPIMPL v{1, 2, 3, 4, 5};
        vector_t idx{0, 3};
        SelectionViewPIMPL p(&v, std::move(idx));
        REQUIRE(p.size() == 2);
    }
}

TEST_CASE("SelectionViewPIMPL push_back") {
    SetPIMPL s{1, 2, 3, 4, 5};
    vector_t v{1, 2, 3};
    SelectionViewPIMPL p(&s, std::move(v));
    REQUIRE_THROWS_AS(p.push_back(9), std::runtime_error);
}
