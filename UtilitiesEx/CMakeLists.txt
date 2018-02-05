cmake_minimum_required(VERSION 3.1)
project(UtilitiesEx-SRC VERSION 0.0.0 LANGUAGES CXX)
include(TargetMacros)
set(UtilitiesEx_SRCS Mathematician/Combinatorics.cpp)
set(UtilitiesEx_INCLUDES Any.hpp
                         Containers/CaseInsensitiveMap.hpp
                         Mathematician/Combinatorics.hpp
                         IterTools/Combinations.hpp
                         TypeTraits/IteratorTypes.hpp
                         IterTools/Permutations.hpp
                         TypeTraits/type_traitsExtensions.hpp
)
set(UtilitiesEx_DEFINITIONS )
set(UtilitiesEx_LINK_FLAGS )
nwchemex_add_library(UtilitiesEx UtilitiesEx_SRCS
                                 UtilitiesEx_INCLUDES
                                 UtilitiesEx_DEFINITIONS
                                 UtilitiesEx_LINK_FLAGS)
