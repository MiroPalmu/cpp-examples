# Sources:
# https://www.kitware.com/import-std-in-cmake-3-30/
# https://libcxx.llvm.org/Modules.html

# How to use libc++ (from the llvm website):
# cmake -GNinja -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS=-stdlib=libc++

# Require a recent version of cmake for import std.
cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# Enables the Standard module support. This needs to be done
# before selecting the languages.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
set(CMAKE_CXX_MODULE_STD ON)

project(
    cmake-module
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "test@foo.com"
    LANGUAGES CXX
)

# Currently CMake requires extensions enabled when using import std.
# https://gitlab.kitware.com/cmake/cmake/-/issues/25916
# https://gitlab.kitware.com/cmake/cmake/-/issues/25539
#
# 2025-02-13: the issues are still open.
set(CMAKE_CXX_EXTENSIONS ON)

add_library(sstd STATIC)
target_sources(sstd
    PUBLIC FILE_SET CXX_MODULES FILES
    sstd.cpp
)

# Tell CMake we're using C++26 and that C++26 is needed to consume it.
target_compile_features(sstd
    PRIVATE
    cxx_std_26
    INTERFACE
    cxx_std_26
)

add_library(matrixutils STATIC)
target_sources(matrixutils
    PRIVATE
    matrixutils.cpp
    PUBLIC FILE_SET HEADERS FILES
    matrixutils.hpp
)

# Tell CMake we're using C++26 and that C++26 is needed to consume it.
target_compile_features(matrixutils
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

target_link_libraries(main PRIVATE sstd matrixutils)
