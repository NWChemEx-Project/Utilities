set(
    sources test_main.cpp
            Testtype_traitsExtensions.cpp
            TestCartesianProduct.cpp
            TestCaseInsensitiveMap.cpp
            TestCombinations.cpp
            TestCombinatorics.cpp
            TestEnumerate.cpp
            TestIntegerUtils.cpp
            TestIteratorTypes.cpp
            TestPermutations.cpp
            TestRange.cpp
            TestRangeContainer.cpp
            TestMathSet.cpp
            TestSmartEnum.cpp
            TestStaticString.cpp
            TestTupleUtilities.cpp
            TestZip.cpp
)

cpp_add_executable(
        test_utilities
        SOURCES ${sources}
        DEPENDS utilities Catch2::Catch2
)
add_test(NAME test_utilities COMMAND test_utilities)
