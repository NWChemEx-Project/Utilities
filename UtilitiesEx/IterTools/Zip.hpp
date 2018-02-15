#pragma once
#include "UtilitiesEx/TypeTraits/IteratorTypes.hpp"
#include "UtilitiesEx/TypeTraits/TupleUtilities.hpp" //Includes tuple always
#include <algorithm>                                 //For std::min
#include <limits>                                    //For numeric_limits
#include <type_traits>

namespace UtilitiesEx {
namespace detail_ {

/// Forward declaration of ZipImpl for meta-programming
template<typename... ContainerTypes>
class ZipImpl;

/// Struct which contains value==false if T is not a ZipImpl instance
template<typename T>
struct IsZipImpl : std::false_type {};

/// Specialization of IsZipImpl to trigger value == true
template<typename... Args>
struct IsZipImpl<ZipImpl<Args...>> : std::true_type {};

/// This will allow us to use iterators which are actually raw pointers
template<typename T>
struct GetValueType {
    using type = typename T::value_type;
};

/// Partial specialization to catch raw pointers
template<typename T>
struct GetValueType<T*> {
    using type = T;
};

template<typename T>
using IteratorType = std::conditional_t<
                       std::is_same<T, std::remove_cv_t<T>>::value,
                       typename T::iterator,
                       typename T::const_iterator>;

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
 *  as if the tuple of elements were in stored in a container (they are
 *  generated on the fly and not actually stored).
 *
 *  @note This class is the implementation of the innards of the Zip function,
 * but for all intents and purposes it is Zip.  The Zip function only exists so
 *  the usage syntax is not:
 *  @code
 *  for(auto& x : Zip<iterator_t1,iterator_t2,...>(iterator1,iterator2,...))
 *  {
 *      ...
 *  }
 *  @endcode
 *
 *  @note If the lengths of the iterators are unequal iteration will stop when
 *  the shortest iterator runs out of elements.
 *
 *  @note At the moment the values returned in the tuples are copies and not the
 *        actual ones in the zipped containers.  Everything is by reference
 *        until apply_functor_to_tuple is called to modify the buffer of the
 *        iterator.
 * @tparam ContainerTypes The types of the containers that will be zipped
 *                       together.
 */
template<typename... ContainerTypes>
class ZipImpl {
    private:
    /// Forward declaration of the actual iterator type
    class ZipIterator;

    /// Functor for calling the size member of each container.
    struct SizeFunctor {
        template<typename T>
        auto operator()(std::size_t curr_min, T&& container) const {
            return std::min(curr_min, container.size());
        }
    };

    /// The type of a tuple holding iterators to the containers
    using iterator_tuple =
    std::tuple<detail_::IteratorType<ContainerTypes>...>;

    public:
    /// The type of an element in this container.
    using value_type = std::tuple<std::remove_cv_t<
      typename GetValueType<detail_::IteratorType<ContainerTypes>>::type>...>;
    /// The type of a reference to an element of this container
    using reference = value_type&;
    /// The type of a read-only element of this container
    using const_reference = const value_type&;
    /// The type of an iterator to this container (can't actually modify)
    using iterator = ZipIterator;
    /// The type of a read-only iterator to this container
    using const_iterator = ZipIterator;
    /// The type of the difference between two pointers
    using difference_type = long int;
    /// The type of an index or offset into this container
    using size_type = std::size_t;

    /** @brief Constructs a container filled with no tuples taken from no
     *         containers.
     *
     *  @throw None. No throw guarantee.
     */
    ZipImpl() noexcept = default;

    ///Same as variadic template version, except for one container
    template<typename InputType, typename X = typename std::enable_if<
                                   !IsZipImpl<InputType>::value>::type>
    ZipImpl(InputType&& container) :
      size_(container.size()),
      begin_(!size_, std::forward<InputType>(container)),
      end_(true, std::forward<InputType>(container)) {}

    /** @brief Constructs a new ZipImpl by grabbing the begin and end iterators
     *         of a container
     *
     * @tparam InputTypes The types of the containers.
     * @param containers The actual container instances to zip.  If any of
     *        the containers are freed the resulting ZipImpl is invalidated.
     *
     * @throw ??? This constructor will call the size(), begin() and end()
     *            members of each container if any of those member functions
     *            throw, then this constructor will also throw.  Strong throw
     *            guarantee.
     */
    template<typename InputType1, typename InputType2, typename... InputTypes>
    ZipImpl(InputType1&& container1, InputType2&& container2,
            InputTypes&&... containers) :
      size_(
        reduce_tuple(
          std::tie(std::forward<InputType1>(container1),
                   std::forward<InputType2>(container2),
                   std::forward<InputTypes>(containers)...),
          SizeFunctor(),
          std::numeric_limits<size_type>::max())
      ),
      begin_(!size_,
             std::forward<InputType1>(container1),
             std::forward<InputType2>(container2),
             std::forward<InputTypes>(containers)...
      ),
      end_(true, std::forward<InputType1>(container1),
                 std::forward<InputType2>(container2),
                 std::forward<InputTypes>(containers)...
      )

    {}

