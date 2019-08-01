#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set/range_view_pimpl.hpp>
#include <utilities/containers/detail_/math_set/set_pimpl.hpp>
#include <vector>

using namespace utilities::detail_;
using vector_t = std::vector<int>;

TEST_CASE("RangeViewPIMPL parent ctor") {
    SetPIMPL s{1, 2, 3};
    RangeViewPIMPL p(&s);
    REQUIRE(p.size() == 0);
}

TEST_CASE("RangeViewPIMPL range ctor") {
    SECTION("Empty range") {
        SetPIMPL<int> s;
        RangeViewPIMPL p(0, 0, &s);
        REQUIRE(p.size() == 0);
        REQUIRE_THROWS_AS(p[0], std::out_of_range);
    }
    SECTION("Actual range") {
        SetPIMPL vec{1, 2, 3};
        RangeViewPIMPL p(1, 2, &vec);
        STATIC_REQUIRE(std::is_same_v<typename decltype(p)::value_type, int>);

        REQUIRE(p.size() == 1);
        REQUIRE(&p[0] == &(vec[1]));
    }
}

TEST_CASE("RangeViewPIMPL get") {
    SetPIMPL s{1, 2, 3, 4, 5};
    RangeViewPIMPL p(1, 3, &s);
    REQUIRE(&p[0] == &s[1]);
    REQUIRE(p[0] == 2);
    REQUIRE(&p[1] == &s[2]);
    REQUIRE(p[1] == 3);

    SECTION("get when insert breaks range") {
        p.insert(5);
        REQUIRE(&p[0] == &s[1]);
        REQUIRE(&p[1] == &s[2]);
        REQUIRE(&p[2] == &s[4]);
    }
}

TEST_CASE("RangeViewPIMPL insert") {
    SECTION("Empty parent, empty range") {
        SetPIMPL<int> p;
        RangeViewPIMPL s(&p);
        s.insert(3);
        REQUIRE(s.size() == 1);
        REQUIRE(p == SetPIMPL{3});
        REQUIRE(&p[0] == &s[0]);
    }

    SECTION("Empty range") {
        SetPIMPL p{1, 2, 3, 4, 5};
        RangeViewPIMPL s(&p);

        SECTION("Element from parent") {
            s.insert(2);
            REQUIRE(s.size() == 1);
            REQUIRE(&s[0] == &p[1]);
            REQUIRE(s[0] == 2);
        }
        SECTION("New element") {
            s.insert(6);
            REQUIRE(s.size() == 1);
            REQUIRE(&s[0] == &p[0]);
            REQUIRE(p == SetPIMPL{6, 1, 2, 3, 4, 5});
        }
    }
    SECTION("Non-empty range") {
        SetPIMPL p{1, 2, 3, 4, 5};
        RangeViewPIMPL s(1, 3, &p);
        SECTION("Insert element 0 from parent") {
            SECTION("At beginning") {
                s.insert(s.begin(), 1);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[0]);
                REQUIRE(&s[1] == &p[1]);
                REQUIRE(&s[2] == &p[2]);
            }
            SECTION("Anywhere else...") {
                s.insert(s.begin() + 1, 1);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[1]);
                REQUIRE(&s[1] == &p[0]);
                REQUIRE(&s[2] == &p[2]);
                REQUIRE(p == SetPIMPL{1, 2, 3, 4, 5});
            }
        }
        SECTION("Insert element 3 from parent") {
            SECTION("At end") {
                s.insert(s.end(), 4);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[1]);
                REQUIRE(&s[1] == &p[2]);
                REQUIRE(&s[2] == &p[3]);
                REQUIRE(p == SetPIMPL{1, 2, 3, 4, 5});
            }
            SECTION("Anywhere else...") {
                s.insert(s.begin() + 1, 4);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[1]);
                REQUIRE(&s[1] == &p[3]);
                REQUIRE(&s[2] == &p[2]);
                REQUIRE(p == SetPIMPL{1, 2, 3, 4, 5});
            }
        }
        SECTION("Insert element 4 from parent") {
            s.insert(s.begin() + 1, 5);
            REQUIRE(&s[0] == &p[1]);
            REQUIRE(&s[1] == &p[4]);
            REQUIRE(&s[2] == &p[2]);
        }
        SECTION("Insert new element") {
            SECTION("At beginning") {
                s.insert(s.begin(), 9);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[1]);
                REQUIRE(&s[1] == &p[2]);
                REQUIRE(&s[2] == &p[3]);
                REQUIRE(p == SetPIMPL{1, 9, 2, 3, 4, 5});
            }
            SECTION("At end") {
                s.insert(s.end(), 9);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[1]);
                REQUIRE(&s[1] == &p[2]);
                REQUIRE(&s[2] == &p[3]);
                REQUIRE(p == SetPIMPL{1, 2, 3, 9, 4, 5});
            }
            SECTION("Somewhere else") {
                s.insert(s.begin() + 1, 9);
                REQUIRE(s.size() == 3);
                REQUIRE(&s[0] == &p[1]);
                REQUIRE(&s[1] == &p[2]);
                REQUIRE(&s[2] == &p[3]);
                REQUIRE(p == SetPIMPL{1, 2, 9, 3, 4, 5});
            }
        }
    }
}

TEST_CASE("RangeViewPIMPL size") {
    SECTION("Empty set") {
        SetPIMPL<int> v;
        RangeViewPIMPL p(&v);
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty set") {
        SetPIMPL v{1, 2, 3, 4, 5};
        RangeViewPIMPL p(1, 3, &v);
        REQUIRE(p.size() == 2);
    }
}

TEST_CASE("RangeViewPIMPL clear") {
    SetPIMPL p{1, 2, 3, 4};
    RangeViewPIMPL s(1, 2, &p);
    REQUIRE(s.size() == 1);

    SECTION("Normal clear") {
        s.clear();
        REQUIRE(s.size() == 0);
        REQUIRE(p == SetPIMPL{1, 3, 4});
    }
    SECTION("Base class instance") {
        s.insert(4);
        REQUIRE(s.size() == 2);
        s.clear();
        REQUIRE(s.size() == 0);
        REQUIRE(p == SetPIMPL{1, 3});
    }
}

TEST_CASE("RangeViewPIMPL erase") {
    SetPIMPL s{1, 2, 3, 4, 5};
    RangeViewPIMPL p(1, 4, &s);
    SECTION("Erase first element") {
        p.erase(2);
        REQUIRE(p.size() == 2);
        REQUIRE(&p[0] == &s[1]);
        REQUIRE(&p[1] == &s[2]);
        REQUIRE(s == SetPIMPL{1, 3, 4, 5});
    }
    SECTION("Erase middle element") {
        p.erase(3);
        REQUIRE(p.size() == 2);
        REQUIRE(&p[0] == &s[1]);
        REQUIRE(&p[1] == &s[2]);
        REQUIRE(s == SetPIMPL{1, 2, 4, 5});
    }
    SECTION("Erase end element") {
        p.erase(4);
        REQUIRE(p.size() == 2);
        REQUIRE(&p[0] == &s[1]);
        REQUIRE(&p[1] == &s[2]);
        REQUIRE(s == SetPIMPL{1, 2, 3, 5});
    }
}
