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
#include <utilities/printing/demangler.hpp>
#include <vector>

using namespace utilities::printing;

const auto corr1 = "int";
const auto corr2 = "double";
#if __APPLE__ && __clang__
const auto corr3 = "std::__1::vector<std::__1::vector<double, "
                   "std::__1::allocator<double> >, "
                   "std::__1::allocator<std::__1::vector<double, "
                   "std::__1::allocator<double> > > >";
#else
const auto corr3 = "std::vector<std::vector<double, std::allocator<double> >,"
                   " std::allocator<std::vector<double, std::allocator<double> "
                   "> > >";
#endif

using matrix_t = std::vector<std::vector<double>>;

TEST_CASE("Demangler : template") {
    REQUIRE(Demangler::demangle<int>() == corr1);
    REQUIRE(Demangler::demangle<double>() == corr2);
    REQUIRE(Demangler::demangle<matrix_t>() == corr3);
}

TEST_CASE("Demangler : type_info") {
    REQUIRE(Demangler::demangle(typeid(int)) == corr1);
    REQUIRE(Demangler::demangle(typeid(double)) == corr2);
    REQUIRE(Demangler::demangle(typeid(matrix_t)) == corr3);
}

TEST_CASE("Demangler : type_index") {
    REQUIRE(Demangler::demangle(std::type_index(typeid(int))) == corr1);
    REQUIRE(Demangler::demangle(std::type_index(typeid(double))) == corr2);
    REQUIRE(Demangler::demangle(std::type_index(typeid(matrix_t))) == corr3);
}

TEST_CASE("Demangler : string literal") {
    REQUIRE(Demangler::demangle(typeid(int).name()) == corr1);
    REQUIRE(Demangler::demangle(typeid(double).name()) == corr2);
    REQUIRE(Demangler::demangle(typeid(matrix_t).name()) == corr3);
}
