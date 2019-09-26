#include <catch2/catch.hpp>
#include <utilities/iterators/bidirectional_iterator_base.hpp>

using namespace utilities::iterators;
namespace {

struct BidirectionalIterator
  : public BidirectionalIteratorBase<BidirectionalIterator> {
    int value_ = 0;

    BidirectionalIterator& increment() {
        ++value_;
        return *this;
    }

    BidirectionalIterator& decrement() {
        --value_;
        return *this;
    }

    const int& dereference() const { return value_; }
    int& dereference() { return value_; }
    bool are_equal(const BidirectionalIterator& other) const noexcept {
        return value_ == other.value_;
    }
};

} // namespace
TEST_CASE("BidirectionalIteratorBase<BidirectionalIterator> : operator--()") {
    BidirectionalIterator itr;
    auto pitr = &(--itr);
    SECTION("Returns *this") { REQUIRE(pitr == &itr); }
    SECTION("Correct value") { REQUIRE(*itr == -1); }
}

TEST_CASE(
  "BidirectionalIteratorBase<BidirectionalIterator> : operator--(int)") {
    BidirectionalIterator itr;
    auto itr2 = itr--;
    SECTION("Returns a copy") { REQUIRE(&itr2 != &itr); }
    SECTION("Return has correct value") { REQUIRE(*itr2 == 0); }
    SECTION("Current iterator has correct value") { REQUIRE(*itr == -1); }
}
