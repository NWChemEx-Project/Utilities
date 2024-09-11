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
#include <utilities/containers/indexable_container_base.hpp>

/* Testing Strategy.
 *
 * We know the iterators work so long as we properly hook them up to operator[].
 * This means we need to test:
 *
 * - empty and size for correct behavior
 * - operator[] is hooked up to derived class correctly
 *   - this includes const correct
 * - begin/end, etc. are hooked up to operator[] correctly
 */

using namespace utilities;

namespace {

// This container tests the usual case of actually returning references
struct ByReference : IndexableContainerBase<ByReference> {
    std::vector<int> data;
    using value_type = int;
    auto size_() const noexcept { return data.size(); }
    int& at_(size_type i) { return data[i]; }
    const int& at_(size_type i) const { return data[i]; }
};

// This container tests returning elements by value
struct ByValue : IndexableContainerBase<ByValue> {
    std::vector<int> data;
    using value_type = int;
    auto size_() const noexcept { return data.size(); }
    int at_(size_type i) { return data[i]; }
    int at_(size_type i) const { return data[i]; }
};

} // namespace

TEST_CASE("IndexableContainerBase<ByReference> : empty") {
    ByReference c;
    SECTION("Empty container") { REQUIRE(c.empty()); }
    SECTION("Not empty") {
        c.data.push_back(1);
        REQUIRE_FALSE(c.empty());
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : size") {
    ByReference c;
    SECTION("Empty container") { REQUIRE(c.size() == 0); }
    SECTION("Not empty") {
        c.data.push_back(1);
        REQUIRE(c.size() == 1);
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : operator[]") {
    ByReference c;
    c.data.push_back(2);
    SECTION("Good access") { REQUIRE(c[0] == 2); }
    SECTION("Is by read-/write-reference") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(c[0])>);
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : operator[]") {
    ByValue c;
    c.data.push_back(2);
    SECTION("Good access") { REQUIRE(c[0] == 2); }
    SECTION("Is by value") {
        STATIC_REQUIRE(std::is_same_v<int, decltype(c[0])>);
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : operator[] const") {
    ByReference c;
    c.data.push_back(2);
    const ByReference& const_c = c;
    SECTION("Good access") { REQUIRE(const_c[0] == 2); }
    SECTION("Is read-only reference") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(const_c[0])>);
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : operator[] const") {
    ByValue c;
    c.data.push_back(2);
    const ByValue& const_c = c;
    SECTION("Good access") { REQUIRE(const_c[0] == 2); }
    SECTION("Is by value") {
        STATIC_REQUIRE(std::is_same_v<int, decltype(const_c[0])>);
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : at") {
    ByReference c;
    c.data.push_back(2);
    SECTION("Good access") { REQUIRE(c.at(0) == 2); }
    SECTION("Is by read-/write-reference") {
        STATIC_REQUIRE(std::is_same_v<int&, decltype(c.at(0))>);
    }
    SECTION("Throws if index is out of range") {
        REQUIRE_THROWS_AS(c.at(1), std::out_of_range);
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : at") {
    ByValue c;
    c.data.push_back(2);
    SECTION("Good access") { REQUIRE(c.at(0) == 2); }
    SECTION("Is by value") {
        STATIC_REQUIRE(std::is_same_v<int, decltype(c.at(0))>);
    }
    SECTION("Throws if index is out of range") {
        REQUIRE_THROWS_AS(c.at(1), std::out_of_range);
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : at const") {
    ByReference c;
    c.data.push_back(2);
    const ByReference& const_c = c;
    SECTION("Good access") { REQUIRE(const_c.at(0) == 2); }
    SECTION("Is read-only reference") {
        STATIC_REQUIRE(std::is_same_v<const int&, decltype(const_c.at(0))>);
    }
    SECTION("Throws if index is out of range") {
        REQUIRE_THROWS_AS(const_c.at(1), std::out_of_range);
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : at const") {
    ByValue c;
    c.data.push_back(2);
    const ByValue& const_c = c;
    SECTION("Good access") { REQUIRE(const_c.at(0) == 2); }
    SECTION("Is by value") {
        STATIC_REQUIRE(std::is_same_v<int, decltype(const_c.at(0))>);
    }
    SECTION("Throws if index is out of range") {
        REQUIRE_THROWS_AS(const_c.at(1), std::out_of_range);
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : begin") {
    ByReference c;
    SECTION("Empty") { REQUIRE(c.begin() == c.end()); }
    SECTION("Non-empty") {
        c.data.push_back(2);
        SECTION("Value") { REQUIRE(*c.begin() == 2); }
        SECTION("Is an alias") { REQUIRE(&(*c.begin()) == &c.data[0]); }
        SECTION("Is read-/write-able") {
            STATIC_REQUIRE(std::is_same_v<int&, decltype(*c.begin())>);
        }
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : begin") {
    ByValue c;
    SECTION("Empty") { REQUIRE(c.begin() == c.end()); }
    SECTION("Non-empty") {
        c.data.push_back(2);
        SECTION("Value") { REQUIRE(*c.begin() == 2); }
        SECTION("Is by value") {
            STATIC_REQUIRE(std::is_same_v<int, decltype(*c.begin())>);
        }
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : begin const") {
    ByReference c;
    const ByReference& const_c = c;
    SECTION("Empty") { REQUIRE(const_c.begin() == const_c.end()); }
    SECTION("Non-empty") {
        c.data.push_back(2);
        SECTION("Value") { REQUIRE(*const_c.begin() == 2); }
        SECTION("Is an alias") { REQUIRE(&(*const_c.begin()) == &c.data[0]); }
        SECTION("Is read-only") {
            STATIC_REQUIRE(
              std::is_same_v<const int&, decltype(*const_c.begin())>);
        }
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : begin const") {
    ByValue c;
    const ByValue& const_c = c;
    SECTION("Empty") { REQUIRE(const_c.begin() == const_c.end()); }
    SECTION("Non-empty") {
        c.data.push_back(2);
        SECTION("Value") { REQUIRE(*const_c.begin() == 2); }
        SECTION("Is by value") {
            STATIC_REQUIRE(std::is_same_v<int, decltype(*const_c.begin())>);
        }
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : cbegin") {
    ByReference c;
    SECTION("Empty") { REQUIRE(c.cbegin() == c.cend()); }
    SECTION("Non-empty") {
        c.data.push_back(2);
        SECTION("Value") { REQUIRE(*c.cbegin() == 2); }
        SECTION("Is an alias") { REQUIRE(&(*c.cbegin()) == &c.data[0]); }
        SECTION("Is read-only") {
            STATIC_REQUIRE(std::is_same_v<const int&, decltype(*c.cbegin())>);
        }
    }
}

TEST_CASE("IndexableContainerBase<ByValue> : cbegin") {
    ByValue c;
    SECTION("Empty") { REQUIRE(c.cbegin() == c.cend()); }
    SECTION("Non-empty") {
        c.data.push_back(2);
        SECTION("Value") { REQUIRE(*c.cbegin() == 2); }
        SECTION("Is by value") {
            STATIC_REQUIRE(std::is_same_v<int, decltype(*c.cbegin())>);
        }
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : end") {
    ByReference c;
    c.data.push_back(2);
    auto itr = c.begin();
    ++itr;
    REQUIRE(itr == c.end());
}

TEST_CASE("IndexableContainerBase<ByReference> : end() const") {
    ByReference c;
    c.data.push_back(2);
    const ByReference& const_c = c;
    auto itr                   = const_c.begin();
    ++itr;
    REQUIRE(itr == const_c.end());
}

TEST_CASE("IndexableContainerBase<ByReference> : cend") {
    ByReference c;
    c.data.push_back(2);
    auto itr = c.cbegin();
    ++itr;
    REQUIRE(itr == c.cend());
}

TEST_CASE("IndexableContainerBase<ByReference> : operator==") {
    ByReference c1, c2;
    SECTION("Both empty") { REQUIRE(c1 == c2); }
    SECTION("Different sizes") {
        c1.data.push_back(2);
        REQUIRE_FALSE(c1 == c2);
    }
    SECTION("Same size, same elements") {
        c1.data.push_back(2);
        c2.data.push_back(2);
        REQUIRE(c1 == c2);
    }
    SECTION("Same size, different elements") {
        c1.data.push_back(2);
        c2.data.push_back(3);
        REQUIRE_FALSE(c1 == c2);
    }
}

TEST_CASE("IndexableContainerBase<ByReference> : operator!=") {
    ByReference c1, c2;
    SECTION("Both empty") { REQUIRE_FALSE(c1 != c2); }
    SECTION("Different sizes") {
        c1.data.push_back(2);
        REQUIRE(c1 != c2);
    }
    SECTION("Same size, same elements") {
        c1.data.push_back(2);
        c2.data.push_back(2);
        REQUIRE_FALSE(c1 != c2);
    }
    SECTION("Same size, different elements") {
        c1.data.push_back(2);
        c2.data.push_back(3);
        REQUIRE(c1 != c2);
    }
}
