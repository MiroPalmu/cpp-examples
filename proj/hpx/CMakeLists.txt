# Require a recent version of cmake
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 26)

project(
    hpx-examples
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "test@foo.com"
    LANGUAGES CXX
)

# Instruct cmake to find the HPX settings
find_package(HPX REQUIRED)
add_hpx_executable(hello
    SOURCES hello.cpp
    COMPONENT_DEPENDENCIES iostreams)
