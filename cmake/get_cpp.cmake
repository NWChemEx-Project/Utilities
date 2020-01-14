include_guard()

#[[
# This function encapsulates the process of getting CMakePP using CMake's
# FetchContent module. We have encapsulated it in a function so we can set the
# options for it's configure step without affecting the options for the parent
# project's configure step.
#]]
function(get_cpp)
    set(BUILD_TESTS OFF)
    include(FetchContent)
    FetchContent_Declare(
        cpp
        GIT_REPOSITORY https://github.com/CMakePP/CMakePackagingProject
        GIT_TAG rewrite
    )
    FetchContent_MakeAvailable(cpp)
    list(APPEND CMAKE_MODULE_PATH "${cpp_SOURCE_DIR}/cmake")
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endfunction()

# Call the function we just wrote to get CMakePP
get_cpp()

# Include CMakePP
include(cpp/cpp)
