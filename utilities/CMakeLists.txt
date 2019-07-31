add_subdirectory(containers)
add_subdirectory(type_traits)
list(APPEND headers   iter_tools/cartesian_product.hpp
                      iter_tools/combinations.hpp
                      iter_tools/enumerate.hpp
                      iter_tools/permutations.hpp
                      iter_tools/range.hpp
                      iter_tools/range_container.hpp
                      iter_tools/tuple_container.hpp
                      iter_tools/zip.hpp
                      iterators/bidirectional_iterator_base.hpp
                      iterators/input_iterator_base.hpp
                      iterators/offset_iterator.hpp
                      iterators/random_access_iterator_base.hpp
                      macros/for_each.hpp
                      mathematician/combinatorics.hpp
                      mathematician/integer_utils.hpp
                      printing/demangler.hpp
                      printing/print_stl.hpp
                      printing/table.hpp
                      printing/word_wrap_stream.hpp
                      strings/string_tools.hpp
                      strings/static_string.hpp

                      forward_stl.hpp
                      iter_tools.hpp
                      smart_enum.hpp
                      timer.hpp
)

set(
    UTILITIES_SRCS mathematician/combinatorics.cpp
                   printing/demangler.cpp
                   printing/table.cpp
                   printing/word_wrap_stream.cpp

)

cpp_add_library(
    utilities
    SOURCES ${UTILITIES_SRCS}
    INCLUDES ${headers}
)
cpp_install(TARGETS utilities)

