cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(Utilities-SRC VERSION ${PROJECT_VERSION} LANGUAGES CXX)
include(TargetMacros)
set(Utilities_SRCS Mathematician/Combinatorics.cpp)
set(Utilities_INCLUDES Any.hpp
                         Containers/CaseInsensitiveMap.hpp
                         Containers/MathSet.hpp
                         Mathematician/Combinatorics.hpp
                         Mathematician/IntegerUtils.hpp
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
set(Utilities_DEFINITIONS )
set(Utilities_LINK_FLAGS )
nwchemex_add_library(Utilities Utilities_SRCS
                                 Utilities_INCLUDES
                                 Utilities_DEFINITIONS
                                 Utilities_LINK_FLAGS)
