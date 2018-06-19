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
make_full_paths(UTILITIES_HEADERS)
set(UTILITIES_HEADERS ${UTILITIES_HEADERS} PARENT_SCOPE)


set(UTILITIES_SRCS Mathematician/Combinatorics.cpp)

add_library(utilities ${UTILITIES_SRCS})
target_compile_features(utilities PUBLIC cxx_std_14)
install_targets("utilities" "${UTILITIES_HEADERS}")
