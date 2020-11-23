#include <catch2/catch.hpp>
#include <utilities/strings/string_tools.hpp>
#include <set>

using namespace utilities::strings;
using string_vec = std::vector<std::string>;

TEST_CASE("split_string") {
    SECTION("Empty string") { REQUIRE(split_string("", "\n").empty()); }
    SECTION("No delimiter") {
        REQUIRE(split_string("Hi there", "\n") == string_vec{"Hi there"});
    }
    SECTION("Delimiter") {
        REQUIRE(split_string("L1\nL2", '\n') == string_vec{"L1", "L2"});
    }
    SECTION("Mulit-character delimiter") {
        REQUIRE(split_string("Hello", "ll") == string_vec{"He", "o"});
    }
}

using types = std::tuple<std::vector<std::string>, std::set<std::string>>;

TEMPLATE_LIST_TEST_CASE("join_string", "", types) {
    using container_type = TestType;

    SECTION("Empty container"){
        container_type c;
        auto rv = join_string(c, ' ');
        REQUIRE(rv == std::string{});
    }

    SECTION("Non-empty") {
        container_type c{"Hello", "World"};

        SECTION("empty delimiter"){
            auto rv = join_string(c, "");
            REQUIRE(rv == "HelloWorld");
        }

        SECTION("Non-empty delimiter") {
            auto rv = join_string(c, ' ');
            REQUIRE(rv == "Hello World");
        }
    }
}
