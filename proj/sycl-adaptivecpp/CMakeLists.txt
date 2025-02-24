# cmake -GNinja -B build -DCMAKE_CXX_COMPILER=clang++
#
# If -DCMAKE_CXX_COMPILER=clang++ is missing and cmake defaults to g++,
# then add_sycl_to_target (from AdaptiveCpp) will change the compiler to clang++.
# By default newer cmake will add flags related to C++ modules which are compiler specific,
# so the compiler switch described above will lead to clang++ receiving g++ flags which lead to error.

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(
    sycl-adaptivecpp-example
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "test@foo.com"
    LANGUAGES CXX
)

find_package(AdaptiveCpp REQUIRED)

# From AdpativeCpp repo examples folder:
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT ACPP_DEBUG_LEVEL)
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        set(ACPP_DEBUG_LEVEL 3 CACHE STRING "Choose the debug level, options are: \
0 (no debug), \
1 (print errors), \
2 (also print warnings), \
3 (also print general information)"
          FORCE)
    else()
        set(ACPP_DEBUG_LEVEL 2 CACHE STRING "Choose the debug level, options are: \
0 (no debug), \
1 (print errors), \
2 (also print warnings), \
3 (also print general information)"
          FORCE)
    endif()
endif()

add_library(syclutils STATIC)
target_sources(syclutils
    PRIVATE
    syclutils.cpp
    PUBLIC FILE_SET HEADERS FILES
    syclutils.hpp
)

add_sycl_to_target(TARGET syclutils)

# Tell CMake we're using C++26 and that C++26 is needed to consume it.
target_compile_features(syclutils
    PRIVATE
    cxx_std_26
    INTERFACE
    cxx_std_26
)

add_library(sstd STATIC)
target_sources(sstd
    PUBLIC FILE_SET CXX_MODULES FILES
    sstd.cpp
)
target_compile_features(sstd
    PRIVATE
    cxx_std_26
    INTERFACE
    cxx_std_26
)

# Make an executable.
add_executable(main)

# Note that this source ends up with C++26 support due to the `sstd` INTERFACE requirements.
target_sources(main
  PRIVATE
    main.cpp
)

target_link_libraries(main PRIVATE syclutils sstd)
