#pragma once
#include <vector>

namespace UtilitiesEx {
namespace detail_ {

///Struct that actually implements the binomial coefficient
template<typename T>
struct BCImpl{
    ///Returns @p n choose @p k
    T eval(std::size_t n, std::size_t k);
};

///Struct that actually implments the multinomial coefficient
template<typename T>
struct MCImpl{
    ///Returns sum(ks) chooses k0, k1, k2,...
    T eval(const std::vector<std::size_t>& ks);
};

}//End namespace detail

/** @brief Computes the binomial coefficient @f${n\choose k}@f$
 *
 *  Depending on the requested return type a different algorithm is used.  If
 *  the user requests an integral return type an algorithm based on Pascal's
 *  triangle will be used.  If the user requests a floating point return type it
 *  will then be assumed that the user is fine with loosing precision *i.e.* the
 *  value will be returned in scientific notation with the significand
 *  truncated to the precision of the floating point value.
 *
 * @tparam T The desired return type.
 * @param n top number of the bc *i.e.* number of objects to pick from
 * @param k bottom number of the bc *i.e.* number of objects to pick at a time
 * @return The requested value of the binomial coefficient
 * @throws std::overflow_error if the requested binomial coefficient can not be
 *         held in the requested return type.
 */
template<typename T>
T binomial_coefficient(std::size_t n, std::size_t k)
{
    return detail_::BCImpl<T>().eval(n,k);
}

template<typename T>
T multinomial_coefficient(const std::vector<std::size_t>& ks)
{
    return detail_::MCImpl<T>().eval(ks);
}

}//End namespace
