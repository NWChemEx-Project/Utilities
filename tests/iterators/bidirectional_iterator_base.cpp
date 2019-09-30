#include <catch2/catch.hpp>
#include <utilities/iterators/bidirectional_iterator_base.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>

using namespace utilities;
namespace {
struct BidirectionalIterator
  : public iterators::BidirectionalIteratorBase<BidirectionalIterator, int> {
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

    bool are_equal(const BidirectionalIterator& other) const noexcept {
        return value_ == other.value_;
    }
};
} // namespace
TEST_CASE("BidirectionalIterator base class") {
    BidirectionalIterator itr;
    SECTION("Satisfies iterator concept") {
        bool is_itr = is_bidirectional_iterator<BidirectionalIterator>::value;
        REQUIRE(is_itr);
    }
    SECTION("Can be prefix decremented") {
        BidirectionalIterator& rv = --itr;
        REQUIRE(*itr == -1);
        REQUIRE(&rv == &itr);
    }
    SECTION("Can be postfix decremented") {
        BidirectionalIterator rv = itr--;
        REQUIRE(&rv.value_ != &itr.value_);
        REQUIRE(*rv == 0);
        REQUIRE(*itr == -1);
    }
}
