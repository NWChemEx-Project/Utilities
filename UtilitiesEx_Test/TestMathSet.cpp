#include <UtilitiesEx/Containers/MathSet.hpp>
#include <UtilitiesEx/TypeTraits/type_traitsExtensions.hpp>
#include <catch/catch.hpp>

using namespace UtilitiesEx;

TEST_CASE("Is a container")
{
    REQUIRE(is_container<MathSet<double>>::value);
}

void check_state(MathSet<double>& set,
                 std::size_t size,
                 std::initializer_list<double> values)
{
    REQUIRE(set.size() == size);
    REQUIRE(set.empty() == (size==0));

}

TEST_CASE("MathSet<double>")
{
    MathSet<double> set1;

    SECTION("Default constructor")
    {
        check_state(set1,0);
    }

    set1.insert(1.1);

    SECTION("Adding an element")
    {
        check_state(set1,1);
    }

    MathSet<double> set2(set1);

    SECTION("Copy constructor")
    {
        check_state(set2,1);
    }

}


