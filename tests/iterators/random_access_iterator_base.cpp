#include <catch2/catch.hpp>
#include <utilities/iterators/random_access_iterator_base.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>

using namespace utilities;
using namespace iterators;
namespace {
struct RandomAccessIterator
  : public RandomAccessIteratorBase<RandomAccessIterator, int> {
    int value_ = 0;

    RandomAccessIterator& increment() {
        ++value_;
        return *this;
    }

    RandomAccessIterator& decrement() {
        --value_;
        return *this;
    }

    const int& dereference() const { return value_; }

    bool are_equal(const RandomAccessIterator& other) const noexcept {
        return value_ == other.value_;
    }

    long int distance_to(const RandomAccessIterator& other) const noexcept {
        return other.value_ - value_;
    }

    RandomAccessIterator& advance(long int n) {
        value_ += n;
        return *this;
    }
};
} // namespace
TEST_CASE("RandomAccessIterator base class") {
    RandomAccessIterator itr;
    SECTION("Satisfies iterator concept") {
        bool is_itr = is_random_access_iterator<RandomAccessIterator>::value;
        REQUIRE(is_itr);
    }
    SECTION("Order comparisons work") {
        RandomAccessIterator itr2;
        RandomAccessIterator itr3;
        itr3.value_ = 10;
        REQUIRE(itr < itr3);
        REQUIRE(itr <= itr2);
        REQUIRE(itr3 > itr);
        REQUIRE(itr2 >= itr);
    }
    SECTION("Advancing works") {
        RandomAccessIterator& rv = (itr += 10);
        REQUIRE(&rv == &itr);
        REQUIRE(rv.value_ == 10);
    }
    SECTION("Advance and copy works") {
        RandomAccessIterator rv = (itr + 10);
        REQUIRE(itr.value_ == 0);
        REQUIRE(rv.value_ == 10);
    }
    SECTION("Go backwards works") {
        RandomAccessIterator& rv = (itr -= 10);
        REQUIRE(&rv == &itr);
        REQUIRE(itr.value_ == -10);
    }
    SECTION("Go backwards and copy works") {
        RandomAccessIterator rv = (itr - 10);
        REQUIRE(itr.value_ == 0);
        REQUIRE(rv.value_ == -10);
    }
    SECTION("Random access works") {
        int rv = itr[100];
        REQUIRE(itr.value_ == 0);
        REQUIRE(rv == 100);
    }
}
