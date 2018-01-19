#include <UtilitiesEx/IterTools/Zip.hpp>
#include <UtilitiesEx/TypeTraits/type_traitsExtensions.hpp>
#include <catch/catch.hpp>

using namespace UtilitiesEx;

TEST_CASE("Two same size arrays")
{
    std::vector<int> numbers({1,2,3});
    std::vector<char> letters({'a','b','c'});
    auto test_container = Zip(numbers,letters);
    REQUIRE(test_container.size() == 3 );
    auto itr = test_container.begin();
    auto end = test_container.end();

    REQUIRE(itr != end);

    SECTION("Test iterator")
    {
        REQUIRE(is_input_iterator<decltype(itr)>::value);
        auto element1 = *itr;
        REQUIRE(std::get<0>(element1) == 1);
        REQUIRE(std::get<1>(element1) == 'a');

        auto& pitr = (++itr);
        REQUIRE(&pitr == &itr);
        REQUIRE( itr != end );
        auto element2 = *itr;
        REQUIRE(std::get<0>(element2) == 2);
        REQUIRE(std::get<1>(element2) == 'b');

        ++itr;
        REQUIRE(itr != end);
        auto element3 = *itr;
        REQUIRE(std::get<0>(element3) == 3);
        REQUIRE(std::get<1>(element3) == 'c');

        ++itr;
        REQUIRE(itr == end);
    }

}
