/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../catch.hpp"
#include <utilities/iterators/input_iterator_base.hpp>

using namespace utilities::iterators;

namespace {
// Tests when we are returning elements by reference
struct Iterator : InputIteratorBase<Iterator> {
    int value_ = 0;

    Iterator& increment() {
        ++value_;
        return *this;
    }

    const int& dereference() const { return value_; }
    int& dereference() { return value_; }

    bool are_equal(const Iterator& other) const noexcept {
        return value_ == other.value_;
    }
};

// Tests when we are not returning elements by reference (for example as view)
struct Iterator2 : InputIteratorBase<Iterator2> {
    int value_ = 0;

    Iterator2& increment() {
        ++value_;
        return *this;
    }

    int dereference() const { return value_; }
    int dereference() { return value_; }

    bool are_equal(const Iterator& other) const noexcept {
        return value_ == other.value_;
    }
};
} // namespace
TEST_CASE("InputIteratorBase<Iterator> : operator*()") {
    Iterator itr;
    SECTION("Value") { REQUIRE(*itr == 0); }
    SECTION("Is alias") { REQUIRE(&(*itr) == &itr.value_); }
}

TEST_CASE("InputIteratorBase<Iterator2> : operator*()") {
    Iterator2 itr;
    SECTION("Value") { REQUIRE(*itr == 0); }
    SECTION("Is not an alias") {
        STATIC_REQUIRE_FALSE(std::is_reference_v<decltype(*itr)>);
    }
}

TEST_CASE("InputIteratorBase<Iterator> : operator*() const") {
    const Iterator itr;
    SECTION("Value") { REQUIRE(*itr == 0); }
    SECTION("Is alias") { REQUIRE(&(*itr) == &itr.value_); }
    SECTION("Is read-only") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(*itr)>);
    }
}

TEST_CASE("InputIteratorBase<Iterator2> : operator*() const") {
    const Iterator2 itr;
    SECTION("Value") { REQUIRE(*itr == 0); }
    SECTION("Is not an alias") {
        STATIC_REQUIRE_FALSE(std::is_reference_v<decltype(*itr)>);
    }
}

TEST_CASE("InputIteratorBase<Iterator> : operator->()") {
    Iterator itr;
    SECTION("Value") { REQUIRE(itr.operator->() == &itr.value_); }
}

TEST_CASE("InputIteratorBase<Iterator> : operator->() const") {
    const Iterator itr;
    SECTION("Value") { REQUIRE(itr.operator->() == &itr.value_); }
    SECTION("Is read-only") {
        STATIC_REQUIRE(std::is_same_v<const int*, decltype(itr.operator->())>);
    }
}

TEST_CASE("InputIteratorBase<Iterator> : operator++()") {
    Iterator itr;
    auto pitr = &(++itr);
    SECTION("Returns this") { REQUIRE(pitr == &itr); }
    SECTION("Value") { REQUIRE(*itr == 1); }
}

TEST_CASE("InputIteratorBase<Iterator> : operator++(int)") {
    Iterator itr;
    auto itr2 = itr++;
    SECTION("Returns copy") { REQUIRE(*itr2 == 0); }
    SECTION("Value") { REQUIRE(*itr == 1); }
}

TEST_CASE("InputIteratorBase<Iterator> : operator==") {
    Iterator itr, itr2;
    SECTION("Same value") { REQUIRE(itr == itr2); }
    SECTION("Different values") {
        ++itr2;
        REQUIRE_FALSE(itr == itr2);
    }
}

TEST_CASE("InputIteratorBase<Iterator> : operator!=") {
    Iterator itr, itr2;
    SECTION("Same value") { REQUIRE_FALSE(itr != itr2); }
    SECTION("Different values") {
        ++itr2;
        REQUIRE(itr != itr2);
    }
}
