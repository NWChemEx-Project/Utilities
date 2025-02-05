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

#Downloads common CMake modules used throughout NWChemEx
include(cmake/get_nwx_cmake.cmake)

#Sets the version to whatever git thinks it is
include(get_version_from_git)
get_version_from_git(utilities_version "${CMAKE_CURRENT_LIST_DIR}")
project(utilities VERSION "${utilities_version}" LANGUAGES CXX)

include(nwx_versions)
include(get_cmaize)
include(nwx_cxx_api_docs)

### Paths ###
set(UTILITIES_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(UTILITIES_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

nwx_cxx_api_docs("${UTILITIES_SOURCE_DIR}" "${UTILITIES_INCLUDE_DIR}")

### Options ###
cmaize_option_list(
    BUILD_TESTING OFF "Should we build the tests?"
)

cmaize_add_library(
    ${PROJECT_NAME}
    SOURCE_DIR "${UTILITIES_SOURCE_DIR}/${PROJECT_NAME}"
    INCLUDE_DIRS "${UTILITIES_INCLUDE_DIR}/${PROJECT_NAME}"
)

if("${BUILD_TESTING}")
    set(UTILITIES_TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/tests/unit_tests")

    include(get_catch2)

    cmaize_add_tests(
        test_unit_${PROJECT_NAME}
        SOURCE_DIR "${UTILITIES_TEST_DIR}"
        INCLUDE_DIRS "${UTILITIES_SOURCE_DIR}/${PROJECT_NAME}"
        DEPENDS Catch2 ${PROJECT_NAME}
    )
endif()

cmaize_add_package(${PROJECT_NAME} NAMESPACE nwx::)
