#pragma once
#include<catch2/catch.hpp>
#include<utilities/type_traits/type_traits_extensions.hpp>
namespace utilities {

template<typename T, typename U, typename V>
void test_input_iterator(T&& itr, U&& value, V init, V next) {
    SECTION("Satisfies input iterators concept") {
        STATIC_REQUIRE(is_input_iterator<std::decay_t<T>>::value);
    }
    SECTION("Can be dereferenced"){
        SECTION("Value"){ REQUIRE(*itr == init); }
        SECTION("By Reference"){ REQUIRE(&(*itr) == &value); }
    }
    SECTION("Can be prefix-incremented"){
        auto& rv = ++itr;
        SECTION("Value") { REQUIRE(*itr == next); }
        SECTION("Supports Chaining"){ REQUIRE(&itr == &rv); }
    }
    SECTION("Can be post-incremented"){
        auto rv = itr++;
        SECTION("Return is unchanged"){ REQUIRE(*rv == init); }
        SECTION("Has been incremented"){ REQUIRE(*itr == next); }
    }
    SECTION("Comparsions") {
        auto rv = itr++;
        SECTION("Different Iterators") {
          REQUIRE_FALSE(rv == itr);
          REQUIRE(rv != itr);
        }
        SECTION("Same Iterators"){
            ++rv;
            REQUIRE(rv == itr);
            REQUIRE_FALSE(rv != itr);
        }
    }
}

template<typename T, typename U, typename V>
void test_bidirectional_iterator(T&& itr, U&& value, V init, V next) {
  test_input_iterator(std::forward<T>(itr), std::forward<U>(value), init, next);
  SECTION("Satisfies bidirectional iterators concept"){
    STATIC_REQUIRE(is_bidirectional_iterator<std::decay_t<T>>::value);
  }
  SECTION("Prefix-decrement") {
    ++itr;
    auto& rv = --itr;
    SECTION("Value") { REQUIRE(*itr == init); }
    SECTION("Supports Chaining"){ REQUIRE(&rv == &itr); }
  }
  SECTION("Postfix-decrement"){
    ++itr;
    auto rv = itr--;
    SECTION("Return is unchanged"){ REQUIRE(*rv == next); }
    SECTION("Has been decremented"){ REQUIRE(*itr == init); }
  }
}

template<typename T, typename U, typename V>
void test_ra_iterator(T&& itr, U&& value, V init, V next) {
  test_bidirectional_iterator(std::forward<T>(itr), std::forward<U>(value),
                              init, next);
  SECTION("operator[]"){

  }
}

} //namespace utilities
