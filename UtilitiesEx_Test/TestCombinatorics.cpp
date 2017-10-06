#include "IterTools/Combinatorics.hpp"
#define CATCH_CONFIG_MAIN
#include "/home/ryan/Catch/single_include/catch.hpp"

using namespace IterTools;

TEST_CASE("Binomial coefficient")
{
    REQUIRE(binomial_coefficient<size_t>(0,0)==1);
    REQUIRE(binomial_coefficient<size_t>(0,1)==0);
    REQUIRE(binomial_coefficient<size_t>(4,0)==1);
    REQUIRE(binomial_coefficient<size_t>(35,4)==52360);
    REQUIRE_THROWS_AS(binomial_coefficient<size_t>(99999,88),std::overflow_error);
}

TEST_CASE("Multinomial coefficient")
{
    using set_type=std::vector<size_t>;
    REQUIRE(multinomial_coefficient<size_t>(set_type{})==1);
    REQUIRE(multinomial_coefficient<size_t>(set_type{5})==1);
    REQUIRE(multinomial_coefficient<size_t>(set_type{5,6,7})==14702688);
    REQUIRE(multinomial_coefficient<size_t>(set_type{4,31})==52360);
}
