#include <catch2/catch.hpp>
#include <utilities/containers/detail_/set_pimpl.hpp>
#include <utilities/iter_tools/zip.hpp>

using namespace utilities::detail_;

TEST_CASE("setPIMPL default ctor") {
    SetPIMPL<int> s;
    REQUIRE(s.size() == 0);
    REQUIRE(s.count(0) == 0);
}

TEST_CASE("SetPIMPL initializer list ctor") {
    SetPIMPL s{1, 2, 3};
    std::vector<int> corr{1, 2, 3};

    REQUIRE(s.size() == 3);
    REQUIRE(s.count(9) == 0);
    for(const auto & [x, y] : utilities::Zip(s, corr)) {
        REQUIRE(x == y);
        REQUIRE(s.count(x) == 1);
    }
}

TEST_CASE("SetPIMPL range ctor") {
    std::vector<int> corr{1, 2, 3};
    SetPIMPL s(corr.begin(), corr.end());

    REQUIRE(s.size() == corr.size());
    REQUIRE(s.count(9) == 0);
    for(const auto & [x, y] : utilities::Zip(s, corr)) {
        REQUIRE(x == y);
        REQUIRE(s.count(x) == 1);
    }
}

TEST_CASE("SetPIMPL begin") {
    SetPIMPL s1{1, 2, 3};
    auto b = s1.begin();

    SECTION("Can write to the value") {
        *b = 9;
        REQUIRE(*b == 9);
    }

    SECTION("Can iterate over container") {
        REQUIRE(*b == 1);
        ++b;
        REQUIRE(*b == 2);
        ++b;
        REQUIRE(*b == 3);
    }
}

TEST_CASE("SetPIMPL begin const") {
    const SetPIMPL s1{1, 2, 3};
    auto b = s1.begin();
    STATIC_REQUIRE(std::is_const_v<std::remove_reference_t<decltype(*b)>>);
    REQUIRE(*b == 1);
    ++b;
    REQUIRE(*b == 2);
    ++b;
    REQUIRE(*b == 3);
}

TEST_CASE("SetPIMPL end") {
    SetPIMPL s1{1, 2, 3};
    auto b = s1.begin();
    std::advance(b, 3);
    REQUIRE(b == s1.end());
}

TEST_CASE("SetPIMPL end const") {
    const SetPIMPL s1{1, 2, 3};
    auto b = s1.begin();
    std::advance(b, 3);
    REQUIRE(b == s1.end());
}

TEST_CASE("SetPIMPL size") {
    SetPIMPL<int> s;
    REQUIRE(s.size() == 0);
    s.insert(1);
    REQUIRE(s.size() == 1);
    s.insert(1);
    REQUIRE(s.size() == 1);
    s.insert(2);
    REQUIRE(s.size() == 2);
}

TEST_CASE("SetPIMPL count") {
    SetPIMPL<int> s;
    REQUIRE(s.count(0) == 0);
    s.insert(0);
    REQUIRE(s.count(0) == 1);
    s.insert(0);
    REQUIRE(s.count(0) == 1);
}

TEST_CASE("SetPIMPL insert") {
    SetPIMPL<int> s;
    REQUIRE(s.size() == 0);
    s.insert(0);
    REQUIRE(s.count(0) == 1);
    REQUIRE(s.size() == 1);
    s.insert(0);
    REQUIRE(s.count(0) == 1);
    REQUIRE(s.size() == 1);
}

TEST_CASE("SetPIMPL copy ctor") {
    SetPIMPL s{1, 2, 3};
    SetPIMPL s2(s);
    REQUIRE(s2 == s);

    SECTION("Is a deep copy") {
        const auto pe01 = &(*s.begin());
        const auto pe02 = &(*s2.begin());
        REQUIRE(pe01 != pe02);
    }
}

TEST_CASE("SetPIMPL copy assignment") {
    SetPIMPL s{1, 2, 3};
    SetPIMPL<int> s2;
    auto ps2 = &(s2 = s);
    REQUIRE(ps2 == &s2);
    REQUIRE(s2 == s);

    SECTION("Is a deep copy") {
        const auto pe01 = &(*s.begin());
        const auto pe02 = &(*s2.begin());
        REQUIRE(pe01 != pe02);
    }
}

TEST_CASE("SetPIMPL move ctor") {
    SetPIMPL s{1, 2, 3};
    SetPIMPL s2(s);
    SetPIMPL s3(std::move(s2));

    REQUIRE(s3 == s);
}

TEST_CASE("SetPIMPL move assignment") {
    SetPIMPL s{1, 2, 3};
    SetPIMPL s2(s);
    SetPIMPL<int> s3;
    auto ps3 = &(s3 = std::move(s2));
    REQUIRE(ps3 == &s3);
    REQUIRE(s3 == s);
}

TEST_CASE("SetPIMPL equality") {
    SetPIMPL s1{1, 2, 3};

    SECTION("Default constructed") {
        SetPIMPL<int> s2;

        REQUIRE(s1 != s2);
        REQUIRE_FALSE(s1 == s2);

        SECTION("Another default constructed instance") {
            SetPIMPL<int> s3;
            REQUIRE(s2 == s3);
            REQUIRE_FALSE(s2 != s3);
        }
    }

    SECTION("Same contents") {
        SetPIMPL s2{1, 2, 3};
        REQUIRE(s1 == s2);
        REQUIRE_FALSE(s1 != s2);
    }

    SECTION("Different contents") {
        SECTION("Different elements") {
            SetPIMPL s2{4, 5, 5};
            REQUIRE(s1 != s2);
            REQUIRE_FALSE(s1 == s2);
        }

        SECTION("Different lengths") {
            SetPIMPL s2{4, 5};
            REQUIRE(s1 != s2);
            REQUIRE_FALSE(s1 == s2);
        }
    }
}
