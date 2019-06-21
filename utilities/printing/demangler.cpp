#include "utilities/printing/demangler.hpp"
#include <memory>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace utilities::printing {

std::string Demangler::demangle(const char* t) {
#ifdef __GNUG__
    int status = 0;

    std::unique_ptr<char, void (*)(void*)> res{
      abi::__cxa_demangle(t, nullptr, nullptr, &status), std::free};

    return status == 0 ? res.get() : t;
#else // Not a known compiler

    return t;
#endif
}

} // namespace utilities::printing
