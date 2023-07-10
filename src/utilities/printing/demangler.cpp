/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "utilities/printing/demangler.hpp"
#include <memory>
#include <regex>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif

namespace utilities::printing {

// Removes all whitespace characters that are immediately
// before a closing angle bracket in a string.
std::string remove_spaces(const std::string& str) {
    static const std::regex r("\\s+(?=\\>)");
    return std::regex_replace(str, r, "");
}

std::string Demangler::demangle(const char* t) {
#if __has_include(<cxxabi.h>)
    int status = 0;

    std::unique_ptr<char, void (*)(void*)> res{
      abi::__cxa_demangle(t, nullptr, nullptr, &status), std::free};

    return status == 0 ? remove_spaces(res.get()) : remove_spaces(t);
#else // Not a known compiler

    return remove_spaces(t);
#endif
}

} // namespace utilities::printing
