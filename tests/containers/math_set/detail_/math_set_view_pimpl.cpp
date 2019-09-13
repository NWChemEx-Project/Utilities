#include <catch2/catch.hpp>
#include <utilities/containers/math_set/detail_/math_set_view_pimpl.hpp>
#include <utilities/iter_tools/zip.hpp>

using namespace utilities::detail_;

TEST_CASE("MathSetViewPIMPL default ctor") {
    MathSetViewPIMPL<int> s;
    REQUIRE(s.size() == 0);
    REQUIRE(s.count(0) == 0);
    REQUIRE(s.begin() == s.end());
}

TEST_CASE("MathSetViewPIMPL range ctor") {
    std::vector<int> v{1, 2, 3};
    SECTION("Empty range") {
        std::vector<std::reference_wrapper<int>> pv;
        MathSetViewPIMPL s(pv);
        REQUIRE(s.size() == 0);
    }
    SECTION("Full range") {
        std::vector pv{std::ref(v[1]), std::ref(v[2])};
        MathSetViewPIMPL s(pv);
        REQUIRE(s.size() == 2);
        REQUIRE(&s[0] == &v[1]);
        REQUIRE(&s[1] == &v[2]);
    }
}

TEST_CASE("MathSetViewPIMPL operator[]") {
    std::vector<int> v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetViewPIMPL s(pv);
    REQUIRE_THROWS_AS(s[3], std::out_of_range);
    SECTION("Initial values") {
        REQUIRE(s[0] == 2);
        REQUIRE(s[1] == 3);
    }
}

TEST_CASE("MathSetViewPIMPL operator[] const") {
    std::vector<int> v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetViewPIMPL s(pv);
    REQUIRE_THROWS_AS(s[3], std::out_of_range);
    SECTION("Initial values") {
        REQUIRE(s[0] == 2);
        REQUIRE(s[1] == 3);
    }
    SECTION("Are read-only") {
        using no_ref = std::remove_reference_t<decltype(s[0])>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSetViewPIMPL begin") {
    std::vector<int> v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetViewPIMPL s(pv);
    auto b = s.begin();

    SECTION("Can iterate over container") {
        REQUIRE(*b == 2);
        ++b;
        REQUIRE(*b == 3);
    }
}

TEST_CASE("MathSetViewPIMPL begin const") {
    std::vector<int> v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    const MathSetViewPIMPL s(pv);
    auto b = s.begin();
    STATIC_REQUIRE(std::is_const_v<std::remove_reference_t<decltype(*b)>>);
    REQUIRE(*b == 2);
    ++b;
    REQUIRE(*b == 3);
}

TEST_CASE("MathSetViewPIMPL end") {
    std::vector<int> v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetViewPIMPL s(pv);
    auto b = s.begin();
    std::advance(b, 2);
    REQUIRE(b == s.end());
}

TEST_CASE("MathSetViewPIMPL end const") {
    std::vector<int> v{1, 2, 3};
    std::vector pv{std::ref(v[1]), std::ref(v[2])};
    MathSetViewPIMPL s(pv);
    auto b = s.begin();
    std::advance(b, 2);
    REQUIRE(b == s.end());
}

TEST_CASE("MathSetViewPIMPL size") {
    SECTION("empty") {
        MathSetViewPIMPL<int> s;
        REQUIRE(s.size() == 0);
    }
    SECTION("full") {
        std::vector<int> v{1, 2, 3};
        std::vector pv{std::ref(v[1]), std::ref(v[2])};
        MathSetViewPIMPL s(pv);
        REQUIRE(s.size() == 2);
    }
}

TEST_CASE("MathSetViewPIMPL count") {
    SECTION("empty") {
        MathSetViewPIMPL<int> s;
        REQUIRE(s.count(2) == 0);
    }
    SECTION("full") {
        std::vector<int> v{1, 2, 3};
        std::vector pv{std::ref(v[1]), std::ref(v[2])};
        MathSetViewPIMPL s(pv);
        REQUIRE(s.count(2) == 1);
        REQUIRE(s.count(0) == 0);
    }
}

TEST_CASE("MathSetViewPIMPL push_back") {
    MathSetViewPIMPL<int> s;
    REQUIRE_THROWS_AS(s.push_back(3), std::runtime_error);
}
