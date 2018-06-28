cmake_minimum_required(VERSION 3.6)
include(cmake/Macros.cmake)
start_hunter() #Must be before project command

project(Utilities VERSION 0.0.0 LANGUAGES CXX)

### Options ###
option(BUILD_TESTS "Should we build the tests?" ON)

### Dependencies ###


### Target ###
add_subdirectory(Utilities)

### Testing ###
if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(Utilities_Test)
endif()
