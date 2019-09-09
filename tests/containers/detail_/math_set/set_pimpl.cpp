#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set/set_pimpl.hpp>
#include <utilities/iter_tools/zip.hpp>

using namespace utilities::detail_;

TEST_CASE("setPIMPL default ctor") {
    SetPIMPL<int> s;
    REQUIRE(s.size() == 0);
    REQUIRE(s.count(0) == 0);
    REQUIRE(s.begin() == s.end());
}

TEST_CASE("SetPIMPL initializer list ctor") {
    SECTION("Single element initializer list") {
        SetPIMPL s{1};
        std::vector<int> corr{1};
        REQUIRE(s.size() == 1);
        for(const auto & [x, y] : utilities::Zip(s, corr)) {
            REQUIRE(x == y);
            REQUIRE(s.count(x) == 1);
        }
    }
    SECTION("Multiple element initializer list") {
        SetPIMPL s{1, 2, 3};
        std::vector<int> corr{1, 2, 3};
        REQUIRE(s.size() == 3);
        for(const auto & [x, y] : utilities::Zip(s, corr)) {
            REQUIRE(x == y);
            REQUIRE(s.count(x) == 1);
        }
    }
    SECTION("Multiple element initializer list with duplicates") {
        SetPIMPL s{1, 1, 2};
        REQUIRE(s.size() == 2);
        REQUIRE(s[0] == 1);
        REQUIRE(s[1] == 2);
    }
}

TEST_CASE("SetPIMPL range ctor") {
    SECTION("Empty range") {
        std::vector<int> v;
        SetPIMPL s(v.begin(), v.end());
        REQUIRE(s.size() == 0);
    }
    SECTION("Full range") {
        std::vector<int> corr{1, 2, 3};
        SetPIMPL s(corr.begin(), corr.end());

        REQUIRE(s.size() == corr.size());
        REQUIRE(s.count(9) == 0);
        for(const auto & [x, y] : utilities::Zip(s, corr)) {
            REQUIRE(x == y);
            REQUIRE(s.count(x) == 1);
        }
    }
    SECTION("Full range with duplicates") {
        std::vector<int> corr{1, 1, 2};
        SetPIMPL s(corr.begin(), corr.end());

        REQUIRE(s.size() == 2);
        REQUIRE(s[0] == 1);
        REQUIRE(s[1] == 2);
    }
}

TEST_CASE("SetPIMPL operator[]") {
    SetPIMPL s{1, 2, 3};

    REQUIRE_THROWS_AS(s[3], std::out_of_range);
    SECTION("Initial values") {
        REQUIRE(s[0] == 1);
        REQUIRE(s[1] == 2);
        REQUIRE(s[2] == 3);
    }
    SECTION("Are read/writeable") {
        s[0] = 9;
        REQUIRE(s[0] == 9);
    }
}

TEST_CASE("SetPIMPL operator[] const") {
    const SetPIMPL s{1, 2, 3};

    REQUIRE_THROWS_AS(s[3], std::out_of_range);
    SECTION("Initial values") {
        REQUIRE(s[0] == 1);
        REQUIRE(s[1] == 2);
        REQUIRE(s[2] == 3);
    }
    SECTION("Are read-only") {
        using no_ref = std::remove_reference_t<decltype(s[0])>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
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
    s.push_back(1);
    REQUIRE(s.size() == 1);
    s.push_back(1);
    REQUIRE(s.size() == 1);
    s.push_back(2);
    REQUIRE(s.size() == 2);
}

TEST_CASE("SetPIMPL count") {
    SetPIMPL<int> s;
    REQUIRE(s.count(0) == 0);
    s.push_back(0);
    REQUIRE(s.count(0) == 1);
    s.push_back(0);
    REQUIRE(s.count(0) == 1);
}

TEST_CASE("SetPIMPL push_back") {
    SetPIMPL<int> s;
    REQUIRE(s.size() == 0);

    s.push_back(0);
    REQUIRE(s.size() == 1);
    REQUIRE(*s.begin() == 0);

    SECTION("Insert same element at end") {
        s.push_back(0);
        REQUIRE(s.size() == 1);
        REQUIRE(s[0] == 0);
    }
    SECTION("Insert different element at end") {
        s.push_back(1);
        REQUIRE(s.size() == 2);
        auto itr = s.begin();
        REQUIRE(*itr == 0);
        ++itr;
        REQUIRE(*itr == 1);
    }
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
            SetPIMPL s2{4, 5, 6};
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

TEST_CASE("SetPIMPL inequality") {
    SetPIMPL<int> s0{1, 2, 3};

    SECTION("Same set") {
        REQUIRE_FALSE(s0 < s0);
        REQUIRE_FALSE(s0 > s0);
        REQUIRE(s0 <= s0);
        REQUIRE(s0 >= s0);
    }

    SECTION("Empty set") {
        SetPIMPL<int> empty;
        REQUIRE_FALSE(empty < empty);
        REQUIRE_FALSE(empty > empty);
        REQUIRE(empty <= empty);
        REQUIRE(empty >= empty);
        REQUIRE(empty < s0);
        REQUIRE_FALSE(empty > s0);
        REQUIRE(empty <= s0);
        REQUIRE_FALSE(empty >= s0);
    }

    SECTION("Smaller set, smaller start") {
        SetPIMPL<int> s1{0, 1};
        REQUIRE(s1 < s0);
        REQUIRE_FALSE(s1 > s0);
        REQUIRE(s1 <= s0);
        REQUIRE_FALSE(s1 >= s0);
    }

    SECTION("Smaller set, same start") {
        SetPIMPL<int> s1{1, 2};
        REQUIRE(s1 < s0);
        REQUIRE_FALSE(s1 > s0);
        REQUIRE(s1 <= s0);
        REQUIRE_FALSE(s1 >= s0);
    }

    SECTION("Smaller set, larger start") {
        SetPIMPL<int> s1{2, 1};
        REQUIRE_FALSE(s1 < s0);
        REQUIRE(s1 > s0);
        REQUIRE_FALSE(s1 <= s0);
        REQUIRE(s1 >= s0);
    }

    SECTION("Same size, smaller start") {
        SetPIMPL<int> s1{0, 1, 2};
        REQUIRE(s1 < s0);
        REQUIRE_FALSE(s1 > s0);
        REQUIRE(s1 <= s0);
        REQUIRE_FALSE(s1 >= s0);
    }

    SECTION("Same size, larger start") {
        SetPIMPL<int> s1{2, 1, 0};
        REQUIRE_FALSE(s1 < s0);
        REQUIRE(s1 > s0);
        REQUIRE_FALSE(s1 <= s0);
        REQUIRE(s1 >= s0);
    }

    SECTION("Larger set, smaller start") {
        SetPIMPL<int> s1{0, 1, 2, 3};
        REQUIRE(s1 < s0);
        REQUIRE_FALSE(s1 > s0);
        REQUIRE(s1 <= s0);
        REQUIRE_FALSE(s1 >= s0);
    }

    SECTION("Larger set, same start") {
        SetPIMPL<int> s1{1, 2, 3, 4};
        REQUIRE_FALSE(s1 < s0);
        REQUIRE(s1 > s0);
        REQUIRE_FALSE(s1 <= s0);
        REQUIRE(s1 >= s0);
    }

    SECTION("Larger set, larger start") {
        SetPIMPL<int> s1{2, 1, 3, 4};
        REQUIRE_FALSE(s1 < s0);
        REQUIRE(s1 > s0);
        REQUIRE_FALSE(s1 <= s0);
        REQUIRE(s1 >= s0);
    }
}
