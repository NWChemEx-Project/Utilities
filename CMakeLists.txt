# Copyright 2022 NWChemEx-Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.14)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" VERSION)
project(utilities VERSION "${VERSION}")

include(FetchContent)
FetchContent_Declare(
    nwx_cmake
    GIT_REPOSITORY https://github.com/NWChemEx-Project/NWXCMake
)
FetchContent_MakeAvailable(nwx_cmake)
list(APPEND CMAKE_MODULE_PATH "${nwx_cmake_SOURCE_DIR}/cmake")

set(
    CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake"
    CACHE STRING "" FORCE
)
include(get_cmaize)
include(nwx_versions)

option(BUILD_TESTING "Should we build the tests?" OFF)
option(BUILD_DOCS "Should we build the documentation?" OFF)

set(utilities_inc_dir "${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}")
set(utilities_src_dir "${CMAKE_CURRENT_LIST_DIR}/src/${PROJECT_NAME}")

include(nwx_cxx_api_docs)
nwx_cxx_api_docs("README.md" "${project_inc_dir}" "${project_src_dir}")

cmaize_add_library(
    ${PROJECT_NAME}
    SOURCE_DIR "${utilities_src_dir}"
    INCLUDE_DIRS "${utilities_inc_dir}"
)

if("${BUILD_TESTING}")
    cmaize_find_or_build_dependency(
        Catch2
        URL github.com/catchorg/Catch2
        BUILD_TARGET Catch2
        FIND_TARGET Catch2::Catch2
        VERSION ${NWX_CATCH2_VERSION}
    )
    cmaize_add_tests(
        test_unit_utilities
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/tests/unit"
        INCLUDE_DIRS "${utilities_src_dir}"
        DEPENDS Catch2 ${PROJECT_NAME}
    )
    cmaize_add_tests(
        test_integration_${PROJECT_NAME}
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/tests/integration"
        INCLUDE_DIRS "${utilities_src_dir}"
        DEPENDS Catch2 ${PROJECT_NAME}
    )
endif()

cmaize_add_package(${PROJECT_NAME} NAMESPACE nwx::)
