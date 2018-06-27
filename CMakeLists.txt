cmake_minimum_required(VERSION 3.6)
include(cmake/Macros.cmake)
start_hunter() #Must be before project command

project(utilities VERSION 0.0.0 LANGUAGES CXX)

### Dependencies ####
hunter_add_package(Catch)
find_package(Catch2 CONFIG REQUIRED)

### Target ###
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Utilities)

### Testing ###
enable_testing()
add_subdirectory(Utilities_Test)