    /** @brief Makes a copy of the current container.
     *
     *  It should be noted that this is a deep copy of the ZipImpl container,
     *  which means that all internal state will be deep copied.  The internal
     *  state will not be copied recursively though (which in turn means
     *  that the tuple copies will still be filled with shallow copies of the
     *  input containers).
     *
     *  @param rhs The container to copy.
     *  @throw None. No throw guarantee. Can't be noexcept b/c tuple copy
     *  constructor is not noexcept; however,
     *
     */
    ZipImpl(const ZipImpl& /*rhs*/) = default;

    /** @brief Takes ownership of another container.
     *
     *  @param rhs The container to take over.  After this call @p rhs is in a
     *  valid, but otherwise undefined state.
     *  @throw None. No throw guarantee.
     *
     */
    ZipImpl(ZipImpl&& /*rhs*/) noexcept = default;

    /** @brief Deletes the current instance.
     *
     *  This doesn't delete the containers that were zipped together.
     *
     *  @throw None. No throw guarantee.
     */
    ~ZipImpl() noexcept = default;

    /** @brief Assigns a deep copy of another ZipImpl to this one.
     *
     *  The same comments as the copy constructor apply here, namely that the
     *  result still contains shallow copies of the actual containers.
     *
     *  @param rhs The ZipImpl to copy the state of.
     *
     *  @throw None. No throw guarantee. Can't be marked as noexcept because
     *  tuple copy constructor is not noexcept
     */
    ZipImpl& operator=(const ZipImpl& /*rhs*/) = default;

    /** @brief Assigns another ZipImpl's state to this one.
     *
     *  @param rhs The ZipImpl to take the state of.  After this call @p rhs is
     * in a valid, but otherwise undefined state.
     *  @return The current instance containing @p rhs 's state.
     *  @throw None. No throw guarantee.
     */
    ZipImpl& operator=(ZipImpl&& /*rhs*/) noexcept = default;

    /** @brief Returns the number of tuples in this container.
     *
     *  Given a list of containers such that the @f$i@f$-th container contains
     *  @f$N_i@f$ elements.  This function returns the smallest @f$N_i@f$ as
     *  that is the maximum number of iterations possible.
     *
     *  @return The number of tuples in this container.
     *  @throw None. No throw guarantee.
     */
    size_type size() const noexcept { return size_; }

    /** @brief Returns the maximum number of elements this container can
     *  possibly hold ignoring hardware considerations.
     *
     *  At the moment this function simply returns the maximum of @p size_type.
     *
     *  @return The maximum number of elements this container can possibly hold
     *  ignoring hardware considerations.
     *
     *  @throw None. No throw guarantee.
     */
    constexpr static size_type max_size() noexcept {
        return std::numeric_limits<size_type>::max();
    }

    /**
     * @brief Returns true if the current instance contains no elements
     *
     * @return True if the current instance holds no elements and false if it
     *         holds even one element.
     * @throw None. No throw guarantee.
     */
    bool empty() const noexcept { return !size_; }

    /**
     * @brief Creates an iterator that points to the first tuple of elements in
     *        the zipped containers.
     *
     * @return An iterator pointing to first tuple of elements inside the
     * zipped container.
     */
    iterator begin() { return begin_; }

    ///@copydoc begin()
    const_iterator cbegin() const { return begin_; }

    /**
     * @brief Creates an iterator that points to just past the last tuple of
     *        elements in the zipped containers.
     *
     * @return An iterator pointing to just past the last tuple of elements
     * inside the zipped container.
     */
    iterator end() { return end_; }

    ///@copydoc end()
    const_iterator cend() const { return end_; }

    /**
     * @brief Determines if two ZipImpl instances are zipping the same
     * containers.
     *
     * Two ZipImpl instances are only equal if they are zipping the same
     * container instances, that is to say only the addresses of the wrapped
     * containers are checked for equality and not the contents.
     *
     * @tparam OtherContainers The types of the other containers.  Must satisfy
     *         the same concepts as ContainerTypes.
     * @param rhs The actual ZipImpl instance to compare to.
     * @return True if the the two instances are zipping the same container
     *         instances and false otherwise.
     * @throw None. No throw guarantee.
     */
    template<typename... OtherContainers>
    bool operator==(const ZipImpl<OtherContainers...>& rhs) const noexcept {
        return std::tie(begin_, end_, size_) ==
               std::tie(rhs.begin_, rhs.end_, rhs.size_);
    }

    /**
     * @brief Determines if two ZipImpl instances are zipping different
     * containers.
     *
     * Two ZipImpl instances are only equal if they are zipping the same
     * container instances, that is to say only the addresses of the wrapped
     * containers are checked for equality and not the contents.
     *
     * @tparam OtherContainers The types of the other containers.  Must satisfy
     *         the same concepts as ContainerTypes.
     * @param rhs The actual ZipImpl instance to compare to.
     * @return True if the the two instances are zipping different container
     *         instances and true if the container instances are the same.
     * @throw None. No throw guarantee.
     */
    template<typename... OtherContainers>
    bool operator!=(const ZipImpl<OtherContainers...>& rhs) const noexcept {
        return !((*this) == rhs);
    }

