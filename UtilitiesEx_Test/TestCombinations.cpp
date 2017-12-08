#include <UtilitiesEx/IterTools/Combinations.hpp>
#include <UtilitiesEx/TypeTraits/type_traitsExtensions.hpp>
#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

using namespace UtilitiesEx;
using set_type=std::vector<int>;
using comb_type=Combinations<set_type>;
using iterator=typename comb_type::iterator;

/* General note: testing of the Permutations class really amounts to testing its
 * iterators.
 */
TEST_CASE("Satisfy STL concepts")
{
    bool is_cont=is_container<comb_type>::value;
    bool is_bid=is_random_access_iterator<iterator>::value;
    REQUIRE(is_cont);
    REQUIRE(is_bid);
}

TEST_CASE("Intentional Fail")
{
  int i=0;
  int j=1;
    REQUIRE(i==j);
}


TEST_CASE("Empty Combination")
{
    comb_type p0;
    auto begin_itr=p0.cbegin();
    auto end_itr=p0.cend();
    REQUIRE(p0.size() == 0);
    REQUIRE(p0.max_size() == std::numeric_limits<std::size_t>::max());
    REQUIRE(p0.empty());
    REQUIRE(begin_itr.are_equal(end_itr));


    SECTION("Iterator is copyable")
    {
        iterator copy_itr(begin_itr);
        auto pbegin = &(*begin_itr);
        auto pcopy = &(*copy_itr);
        REQUIRE(copy_itr == begin_itr);
        REQUIRE(pbegin != pcopy);
    }

    SECTION("Iterator is assignable")
    {
        end_itr=begin_itr;
        REQUIRE(end_itr == begin_itr);
    }

    SECTION("Iterator is move constructable")
    {
        iterator    move_itr(std::move(begin_itr));
        REQUIRE(move_itr == p0.begin());
    }

    SECTION("Iterator is move assignable")
    {
        auto& pend_itr=(end_itr=std::move(begin_itr));
        REQUIRE(&pend_itr == &end_itr);
        REQUIRE(end_itr == p0.begin());
    }
}

TEST_CASE("Combinations of empty set")
{
    comb_type p0(set_type({}),0);
    auto begin_itr=p0.cbegin();
    auto end_itr=p0.cend();
    REQUIRE(p0.size() == 1);
    REQUIRE(p0.max_size() == std::numeric_limits<std::size_t>::max());
    REQUIRE(!p0.empty());
    REQUIRE(begin_itr.dereference() == set_type());
    REQUIRE(! begin_itr.are_equal(end_itr));

    SECTION("One prefix increment ends and doesn't copy")
    {
        auto& pbegin_itr=begin_itr.increment();
        REQUIRE(&pbegin_itr == &begin_itr);
        REQUIRE(begin_itr == end_itr);
    }
}

TEST_CASE("Combinations instance {1,2,3} (i.e. no duplicates)")
{
    set_type numbers({1, 2, 3});
    comb_type p0(numbers,2);
    auto begin_itr = p0.cbegin();
    auto end_itr = p0.cend();
    REQUIRE(p0.size() == 3);
    REQUIRE(*begin_itr == set_type({1,2}));
    REQUIRE(begin_itr.distance_to(end_itr)==3);
    REQUIRE(begin_itr != end_itr);

    SECTION("Combinations is copyable")
    {
        comb_type p1(p0);
        REQUIRE(p1 == p0);

        SECTION("Combinations are swappable")
        {
            comb_type p2;
            p2.swap(p0);
            REQUIRE(p2 == p1);
        }

        SECTION("Combinations is movable")
        {
            comb_type p2(std::move(p0));
            REQUIRE(p1 == p2);
        }

        SECTION("Combinations is move assignable")
        {
            comb_type p2;
            REQUIRE(p2 != p1);
            auto& pp2 = (p2=std::move(p0));
            REQUIRE(&p2 == &pp2);
            REQUIRE(p2 == p1);
        }
    }

    SECTION("Combinations are assignable")
    {
        comb_type p1;
        REQUIRE(p1 != p0);
        auto& pp1 = (p1=p0);//Grabs the return value
        REQUIRE(&p1 == &pp1);
        REQUIRE(p1 == p0);
    }

    SECTION("prefix increment leads to {1,3} and no copy")
    {
        set_type next_perm({1,3});
        ++begin_itr;
        REQUIRE(*begin_itr == next_perm);
        REQUIRE(begin_itr != end_itr);

        SECTION("postfix increment leads to {2,3} and copy")
        {
            auto rv = begin_itr++;
            REQUIRE(*begin_itr == set_type({2,3}));
            REQUIRE(begin_itr != end_itr);
            REQUIRE(*rv== next_perm);
            REQUIRE(rv.advance(1) == begin_itr);
        }
        SECTION("incrementing 2 more time ends")
        {
            ++begin_itr;++begin_itr;
            REQUIRE(begin_itr == end_itr);
        }


        SECTION("prefix decrement leads to {1,2} and no copy")
        {
            auto& pbegin_itr = --begin_itr;
            REQUIRE(*pbegin_itr == set_type({1,2}));
            REQUIRE(&pbegin_itr == &begin_itr);
            REQUIRE(begin_itr != end_itr);
        }

        SECTION("postfix decrement leads to {1,2} and copy")
        {
            auto rv = begin_itr--;
            REQUIRE(*begin_itr == set_type({1,2}));
            REQUIRE(*rv == set_type({1,3}));
            REQUIRE(begin_itr != end_itr);
        }
    }
}

TEST_CASE("Multicombinations")
{
    SECTION("n==k==0")
    {
        set_type elems;
        Combinations<set_type,true> c0(elems,0);
        REQUIRE(c0.size()==1);
        REQUIRE(c0[0]==elems);
    }

    SECTION("n>0 k<n")
    {
        set_type elems({1,2,3});
        Combinations<set_type,true> c0(elems,2);
        REQUIRE(c0.size()==6);
        REQUIRE(c0[0]==set_type({1,1}));
        REQUIRE(c0[1]==set_type({1,2}));
        REQUIRE(c0[2]==set_type({1,3}));
        REQUIRE(c0[3]==set_type({2,2}));
        REQUIRE(c0[4]==set_type({2,3}));
        REQUIRE(c0[5]==set_type({3,3}));
    }

    SECTION("n>0 k>n")
    {
        set_type elems({1,2});
        Combinations<set_type,true> c0(elems,3);
        REQUIRE(c0.size()==4);
        REQUIRE(c0[0]==set_type({1,1,1}));
        REQUIRE(c0[1]==set_type({1,1,2}));
        REQUIRE(c0[2]==set_type({1,2,2}));
        REQUIRE(c0[3]==set_type({2,2,2}));
    }
}
