#include <catch2/catch.hpp>
#include <utilities/containers/math_set/detail_/math_set_view_pimpl.hpp>
#include <utilities/iter_tools/zip.hpp>

using namespace utilities::detail_;

using vector_t = std::vector<int>;

template<typename T, typename U>
static void check_math_set_view_pimpl(T&& view, U&& corr) {
    SECTION("Size") { REQUIRE(view.size() == corr.size()); }
    SECTION("Elements") {
        std::size_t counter = 0;
        auto view_begin     = view.begin();
        for(auto& x : corr) {
            REQUIRE(view.count(x) == 1);
            REQUIRE(&x == &view[counter]);
            REQUIRE(&(*view_begin) == &x);
            ++counter;
            ++view_begin;
        }
    }
}

TEST_CASE("MathSetViewPIMPL<int>: default ctor") {
    MathSetViewPIMPL<int> s;
    check_math_set_view_pimpl(s, vector_t{});
    SECTION("For empty container begin() == end()") {
        REQUIRE(s.begin() == s.end());
    }
}

TEST_CASE("MathSetViewPIMPL<int>: non-const views") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[0], &v[1], &v[2]};
    MathSetViewPIMPL<int> s(pv);
    check_math_set_view_pimpl(s, v);
}

TEST_CASE("MathSetViewPIMPL<int>: get_") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[0], &v[1], &v[2]};
    MathSetViewPIMPL<int> s(pv);
    SECTION("Bad index throws") { REQUIRE_THROWS_AS(s[3], std::out_of_range); }
    check_math_set_view_pimpl(s, v);
}

TEST_CASE("MathSetViewPIMPL count") {
    SECTION("empty") {
        MathSetViewPIMPL<int> s;
        REQUIRE(s.count(2) == 0);
    }
    SECTION("full") {
        vector_t v{1, 2, 3};
        std::vector pv{&v[0], &v[1], &v[2]};
        MathSetViewPIMPL<int> s(pv);
        REQUIRE(s.count(2) == 1);
        REQUIRE(s.count(0) == 0);
    }
}

TEST_CASE("MathSetViewPIMPL push_back") {
    MathSetViewPIMPL<int> s;
    REQUIRE_THROWS_AS(s.push_back(3), std::runtime_error);
}

TEST_CASE("MathSetViewPIMPL size") {
    SECTION("empty") {
        MathSetViewPIMPL<int> s;
        REQUIRE(s.size() == 0);
    }
    SECTION("full") {
        vector_t v{1, 2, 3};
        std::vector pv{&v[0], &v[1], &v[2]};
        MathSetViewPIMPL<int> s(pv);
        REQUIRE(s.size() == 3);
    }
}
