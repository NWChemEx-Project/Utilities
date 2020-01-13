include_guard()

# Throw this in a function so options we set only affect it
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
get_cpp()
include(cpp/cpp)
