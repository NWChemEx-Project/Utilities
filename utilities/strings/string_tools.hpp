#pragma once
#include <string>
#include <vector>

namespace utilities::strings {

/** @brief Splits a string on a delimiter.
 *
 *  This was shamelessly stolen from https://stackoverflow.com/a/13172514.
 *
 */
inline std::vector<std::string> split_string(const std::string& str,
                                             const std::string& delimiter) {
    std::vector<std::string> strings;
    std::string::size_type pos  = 0;
    std::string::size_type prev = 0;
    while((pos = str.find(delimiter, prev)) != std::string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

} // namespace utilities::strings
