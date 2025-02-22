# https://github.com/intel/llvm/blob/sycl/sycl/doc/GetStartedGuide.md#build-dpc-application-with-cmake

cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# Modifying the compiler should be done before the project line
# If default clang would have sycl:
# https://github.com/llvm/llvm-project/pulls?q=is%3Apr+sycl+label%3ASYCL
# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl")

# Using OneAPI icpx:
#
# Was broken on Arch Linux: https://github.com/intel/llvm/issues/16903
# Got fixed: https://gitlab.archlinux.org/archlinux/packaging/packages/glibc/-/commit/f4ffb03813be82be083c382ca07a492b2225522c
#
# At least on my personal Arch Linux laptop with AMD Ryzen 3 PRO 3300U w/ Radeon Vega Mobile Gfx
# a working driver comes with intel-oneapi-basekit which includes intel-oneapi-dcpp-cpp.
#
# source /opt/intel/oneapi/setvars.sh
# cmake -GNinja -B build
set(CMAKE_CXX_COMPILER "icpx")
# + find_package(IntelSYCL) after project.

project(
    sycl-clang-example
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "test@foo.com"
    LANGUAGES CXX
)

# Should not be needed if pure clang toolchain worked.
find_package(IntelSYCL REQUIRED)

add_library(syclutils STATIC)
target_sources(syclutils
    PRIVATE
    syclutils.cpp
    PUBLIC FILE_SET HEADERS FILES
    syclutils.hpp
)

# Add SYCL support specifically to syclutils.cpp.
# Without SOURCES default behaviour is to add SYCL support to everything.
add_sycl_to_target(TARGET syclutils SOURCES syclutils.cpp)

# Tell CMake we're using C++26 and that C++26 is needed to consume it.
target_compile_features(syclutils
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

target_link_libraries(main PRIVATE syclutils)
