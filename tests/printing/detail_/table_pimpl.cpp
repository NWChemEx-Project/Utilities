#include <catch2/catch.hpp>
#include <utilities/printing/detail_/table_pimpl.hpp>

using namespace utilities::printing::detail_;

TEST_CASE("TablePIMPL(int)") {
    TablePIMPL t(80);
    REQUIRE(t.nrows() == 0);
    REQUIRE(t.ncols() == 0);
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
