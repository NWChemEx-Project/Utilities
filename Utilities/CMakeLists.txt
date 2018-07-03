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
add_nwx_library(
        NAME utilities
        SOURCES ${UTILITIES_SRCS}
)
install_targets(
        TARGETS utilities
        INCLUDES ${UTILITIES_HEADERS}
)

