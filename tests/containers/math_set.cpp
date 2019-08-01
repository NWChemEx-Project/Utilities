#include <catch2/catch.hpp>
#include <utilities/containers/math_set.hpp>
#include <utilities/iter_tools/zip.hpp>
#include <vector>

using namespace utilities;
using vector_t = std::vector<int>;
using set_t    = MathSet<int>;

TEST_CASE("MathSet<int> default ctor") {
    set_t s;

    REQUIRE(s.size() == 0);
    REQUIRE(s.empty());
    REQUIRE(s.begin() == s.end());
    REQUIRE(s.cbegin() == s.cend());
    REQUIRE(s.count(0) == 0);
}

TEST_CASE("MathSet<int> initializer list ctor") {
    SECTION("Empty list") {
        MathSet s(std::initializer_list<int>{});
        REQUIRE(s == MathSet<int>{});
    }
    SECTION("Single entry") {
        SECTION("Non-MathSet content") {
            MathSet s{1};
            REQUIRE(s.size() == 1);
            REQUIRE_FALSE(s.empty());
            REQUIRE(s[0] == 1);
            REQUIRE(*(s.begin()) == 1);
            REQUIRE(*(s.cbegin()) == 1);
            REQUIRE(s.count(1) == 1);
        }
        SECTION("MathSet content") {
            MathSet<set_t> s{set_t{1, 2}};
            REQUIRE(s.size() == 1);
            REQUIRE_FALSE(s.empty());
            REQUIRE(s[0] == set_t{1, 2});
            REQUIRE((*s.begin()) == set_t{1, 2});
            REQUIRE((*s.cbegin()) == set_t{1, 2});
            REQUIRE(s.count(set_t{1, 2}) == 1);
        }
    }

    SECTION("Multiple entries") {
        SECTION("Non-MathSet content") {
            MathSet s{1, 2, 3};
            REQUIRE(s.size() == 3);
            REQUIRE_FALSE(s.empty());
            REQUIRE(s[0] == 1);
            REQUIRE(s[1] == 2);
            REQUIRE(s[2] == 3);

            auto itr = s.begin();
            for(int i = 1; i < 4; ++i, ++itr) {
                REQUIRE(*itr == i);
                REQUIRE(s.count(i) == 1);
            }
        }
        SECTION("MathSet content") {
            MathSet s{set_t{1}, set_t{2, 3}, set_t{}};
            REQUIRE(s.size() == 3);
            REQUIRE_FALSE(s.empty());
            REQUIRE(s[0] == set_t{1});
            REQUIRE(s[1] == set_t{2, 3});
            REQUIRE(s[2] == set_t{});
            REQUIRE(s.count(set_t{1}) == 1);
            REQUIRE(s.count(set_t{2, 3}) == 1);
            REQUIRE(s.count(set_t{}) == 1);
        }
    }
}

TEST_CASE("MathSet<int> range ctor") {
    SECTION("Empty list") {
        vector_t v;
        MathSet s(v.begin(), v.end());
        REQUIRE(s == MathSet<int>{});
    }
    SECTION("Non-MathSet content") {
        vector_t v{1, 2, 3};
        MathSet s(v.begin(), v.end());
        REQUIRE(s == MathSet{1, 2, 3});
    }
    SECTION("MathSet content") {
        std::vector v{set_t{1}, set_t{2, 3}, set_t{}};
        MathSet s(v.begin(), v.end());
        REQUIRE(s == MathSet{set_t{1}, set_t{2, 3}, set_t{}});
    }
}

TEST_CASE("MathSet copy ctor") {
    SECTION("Normal elements") {
        MathSet s{1, 2, 3};
        MathSet s2(s);
        REQUIRE(s == s2);
        REQUIRE_FALSE(&s[0] == &s2[0]);
        REQUIRE_FALSE(&s[1] == &s2[1]);
        REQUIRE_FALSE(&s[2] == &s2[2]);
    }
    SECTION("MathSet elements") {
        MathSet s{set_t{1}, set_t{}};
        MathSet s2{s};
        REQUIRE(s == s2);
        REQUIRE_FALSE(&s[0][0] == &s2[0][0]);
    }
}

// TEST_CASE("MathSet move ctor") {
//    SECTION("Normal elements") {
//        MathSet s{1, 2, 3};
//        MathSet s2(s);
//        MathSet s3(std::move(s));
//        REQUIRE(s3 == s2);
//    }
//    SECTION("MathSet elements") {
//        MathSet s{set_t{1}, set_t{}};
//        MathSet s2{s};
//        MathSet s3(std::move(s));
//        REQUIRE(s3 == s2);
//    }
//}

TEST_CASE("MathSet begin") {
    MathSet s{1, 2, 3};
    auto itr = s.begin();

    SECTION("State") {
        for(int i = 1; i < 4; ++i, ++itr) REQUIRE(*itr == i);
    }
    SECTION("Writability") {
        *itr = 9;
        REQUIRE(s[0] == 9);
    }
}

