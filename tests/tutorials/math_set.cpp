#include <utilities/containers/math_set.hpp>

TEST_CASE("MathSet usage") {
    MathSet<int> s1{1, 2, 3, 4};
    MathSet<int> s2{2, 4, 6, 8};

    MathSet<int> s3 = s1 + s2; // s3 has copies of unique elements in s1 and s2
    MathSet<int> s4 = s1 ^ s2; // s4 has copies of elements common to s1 and s2
    MathSet<int> s5 = s1 - s2; // s5 has copies of elements in s1, but not s2

    auto s6 = s1 + s2; // Read-only view of unique elements in s1 and s2
    auto s7 = s1 ^ s2; // Read-only view of elements common to s1 and s2
    auto s8 = s1 - s2; // Writable view of elements in s1, but not s2

    // s6 and s7 are MathSetView<const int>
    // s8 is MathSetView<int>
    // MathSetView does not support insertion
    // MathSetView<T> can be implicitly cast to const MathSet<T>&
}
