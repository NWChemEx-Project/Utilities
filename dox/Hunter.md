Using the Hunter Package Manager
================================

Introduction
------------

The [Hunter package manager](https://github.com/ruslo/hunter) is a C++ package
manager written entirely within CMake released under the two clause BSD 
license.  The fact that it is in CMake means that we don't introduce any 
additional dependencies into our software stack aside from a single CMake 
file, `HunterGate.cmake`, which contains the macros necessary to invoke Hunter.
The fact that it's written in CMake means its use is transparent to the end 
user.

Using Hunter
------------

This section focuses on how to use the Hunter package manager as seen by several
different categories of users.


### End-User Perspective

To an end user Hunter is transparent.  The build cycle occurs via:

1. `cmake -H. -B<build_dir> -D<option_1> -D...`
2. `cd <build_dir> && make`
3. `ctest`
4. `make install`

That's not to say Hunter did nothing, rather because Hunter is implemented 
entirely within CMake, the user doesn't need to do anything special to use it.
Behind the scenes the first thing Hunter will do is create a local cache if 
it doesn't exist.  The default location is `~/.hunter`, but this can be 
changed in a variety of ways (easiest being the environment variable 
`HUNTER_ROOT`).  The local cache is where Hunter will store the source code for 
any dependencies of the project.  Hunter will also store the assets of these
dependencies in the cache.  All entries in the cache are stored under hash 
tags that reflect the exact version of the source code used and the flags 
that were set.  Hunter will automatically attempt to locate source code and 
assets matching the parameters of the build, if it can't find them it will 
download new source code and/or build new assets.

### Developer's Perspective

Hunter's presence within your `CMakeLists.txt` is quite minimal.  Your top level
`CMakeLists.txt` must start with some obligatory boilerplate:

```.cmake
include("cmake/HunterGate.cmake")
HunterGate(
           URL "https://github.com/ruslo/hunter/archive/v0.22.14.tar.gz"
           SHA1 "f194eab02248f7d8792f7fc0158f6194d005bf86"
)
```

which starts the hunter package manager with the specified version and SHA1 
hash.  In an attempt to decouple the build of a project from the version of
Hunter used we define the CMake function `start_hunter`, which wraps these
lines of boilerplate.  The result is all of our `CMakeLists.txt` files will 
start with:

```.cmake
cmake_minimum_required(VERSION 3.6) #Should be first line of all CMake projects
include(cmake/Macros.cmake) #Load our helper macros
start_hunter()
``` 

We next proceed to load our dependencies.

```.cmake
hunter_find_package(Catch)
find_package(Catch2 CONFIG REQUIRED)
```

The first line tells hunter to locate `Catch`.  It will check it's cache and if
it doesn't find a `Catch` built with identical compilation options it will 
interject a request to build one.  It also automatically changes the paths that
will be used by `find_package` to ensure it uses the corresponding version of
`Catch`.  These two lines would be repeated for every dependency required by 
the project that is also available in the hunter package repository.   

After all dependencies are accounted for one then sets their project up in the
usual CMake manner (*i.e.*, adding a bunch of source files to a 
`add_library` or `add_executable` command).  After setting up all the 
targets for the project they should be passed to our macro 
`install_targets` which will take care of the boilerplate required to make 
the various config files for our project to work with hunter.  Tests are 
handled similar to libraries and executables, except that they must also be
registered with `ctest`; we provide a macro `add_catch_cxx_tests` which helps
with that.

Changes Relative to CMakeBuild
------------------------------

### Testing Using Catch

CMakeBuild required Catch to be compiled into a library and all tests to link
against it.  This is not the standard usage of Catch and the Catch package 
available from the Hunter repository will not obey this behavior.  The 
recommended solution is to include a source file that contains two lines:

```.cpp
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
```

For sake of argument we'll call this file `test_main.cpp`.  One then implements
tests in the same manner as under CMakeBuild, for example:

```.cpp
#include <AwesomeClass.hpp>
#include <catch.hpp>

TEST_CASE("Awesome name"){...
```

For sake of argument we'll call this file `test1.cpp`.  Finally, all tests are 
compiled into a single executable *via* the usual CTest procedure:

```.cmake
enable_testing()
add_executable(test_example test_main.cpp test1.cpp)
target_link_libraries(test_example PRIVATE Catch2::Catch target2test)
add_test(NAME "test example" COMMAND test_example)
```

For convenience we define the CMake function `add_catch_cxx_tests` which allows
the above to be written in a manner with much less boilerplate:

```.cmake
add_catch_cxx_tests(test_example "test_main.cpp;test1.cpp" target2test)
```

Note that running of a single test will has to be done through `catch` instead
of `ctest`.

General Notes
-------------

The biggest problem with Hunter is its assumption that recipes for obtaining a 
package are all centrally located within the main hunter repo on GitHub.  
`HunterGate` ultimately will clone this repo into the Cache and use those 
recipes for locating packages.  If we want to have recipes for local packages
there's several options:
1. Add the recipe to the local clone
2. Use the submodule option
3. Patch [`hunter_get_package_files_to_load.cmake`](https://github.com/ruslo/hunter/blob/08a6cbcf06bb5934b6b18aa1f2028cf56a1063b7/cmake/modules/hunter_get_project_files_to_load.cmake)
   to look for package files in multiple places.
   


Further Reading
---------------

The Hunter [documentation](https://docs.hunter.sh/en/latest/) is quite thorough,
(albeit hard to read at times owing to the author presumably not being a 
native English speaker).

