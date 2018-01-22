cmake_minimum_required(VERSION 3.1)
project(UtilitiesEx-SRC VERSION 0.0.0 LANGUAGES CXX)

include(TargetMacros)
set(UtilitiesEx_SRCS Mathematician/Combinatorics.cpp)
set(UtilitiesEx_INCLUDES Any.hpp
                         Containers/MathSet.hpp
                         Containers/RangeContainer.hpp
                         Mathematician/Combinatorics.hpp
                         IterTools/Combinations.hpp
                         IterTools/Permutations.hpp
                         IterTools/Range.hpp
                         IterTools/Zip.hpp
                         TypeTraits/IteratorTypes.hpp
                         TypeTraits/TupleUtilities.hpp
                         TypeTraits/type_traitsExtensions.hpp
)
set(UtilitiesEx_DEFINITIONS )
set(UtilitiesEx_LINK_FLAGS )
nwchemex_add_library(UtilitiesEx UtilitiesEx_SRCS
                                 UtilitiesEx_INCLUDES
                                 UtilitiesEx_DEFINITIONS
                                 UtilitiesEx_LINK_FLAGS)
