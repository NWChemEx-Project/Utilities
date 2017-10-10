cmake_minimum_required(VERSION 3.1)
project(UtilitiesEx-Test VERSION 0.0.0 LANGUAGES CXX)
find_package(UtilitiesEx REQUIRED)
include(TargetMacros)

add_cxx_unit_test(Testtype_traitsExtensions UtilitiesEx)
add_cxx_unit_test(TestCombinations UtilitiesEx)
add_cxx_unit_test(TestCombinatorics UtilitiesEx)
add_cxx_unit_test(TestPermutations UtilitiesEx)
