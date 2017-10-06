#include <IterTools/Permutations.hpp>
#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
#include <type_traits>

using namespace IterTools;
using set_type=std::vector<int>;
using perm_type=Permuations<set_type>;

/* General note: testing of the Permutations class really amounts to testing its
 * iterators.
 */

TEST_CASE("Permutation satisfies container concept")
{

    SECTION("Typedefs")
    {
        REQUIRE(std::is_same<typename perm_type::value_type,set_type>::value);
        REQUIRE(std::is_same<typename perm_type::reference,
                set_type &>::value);
        REQUIRE(std::is_same<typename perm_type::const_reference,
                const set_type&>::value);
        REQUIRE(std::is_same<typename perm_type::iterator,
                typename perm_type::PermutationItr>::value);
        REQUIRE(std::is_same<typename perm_type::const_iterator,
                const typename perm_type::PermutationItr>::value);
        REQUIRE(std::is_same<typename perm_type::difference_type,
                long int>::value);
        REQUIRE(std::is_same<typename perm_type::size_type,
                std::size_t>::value);
    }
    SECTION("Members")
    {
        REQUIRE(std::is_default_constructible<perm_type>::value);
        REQUIRE(std::is_copy_constructible<perm_type>::value);
        REQUIRE(std::is_assignable<perm_type>::value);
        REQUIRE(std::is_destructible<perm_type>::value);
        REQUIRE(std::is_member_function<decltype(&perm_type::begin)>::value);
        REQUIRE(std::is_swappable<perm_type>::value);
    }
}


TEST_CASE("Empty Permutation")
{
    Permutations<set_type> p0;
    auto begin_itr=p0.cbegin();
    auto end_itr=p0.cend();
    REQUIRE(p0.size() == 1);
    REQUIRE(*begin_itr == set_type());
    REQUIRE(begin_itr->size()==0);
    REQUIRE(begin_itr != end_itr);//Empty set has 1 permutation

    SECTION("Iterator is copyable")
    {
        typename Permutations<set_type>::const_iterator copy_itr(begin_itr);
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
        typename Permutations<set_type>::const_iterator
                move_itr(std::move(begin_itr));
        REQUIRE(move_itr == p0.begin());
    }

    SECTION("Iterator is move assignable")
    {
        auto& pend_itr=(end_itr=std::move(begin_itr));
        REQUIRE(&pend_itr == &end_itr);
        REQUIRE(end_itr == p0.begin());
    }

    SECTION("One prefix increment ends and doesn't copy")
    {
        auto& pbegin_itr=++begin_itr;
        REQUIRE(&pbegin_itr == &begin_itr);
        REQUIRE(begin_itr == end_itr);

    }
}

TEST_CASE("Permutations instance {1,2,3} (i.e. no duplicates)")
{
    set_type numbers({1, 2, 3});
    Permutations <set_type> p0(numbers);
    auto begin_itr = p0.cbegin();
    auto end_itr = p0.cend();
    REQUIRE(p0.size() == 6);
    REQUIRE(*begin_itr == numbers);
    REQUIRE(begin_itr != end_itr);

    SECTION("Permutations is copyable")
    {
        Permutations<set_type> p1(p0);
        REQUIRE(p1 == p0);

        SECTION("Permutations is movable")
        {
            Permutations<set_type> p2(std::move(p0));
            REQUIRE(p1 == p2);
        }

        SECTION("Permutations is move assignable")
        {
            Permutations<set_type> p2;
            REQUIRE(p2 != p1);
            auto& pp2 = (p2=std::move(p0));
            REQUIRE(&p2 == &pp2);
            REQUIRE(p2 == p1);
        }
    }

    SECTION("Permutations is assignable")
    {
        Permutations<set_type> p1;
        REQUIRE(p1 != p0);
        auto& pp1 = (p1=p0);//Grabs the return value
        REQUIRE(&p1 == &pp1);
        REQUIRE(p1 == p0);
    }




    SECTION("prefix increment leads to {1,3,2} and no copy")
    {
        set_type next_perm({1,3,2});
        ++begin_itr;
        REQUIRE(*begin_itr == next_perm);
        REQUIRE(begin_itr != end_itr);

        SECTION("postfix increment leads to {2,1,3} and copy")
        {
            auto rv=begin_itr++;
            REQUIRE(*begin_itr == set_type({2,1,3}));
            REQUIRE(begin_itr != end_itr);
            REQUIRE(*rv == next_perm);

            SECTION("incrementing 3 more times leads to {3,2,1}")
            {
                ++begin_itr;++begin_itr;++begin_itr;
                REQUIRE(*begin_itr == set_type({3,2,1}));
                REQUIRE(begin_itr != end_itr);

                SECTION("Next increment ends")
                {
                    ++begin_itr;
                    REQUIRE(begin_itr==end_itr);
                }

            }
        }

        SECTION("prefix decrement leads to {1,2,3} and no copy")
        {
            auto& pbegin_itr = --begin_itr;
            REQUIRE(*pbegin_itr == numbers);
            REQUIRE(&pbegin_itr == &begin_itr);
            REQUIRE(begin_itr != end_itr);
        }

        SECTION("postfix decrement leads to {1,2,3} and copy")
        {
            auto rv = begin_itr--;
            REQUIRE(*begin_itr == numbers);
            REQUIRE(*rv ==next_perm);
            REQUIRE(begin_itr != end_itr);
        }
    }
}

TEST_CASE("Permutations instance {1,2,2} (i.e. duplicates)")
{
    set_type numbers({1, 2, 2});
    Permutations <set_type> p0(numbers);
    auto begin_itr = p0.cbegin();
    auto end_itr = p0.cend();
    REQUIRE(p0.size() == 3);
    REQUIRE(*begin_itr == numbers);
    REQUIRE(begin_itr != end_itr);

    SECTION("prefix increment leads to {2,1,2}")
    {
        ++begin_itr;
        REQUIRE(*begin_itr == set_type({2,1,2}));
        REQUIRE(begin_itr != end_itr);

        SECTION("prefix increment leads to {2,2,1}")
        {
            ++begin_itr;
            REQUIRE(*begin_itr == set_type({2,2,1}));
            REQUIRE(begin_itr != end_itr);

            SECTION("Next increment ends")
            {
                ++begin_itr;
                REQUIRE(begin_itr==end_itr);
            }
        }

    }
}

