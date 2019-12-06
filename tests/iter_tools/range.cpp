#include <catch2/catch.hpp>
#include <utilities/iter_tools/range.hpp>

using namespace utilities;

TEST_CASE("Range<int> : endpoint-only ctor") {
    SECTION("Empty range") {
        Range r(0);
        SECTION("Size") { REQUIRE(r.size() == 0); }
    }
    SECTION("Non-empty") {
        Range r(2);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 0); }
    }
}

TEST_CASE("Range<int> : begin and endpoint ctor") {
    SECTION("Empty range") {
        Range r(0, 0);
        SECTION("Size") { REQUIRE(r.size() == 0); }
    }
    SECTION("0 to n range") {
        Range r(0, 2);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 0); }
    }
    SECTION("0 to n range in m's (n % m == 0)") {
        Range r(0, 4, 2);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 0); }
    }
    SECTION("0 to n range in m's (n % m != 0)") {
        Range r(0, 3, 2);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 0); }
    }
    SECTION("n to m (m > n) range") {
        Range r(1, 3);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 1); }
    }
    SECTION("n to m (m > n) in k's ( n % k == 0)") {
        Range r(1, 3, 2);
        SECTION("Size") { REQUIRE(r.size() == 1); }
        SECTION("Initial value") { REQUIRE(r[0] == 1); }
    }
    SECTION("n to m (m > n) in k's ( n % k != 0)") {
        Range r(1, 4, 2);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 1); }
    }
    SECTION("n to m (n > m) range") {
        Range r(3, 1);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 3); }
    }
    SECTION("n to m (n > m) in k's ( n % k == 0)") {
        Range r(3, 1, 2);
        SECTION("Size") { REQUIRE(r.size() == 1); }
        SECTION("Initial value") { REQUIRE(r[0] == 3); }
    }
    SECTION("n to m (n > m) in k's ( n % k != 0)") {
        Range r(4, 1, 2);
        SECTION("Size") { REQUIRE(r.size() == 2); }
        SECTION("Initial value") { REQUIRE(r[0] == 4); }
    }
}

TEST_CASE("Range<int> : size") {
    SECTION("Zero to n") {
        Range r(3);
        REQUIRE(r.size() == 3);
    }
    SECTION("n to m (n < m)") {
        Range r(1, 4);
        REQUIRE(r.size() == 3);
    }
    SECTION("n to m (n < m) in k's (n % k == 0)") {
        Range r(1, 5, 2);
        REQUIRE(r.size() == 2);
    }
    SECTION("n to m (n < m) in k's (n % k != 0)") {
        Range r(1, 6, 2);
        REQUIRE(r.size() == 3);
    }
    SECTION("n to m (m < n)") {
        Range r(4, 1);
        REQUIRE(r.size() == 3);
    }
    SECTION("n to m (m < n) in k's (n % k == 0)") {
        Range r(7, 1, 2);
        REQUIRE(r.size() == 3);
    }
    SECTION("n to m (m < n) in k's (n % k != 0)") {
        Range r(6, 1, 2);
        REQUIRE(r.size() == 3);
    }
}

TEST_CASE("Range<int> : at") {
    SECTION("Zero to n") {
        Range r(3);
        SECTION("Element 0") { REQUIRE(r[0] == 0); }
        SECTION("Element 1") { REQUIRE(r[1] == 1); }
        SECTION("Element 2") { REQUIRE(r[2] == 2); }
    }
    SECTION("n to m (n < m)") {
        Range r(1, 4);
        SECTION("Element 0") { REQUIRE(r[0] == 1); }
        SECTION("Element 1") { REQUIRE(r[1] == 2); }
        SECTION("Element 2") { REQUIRE(r[2] == 3); }
    }
    SECTION("n to m (n < m) in k's (n % k == 0)") {
        Range r(1, 5, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 1); }
        SECTION("Element 1") { REQUIRE(r[1] == 3); }
    }
    SECTION("n to m (n < m) in k's (n % k != 0)") {
        Range r(1, 6, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 1); }
        SECTION("Element 1") { REQUIRE(r[1] == 3); }
        SECTION("Element 2") { REQUIRE(r[2] == 5); }
    }
    SECTION("n to m (m < n)") {
        Range r(4, 1);
        SECTION("Element 0") { REQUIRE(r[0] == 4); }
        SECTION("Element 1") { REQUIRE(r[1] == 3); }
        SECTION("Element 2") { REQUIRE(r[2] == 2); }
    }
    SECTION("n to m (m < n) in k's (n % k == 0)") {
        Range r(7, 1, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 7); }
        SECTION("Element 1") { REQUIRE(r[1] == 5); }
        SECTION("Element 2") { REQUIRE(r[2] == 3); }
    }
    SECTION("n to m (m < n) in k's (n % k != 0)") {
        Range r(6, 1, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 6); }
        SECTION("Element 1") { REQUIRE(r[1] == 4); }
        SECTION("Element 2") { REQUIRE(r[2] == 2); }
    }
}

TEST_CASE("Range<std::size_t> : at") {
    SECTION("Zero to n") {
        Range<std::size_t> r(3);
        SECTION("Element 0") { REQUIRE(r[0] == 0); }
        SECTION("Element 1") { REQUIRE(r[1] == 1); }
        SECTION("Element 2") { REQUIRE(r[2] == 2); }
    }
    SECTION("n to m (n < m)") {
        Range<std::size_t> r(1, 4);
        SECTION("Element 0") { REQUIRE(r[0] == 1); }
        SECTION("Element 1") { REQUIRE(r[1] == 2); }
        SECTION("Element 2") { REQUIRE(r[2] == 3); }
    }
    SECTION("n to m (n < m) in k's (n % k == 0)") {
        Range<std::size_t> r(1, 5, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 1); }
        SECTION("Element 1") { REQUIRE(r[1] == 3); }
    }
    SECTION("n to m (n < m) in k's (n % k != 0)") {
        Range<std::size_t> r(1, 6, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 1); }
        SECTION("Element 1") { REQUIRE(r[1] == 3); }
        SECTION("Element 2") { REQUIRE(r[2] == 5); }
    }
    SECTION("n to m (m < n)") {
        Range<std::size_t> r(4, 1);
        SECTION("Element 0") { REQUIRE(r[0] == 4); }
        SECTION("Element 1") { REQUIRE(r[1] == 3); }
        SECTION("Element 2") { REQUIRE(r[2] == 2); }
    }
    SECTION("n to m (m < n) in k's (n % k == 0)") {
        Range<std::size_t> r(7, 1, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 7); }
        SECTION("Element 1") { REQUIRE(r[1] == 5); }
        SECTION("Element 2") { REQUIRE(r[2] == 3); }
    }
    SECTION("n to m (m < n) in k's (n % k != 0)") {
        Range<std::size_t> r(6, 1, 2);
        SECTION("Element 0") { REQUIRE(r[0] == 6); }
        SECTION("Element 1") { REQUIRE(r[1] == 4); }
        SECTION("Element 2") { REQUIRE(r[2] == 2); }
    }
}

TEST_CASE("Range<int> : can be used in a foreach loop") {
    int counter = 0;
    for(auto x : Range(3)) { REQUIRE(x == counter++); }
}
