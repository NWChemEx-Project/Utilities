#include <UtilitiesEx/IterTools/Permutations.hpp>
#include <UtilitiesEx/TypeTraits/type_traitsExtensions.hpp>
#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
#include <type_traits>

using namespace UtilitiesEx;
using set_type=std::vector<int>;
using perm_type=Permutations<set_type>;
using iterator=typename perm_type::iterator;

/* General note: testing of the Permutations class really amounts to testing its
 * iterators.
 */
TEST_CASE("Satisfy STL concepts")
{
    bool is_cont=is_container<perm_type>::value;
    bool is_bid=is_random_access_iterator<iterator>::value;
    REQUIRE(is_cont);
    REQUIRE(is_bid);
}


TEST_CASE("Empty Permutation")
{
    Permutations<set_type> p0;
    auto begin_itr=p0.cbegin();
    auto end_itr=p0.cend();
    REQUIRE(p0.size() == 1);
    REQUIRE(p0.max_size() == std::numeric_limits<std::size_t>::max());
    REQUIRE(p0.empty());
    REQUIRE(*begin_itr == set_type());
    REQUIRE(!(end_itr < begin_itr));
    REQUIRE(begin_itr < end_itr);
    REQUIRE(!(begin_itr > end_itr));
    REQUIRE(end_itr > begin_itr);
    REQUIRE(begin_itr <= begin_itr);
    REQUIRE(end_itr >= end_itr);
    REQUIRE(p0[0] == set_type());
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
    SECTION("Adding one to iterator terminates and copy")
    {
        auto new_itr = begin_itr +1;
        REQUIRE(new_itr == end_itr);
        REQUIRE(begin_itr != end_itr);
    }
    SECTION("Incrementing by 1 leads to no copy and termination")
    {
        auto& old_itr = (begin_itr+=1);
        REQUIRE(begin_itr == end_itr);
        REQUIRE(&old_itr == & begin_itr);
    }
}

TEST_CASE("Permutations instance {1,2,3} (i.e. no duplicates)")
{
    set_type numbers({1, 2, 3});
    Permutations <set_type> p0(numbers);
    auto begin_itr = p0.cbegin();
    auto end_itr = p0.cend();
    REQUIRE(p0.size() == 6);
    REQUIRE(p0[0] == numbers);
    REQUIRE(*begin_itr == numbers);
    REQUIRE(begin_itr != end_itr);

    SECTION("Permutations is copyable")
    {
        Permutations<set_type> p1(p0);
        REQUIRE(p1 == p0);

        SECTION("Permutation is swappable")
        {
            Permutations<set_type> p2;
            p2.swap(p0);
            REQUIRE(p2 == p1);
        }


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
        REQUIRE(p0[1] == next_perm);
        REQUIRE(*begin_itr == next_perm);
        REQUIRE(begin_itr != end_itr);

        SECTION("postfix increment leads to {2,1,3} and copy")
        {
            auto rv=begin_itr++;
            set_type next_next_perm({2,1,3});
            REQUIRE(p0[2] == next_next_perm);
            REQUIRE(*begin_itr == next_next_perm);
            REQUIRE(begin_itr[-1] == next_perm);
            REQUIRE(rv[1] == next_next_perm);
            REQUIRE(begin_itr != end_itr);
            REQUIRE(*rv == next_perm);

            SECTION("incrementing 3 more times leads to {3,2,1}")
            {
                ++begin_itr;++begin_itr;++begin_itr;
                set_type last_perm({3,2,1});
                REQUIRE(p0[5] == last_perm);
                REQUIRE(*begin_itr == last_perm);
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

        SECTION("Subtracting 1 leads to {1,2,3} and copy")
        {
            auto new_itr = begin_itr-1;
            REQUIRE(*new_itr == numbers);
            REQUIRE(*begin_itr == next_perm);
        }
        SECTION("Decrementing by one leads to {1,2,3} and a copy")
        {
            auto& old_itr = begin_itr-=1;
            REQUIRE(&old_itr == &begin_itr);
            REQUIRE(*old_itr == numbers);
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
    REQUIRE(p0[0] == numbers);
    REQUIRE(*begin_itr == numbers);
    REQUIRE(begin_itr != end_itr);

    SECTION("prefix increment leads to {2,1,2}")
    {
        ++begin_itr;
        set_type next_perm({2,1,2});
        REQUIRE(p0[1] == next_perm);
        REQUIRE(*begin_itr == next_perm);
        REQUIRE(begin_itr != end_itr);

        SECTION("prefix increment leads to {2,2,1}")
        {
            ++begin_itr;
            set_type last_perm({2,2,1});
            REQUIRE(p0[2] == last_perm);
            REQUIRE(*begin_itr == last_perm);
            REQUIRE(begin_itr != end_itr);

            SECTION("Next increment ends")
            {
                ++begin_itr;
                REQUIRE(begin_itr==end_itr);
            }
        }

    }
}

