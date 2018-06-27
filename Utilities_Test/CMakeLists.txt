set(
    UTILITIES_TESTS test_main.cpp
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

add_catch_cxx_tests(test_utilities "${UTILITIES_TESTS}" utilities)
