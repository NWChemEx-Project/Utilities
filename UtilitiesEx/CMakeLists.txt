cmake_minimum_required(VERSION 3.1)
project(UtilitiesEx-SRC VERSION 0.0.0 LANGUAGES CXX)
include(TargetMacros)
set(UtilitiesEx_SRCS Mathematician/Combinatorics.cpp)
set(UtilitiesEx_INCLUDES Mathematician/Combinatorics.hpp
                         IterTools/Combinations.hpp
                         IterTools/IteratorTypes.hpp
                         IterTools/Permutations.hpp
        TypeTraits/type_traitsExtensions.hpp
)

nwchemex_add_library(UtilitiesEx UtilitiesEx_SRCS
                                 UtilitiesEx_INCLUDES
                                 ""
)
