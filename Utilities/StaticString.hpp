#pragma once
#include <cstddef>
#include <iostream>

namespace Utilities {

/**
 *  @brief A class capable of holding a compile-time manipulatable string.
 *
 *  Particularly when working with enums we often want string semantics.  This
 *  class allows us to define an instance like:
 *
 *  ```.cpp
 *  constexpr StaticString value1{"value 1"};
 *  ```
 *  that is usable like an enum with only compile-time overhead.
 *
 *  As a general note this class is really only intended to be used for
 *  compile-time strings (in fact I don't see how it could be used with
 *  dynamically allocated strings).  For this reason all function documentation
 *  is written assuming it will be called at compile-time.  In particular this
 *  means that all functions are thread-safe and noexcept.
 */
class StaticString {
public:
    /**
     * @brief Wraps a string literal (a compile-time string) in a more C++-like
     *        API.
     *
     *
     * @tparam Length the compile-time length of the string to wrap (will
     *         include the `'\n'` character).
     * @param str the string literal to wrap.
     *
     * @par Complexity:
     * Constant.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     *
     */
    template<std::size_t Length>
    constexpr StaticString(const char (&str)[Length]) noexcept :
      begin_(str),
      size_(Length - 1) {}

    /**
     * @brief Returns the i-th character of the
     *
     * This function assumes that @p i is in bounds.  If @p i is not in bounds
     * you're likely to get a compiler error; however, this is not guaranteed.
     *
     * @param i the offset of the requested character.  @p i should be in the
     *        range [0, N) where @p N is the length of the string as returned
     *        by `size()`.
     * @return The requested character.
     * @par Complexity:
     * Constant.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr char operator[](unsigned i) const noexcept { return begin_[i]; }

    /**
     * @brief An accessor wrapping the length of the string.
     *
     * @return The length of the string not counting the `'\n'` character.
     * @throw None. No throw guarantee.
     * @par Complexity:
     * Constant.
     * @par Data Races:
     * None.
     */
    constexpr std::size_t size() const noexcept { return size_; }

    /**
     * @brief Determines if two strings are equal.
     *
     * Two strings are equal if they are the same length and the i-th character
     * is the same for both strings, for all i.
     *
     * @param rhs The string to compare to.
     * @return True if the strings are the same and false otherwise.
     * @par Complexity:
     * Worst case linear in the length of the string.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr bool operator==(const StaticString& rhs) const noexcept {
        if(size_ != rhs.size()) return false;
        for(std::size_t i = 0; i < size_; ++i) // know they are same size
            if((*this)[i] != rhs[i]) return false;
        return true;
    }

    /**
     * @brief Determines if two strings are not equal.
     *
     * Two strings are equal if they are the same length and the i-th character
     * is the same for both strings, for all i.
     *
     * @param rhs The string to compare to.
     * @return True if the strings are not the same and false otherwise.
     * @par Complexity:
     * Worst case linear in the length of the string.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr bool operator!=(const StaticString& rhs) const noexcept {
        return !((*this) == rhs);
    }

    /**
     * @brief Determines if the current string is less than another string
     *
     * A string is less than another string if it compares lexicographically
     * lower than the other string.  Note this comparison is done character by
     * character and will only be alphabetic if all characters have the same
     * case.  In ASCII upper case numbers are first and thus will come first
     * "alphabetically".
     *
     * @param rhs The string to compare to.
     * @return True if the current string is less than @p rhs.
     * @par Complexity:
     * Worst case linear in the length of the string.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr bool operator<(const StaticString& rhs) const noexcept {
        const bool am_smaller      = (size() < rhs.size());
        const std::size_t min_size = (am_smaller ? size_ : rhs.size());
        for(std::size_t i = 0; i < min_size; ++i) {
            const char li = (*this)[i];
            const char ri = rhs[i];
            if(li < ri)
                return true;
            else if(li > ri)
                return false;
            // must be that li == ri
        }
        // At this point either they are the same up the length of the shortest
        // string.  Thus we simply need to know which is shorter.
        return am_smaller;
    }

    /**
     * @brief Determines if the current string is greater than another string
     *
     * A string is greater than another string if it compares lexicographically
     * greater than the other string.  Note this comparison is done character by
     * character and will only be alphabetic if all characters have the same
     * case.  In ASCII upper case numbers are first and thus will come first
     * "alphabetically".
     *
     * @param rhs The string to compare to.
     * @return True if the current string is greater than @p rhs.
     * @par Complexity:
     * Worst case linear in the length of the string.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr bool operator>(const StaticString& rhs) const {
        return rhs < *this;
    }

    /**
     * @brief Determines if the current string is less than or equal to another
     * string
     *
     * A string is less than another string if it compares lexicographically
     * lower than the other string.  Note this comparison is done character by
     * character and will only be alphabetic if all characters have the same
     * case.  In ASCII upper case numbers are first and thus will come first
     * "alphabetically".
     *
     * Two strings are equal if they are the same length and the i-th character
     * is the same for both strings, for all i.
     *
     * @param rhs The string to compare to.
     * @return True if the current string is less than or equal to @p rhs.
     * @par Complexity:
     * Worst case linear in the length of the string.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr bool operator<=(const StaticString& rhs) const {
        return (*this) < rhs || (*this) == rhs;
    }

    /**
     * @brief Determines if the current string is greater than or equal to
     *        another string
     *
     * A string is greater than another string if it compares lexicographically
     * greater than the other string.  Note this comparison is done character by
     * character and will only be alphabetic if all characters have the same
     * case.  In ASCII upper case numbers are first and thus will come first
     * "alphabetically".
     *
     * Two strings are equal if they are the same length and the i-th character
     * is the same for both strings, for all i.
     *
     * @param rhs The string to compare to.
     * @return True if the current string is greater than or equal to @p rhs.
     * @par Complexity:
     * Worst case linear in the length of the string.
     * @par Data Races:
     * None.
     * @throws None. No throw guarantee.
     */
    constexpr bool operator>=(const StaticString& rhs) const noexcept {
        return rhs <= *this;
    }

private:
    /// A pointer to the start of the string
    const char* begin_;

    /// The length of the string (not including the `'\n'` character.
    std::size_t size_;
}; // StaticString

} // namespace Utilities

/**
 * @brief Overloads the std::ostream so that it can print the string
 *
 * It should be noted that this function is **NOT** constexpr.  That is to say
 * the string will not be printed at compile-time, but at run-time.
 *
 * @param[in,out] os The ostream to add @p str to.
 * @param[in] str The StaticString instance to print out.
 * @return @p os in a manner ammenable for operator chaining.
 * @throw std::failure if writing to the stream fails for any reason.  Weak
 * throw guarantee regarding the state of @p os.
 * @par Complexity:
 * Presumably linear in the length of the string, but not certain.
 * @par Data Races:
 * Modifies @p os and data races may ensure if @p os is concurrently modified or
 * accessed.  @p str can is a compile-time constant and thus is immune to data
 * races.
 */
std::ostream& operator<<(std::ostream& os, const Utilities::StaticString& str) {
    for(std::size_t i = 0; i < str.size(); ++i) os << str[i];
    return os;
}
