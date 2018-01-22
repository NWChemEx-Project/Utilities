#include <UtilitiesEx/TypeTraits/TupleUtilities.hpp>
#include <catch/catch.hpp>

using namespace UtilitiesEx;

//Just returns the value given to it
struct functor1 {
    template<typename T>
    T operator()(T in)const noexcept{return in;}
};

//Returns the value of calling size()
struct functor2 {
    template<typename T>
    auto operator()(T in)const noexcept{ return in.size();}
};

//Sums the second element of each input
struct functor3 {
    template<typename T>
    int operator()(int val, T in)const noexcept{return val + in[1];}
};

//Sums the second element of each element
struct functor4 {
    template<typename lhs_type, typename rhs_type>
    auto operator()(lhs_type lhs,rhs_type rhs)const noexcept
    {
        return lhs[1]+rhs[1];
    }

};

TEST_CASE("Helper Types")
{
    REQUIRE(detail_::recursion_done<2, std::tuple<int,int>>::value);
    REQUIRE(!detail_::recursion_done<1, std::tuple<int,int>>::value);
    REQUIRE(!detail_::recursion_not_done<2, std::tuple<int,int>>::value);
    REQUIRE(detail_::recursion_not_done<1, std::tuple<int,int>>::value);
}


TEST_CASE("apply_functor")
{
    SECTION("Empty tuple")
    {
        auto my_tuple = std::make_tuple();
        auto result = apply_functor_to_tuple(my_tuple, functor1());
        REQUIRE(result == my_tuple);
    }

    SECTION("Simple functor and simple tuple")
    {
        auto my_tuple = std::make_tuple(1,'a',4.5);
        auto result = apply_functor_to_tuple(my_tuple,functor1());
        REQUIRE(std::get<0>(result) == 1);
        REQUIRE(std::get<1>(result) == 'a');
        REQUIRE(std::get<2>(result) == 4.5);
    }

    SECTION("More complicated functor and tuple")
    {
        auto my_tuple = std::make_tuple(std::vector<int>({1,2,3}),
                                        std::array<char,2>({'a','b'}));
        auto result = apply_functor_to_tuple(my_tuple, functor2());
        REQUIRE(std::get<0>(result) == 3);
        REQUIRE(std::get<1>(result) == 2);
    }

    SECTION("Constant tuple")
    {
        std::tuple<int,int> my_tuple({3,4});
        const auto& cmy_tuple =
                const_cast<const std::tuple<int,int>&>(my_tuple);
        auto result = apply_functor_to_tuple(cmy_tuple, functor1());
        REQUIRE(std::get<0>(result) == 3);
        REQUIRE(std::get<1>(result) == 4);
    }
}

TEST_CASE("reduce_tuple")
{
    SECTION("Empty tuple")
    {
        auto my_tuple = std::make_tuple();
        auto sum = reduce_tuple(my_tuple, functor3(), 100);
        REQUIRE(sum == 100);
    }

    SECTION("Non-trivial tuple")
    {
        auto my_tuple = std::make_tuple(std::vector<int>({1,2,3}),
                                        std::vector<int>({6,2,9}));
        auto sum = reduce_tuple(my_tuple,functor3(),0);
        REQUIRE(sum == 4);
    }
}

TEST_CASE("combine_tuples")
{
    SECTION("Trivial tuples")
    {
        auto lhs_tuple = std::make_tuple();
        auto rhs_tuple = std::make_tuple();
        auto sum = combine_tuples(lhs_tuple, rhs_tuple, functor4());
        REQUIRE(sum == lhs_tuple);
    }

    SECTION("Non-trivial tuples")
    {
        auto lhs_tuple = std::make_tuple(std::array<int,3>({1,2,3}),
                                         std::array<int,2>({6,7}));
        auto sum = combine_tuples(lhs_tuple, lhs_tuple, functor4());
        REQUIRE(std::get<0>(sum) == 4);
        REQUIRE(std::get<1>(sum) == 14);
    }


}
