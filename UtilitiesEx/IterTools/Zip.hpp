#pragma once
#include "UtilitiesEx/TupleUtilities.hpp" //Includes tuple always
#include "UtilitiesEx/TypeTraits/IteratorTypes.hpp"
#include <algorithm>//For std::min
#include <limits> //For numeric_limits
#include <functional> //For reference wrapper

namespace UtilitiesEx {
namespace detail_ {

/** @brief Simulates a container filled with all tuples of the elements in a
 *         series of iterators.
 *
 *  The purpose/usage of this class is perhaps best explained with an example.
 *  Consider the following snippet.
 *
 *  @code
 *  std::vector<int> list1({1,2,3,4});
 *  std::vector<int> list2({2,3,4,5});
 *  for(auto& x : Zip(list1.begin(),list2.begin())
 *      std::cout<< std::get<0>(x) << " " << std::get<1>(x) <<std::endl;
 *  @endcode
 *
 *  The result would be:
 *
 *  @verbatim
 *  1 2
 *  2 3
 *  3 4
 *  4 5
 *  @endverbatim
 *
 *  Thus this class increments an arbitrary number of iterators in unison
 *  as if the tuple of elements were in stored in a container.
 *
 *  @note This class is the implementation of the innards of the Zip function, but
 *  for all intents and purposes it is Zip.  The Zip function only exists so
 *  the usage syntax is not:
 *  @code
 *  for(auto x : Zip<iterator_t1,iterator_t2,...>(iterator1,iterator2,...))
 *  {
 *      ...
 *  }
 *  @endcode
 *
 *  @note If the lengths of the iterators are unequal iteration will stop when
 *  the shortest iterator runs out of elements.
 *
 *
 * @tparam ContainerTypes The types of the containers that will be zipped
 *                       together.
 */

template <typename...ContainerTypes>
class ZipImpl {
private:
    ///Forward declaration of the actual iterator type
    class ZipIterator;

    ///Functor for calling the size member of each container.
    struct SizeFunctor {
        template<typename T>
        auto operator()(std::size_t curr_min, T container)const
        {
            return std::min(curr_min,container.get().size());
        }
    };
public:
    ///The type of an element in this container.
    using value_type =
        std::tuple<typename ContainerTypes::iterator::value_type...>;
    using reference = value_type&;
    using const_reference = const value_type&;

    using iterator = ZipIterator;
    using const_iterator = ZipIterator;

    template<typename...InputTypes>
    ZipImpl(InputTypes&&...containers) :
            itrs_(std::ref(std::forward<InputTypes>(containers))...),
            size_(reduce_tuple(itrs_, SizeFunctor(),
                               std::numeric_limits<std::size_t>::max()))
    {
    }

    /** @brief Returns the number of tuples in this container.
     *
     *  Given a list of containers such that the @f$i@f$-th container contains
     *  @f$N_i@f$ elements.  This function returns the smallest @f$N_i@f$ as
     *  that is the maximum number of iterations possible.
     *
     *  @return The number of tuples in this container.
     */
    std::size_t size()const noexcept { return size_; }

    iterator begin()
    {
        return iterator(itrs_, false);
    }

    const_iterator cbegin() const
    {
        return const_iterator(itrs_, false);
    }

    iterator end()
    {
        return iterator(itrs_, true);
    }

    const_iterator cend() const
    {
        return const_iterator(itrs_, true);
    }

private:
    ///The number of containers we are zipping together
    constexpr static std::size_t ncontainers_ = sizeof...(ContainerTypes);

    ///The type of the tuple holding references to our containers
    using tuple_type = std::tuple<std::reference_wrapper<ContainerTypes>...>;

    ///The type of a tuple holding iterators to the containers
    using iterator_tuple = std::tuple<typename ContainerTypes::iterator...>;

    ///The actual tuple of containers
    tuple_type itrs_;

    ///The number of iterations in this container
    std::size_t size_;

    ///The class responsible for iterating over the container
    class ZipIterator :
            public detail_::InputIteratorBase<ZipIterator, value_type> {
    private:
        friend class ZipImpl<ContainerTypes...>;
        friend class detail_::InputIteratorBase<ZipIterator, value_type>;

        ZipIterator(tuple_type itrs, bool at_end) :
                start_(apply_functor_to_tuple(itrs, CallBegin())),
                end_(apply_functor_to_tuple(itrs, CallEnd())),
                value_(!at_end ? start_ : end_),
                buffer_(apply_functor_to_tuple(value_,Derefer()))
        { }

        reference dereference()
        {
            return buffer_;
        }

        ///Implements the mechanism for dereferencing this iterator
        const_reference dereference()const
        {

            return buffer_;
        }

        ///Implements the mechansim for incrementing this iterator
        ZipIterator& increment()noexcept
        {
            return increment_imp(std::make_index_sequence<ncontainers_>());
        }

        ///Implements the mechanism for checking if this iterator equals another
        bool are_equal(const ZipIterator& rhs) const noexcept
        {
            return value_ == rhs.value_;
        }


        ///Functor for calling begin() on all containers
        struct CallBegin {
            template<typename T>
            auto operator()(T container){ return container.get().begin();}
        };

        ///Functor for calling end() on all containers
        struct CallEnd {
            template<typename T>
            auto operator()(T container)const{ return container.get().end();}
        };

        struct Derefer {
            template<typename T>
            auto operator()(T itr)const{ return *itr;}
        };

        ///Functor for comparing
        struct Comparer {
            template<typename lhs_type, typename rhs_type>
            auto operator()(lhs_type lhs, rhs_type rhs) const
            {
                return lhs == rhs;
            }
        };

        struct AnyTrue{
            template<typename T>
            bool operator()(bool val, T element)const{return val || element;}
        };


        template <std::size_t...I>
        ZipIterator& increment_imp(std::index_sequence<I...>)
        {
            value_ = std::make_tuple((++std::get<I>(value_))...);
            //Check if any iterator is at the end, if so set them all to end
            auto at_end = combine_tuples(value_, end_, Comparer());
            bool done = reduce_tuple(at_end, AnyTrue(), false);
            if(done)
                value_ = end_;
            else
                buffer_ = apply_functor_to_tuple(value_, Derefer());
            return *this;
        }

        iterator_tuple start_;
        iterator_tuple end_;
        iterator_tuple value_;

        value_type buffer_;


    };
};
}//End namespace detail_

template<typename...ContainerTypes>
auto Zip(ContainerTypes&&...containers)
{
    return detail_::ZipImpl<std::decay_t<ContainerTypes>...>
            (std::forward<ContainerTypes>(containers)...);
}

}//End namespace


