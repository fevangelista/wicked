cmake_minimum_required(VERSION 2.8)

project(pywicked)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")


aux_source_directory(src SRC_LIST)
aux_source_directory(src/algebra SRC_LIST)
aux_source_directory(src/diagrams SRC_LIST)

add_library(wicked_lib ${SRC_LIST})
add_executable(wicked ${SRC_LIST})

add_subdirectory(external/pybind11)
set(module_SOURCES src_module/python_wrapper.cc)
pybind11_add_module(pywicked ${SRC_LIST} ${module_SOURCES})

# CTest
enable_testing()
#include(CTest)
add_subdirectory(tests)

include_directories(src)
include_directories(src/algebra)
include_directories(src/diagrams)
