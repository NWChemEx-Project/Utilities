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

#include <catch2/catch.hpp>
#include <utilities/printing/table.hpp>

using namespace utilities::printing;
using table_type = typename Table::table_type;

static inline auto make_table(std::string elem) {
    return std::vector{
      std::vector{std::string("Header 1"), std::string("Header 2")},
      std::vector{std::string("Value 1"), std::move(elem)}};
}

TEST_CASE("Table(int)") {
    Table t(80);
    REQUIRE(t.nrows() == 0);
    REQUIRE(t.ncols() == 0);
}

TEST_CASE("Table(int, int, int)") {
    Table t(2, 2, 80);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
}

TEST_CASE("Table(table, int)") {
    Table t(make_table("Value 2"), 80);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
}

TEST_CASE("Table::add_rows") {
    Table t(80);
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

TEST_CASE("Table::add_cols") {
    Table t(80);

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

TEST_CASE("Table::set_size") {
    Table t(80);

    t.set_size(2, 2);
    REQUIRE(t.nrows() == 2);
    REQUIRE(t.ncols() == 2);
}

TEST_CASE("Table::operator()") {
    SECTION("Bad access") {
        Table p(80);
        REQUIRE_THROWS_AS(p(10, 10), std::out_of_range);
    }
    SECTION("Good access") {
        Table p(2, 2, 80);
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

TEST_CASE("Table::str()") {
    SECTION("Empty") {
        Table p(2, 2, 80);
        auto corr = "+--+--+\n"
                    "+==+==+\n"
                    "+--+--+";
        REQUIRE(p.str() == corr);
    }
    SECTION("All single lines") {
        Table p(make_table("Data"), 80);
        auto corr = "+----------+----------+\n"
                    "| Header 1 | Header 2 |\n"
                    "+==========+==========+\n"
                    "| Value 1  | Data     |\n"
                    "+----------+----------+";
        REQUIRE(p.str() == corr);
    }
    SECTION("Multi line cell") {
        Table p(make_table("Data\nMore Data"), 80);
        auto corr = "+----------+-----------+\n"
                    "| Header 1 | Header 2  |\n"
                    "+==========+===========+\n"
                    "| Value 1  | Data      |\n"
                    "|          | More Data |\n"
                    "+----------+-----------+";
        REQUIRE(p.str() == corr);
    }
    SECTION("Long cell that wraps") {
        Table p(make_table("This is data for (1,1)"), 23);
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
        Table p(make_table("Not-going-to-fit-in-the-column"), 23);
        REQUIRE_THROWS_AS(p.str(), std::runtime_error);
    }
}
