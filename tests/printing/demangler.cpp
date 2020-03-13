#include <catch2/catch.hpp>
#include <utilities/printing/demangler.hpp>
#include <vector>

using namespace utilities::printing;

const auto corr1 = "int";
const auto corr2 = "double";
#if __clang__
const auto corr3 = "std::__1::vector<std::__1::vector<double, "
                   "std::__1::allocator<double> >, "
                   "std::__1::allocator<std::__1::vector<double, "
                   "std::__1::allocator<double> > > >";
#elif __GNUC__
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
