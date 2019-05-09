#include "tests/iterators/test_iterator.hpp"
#include <utilities/iterators/input_iterator_base.hpp>

template<typename T>
struct Iterator :
    public utilities::iterators::InputIteratorBase<Iterator<T>, T> {
    int value_ = 0;
    Iterator& increment() override {
        ++value_;
        return *this;
    }

    T& dereference() { return value_; }

    bool are_equal(const Iterator& other) const noexcept {
        return value_ == other.value_;
    }
};

TEST_CASE("InputIterator") {
    Iterator<int> itr;
    utilities::test_input_iterator(itr, itr.value_, 0, 1);
}
