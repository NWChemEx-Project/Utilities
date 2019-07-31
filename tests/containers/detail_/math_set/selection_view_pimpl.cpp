#include <catch2/catch.hpp>
#include <utilities/containers/detail_/math_set/selection_view_pimpl.hpp>
#include <utilities/containers/detail_/math_set/set_pimpl.hpp>
#include <vector>

using namespace utilities::detail_;
using vector_t = std::vector<int>;

TEST_CASE("SelectionViewPIMPL parent ctor") {
    SetPIMPL<int> s;
    SelectionViewPIMPL p(&s);
    REQUIRE(p.size() == 0);
}

TEST_CASE("SelectionViewPIMPL range ctor") {
    SECTION("Empty set") {
        vector_t v;
        SetPIMPL<int> s;
        SelectionViewPIMPL p(v.begin(), v.end(), &s);
        REQUIRE(v.size() == 0);
        REQUIRE_THROWS_AS(p[0], std::out_of_range);
    }
    SECTION("Non-empty set") {
        SetPIMPL s{1, 2, 3};
        vector_t idx{0, 2};
        SelectionViewPIMPL p(idx.begin(), idx.end(), &s);
        REQUIRE(p.size() == 2);
        REQUIRE(p[0] == 1);
        REQUIRE(&p[0] == &s[0]);
        REQUIRE(p[1] == 3);
        REQUIRE(&p[1] == &s[2]);
    }
}

TEST_CASE("SelectionViewPIMPL assignment") {
    SetPIMPL p{1, 2, 3, 4};
    SelectionViewPIMPL s(p.begin(), p.begin() + 2, &p);

    SECTION("Same elements") {
        s = SetPIMPL{2, 3};
        REQUIRE(&s[0] == &p[1]);
        REQUIRE(&s[1] == &p[2]);
        REQUIRE(p == SetPIMPL{1, 2, 3, 4});
    }
    SECTION("Different elements") {
        s = SetPIMPL{5, 6};
        REQUIRE(&s[0] == &p[2]);
        REQUIRE(&s[1] == &p[3]);
        REQUIRE(p == SetPIMPL{1, 4, 5, 6});
    }
    SECTION("Mix of elements") {
        s = SetPIMPL{3, 5};
        REQUIRE(&s[0] == &p[1]);
        REQUIRE(&s[1] == &p[3]);
        REQUIRE(p == SetPIMPL{1, 3, 4, 5});
    }
}

TEST_CASE("SelectionViewPIMPL get") {
    SECTION("Empty set") {
        SetPIMPL<int> v;
        SelectionViewPIMPL p(&v);
        p.insert(0);
        REQUIRE(&p[0] == &v[0]);
    }
    SECTION("Non-empty set") {
        SetPIMPL v{1, 2, 3, 4};
        vector_t idx{0, 2};
        SelectionViewPIMPL p(idx.begin(), idx.end(), &v);
        REQUIRE(p[0] == 1);
        REQUIRE(&p[0] == &v[0]);
        REQUIRE(p[1] == 3);
        REQUIRE(&p[1] == &v[2]);
    }
}

TEST_CASE("SelectionViewPIMPL insert") {
    SECTION("empty parent and empty indices") {
        SetPIMPL<int> vec;
        SelectionViewPIMPL p(&vec);
        p.insert(0);
        REQUIRE(p.size() == 1);
        REQUIRE(vec.size() == 1);
        REQUIRE(vec[0] == 0);
        REQUIRE(&p[0] == &vec[0]);
    }
    SECTION("Non-empty parent") {
        SetPIMPL<int> vec{1, 2, 3};

        SECTION("Empty indices") {
            SelectionViewPIMPL p(&vec);
            REQUIRE(p.size() == 0);

            SECTION("Add element from vec") {
                p.insert(1);
                REQUIRE(&p[0] == &vec[0]);
                REQUIRE(p[0] == 1);
            }
            SECTION("New element") {
                p.insert(4);
                REQUIRE(vec.size() == 4);
                REQUIRE(&p[0] == &vec[3]);
                REQUIRE(p[0] == 4);
            }
        }
        SECTION("Non-empty indices") {
            vector_t indices{0};
            SelectionViewPIMPL p(indices.begin(), indices.end(), &vec);

            SECTION("At the beginning") {
                SECTION("Element from vec") {
                    p.insert(p.begin(), 3);
                    REQUIRE(p.size() == 2);
                    REQUIRE(&p[0] == &vec[2]);
                    REQUIRE(&p[1] == &vec[0]);
                    REQUIRE(vec == SetPIMPL{1, 2, 3});
                }
                SECTION("New element") {
                    p.insert(p.begin(), 4);
                    REQUIRE(p.size() == 2);
                    REQUIRE(&p[0] == &vec[3]);
                    REQUIRE(&p[1] == &vec[0]);
                    REQUIRE(vec == SetPIMPL{1, 2, 3, 4});
                }
            }

            SECTION("At the end") {
                SECTION("Element from vec") {
                    p.insert(p.end(), 3);
                    REQUIRE(p.size() == 2);
                    REQUIRE(&p[0] == &vec[0]);
                    REQUIRE(&p[1] == &vec[2]);
                    REQUIRE(vec == SetPIMPL{1, 2, 3});
                }
                SECTION("New element") {
                    p.insert(p.end(), 4);
                    REQUIRE(p.size() == 2);
                    REQUIRE(&p[0] == &vec[0]);
                    REQUIRE(&p[1] == &vec[3]);
                    REQUIRE(vec == SetPIMPL{1, 2, 3, 4});
                }
            }
            SECTION("Element from us") {
                p.insert(1);
                REQUIRE(p.size() == 1);
                REQUIRE(p[0] == 1);
            }
        }
    }
}

TEST_CASE("SelectionViewPIMPL size") {
    SECTION("Empty set") {
        SetPIMPL<int> v;
        SelectionViewPIMPL p(&v);
        REQUIRE(p.size() == 0);
    }
    SECTION("Non-empty set") {
        SetPIMPL v{1, 2, 3, 4, 5};
        vector_t idx{0, 3};
        SelectionViewPIMPL p(idx.begin(), idx.end(), &v);
        REQUIRE(p.size() == 2);
    }
}

TEST_CASE("SelectionViewPIMPL clear") {
    SetPIMPL s{1, 2, 3};
    vector_t v{1, 2};
    SelectionViewPIMPL p(v.begin(), v.end(), &s);
    REQUIRE(p.size() == 2);
    p.clear();
    REQUIRE(p.size() == 0);
    REQUIRE(s == SetPIMPL{1});
}

TEST_CASE("SelectionViewPIMPL erase") {
    SetPIMPL s{1, 2, 3, 4, 5};
    vector_t v{1, 2, 3};
    SelectionViewPIMPL p(v.begin(), v.end(), &s);

    SECTION("Erase element not in set") {
        p.erase(5);
        REQUIRE(p.size() == 3);
        REQUIRE(&p[0] == &s[1]);
        REQUIRE(&p[1] == &s[2]);
        REQUIRE(&p[2] == &s[3]);
        REQUIRE(s == SetPIMPL{1, 2, 3, 4, 5});
    }
    SECTION("Element in set") {
        p.erase(2);
        REQUIRE(p.size() == 2);
        REQUIRE(&p[0] == &s[1]);
        REQUIRE(&p[1] == &s[2]);
        REQUIRE(s == SetPIMPL{1, 3, 4, 5});
    }
}
