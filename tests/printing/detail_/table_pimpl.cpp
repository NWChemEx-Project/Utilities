#include <catch2/catch.hpp>
#include <utilities/printing/detail_/table_pimpl.hpp>

using namespace utilities::printing::detail_;
using size_vec   = typename TablePIMPL::size_vec;
using row_type   = typename TablePIMPL::row_type;
using table_type = typename TablePIMPL::table_type;

static inline auto make_table(std::string elem) {
    return std::vector{
      std::vector{std::string("Header 1"), std::string("Header 2")},
      std::vector{std::string("Value 1"), std::move(elem)}};
}

TEST_CASE("row_spacer") {
    std::array<std::string, 3> borders{"| ", " | ", " |"};
    SECTION("No width") {
        std::array widths{0, 0};
        REQUIRE(row_spacer(widths, borders, '-', '+') == "+--+--+");
    }
    SECTION("Even widths") {
        std::array widths{4, 4};
        REQUIRE(row_spacer(widths, borders, '-', '+') == "+------+------+");
    }
    SECTION("Different widths") {
        std::array widths{8, 4};
        REQUIRE(row_spacer(widths, borders, '-', '+') == "+----------+------+");
    }
    SECTION("Throws if middle border is not symmetric") {
        std::array widths{4, 4};
        borders[1] = "| ";
        REQUIRE_THROWS_AS(row_spacer(widths, borders, '-', '+'),
                          std::runtime_error);
    }
}

TEST_CASE("print_row") {
    std::array widths{10, 10};
    std::array<std::string, 3> borders{"| ", " | ", " |"};

    SECTION("Blank row") {
        table_type data(2);
        REQUIRE(print_row(widths, data, borders).empty());
    }
    SECTION("Exactly fitting") {
        table_type data{2, row_type{"0123456789"}};
        auto corr = "| 0123456789 | 0123456789 |\n";
        REQUIRE(print_row(widths, data, borders) == corr);
    }
    SECTION("Requires padding") {
        table_type data{2, row_type{"0123456"}};
        auto corr = "| 0123456    | 0123456    |\n";
        REQUIRE(print_row(widths, data, borders) == corr);
    }
    SECTION("Multiline") {
        table_type data{2, row_type{2, "0123456"}};
        auto corr =
          "| 0123456    | 0123456    |\n| 0123456    | 0123456    |\n";
        REQUIRE(print_row(widths, data, borders) == corr);
    }
}

TEST_CASE("TablePIMPL(int)") {
    TablePIMPL t(80);
    REQUIRE(t.nrows() == 0);
    REQUIRE(t.ncols() == 0);
    REQUIRE_THROWS_AS(t.width_of_col(0), std::out_of_range);
    REQUIRE_THROWS_AS(t.height_of_row(0), std::out_of_range);
    REQUIRE(t.space_out_cols().empty());
}

TEST_CASE("TablePIMPL(int, int, int)") {
    TablePIMPL t(2, 2, 80);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
    REQUIRE(t.width_of_col(0) == 0);
    REQUIRE(t.width_of_col(1) == 0);
    REQUIRE(t.space_out_cols() == size_vec{0, 0});
    REQUIRE(t.height_of_row(0) == 0);
    REQUIRE(t.height_of_row(1) == 0);
}

TEST_CASE("TablePIMPL(table, int)") {
    TablePIMPL t(make_table("Value 2"), 80);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
    REQUIRE(t.width_of_col(0) == 8);
    REQUIRE(t.width_of_col(1) == 8);
    REQUIRE(t.space_out_cols() == size_vec{8, 8});
    REQUIRE(t.height_of_row(0) == 1);
    REQUIRE(t.height_of_row(1) == 1);
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

TEST_CASE("TablePIMPL::space_out_cols") {
    SECTION("Can't honor user's request") {
        TablePIMPL p(make_table("Data"), 10);
        p.set_width(0, 5);
        REQUIRE_THROWS_AS(p.space_out_cols(), std::runtime_error);
    }
    SECTION("All set by user") {
        TablePIMPL p(make_table("Data"), 80);
        p.set_width(0, 10);
        p.set_width(1, 10);
        REQUIRE(p.space_out_cols() == std::vector<unsigned short>{10, 10});
    }
    SECTION("Set by length") {
        TablePIMPL p(make_table("This is data for (1,1)"), 80);
        REQUIRE(p.space_out_cols() == std::vector<unsigned short>{8, 22});
    }
    SECTION("Average widths") {
        TablePIMPL p(make_table("This is data for (1,1)"), 23);
        REQUIRE(p.space_out_cols() == std::vector<unsigned short>{8, 8});
    }
}

TEST_CASE("TablePIMPL::height_of_row") {
    SECTION("Set by user") {
        TablePIMPL p(make_table("Data"), 80);
        p.set_height(0, 10);
        REQUIRE(p.height_of_row(0) == 10);
        REQUIRE(p.height_of_row(1) == 1);
    }
    SECTION("Single line") {
        TablePIMPL p(make_table("Data"), 80);
        REQUIRE(p.height_of_row(0) == 1);
        REQUIRE(p.height_of_row(1) == 1);
    }
    SECTION("Multi line cell") {
        TablePIMPL p(make_table("Data\n More Data"), 80);
        REQUIRE(p.height_of_row(0) == 1);
        REQUIRE(p.height_of_row(1) == 2);
    }
    SECTION("Long cell that wraps") {
        TablePIMPL p(make_table("This is data for (1,1)"), 23);
        REQUIRE(p.height_of_row(0) == 1);
        REQUIRE(p.height_of_row(1) == 3);
    }
}

TEST_CASE("TablePIMPL::operator()") {
    SECTION("Bad access") {
        TablePIMPL p(80);
        REQUIRE_THROWS_AS(p(10, 10), std::out_of_range);
    }
    SECTION("Good access") {
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
}

TEST_CASE("TablePIMPL::str()") {
    SECTION("Empty") {
        TablePIMPL p(2, 2, 80);
        auto corr = "+--+--+\n"
                    "+==+==+\n"
                    "+--+--+";
        REQUIRE(p.str() == corr);
    }
    SECTION("All single lines") {
        TablePIMPL p(make_table("Data"), 80);
        auto corr = "+----------+----------+\n"
                    "| Header 1 | Header 2 |\n"
                    "+==========+==========+\n"
                    "| Value 1  | Data     |\n"
                    "+----------+----------+";
        REQUIRE(p.str() == corr);
    }
    SECTION("Multi line cell") {
        TablePIMPL p(make_table("Data\nMore Data"), 80);
        auto corr = "+----------+-----------+\n"
                    "| Header 1 | Header 2  |\n"
                    "+==========+===========+\n"
                    "| Value 1  | Data      |\n"
                    "|          | More Data |\n"
                    "+----------+-----------+";
        REQUIRE(p.str() == corr);
    }
    SECTION("Long cell that wraps") {
        TablePIMPL p(make_table("This is data for (1,1)"), 23);
        auto corr = "+----------+----------+\n"
                    "| Header 1 | Header 2 |\n"
                    "+==========+==========+\n"
                    "| Value 1  | This is  |\n"
                    "|          | data for |\n"
                    "|          | (1,1)    |\n"
                    "+----------+----------+";
        REQUIRE(p.str() == corr);
    }
    SECTION("Throws if line does not fit") {
        TablePIMPL p(make_table("Not-going-to-fit-in-the-column"), 23);
        REQUIRE_THROWS_AS(p.str(), std::runtime_error);
    }
}
