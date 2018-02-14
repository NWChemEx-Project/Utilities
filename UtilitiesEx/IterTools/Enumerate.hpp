#pragma once
#include "UtilitiesEx/IterTools/Range.hpp"
#include "UtilitiesEx/IterTools/Zip.hpp"

namespace UtilitiesEx {

template<typename container_type>
auto Enumerate(container_type&& c) {
    return Zip(Range(c.size()), std::forward<container_type>(c));
}

} // namespace UtilitiesEx
