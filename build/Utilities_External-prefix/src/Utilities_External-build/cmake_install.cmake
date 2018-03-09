# Install script for directory: /Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/NWChemEx")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Utilities" TYPE SHARED_LIBRARY FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/build/Utilities_External-prefix/src/Utilities_External-build/libUtilities.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Utilities/libUtilities.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Utilities/libUtilities.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Utilities/libUtilities.dylib")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/Utilities" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/build/Utilities_External-prefix/src/Utilities_External-build/UtilitiesConfig.cmake")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/Any.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/Containers" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/Containers/CaseInsensitiveMap.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/Containers" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/Containers/MathSet.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/Mathematician" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/Mathematician/Combinatorics.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/Mathematician" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/Mathematician/IntegerUtils.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/CartesianProduct.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/Combinations.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/Enumerate.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/Permutations.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/Range.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/RangeContainer.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/TupleContainer.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/IterTools" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/IterTools/Zip.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/TypeTraits" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/TypeTraits/IteratorTypes.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/TypeTraits" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/TypeTraits/TupleUtilities.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/TypeTraits" TYPE FILE FILES "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/Utilities/TypeTraits/type_traitsExtensions.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/wadejong/Desktop/NWChemEx_Project/NWChemEx-Project/Working/Utilities/build/Utilities_External-prefix/src/Utilities_External-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
