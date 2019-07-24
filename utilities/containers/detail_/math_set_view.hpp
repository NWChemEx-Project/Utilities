#pragma once
#include <memory>

namespace utilities::detail_ {

/** @brief Provides an alias of a subset of a MathSet
 *
 *  When manipulating sets we often need to grab slices of them. These slices
 *  are just subsets of the original set. This class wraps a pointer to the
 *  original class and the starting and ending indices
 *
 *
 *  @tparam WrappedType The type of the container we are aliasing. In the
 *                      context of the MathSet class this will be something like
 *                      MathSet<T>. However, the MathSetView class itself will
 *                      work with any container.
 */
template<typename WrappedType>
class MathSetView {
public:
    using value_type      = typename WrappedType::value_type;
    using size_type       = typename WrappedType::size_type;
    using reference       = typename WrappedType::reference;
    using const_reference = typename WrappedType::const_reference;
    using iterator        = typename WrappedType::iterator;
    using const_iterator  = typename WrappedType::const_iterator;

    MathSetView(size_type start, size_type end,
                std::shared_ptr<WrappedType> data);

    iterator begin() { return set_itr_to_(m_data_->begin(), m_start_); }
    const_iterator begin() const {
        return set_itr_to_(m_data_->begin(), m_start_);
    }
    const_iterator cbegin() const { return begin(); }
    iterator end() { return set_itr_to_(m_data_->begin(), m_end_); }
    const_iterator end() const { return set_itr_to_(m_data_->begin(), m_end_); }
    const_iterator cend() const { return end(); }

    size_type size() const noexcept { return m_end_ - m_start_; }

private:
    /** @brief Code factorization for getting a begin/end iterator
     *
     *  `begin()` and `end()` for this class work by advancing the underlying
     *  container's iterators to the first/last element corresponding to the
     *  container this view is modeling.
     *
     *  @note This function takes an iterator as input, instead of calling
     *        `m_data_->begin()` itself, in order to get the const-ness of the
     *        iterator from the calling function.
     *
     *  @tparam Itr The type of the iterator to advance.
     *
     *  @param[in] itr The iterator to advance by @p off increments.
     *  @param[in] off How many increments should we advance @p itr by?
     *
     *  @return The iterator advanced by the specified number of increments.
     */
    template<typename Itr>
    auto set_itr_to_(Itr itr, size_type off) const;

    size_type m_start_ = 0;
    size_type m_end_   = 0;
    std::shared_ptr<WrappedType> m_data_;
};

//---------------------------------Implementations------------------------------

template<typename WrappedType>
MathSetView(typename MathSetView<WrappedType>::size_type,
            typename MathSetView<WrappedType>::size_type,
            std::shared_ptr<WrappedType>)
  ->MathSetView<WrappedType>;

template<typename WrappedType>
MathSetView<WrappedType>::MathSetView(size_type start, size_type end,
                                      std::shared_ptr<WrappedType> data) :
  m_start_(start),
  m_end_(end),
  m_data_(data) {}

template<typename WrappedType>
template<typename Itr>
auto MathSetView<WrappedType>::set_itr_to_(Itr itr, size_type off) const {
    std::advance(itr, off);
    return itr;
}

} // namespace utilities::detail_
