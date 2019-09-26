#include <catch2/catch.hpp>
#include <map>
#include <utilities/iterators/random_access_iterator_base.hpp>
using namespace utilities::iterators;

namespace {
struct RAIterator : public RandomAccessIteratorBase<RAIterator> {
    int value_ = 0;

    RAIterator& increment() {
        ++value_;
        return *this;
    }

    RAIterator& decrement() {
        --value_;
        return *this;
    }

    int dereference() const { return value_; }

    bool are_equal(const RAIterator& other) const noexcept {
        return value_ == other.value_;
    }

    long int distance_to(const RAIterator& other) const noexcept {
        return other.value_ - value_;
    }

    RAIterator& advance(long int n) {
        value_ += n;
        return *this;
    }
};

struct RAIterator2 : public RandomAccessIteratorBase<RAIterator2> {
    std::map<int, int>* m_ptr_ = nullptr;
    int value_                 = 0;

    RAIterator2& increment() {
        ++value_;
        return *this;
    }

    RAIterator2& decrement() {
        --value_;
        return *this;
    }

    int& dereference() const { return (*m_ptr_)[value_]; }

    bool are_equal(const RAIterator2& other) const noexcept {
        return value_ == other.value_;
    }

    long int distance_to(const RAIterator2& other) const noexcept {
        return other.value_ - value_;
    }

    RAIterator2& advance(long int n) {
        value_ += n;
        return *this;
    }
};
} // namespace
TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator+=") {
    RAIterator itr;
    auto pitr = &(itr += 2);
    SECTION("Returns this") { REQUIRE(pitr == &itr); }
    SECTION("Advances by 2") { REQUIRE(itr.value_ == 2); }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator+") {
    RAIterator itr;
    auto itr2 = itr + 2;
    SECTION("Returns a copy") { REQUIRE(&itr != &itr2); }
    SECTION("Leaves this alone") { REQUIRE(itr.value_ == 0); }
    SECTION("Advances result by 2") { REQUIRE(itr2.value_ == 2); }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator-=") {
    RAIterator itr;
    auto pitr = &(itr -= 2);
    SECTION("Returns this") { REQUIRE(pitr == &itr); }
    SECTION("Retreats by 2") { REQUIRE(itr.value_ == -2); }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator-") {
    RAIterator itr;
    auto itr2 = itr - 2;
    SECTION("Returns a copy") { REQUIRE(&itr != &itr2); }
    SECTION("Leaves this alone") { REQUIRE(itr.value_ == 0); }
    SECTION("Retreats result by 2") { REQUIRE(itr2.value_ == -2); }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator[]") {
    RAIterator itr;
    SECTION("Value") {
        auto rv = itr[2];
        REQUIRE(itr[2] == 2);
    }
    SECTION("Is not an alias") {
        STATIC_REQUIRE(std::is_same_v<int, decltype(itr[2])>);
    }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator2> : operator[]") {
    std::map<int, int> values{{2, 2}};
    RAIterator2 itr;
    itr.m_ptr_ = &values;
    auto& rv   = itr[2];
    SECTION("Value") { REQUIRE(rv == 2); }
    SECTION("Is alias") { REQUIRE(&rv == &values[2]); }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator<") {
    RAIterator itr, itr2;
    SECTION("Same value") { REQUIRE_FALSE(itr < itr2); }
    SECTION("Earlier value") {
        --itr;
        REQUIRE(itr < itr2);
    }
    SECTION("Later value") {
        ++itr;
        REQUIRE_FALSE(itr < itr2);
    }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator<=") {
    RAIterator itr, itr2;
    SECTION("Same value") { REQUIRE(itr <= itr2); }
    SECTION("Earlier value") {
        --itr;
        REQUIRE(itr <= itr2);
    }
    SECTION("Later value") {
        ++itr;
        REQUIRE_FALSE(itr <= itr2);
    }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator>") {
    RAIterator itr, itr2;
    SECTION("Same value") { REQUIRE_FALSE(itr > itr2); }
    SECTION("Earlier value") {
        --itr;
        REQUIRE_FALSE(itr > itr2);
    }
    SECTION("Later value") {
        ++itr;
        REQUIRE(itr > itr2);
    }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator>=") {
    RAIterator itr, itr2;
    SECTION("Same value") { REQUIRE(itr >= itr2); }
    SECTION("Earlier value") {
        --itr;
        REQUIRE_FALSE(itr >= itr2);
    }
    SECTION("Later value") {
        ++itr;
        REQUIRE(itr >= itr2);
    }
}

TEST_CASE("RandomAccessRAIteratorBase<RAIterator> : operator-(RAIterator)") {
    RAIterator itr, itr2;
    SECTION("Same element") { REQUIRE((itr - itr2) == 0); }
    SECTION("Is behind") {
        --itr;
        REQUIRE((itr - itr2) == -1);
    }
    SECTION("Is ahead") {
        ++itr;
        REQUIRE((itr - itr2) == 1);
    }
}
