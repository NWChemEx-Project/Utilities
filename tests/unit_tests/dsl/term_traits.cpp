/*
 * Copyright 2024 NWChemEx-Project
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
#include <utilities/dsl/add.hpp>
#include <utilities/dsl/term_traits.hpp>

using namespace utilities;

/* Testing strategy.
 *
 * Our goal here is to verify that we implemented the TMP in the TermTraits
 * class correctly. That really depends on whether or not the template type
 * parameter T:
 *
 * - is const qualified,
 * - is derived from dsl::Term,
 * - is a floating point type
 */

TEST_CASE("TermTraits<char>") {
    using traits = dsl::TermTraits<char>;
    STATIC_REQUIRE_FALSE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, char>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const char&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, char&>);
    STATIC_REQUIRE_FALSE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, char*>);
    char c('c');
    REQUIRE(traits::make_holder(c) == &c);
    REQUIRE(&traits::unwrap_holder(traits::make_holder(c)) == &c);
}

TEST_CASE("TermTraits<const char>") {
    using traits = dsl::TermTraits<const char>;
    STATIC_REQUIRE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, char>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const char&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, const char&>);
    STATIC_REQUIRE_FALSE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, const char*>);
    const char c('c');
    REQUIRE(traits::make_holder(c) == &c);
    REQUIRE(&traits::unwrap_holder(traits::make_holder(c)) == &c);
}

TEST_CASE("TermTraits<double>") {
    using traits = dsl::TermTraits<double>;
    STATIC_REQUIRE_FALSE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, double>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const double&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, double&>);
    STATIC_REQUIRE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, double>);
    double c(42.0);
    REQUIRE(traits::make_holder(c) == c);
    REQUIRE(traits::unwrap_holder(traits::make_holder(c)) == c);
}

TEST_CASE("TermTraits<const double>") {
    using traits = dsl::TermTraits<const double>;
    STATIC_REQUIRE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, double>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const double&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, const double&>);
    STATIC_REQUIRE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, double>);
    const double c(42.0);
    REQUIRE(traits::make_holder(c) == c);
    REQUIRE(traits::unwrap_holder(traits::make_holder(c)) == c);
}

TEST_CASE("TermTraits<std::string>") {
    using traits = dsl::TermTraits<std::string>;
    STATIC_REQUIRE_FALSE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, std::string>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const std::string&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, std::string&>);
    STATIC_REQUIRE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, std::string>);
    std::string c("42.0");
    REQUIRE(traits::make_holder(c) == c);
    REQUIRE(traits::unwrap_holder(traits::make_holder(c)) == c);
}

TEST_CASE("TermTraits<const std::string>") {
    using traits = dsl::TermTraits<const std::string>;
    STATIC_REQUIRE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, std::string>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const std::string&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, const std::string&>);
    STATIC_REQUIRE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, std::string>);
    const std::string c("42.0");
    REQUIRE(traits::make_holder(c) == c);
    REQUIRE(traits::unwrap_holder(traits::make_holder(c)) == c);
}

TEST_CASE("TermTraits<Add<int, double>>") {
    using op_t   = dsl::Add<int, double>;
    using traits = dsl::TermTraits<op_t>;
    STATIC_REQUIRE_FALSE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, op_t>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const op_t&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, op_t&>);
    STATIC_REQUIRE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, op_t>);
    int a(42);
    double b(42.0);
    op_t c(a, b);
    REQUIRE(traits::make_holder(c) == c);
    REQUIRE(traits::unwrap_holder(traits::make_holder(c)) == c);
}

TEST_CASE("TermTraits<const Add<int, double>>") {
    using op_t   = dsl::Add<int, double>;
    using traits = dsl::TermTraits<const op_t>;
    STATIC_REQUIRE(traits::is_const_v);
    STATIC_REQUIRE(std::is_same_v<traits::value_type, op_t>);
    STATIC_REQUIRE(std::is_same_v<traits::const_reference, const op_t&>);
    STATIC_REQUIRE(std::is_same_v<traits::reference, const op_t&>);
    STATIC_REQUIRE(traits::is_dsl_term_v);
    STATIC_REQUIRE(std::is_same_v<traits::holder_type, op_t>);
    int a(42);
    double b(42.0);
    const op_t c(a, b);
    REQUIRE(traits::make_holder(c) == c);
    REQUIRE(traits::unwrap_holder(traits::make_holder(c)) == c);
}