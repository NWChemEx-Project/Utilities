#include <catch2/catch.hpp>
#include <utilities/type_traits/apply_type_to_tuple.hpp>

using namespace utilities;

template<typename T>
using vector = std::vector<T>;

TEST_CASE("ApplyTypeToTuple") {
    using type1 = apply_type_to_tuple_t<vector, std::tuple<int, double>>;
    using corr1 = std::tuple<vector<int>, vector<double>>;
    STATIC_REQUIRE(std::is_same_v<type1, corr1>);
}
