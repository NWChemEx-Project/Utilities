set(
    UTILITIES_HEADERS Containers/CaseInsensitiveMap.hpp
                      Containers/MathSet.hpp
                      IterTools/CartesianProduct.hpp
                      IterTools/Combinations.hpp
                      IterTools/Enumerate.hpp
                      IterTools/Permutations.hpp
                      IterTools/Range.hpp
                      IterTools/RangeContainer.hpp
                      IterTools/TupleContainer.hpp
                      IterTools/Zip.hpp
                      Macros/ForEach.hpp
                      Mathematician/Combinatorics.hpp
                      Mathematician/IntegerUtils.hpp
                      TypeTraits/IteratorTypes.hpp
                      TypeTraits/TupleUtilities.hpp
                      TypeTraits/type_traitsExtensions.hpp
                      IterTools.hpp
                      SmartEnum.hpp
                      StaticString.hpp
)

set(UTILITIES_SRCS Mathematician/Combinatorics.cpp)

cpp_add_library(
    utilities
    SOURCES ${UTILITIES_SRCS}
    INCLUDES ${UTILITIES_HEADERS}
)
cpp_install(TARGETS utilities)

