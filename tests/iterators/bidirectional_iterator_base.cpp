#include "tests/iterators/test_iterator.hpp"
#include <utilities/iterators/bidirectional_iterator_base.hpp>

template<typename T>
struct Iterator :
public utilities::iterators::BidirectionalIteratorBase<Iterator<T>, T> {
    int value_ = 0;
    Iterator& increment() override {
        ++value_;
        return *this;
    }

    Iterator& decrement() override {
        --value_;
        return *this;
    }
    T& dereference() override { return value_; }

    bool are_equal(const Iterator& other) const noexcept override {
        return value_ == other.value_;
    }
};

TEST_CASE("BidirectionalIterator") {
    Iterator<int> itr;
    utilities::test_bidirectional_iterator(itr, itr.value_, 0, 1);
}
