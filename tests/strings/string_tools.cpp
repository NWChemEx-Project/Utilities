#include <catch2/catch.hpp>
#include <set>
#include <utilities/strings/string_tools.hpp>

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

    SECTION("Empty container") {
        container_type c;
        auto rv = join_string(c, ' ');
        REQUIRE(rv == std::string{});
    }

    SECTION("Non-empty") {
        container_type c{"Hello", "World"};

        SECTION("empty delimiter") {
            auto rv = join_string(c, "");
            REQUIRE(rv == "HelloWorld");
        }

        SECTION("Non-empty delimiter") {
            auto rv = join_string(c, ' ');
            REQUIRE(rv == "Hello World");
        }
    }
}

TEST_CASE("replace") {
    SECTION("Normal replace") {
        const std::string msg  = "Needle in a haystack";
        auto rv                = replace("haystack", "pincushion", msg);
        const std::string corr = "Needle in a pincushion";
        REQUIRE(rv == corr);
    }

    SECTION("Empty replace") {
        const std::string msg  = "Needle in a haystack";
        auto rv                = replace("haystack", "", msg);
        const std::string corr = "Needle in a ";
        REQUIRE(rv == corr);
    }

    SECTION("Search string has special regex character") {
        const std::string msg  = "Come here?";
        auto rv                = replace("\\?", "!", msg);
        const std::string corr = "Come here!";
        REQUIRE(rv == corr);
    }

    // These last two unit tests are for one of the common use cases: index
    // annotation manipulations
    SECTION("Semicolon") {
        const std::string idx  = "i0,i1;j0,j1";
        auto rv                = replace(";", ",", idx);
        const std::string corr = "i0,i1,j0,j1";
        REQUIRE(rv == corr);
    }

    SECTION("Comma") {
        const std::string idx  = "i0,i1;j0,j1";
        auto rv                = replace(",", ";", idx);
        const std::string corr = "i0;i1;j0;j1";
        REQUIRE(rv == corr);
    }
}