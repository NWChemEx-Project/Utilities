#include <catch2/catch.hpp>
#include <utilities/containers/math_set/math_set_view.hpp>
#include <vector>

using namespace utilities;
using vector_t = std::vector<int>;
using set_t    = MathSetView<int>;

TEST_CASE("MathSetView<int> default ctor") {
    set_t s;
    REQUIRE(s.size() == 0);
    REQUIRE(s.empty());
    REQUIRE(s.begin() == s.end());
    REQUIRE(s.cbegin() == s.cend());
    REQUIRE(s.count(0) == 0);
}

TEST_CASE("MathSetView<int> list ctor") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    SECTION("Empty list") {
        MathSetView s(std::vector<std::reference_wrapper<int>>{});
        REQUIRE(s.empty());
    }
    SECTION("Full content") {
        MathSetView s(pv);
        REQUIRE(s.size() == 2);
        REQUIRE(s[0] == 2);
        REQUIRE(s[1] == 3);
    }
}

TEST_CASE("MathSetView<int> begin const") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetView<int> s(pv);
    auto itr = s.begin();
    REQUIRE(&(*itr) == &v[1]);
    ++itr;
    REQUIRE(&(*itr) == &v[2]);
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(*itr)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSetView<int> cbegin") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetView<int> s(pv);
    auto itr = s.cbegin();
    REQUIRE(&(*itr) == &v[1]);
    ++itr;
    REQUIRE(&(*itr) == &v[2]);
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(*itr)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSetView<int> end const") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetView<int> s(pv);
    auto itr1 = s.begin();
    auto itr2 = s.end();
    ++itr1;
    ++itr1;
    REQUIRE(itr1 == itr2);
}

TEST_CASE("MathSetView<int> cend") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetView<int> s(pv);
    auto itr1 = s.cbegin();
    auto itr2 = s.cend();
    ++itr1;
    ++itr1;
    REQUIRE(itr1 == itr2);
}

TEST_CASE("MathSetView<int> operator[]") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetView<int> s(pv);
    REQUIRE(&s[0] == &v[1]);
    REQUIRE(&s[1] == &v[2]);
}

TEST_CASE("MathSetView<int> count") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetView<int> s(pv);

    REQUIRE(s.count(2) == 1);
    REQUIRE(s.count(0) == 0);
}

TEST_CASE("MathSetView<int> size") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetView<int> s(pv);
    MathSetView<int> s2;
    REQUIRE(s2.size() == 0);
    REQUIRE(s.size() == 2);
}

TEST_CASE("MathSetView<int> empty") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetView<int> s(pv);
    MathSetView<int> s2;
    REQUIRE(s2.empty());
    REQUIRE_FALSE(s.empty());
}

TEST_CASE("MathSetView<int> implicit MathSet<int> conversion") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetView s(pv);
    MathSet<int> s2(s);
    const MathSet<int> s3(s);
    SECTION("Is a deep copy") {
        for(std::size_t i = 0; i < 2; ++i) {
            REQUIRE(&s[i] != &s2[i]);
            REQUIRE(&s[i] != &s3[i]);
            REQUIRE(s[i] == s2[i]);
            REQUIRE(s[i] == s3[i]);
        }
    }
}

TEST_CASE("MathSetView<int> implicit const MathSet<int>& conversion") {
    vector_t v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetView s(pv);
    const MathSet<int>& s2(s);
    SECTION("Is an alias") {
        for(std::size_t i = 0; i < 2; ++i) {
            REQUIRE(&s[i] == &s2[i]);
            REQUIRE(s[i] == s2[i]);
        }
    }
}