TEST_CASE("MathSet begin const") {
    const MathSet s{1, 2, 3};
    auto itr = s.begin();

    SECTION("State") {
        for(int i = 1; i < 4; ++i, ++itr) REQUIRE(*itr == i);
    }
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(*itr)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSet cbegin") {
    MathSet s{1, 2, 3};
    auto itr = s.cbegin();

    SECTION("State") {
        for(int i = 1; i < 4; ++i, ++itr) REQUIRE(*itr == i);
    }
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(*itr)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSet end") {
    MathSet s{1, 2};
    auto itr = s.begin();
    REQUIRE_FALSE(itr == s.end());
    ++itr;
    REQUIRE_FALSE(itr == s.end());
    ++itr;
    REQUIRE(itr == s.end());
}

TEST_CASE("MathSet end const") {
    const MathSet s{1, 2};
    auto itr = s.begin();
    REQUIRE_FALSE(itr == s.end());
    ++itr;
    REQUIRE_FALSE(itr == s.end());
    ++itr;
    REQUIRE(itr == s.end());
}

TEST_CASE("MathSet cend") {
    MathSet s{1, 2};
    auto itr = s.cbegin();
    REQUIRE_FALSE(itr == s.cend());
    ++itr;
    REQUIRE_FALSE(itr == s.cend());
    ++itr;
    REQUIRE(itr == s.cend());
}

TEST_CASE("MathSet operator[]") {
    MathSet s{1, 2, 3};

    SECTION("State") {
        REQUIRE(s[0] == 1);
        REQUIRE(s[1] == 2);
        REQUIRE(s[2] == 3);
    }
    SECTION("Writability") {
        s[0] = 9;
        REQUIRE(s[0] == 9);
    }
}

