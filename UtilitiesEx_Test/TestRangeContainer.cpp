#include <UtilitiesEx/IterTools/RangeContainer.hpp>
#include <UtilitiesEx/TypeTraits/type_traitsExtensions.hpp>
#include <catch/catch.hpp>

using namespace UtilitiesEx;
using iterator_type  = typename std::vector<int>::const_iterator;
using container_type = detail_::RangeContainer<iterator_type>;

TEST_CASE("Is a container") { REQUIRE(is_container<container_type>::value); }

void check_state(container_type& C, const std::vector<int>& contents) {
    REQUIRE(C.size() == contents.size());
    REQUIRE(C.max_size() == std::numeric_limits<std::size_t>::max());
    REQUIRE(C.empty() == (contents.begin() == contents.end()));
    auto corr_begin = contents.begin();
    for(auto x : C) REQUIRE(*corr_begin++ == x);
}

TEST_CASE("Default Ctor") {
    std::vector<int> empty{};
    container_type C;
    check_state(C, empty);

    SECTION("Equality") {
        container_type C2;
        REQUIRE(C == C2);
    }
}

TEST_CASE("Range Ctor w/Empty Range") {
    std::vector<int> empty{};
    container_type C{empty.begin(), empty.end(), empty.size()};
    check_state(C, empty);

    SECTION("Equality w/default Ctor") {
        container_type C2;
        REQUIRE(C == C2);
    }
}

TEST_CASE("Range Ctor") {
    std::vector<int> nonempty{1, 2, 3};
    container_type C{nonempty.begin(), nonempty.end(), nonempty.size()};
    check_state(C, nonempty);

    SECTION("Inequality") {
        container_type C2;
        REQUIRE(C != C2);
    }
    SECTION("Copy Construct") {
        container_type C2{C};
        check_state(C2, nonempty);
        REQUIRE(C2 == C);
    }
    SECTION("Copy Assignment") {
        container_type C2{};
        REQUIRE(C2 != C);
        C2 = C;
        REQUIRE(C2 == C);
    }
    SECTION("Move Ctor") {
        container_type C2{std::move(C)};
        check_state(C2, nonempty);
    }
    SECTION("Move Assignment") {
        container_type C2;
        REQUIRE(C2 != C);
        C2 = std::move(C);
        check_state(C2, nonempty);
    }
    SECTION("Swap") {
        container_type C2;
        C2.swap(C);
        check_state(C, {});
        check_state(C2, nonempty);
    }
}
