cmake_minimum_required(VERSION 3.6)
include(cmake/Macros.cmake)
start_hunter() #Must be before project command

project(utilities VERSION 0.0.0 LANGUAGES CXX)

### Options ###
option(BUILD_TESTS "Should we build the tests?" ON)

### Dependencies ###
hunter_add_package(Catch)
find_package(Catch2 CONFIG REQUIRED)

### Target ###
add_subdirectory(Utilities)

### Testing ###
if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(Utilities_Test)
endif()


