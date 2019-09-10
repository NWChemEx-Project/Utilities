add_subdirectory(containers)
add_subdirectory(iter_tools)
add_subdirectory(iterators)
add_subdirectory(mathematician)
add_subdirectory(printing)
add_subdirectory(strings)
add_subdirectory(type_traits)
list(APPEND sources test_main.cpp
                    smart_enum.cpp
                    timer.cpp
                    tutorials/math_set.cpp
)

cpp_add_executable(
        test_utilities
        SOURCES ${sources}
        DEPENDS utilities Catch2::Catch2
)
add_test(NAME test_utilities COMMAND test_utilities)
