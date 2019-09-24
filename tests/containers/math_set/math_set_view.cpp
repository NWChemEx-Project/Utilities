#include <catch2/catch.hpp>
#include <utilities/containers/math_set/math_set_view.hpp>
#include <vector>

using namespace utilities;
using vector_t = std::vector<int>;
using set_t    = MathSetView<int>;

template<typename T, typename U>
static void check_math_set_view(T&& set, U&& corr) {
    SECTION("Size") { REQUIRE(set.size() == corr.size()); }
    SECTION("Empty-ness") { REQUIRE(set.empty() == corr.empty()); }
    SECTION("State") {
        std::size_t counter = 0;
        auto set_begin      = set.begin();
        for(auto& x : corr) {
            REQUIRE(&set[counter] == &x);
            REQUIRE(&(*set_begin) == &x);
            REQUIRE(set.count(x) == 1);
            ++counter;
            ++set_begin;
        }
    }
}

static auto make_pimpl() {
    vector_t v{1, 2, 3};
    std::vector pv{&v[0], &v[1], &v[2]};
    MathSetView s(pv.begin(), pv.end());
    return std::make_pair(std::move(v), std::move(s));
}

TEST_CASE("MathSetView<int>: default ctor") {
    set_t s;
    check_math_set_view(s, vector_t{});
}

TEST_CASE("MathSetView<int>: copy ctor") {
    auto[v, s] = make_pimpl();
    MathSetView s2(s);
    check_math_set_view(s2, v);
}

TEST_CASE("MathSetView<int>: move ctor") {
    auto[v, s]   = make_pimpl();
    auto s_begin = s.begin();
    MathSetView s2(std::move(s));
    check_math_set_view(s2, v);
    SECTION("Iterators remain valid") {
        for(std::size_t i = 0; i < 3; ++i, ++s_begin)
            REQUIRE(&(*s_begin) == &v[i]);
    }
}

TEST_CASE("MathSetView<int>: copy assignment") {
    auto[v, s] = make_pimpl();
    MathSetView<int> s2;
    auto ps2 = &(s2 = s);
    SECTION("Returns this") { REQUIRE(ps2 == &s2); }
    check_math_set_view(s2, v);
}

TEST_CASE("MathSetView<int>: move assignment") {
    auto[v, s] = make_pimpl();
    MathSetView<int> s2;
    auto ps2 = &(s2 = std::move(s));
    SECTION("Returns this") { REQUIRE(ps2 == &s2); }
    check_math_set_view(s2, v);
}

TEST_CASE("MathSetView<int>: range ctor") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[0], &v[1], &v[2]};
    SECTION("Empty list") {
        MathSetView s(pv.begin(), pv.begin());
        check_math_set_view(s, vector_t{});
    }
    SECTION("Full content") {
        MathSetView s(pv.begin(), pv.end());
        check_math_set_view(s, v);
    }
}

TEST_CASE("MathSetView<int>: begin") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[1], &v[2]};
    MathSetView<int> s(pv.begin(), pv.end());
    auto itr = s.begin();
    REQUIRE(&(*itr) == &v[1]);
    ++itr;
    REQUIRE(&(*itr) == &v[2]);
}

TEST_CASE("MathSetView<int>: begin const") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[1], &v[2]};
    const MathSetView<int> s(pv.begin(), pv.end());
    auto itr = s.begin();
    REQUIRE(&(*itr) == &v[1]);
    ++itr;
    REQUIRE(&(*itr) == &v[2]);
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(*itr)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSetView<int>: cbegin") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[1], &v[2]};
    const MathSetView<int> s(pv.begin(), pv.end());
    auto itr = s.cbegin();
    REQUIRE(&(*itr) == &v[1]);
    ++itr;
    REQUIRE(&(*itr) == &v[2]);
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(*itr)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSetView<int>: end") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[1], &v[2]};
    MathSetView<int> s(pv.begin(), pv.end());
    auto itr1 = s.begin();
    auto itr2 = s.end();
    ++itr1;
    ++itr1;
    REQUIRE(itr1 == itr2);
}

TEST_CASE("MathSetView<int>: end const") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[1], &v[2]};
    const MathSetView<int> s(pv.begin(), pv.end());
    auto itr1 = s.begin();
    auto itr2 = s.end();
    ++itr1;
    ++itr1;
    REQUIRE(itr1 == itr2);
}

TEST_CASE("MathSetView<int>: cend") {
    vector_t v{1, 2, 3};
    std::vector pv{&v[1], &v[2]};
    const MathSetView<int> s(pv.begin(), pv.end());
    auto itr1 = s.cbegin();
    auto itr2 = s.cend();
    ++itr1;
    ++itr1;
    REQUIRE(itr1 == itr2);
}

TEST_CASE("MathSetView<int>: operator[]") {
    auto[v, s] = make_pimpl();
    REQUIRE(&s[0] == &v[0]);
    REQUIRE(&s[1] == &v[1]);
}

TEST_CASE("MathSetView<int>: operator[] const") {
    const auto[v, s] = make_pimpl();
    REQUIRE(&s[0] == &v[0]);
    REQUIRE(&s[1] == &v[1]);
    SECTION("Is read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(s[0])>);
    }
}

TEST_CASE("MathSetView<int> count") {
    auto[v, s] = make_pimpl();
    REQUIRE(s.count(2) == 1);
    REQUIRE(s.count(0) == 0);
}

TEST_CASE("MathSetView<int> empty") {
    auto[v, s] = make_pimpl();
    MathSetView<int> s2;
    REQUIRE(s2.empty());
    REQUIRE_FALSE(s.empty());
}

TEST_CASE("MathSetView<int> size") {
    auto[v, s] = make_pimpl();
    MathSetView<int> s2;
    REQUIRE(s2.size() == 0);
    REQUIRE(s.size() == 3);
}

TEST_CASE("MathSetView<int> : operator const MathSet<int>&") {
    auto[v, s] = make_pimpl();
    const MathSet<int>& s2(s);

    for(std::size_t i = 0; i < 2; ++i) {
        SECTION("Is an alias") { REQUIRE(&s[i] == &s2[i]); }
        SECTION("Correct value") { REQUIRE(s[i] == s2[i]); }
    }
}
