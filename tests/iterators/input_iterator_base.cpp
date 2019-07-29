#include <catch2/catch.hpp>
#include <utilities/iterators/input_iterator_base.hpp>
#include <utilities/type_traits/type_traits_extensions.hpp>

using namespace utilities;
using namespace iterators;

struct Iterator : public InputIteratorBase<Iterator, int> {
    int value_ = 0;

    Iterator& increment() {
        ++value_;
        return *this;
    }

    const int& dereference() const { return value_; }

    bool are_equal(const Iterator& other) const noexcept {
        return value_ == other.value_;
    }
};

struct Iterator2 : public InputIteratorBase<Iterator, const int> {};

TEST_CASE("InputIterator base class") {
    Iterator itr;
    SECTION("Satisfies iterator concept") {
        bool is_itr = is_input_iterator<Iterator>::value;
        REQUIRE(is_itr);
    }
    SECTION("Can be dereferenced in non-const state") {
        int& value = *itr;
        REQUIRE(value == 0);
        REQUIRE(&value == &itr.value_);
    }
    SECTION("Can be dereferenced in a const state") {
        const int& value = *const_cast<const Iterator&>(itr); // NOLINT
        REQUIRE(value == 0);
        REQUIRE(&value == &itr.value_);
    }
    SECTION("Can be prefix-incremented") {
        auto& rv = ++itr;
        REQUIRE(&rv == &itr);
        REQUIRE(*rv == 1);

        SECTION("Equality works") { REQUIRE(rv == itr); }
    }
    SECTION("Can be post-incremented") {
        auto rv = itr++;
        REQUIRE(*rv == 0);
        REQUIRE(&rv.value_ != &itr.value_);
        REQUIRE(*itr == 1);
        SECTION("Inequality works") { REQUIRE(rv != itr); }
    }
    SECTION("Const is obeyed") {
        bool is_const =
          std::is_same<const int&, typename Iterator2::reference>::value;
        REQUIRE(is_const);
    }
}