    private:
    /// The number of containers we are zipping together
    constexpr static std::size_t ncontainers_ = sizeof...(ContainerTypes);

    /// The number of iterations in this container
    std::size_t size_ = 0;

    /// Points to start of each container
    ZipIterator begin_;

    /// Points to end of each container
    ZipIterator end_;

    /** @brief The class responsible for iterating over the containers stored in
     *  a ZipImpl instance.
     *
     *  ZipIterator instances store iterators to the zipped containers and are
     *  only valid so long as said containers are in scope.  Since the actual
     *  iterators are stored within the ZipIterator instance, ZipIterators
     *  remain valid even if the creating ZipImpl goes out of scope.
     *
     */
    class ZipIterator
      : public detail_::InputIteratorBase<ZipIterator, value_type> {
        private:
        /// Allows ZipImpl to create an iterator
        friend class ZipImpl<ContainerTypes...>;
        /// Allows the base class to call the implementation functions
        friend class detail_::InputIteratorBase<ZipIterator, value_type>;

        /** @brief Creates a ZipIterator instance.
         *
         *  The ZipIterator keeps a copy of the iterators internally and thus is
         *  valid even if the ZipImpl goes out of scope.  If any of the
         *  containers being iterated over go out of scope the ZipIterator
         *  is invalidated.
         *
         * @param containers The tuple of containers to iterate over.
         * @param at_end If true the resulting iterator will represent an
         * iterator just past the last element, otherwise it's pointing to
         * the first element.
         */
        template<typename...InputTypes>
        ZipIterator(bool at_end, InputTypes&&...containers) :
          start_(
            apply_functor_to_tuple(
              std::tie(std::forward<InputTypes>(containers)...), CallBegin())
          ),
          end_(
            apply_functor_to_tuple(
              std::tie(std::forward<InputTypes>(containers)...), CallEnd())
          ),
          value_(!at_end ? start_ : end_),
          buffer_(!at_end ? apply_functor_to_tuple(value_, Derefer()) :
                            value_type()) {}

        /// Implements the means by which this class can be dereferenced
        reference dereference() { return buffer_; }

        /// Implements the mechanism for dereferencing a read-only iterator
        const_reference dereference() const { return buffer_; }

        /// Implements the mechansim for incrementing this iterator
        ZipIterator& increment() noexcept {
            return increment_imp(std::make_index_sequence<ncontainers_>());
        }

        /// Implements the mechanism for checking if this iterator equals
        /// another
        bool are_equal(const ZipIterator& rhs) const noexcept {
            return value_ == rhs.value_;
        }

        // Check if any iterator is at the end, if so set them all to end
        void check_itrs() {
            auto at_end = combine_tuples(value_, end_, Comparer());
            bool done   = reduce_tuple(at_end, AnyTrue(), false);
            if(done) value_ = end_;
        }

        template<std::size_t... I>
        ZipIterator& increment_imp(std::index_sequence<I...>) {
            value_ = std::make_tuple((++std::get<I>(value_))...);
            check_itrs();
            if(value_ != end_)
                buffer_ = apply_functor_to_tuple(value_, Derefer());
            return *this;
        }

        /// A tuple containing the starting iterators to the containers
        iterator_tuple start_;

        /// A tuple containing the ending iterators of the containers
        iterator_tuple end_;

        /// The tuple containing our current iterators
        iterator_tuple value_;

        /// The actual element
        value_type buffer_;

        /// Functor for calling begin() on all containers
        struct CallBegin {
            template<typename T>
            auto operator()(T&& container) {
                return container.begin();
            }
        };

        /// Functor for calling end() on all containers
        struct CallEnd {
            template<typename T>
            auto operator()(T&& container) const {
                return container.end();
            }
        };

        /// Functor for derefrencing an argument
        struct Derefer {
            template<typename T>
            auto& operator()(T&& itr) const {
                return *itr;
            }
        };

        /// Functor for comparing two arguments
        struct Comparer {
            template<typename lhs_type, typename rhs_type>
            auto operator()(lhs_type&& lhs, rhs_type&& rhs) const {
                return lhs == rhs;
            }
        };

        /// Functor for finding if any element is true via reduction
        struct AnyTrue {
            template<typename T>
            bool operator()(bool val, T&& element) const {
                return val || element;
            }
        };

        public:
        ZipIterator() noexcept = default;
    };
};
} // End namespace detail_

/** @brief Wrapper function that imparts the desired API to ZipImpl.
 *
 * See the documentation for ZipImpl for full details.
 *
 * @tparam ContainerTypes The types of the containers to zip together.  Must
 *         minimally meet the concept of container.
 * @param containers The actual container instances to zip together.  Containers
 *        will be captured by reference or constant reference as appropriate.
 * @return A ZipImpl instance that is usable in a foreach loop as
 * @throws ??? Throws if any of the size() members throw.  Strong throw
 *             guarantee.
 */
template<typename... ContainerTypes>
auto Zip(ContainerTypes&&... containers) {
    return detail_::ZipImpl<std::remove_reference_t<ContainerTypes>...>(
      std::forward<ContainerTypes>(containers)...);
}

} // namespace UtilitiesEx
