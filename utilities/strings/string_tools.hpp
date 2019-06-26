#pragma once
#include <string>
#include <vector>

namespace utilities::strings {

/** @brief Splits a string on a delimiter.
 *
 *  For whatever reason, C++ makes it a pain to split a string on a substring.
 *  Particularly for text parsing applications splitting a string is a common
 *  occurrence so we've factored out the code to do it into this function.
 *
 *  This was shamelessly stolen from https://stackoverflow.com/a/13172514 and
 *  then slightly tweaked.
 *
 *  @param[in] str The string to split.
 *  @param[in] delimiter The string to split on.
 *
 *  @return An std::vector of the substrings.
 *
 *  @throw std::bad_alloc if there is insufficient memory to copy the substrings
 *                        to the vector. Strong throw guarantee.
 */
inline std::vector<std::string> split_string(const std::string& str,
                                             const std::string& delimiter) {
    std::vector<std::string> strings;

    // Handle case that string is empty
    if(str.empty()) return strings;

    std::string::size_type pos  = 0;
    std::string::size_type prev = 0;
    while((pos = str.find(delimiter, prev)) != std::string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }

    // We're out of delimiters, but haven't printed from the last delimiter to
    // the end, so one more push_back
    strings.push_back(str.substr(prev));

    return strings;
}

/** @brief Overloads split_string for case when delimiter is a single character.
 *
 *  It is pretty common to want to split on a single character. If you specify
 *  the character using single quotes it won't auto convert to an std::string
 *  like double quotes will. This overload takes care of converting it for you.
 *
 * @param[in] str The string to split.
 * @param[in] c The character to split on.
 *
 * @return A vector of the substrings.
 *
 * @throw std::bad_alloc if there is insufficient memory to copy the substrings
 *                       to the vector. Strong throw guarantee.
 */
inline auto split_string(const std::string& str, char c) {
    return split_string(str, std::string(1, c));
}

} // namespace utilities::strings
