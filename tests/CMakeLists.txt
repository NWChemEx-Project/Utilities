set(sources test_main.cpp
            cartesian_product.cpp
            case_insensitive_map.cpp
            combinations.cpp
            combinatorics.cpp
            enumerate.cpp
            integer_utils.cpp
            iterator_types.cpp
            permutations.cpp
            printing/demangler.cpp
            printing/print_stl.cpp
            printing/detail_/table_pimpl.cpp
            printing/word_wrap_stream.cpp
            range.cpp
            range_container.cpp
            math_set.cpp
            smart_enum.cpp
            strings/static_string.cpp
            timer.cpp
            tuple_utilities.cpp
            type_traits_extensions.cpp
            zip.cpp
)

cpp_add_executable(
        test_utilities
        SOURCES ${sources}
        DEPENDS utilities Catch2::Catch2
)
add_test(NAME test_utilities COMMAND test_utilities)
