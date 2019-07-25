#include <catch2/catch.hpp>
#include <utilities/iterators/offset_iterator.hpp>

using namespace utilities::iterators;
using vector_t = std::vector<int>;

TEST_CASE("offset_itr_value_t") {
    STATIC_REQUIRE(std::is_same_v<detail_::offset_itr_value_t<vector_t>, int>);
    STATIC_REQUIRE(
      std::is_same_v<detail_::offset_itr_value_t<const vector_t>, const int>);
}

TEST_CASE("OffsetIterator wrapping ctor : non-const") {
    vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);

    SECTION("Type deduction") {
        using test_t = decltype(itr);
        STATIC_REQUIRE(std::is_same_v<test_t, OffsetIterator<vector_t>>);
        STATIC_REQUIRE(std::is_same_v<typename test_t::reference, int&>);
    }

    REQUIRE(&(*itr) == &corr[0]);
}

TEST_CASE("OffsetIterator wrapping ctor : const") {
    const vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);

    SECTION("Type deduction") {
        using test_t = decltype(itr);

        STATIC_REQUIRE(std::is_same_v<test_t, OffsetIterator<const vector_t>>);
        STATIC_REQUIRE(std::is_same_v<typename test_t::reference, const int&>);
    }

    REQUIRE(&(*itr) == &corr[0]);
}

TEST_CASE("OffsetIterator: dereference") {
    vector_t corr{1, 2, 3};

    OffsetIterator s(0, &corr);
    REQUIRE(*s == 1);
    *s = 9;
    REQUIRE(*s == 9);
}

TEST_CASE("OffsetIterator: increment") {
    vector_t corr{1, 2, 3};

    OffsetIterator s(0, &corr);
    auto ps = &(++s);
    REQUIRE(ps == &s);
    REQUIRE(*s == 2);
}

TEST_CASE("OffsetIterator: decrement") {
    vector_t corr{1, 2, 3};

    OffsetIterator s(1, &corr);
    auto ps = &(--s);
    REQUIRE(ps == &s);
    REQUIRE(*s == 1);
}

TEST_CASE("OffsetIterator: are_equal") {
    vector_t corr{1, 2, 3};
    OffsetIterator s{0, &corr};

    SECTION("Same container") {
        OffsetIterator s1{0, &corr};
        REQUIRE(s == s1);
        ++s1;
        REQUIRE(s != s1);
    }

    SECTION("Different container instance") {
        vector_t corr2{1, 2, 3};
        OffsetIterator s1{0, &corr2};
        REQUIRE(s != s1);
    }
}

TEST_CASE("OffsetIterator: advance") {
    vector_t corr{1, 2, 3};

    OffsetIterator s(0, &corr);
    auto ps = &(s += 2);
    REQUIRE(ps == &s);
    REQUIRE(*s == 3);

    s -= 2;
    REQUIRE(*s == 1);
}

TEST_CASE("OffsetIterator: distance_to") {
    vector_t corr{1, 2, 3};
    OffsetIterator s(0, &corr);
    OffsetIterator s1(0, &corr);
    REQUIRE(s1 - s == 0);
    ++s1;
    REQUIRE(s1 - s == 1);
    REQUIRE(s - s1 == -1);
    ++s1;
    REQUIRE(s1 - s == 2);
    REQUIRE(s - s1 == -2);
}

TEST_CASE("OffsetIterator copy ctor : non-const") {
    vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);
    OffsetIterator itr1(itr);

    SECTION("Type deduction") {
        using test_t = decltype(itr1);
        STATIC_REQUIRE(std::is_same_v<test_t, OffsetIterator<vector_t>>);
        STATIC_REQUIRE(std::is_same_v<typename test_t::reference, int&>);
    }

    REQUIRE(itr1 == itr);
}

TEST_CASE("OffsetIterator copy ctor : const") {
    const vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);
    OffsetIterator itr1(itr);

    SECTION("Type deduction") {
        using test_t = decltype(itr1);
        STATIC_REQUIRE(std::is_same_v<test_t, OffsetIterator<const vector_t>>);
        STATIC_REQUIRE(std::is_same_v<typename test_t::reference, const int&>);
    }

    REQUIRE(itr1 == itr);
}

TEST_CASE("OffsetIterator copy assignment") {
    vector_t corr{1, 2, 3};

    OffsetIterator s(0, &corr);
    OffsetIterator s1(1, &corr);
    auto ps1 = &(s1 = s);
    REQUIRE(ps1 == &s1);
    REQUIRE(s1 == s);
}

TEST_CASE("OffsetIterator move ctor : non-const") {
    vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);
    OffsetIterator itr1(itr);
    OffsetIterator itr2(std::move(itr));

    SECTION("Type deduction") {
        using test_t = decltype(itr2);
        STATIC_REQUIRE(std::is_same_v<test_t, OffsetIterator<vector_t>>);
        STATIC_REQUIRE(std::is_same_v<typename test_t::reference, int&>);
    }

    REQUIRE(itr1 == itr2);
}

TEST_CASE("OffsetIterator move ctor : const") {
    const vector_t corr{1, 2, 3};

    OffsetIterator itr(0, &corr);
    OffsetIterator itr1(itr);
    OffsetIterator itr2(std::move(itr));

    SECTION("Type deduction") {
        using test_t = decltype(itr2);
        STATIC_REQUIRE(std::is_same_v<test_t, OffsetIterator<const vector_t>>);
        STATIC_REQUIRE(std::is_same_v<typename test_t::reference, const int&>);
    }

    REQUIRE(itr1 == itr2);
}

TEST_CASE("OffsetIterator move assignment") {
    vector_t corr{1, 2, 3};

    OffsetIterator s(0, &corr);
    OffsetIterator s1(1, &corr);
    OffsetIterator s2(s);
    auto ps1 = &(s1 = std::move(s));
    REQUIRE(ps1 == &s1);
    REQUIRE(s1 == s2);
}
