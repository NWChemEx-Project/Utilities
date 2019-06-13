#include <catch2/catch.hpp>
#include <utilities/printing/detail_/table_pimpl.hpp>

using namespace utilities::printing::detail_;

static inline auto make_table(std::string elem) {
    return std::vector{
      std::vector{std::string("Header 1"), std::string("Header 2")},
      std::vector{std::string("Value 1"), std::move(elem)}};
}

TEST_CASE("TablePIMPL(int)") {
    TablePIMPL t(80);
    REQUIRE(t.nrows() == 0);
    REQUIRE(t.ncols() == 0);
}

TEST_CASE("TablePIMPL(int, int, int)") {
    TablePIMPL t(2, 2, 80);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
    REQUIRE(t.width_of_col(0) == 0);
    REQUIRE(t.width_of_col(1) == 0);
}

TEST_CASE("TablePIMPL(table, int)") {
    TablePIMPL t(make_table("Value 2"), 80);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
    REQUIRE(t.width_of_col(0) == 8);
    REQUIRE(t.width_of_col(1) == 8);
}

TEST_CASE("TablePIMPL::add_rows") {
    TablePIMPL t(80);
    SECTION("Add a row") {
        t.add_rows();
        REQUIRE(t.nrows() == 1);
        REQUIRE(t.ncols() == 0);
    }

    SECTION("Add 4 rows") {
        t.add_rows(4);
        REQUIRE(t.nrows() == 4);
        REQUIRE(t.ncols() == 0);
    }
}

TEST_CASE("TablePIMPL::add_cols") {
    TablePIMPL t(80);

    SECTION("Add a column w/no rows") {
        t.add_cols();
        REQUIRE(t.nrows() == 0);
        REQUIRE(t.ncols() == 0);
    }

    SECTION("Add a column w/rows") {
        t.add_rows();
        t.add_cols();
        REQUIRE(t.nrows() == 1);
        REQUIRE(t.ncols() == 1);
        REQUIRE(t.width_of_col(0) == 0);
    }

    SECTION("Add 4 columns w/no rows") {
        t.add_cols(4);
        REQUIRE(t.nrows() == 0);
        REQUIRE(t.ncols() == 0);
    }

    SECTION("Add 4 columns w/rows") {
        t.add_rows();
        t.add_cols(4);
        REQUIRE(t.nrows() == 1);
        REQUIRE(t.ncols() == 4);
    }
}

TEST_CASE("TablePIMPL::set_size") {
    TablePIMPL t(80);

    t.set_size(2, 2);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
    REQUIRE(t.width_of_col(0) == 0);
    REQUIRE(t.width_of_col(1) == 0);
}

TEST_CASE("TablePIMPL::width_of_col") {
    SECTION("Under max") {
        TablePIMPL p(make_table("This is data for (1,1)"), 80);
        REQUIRE(p.width_of_col(1) == 22);
    }
    SECTION("Honors newline") {
        const auto p1 = "This is the first line\n";
        const auto p2 = "This is the second line.";
        TablePIMPL p(make_table(std::string(p1) + p2), 80);
        REQUIRE(p.width_of_col(1) == 24);
    }
    SECTION("Honors user's width") {
        TablePIMPL p(make_table("This is data for (1,1)"), 80);
        p.set_width(1, 10);
        REQUIRE(p.width_of_col(1) == 10);
    }
}

TEST_CASE("TablePIMPL::operator()") {
    TablePIMPL p(2, 2, 80);
    for(std::size_t i = 0; i < 2; ++i) {
        auto half1 = std::string("(") + std::to_string(i) + ", ";
        for(std::size_t j = 0; j < 2; ++j) {
            auto half2 = std::to_string(j) + ")";
            p(i, j)    = half1 + half2;
            REQUIRE(p(i, j) == half1 + half2);
        }
    }
}
