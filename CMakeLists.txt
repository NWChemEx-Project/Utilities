cmake_minimum_required(VERSION 3.14)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" VERSION)
project(utilities VERSION "${VERSION}")

option(BUILD_TESTING "Should we build the tests?" OFF)
option(BUILD_DOCS "Should we build the documentation?" ON)

set(
    CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake"
    CACHE STRING "" FORCE
)
include(get_cpp)

cpp_add_library(
    utilities
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/utilities"
    INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/utilities"
)

if("${BUILD_DOCS}")
    find_package(Doxygen REQUIRED)
    set(DOXYGEN_EXTRA_PACKAGES amsmath) # Extra LaTeX packages for documentation
    doxygen_add_docs(cxx_api "${CMAKE_CURRENT_LIST_DIR}/utilities")
endif()

if("${BUILD_TESTING}")
    cpp_find_or_build_dependency(
        Catch2
        URL github.com/catchorg/Catch2
        BUILD_TARGET Catch2
        FIND_TARGET Catch2::Catch2
    )
    cpp_add_tests(
        test_utilities
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/tests"
        INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/utilities"
        DEPENDS Catch2::Catch2 utilities
    )
endif()

