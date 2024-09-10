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
