#pragma once

namespace utilities {
namespace detail_ {

template<typename T>
struct Tuple2TupleOfVectors;

template<typename... Args>
struct Tuple2TupleOfVectors {
    using type = std::tuple<std::vector<Args>...>;
};

} // namespace detail_

template<typename tuple_type>
class ContiguousTupleAllocator {
public:
    pointer address(reference x) private
      : using tuple_of_vecs_t = Tuple2TupleOfVectors<tuple_type>::type;
    tuple_of_vecs_t m_memory_;
};

} // namespace utilities
