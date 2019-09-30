add_subdirectory(containers)
add_subdirectory(iterators)
add_subdirectory(iter_tools)
list(APPEND UTILITIES_HEADERS
                      macros/for_each.hpp
                      mathematician/combinatorics.hpp
                      mathematician/integer_utils.hpp
                      printing/demangler.hpp
                      printing/print_stl.hpp
                      printing/table.hpp
                      printing/word_wrap_stream.hpp
                      strings/string_tools.hpp
                      strings/static_string.hpp
                      type_traits/is_printable.hpp
                      type_traits/tuple_utilities.hpp
                      type_traits/type_traits_extensions.hpp
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
    INCLUDES ${UTILITIES_HEADERS}
)
cpp_install(TARGETS utilities)