TEST_CASE("MathSet operator[] const") {
    const MathSet s{1, 2, 3};

    SECTION("State") {
        REQUIRE(s[0] == 1);
        REQUIRE(s[1] == 2);
        REQUIRE(s[2] == 3);
    }
    SECTION("Read-only") {
        using no_ref = std::remove_reference_t<decltype(s[0])>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSet count") {
    MathSet s{1, 2, 3};
    REQUIRE(s.count(1) == 1);
    REQUIRE(s.count(0) == 0);
}

TEST_CASE("MathSet size") {
    set_t s;
    REQUIRE(s.size() == 0);
    s.insert(1);
    REQUIRE(s.size() == 1);
}

TEST_CASE("MathSet empty") {
    set_t s;
    REQUIRE(s.empty());
    s.insert(2);
    REQUIRE_FALSE(s.empty());
}

TEST_CASE("MathSet insert(elem)") {
    set_t s;
    REQUIRE(s.size() == 0);
    s.insert(1);
    REQUIRE(s[0] == 1);
    REQUIRE(s.size() == 1);
    s.insert(2);
    REQUIRE(s[0] == 1);
    REQUIRE(s[1] == 2);
    REQUIRE(s.size() == 2);
}

TEST_CASE("MathSet insert(pos, elem)") {
    set_t s;

    SECTION("Beginning") {
        s.insert(s.begin(), 1);
        REQUIRE(s.size() == 1);
        REQUIRE(s[0] == 1);

        SECTION("Beginning again") {
            s.insert(s.begin(), 2);
            REQUIRE(s.size() == 2);
            REQUIRE(s[0] == 2);
            REQUIRE(s[1] == 1);

            SECTION("Middle") {
                s.insert(s.begin() + 1, 3);
                REQUIRE(s.size() == 3);
                REQUIRE(s[0] == 2);
                REQUIRE(s[1] == 3);
                REQUIRE(s[2] == 1);
            }
        }

        SECTION("At end") {
            s.insert(s.end(), 2);
            REQUIRE(s.size() == 2);
            REQUIRE(s[0] == 1);
            REQUIRE(s[1] == 2);
        }
    }
    SECTION("End") {
        s.insert(s.end(), 1);
        REQUIRE(s.size() == 1);
        REQUIRE(s[0] == 1);

        SECTION("Beginning") {
            s.insert(s.begin(), 2);
            REQUIRE(s.size() == 2);
            REQUIRE(s[0] == 2);
            REQUIRE(s[1] == 1);
        }

        SECTION("End again") {
            s.insert(s.end(), 2);
            REQUIRE(s.size() == 2);
            REQUIRE(s[0] == 1);
            REQUIRE(s[1] == 2);
        }
    }
}

TEST_CASE("MathSet clear") {
    SECTION("Normal MathSet") {
        MathSet s{1, 2, 3};
        s.clear();
        REQUIRE(s == MathSet<int>{});
    }
}

TEST_CASE("MathSet erase") {
    SECTION("Normal MathSet") {
        MathSet s{1, 2, 3};
        SECTION("Non-existing value") {
            s.erase(4);
            REQUIRE(s == MathSet{1, 2, 3});
        }
        SECTION("Existing value") {
            s.erase(2);
            REQUIRE(s == MathSet{1, 3});
        }
    }
    SECTION("Aliased MathSet") {
        MathSet s{MathSet{1, 2}, MathSet<int>{}, MathSet{2, 3}};
        SECTION("Non-existing value") {
            s.erase(MathSet{1, 3});
            REQUIRE(s == MathSet{MathSet{1, 2}, MathSet<int>{}, MathSet{2, 3}});
        }
        SECTION("Existing value") {
            s.erase(MathSet<int>{});
            REQUIRE(s == MathSet{MathSet{1, 2}, MathSet{2, 3}});
        }
    }
}

TEST_CASE("MathSet intersection") {
    MathSet s{1, 2, 3};

    SECTION("Empty set") {
        set_t s2;
        REQUIRE((s2 ^ s2) == set_t{});
        REQUIRE((s ^ s2) == set_t{});
        REQUIRE((s2 ^ s) == set_t{});
    }
    SECTION("Same set") {
        MathSet s2{1, 2, 3};
        REQUIRE((s ^ s2) == s);
    }
    SECTION("Subset") {
        MathSet s2{1, 2, 5};
        REQUIRE((s ^ s2) == set_t{1, 2});
    }
    SECTION("Superset") {
        MathSet s2{1, 2, 3, 4};
        REQUIRE((s ^ s2) == s);
    }
    SECTION("Writable-ness") {
        MathSet s2{1, 2, 4};
        auto& s3 = s ^ s2;
        REQUIRE(&s3[0] == &s[0]);
        REQUIRE(&s3[1] == &s[1]);
    }
}

TEST_CASE("MathSet intersection const") {
    const MathSet s{1, 2, 3};

    SECTION("Empty set") {
        set_t s2;
        REQUIRE((s2 ^ s2) == set_t{});
        REQUIRE((s ^ s2) == set_t{});
        REQUIRE((s2 ^ s) == set_t{});
    }
    SECTION("Same set") {
        MathSet s2{1, 2, 3};
        REQUIRE((s ^ s2) == s);
    }
    SECTION("Subset") {
        MathSet s2{1, 2, 5};
        REQUIRE((s ^ s2) == set_t{1, 2});
    }
    SECTION("Superset") {
        MathSet s2{1, 2, 3, 4};
        REQUIRE((s ^ s2) == s);
    }
    SECTION("Read-only") {
        MathSet s2{1, 2, 4};
        auto& s3     = s ^ s2;
        using no_ref = std::remove_reference_t<decltype(s3)>;
        STATIC_REQUIRE(std::is_const_v<no_ref>);
    }
}

TEST_CASE("MathSet difference") {
    MathSet s1{1, 2, 3};

    SECTION("Empty set") {
        MathSet<int> empty;
        SECTION("Empty with Empty") {
            auto& s2 = empty - empty;
            REQUIRE(s2 == MathSet<int>{});
        }
        SECTION("Empty with Full") {
            auto& s2 = empty - s1;
            REQUIRE(s2 == MathSet<int>{});
        }
        SECTION("Full with Empty") {
            auto& s2 = s1 - empty;
            REQUIRE(s2 == MathSet{1, 2, 3});
        }
    }
    SECTION("Subset") {
        MathSet s2{1};
        SECTION("Superset with subset") {
            auto& s3 = s1 - s2;
            REQUIRE(s3 == MathSet{2, 3});
        }
        SECTION("Subset with superset") {
            auto& s3 = s2 - s1;
            REQUIRE(s3 == MathSet<int>{});
        }
    }
}

TEST_CASE("MathSet union") {
    MathSet s1{1, 2, 3};

    SECTION("Empyt") {
        MathSet<int> s2;
        auto s3 = s1 + s2;
        REQUIRE(s3 == MathSet{1, 2, 3});
    }

    SECTION("Two full sets") {
        MathSet s2{2, 4, 5};
        auto s3 = s1 + s2;
        REQUIRE(s3 == MathSet{1, 2, 3, 4, 5});
    }
}

TEST_CASE("MathSet equality") {
    MathSet s{1, 2, 3};

    SECTION("Empty set") {
        set_t empty;
        REQUIRE(empty == empty);
        REQUIRE_FALSE(empty != empty);

        REQUIRE_FALSE(s == empty);
        REQUIRE(s != empty);
    }

    SECTION("Same set") {
        MathSet s2{1, 2, 3};

        REQUIRE(s == s2);
        REQUIRE_FALSE(s != s2);
    }

    SECTION("Super set") {
        MathSet s2{1, 2, 3, 4};

        REQUIRE(s != s2);
        REQUIRE_FALSE(s == s2);
    }

    SECTION("Sub set") {
        MathSet s2{1, 2};

        REQUIRE(s != s2);
        REQUIRE_FALSE(s == s2);
    }

    SECTION("Different order") {
        MathSet s2{2, 1, 3};

        REQUIRE(s != s2);
        REQUIRE_FALSE(s == s2);
    }
}
