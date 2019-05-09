#include "tests/iterators/test_iterator.hpp"
#include <utilities/iterators/random_access_iterator_base.hpp>

template<typename T>
struct Iterator :
public utilities::iterators::RandomAccessIteratorBase<Iterator<T>, T> {
    int value_ = 0;
    Iterator& advance(long int n) {
      value_+=n;
      return *this;
    }
    T& dereference() override { return value_; }

    bool are_equal(const Iterator& other) const noexcept override {
        return value_ == other.value_;
    }
};

TEST_CASE("RandomAccessIterator") {
    Iterator<int> itr;
    test_random_access_iterator(itr, itr.value_);
}
