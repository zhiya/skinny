cmake_minimum_required(VERSION 2.8)

project(skinny)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE src_list "src/*.cpp")

add_executable(skinny ${src_list})

add_executable(skinnyclient "client/main.cpp")

