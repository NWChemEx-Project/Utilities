#pragma once
#include "Utilities/IterTools/Range.hpp"
#include "Utilities/IterTools/Zip.hpp"

namespace Utilities {

template<typename container_type>
auto Enumerate(container_type&& c) {
    auto r = Range(c.size());
    return Zip(r, std::forward<container_type>(c));
}

} // namespace Utilities
