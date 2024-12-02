cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 26)

project(
    exec-module
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "test@foo.com"
    LANGUAGES CXX
)

execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -fmodules -std=c++26 -fsearch-include-path -c bits/std.cc
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE compile_result
    ERROR_VARIABLE compile_error
)
if(NOT compile_result EQUAL 0)
    message(FATAL_ERROR "Failed to compile std module: ${compile_error}")
endif()

add_library(std-module OBJECT ${CMAKE_BINARY_DIR}/std.o)
set_target_properties(std-module PROPERTIES LINKER_LANGUAGE CXX)
execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libstdc++.modules.json 
    RESULT_VARIABLE std_module_result
    ERROR_VARIABLE std_module_error
    OUTPUT_VARIABLE std_module_json_path
)
if(NOT std_module_result EQUAL 0)
    message(FATAL_ERROR "Failed to compile std module: ${compile_error}")
endif()
message(INFO "found: ${std_module_json_path}")
add_compile_options("-fmodule-mapper=${std_module_json_path} ")

add_library(libexec-module)
target_sources(libexec-module
    PUBLIC
    FILE_SET CXX_MODULES FILES
    exec-module.cpp
)
target_link_libraries(libexec-module PRIVATE std-module)

add_executable(foo main.cpp)
target_link_libraries(foo PRIVATE std-module)
target_link_libraries(foo PRIVATE libexec-module)
