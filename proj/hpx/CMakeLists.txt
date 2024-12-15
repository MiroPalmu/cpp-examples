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

add_executable(eg1 eg1.cpp)
target_include_directories(eg1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(TBB REQUIRED)
# For some reason this needs to be linked with tbb.
target_link_libraries(eg1 PRIVATE tbb)
target_link_libraries(eg1 PRIVATE HPX::hpx)

add_executable(intrest-calculator dataflow-intrest-calculator.cpp)
target_link_libraries(intrest-calculator PRIVATE tbb)
target_link_libraries(intrest-calculator PRIVATE HPX::hpx)

add_executable(async-fibo async-fibo.cpp)
target_link_libraries(async-fibo PRIVATE tbb)
target_link_libraries(async-fibo PRIVATE HPX::hpx)

add_executable(dataflow-fibo dataflow-fibo.cpp)
target_link_libraries(dataflow-fibo PRIVATE tbb)
target_link_libraries(dataflow-fibo PRIVATE HPX::hpx)

include(FetchContent)

FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master) # or whatever tag you want

FetchContent_MakeAvailable(matplotplusplus)
