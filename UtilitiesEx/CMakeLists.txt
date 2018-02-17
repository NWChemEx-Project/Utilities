cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(UtilitiesEx-SRC VERSION ${PROJECT_VERSION} LANGUAGES CXX)
include(TargetMacros)
set(UtilitiesEx_SRCS Mathematician/Combinatorics.cpp)
set(UtilitiesEx_INCLUDES Any.hpp
                         Containers/CaseInsensitiveMap.hpp
                         Containers/MathSet.hpp
                         Mathematician/Combinatorics.hpp
                         IterTools/CartesianProduct.hpp
                         IterTools/Combinations.hpp
                         IterTools/Enumerate.hpp
                         IterTools/Permutations.hpp
                         IterTools/Range.hpp
                         IterTools/RangeContainer.hpp
                         IterTools/TupleContainer.hpp
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
