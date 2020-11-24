cmake_minimum_required(VERSION 3.0)

project(wicked)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall")

find_package(pybind11 REQUIRED)

include_directories(src)
include_directories(src/algebra)
include_directories(src/diagrams)

aux_source_directory(src SRC_LIST)
aux_source_directory(src/algebra SRC_LIST)
aux_source_directory(src/diagrams SRC_LIST)
aux_source_directory(src/api SRC_LIST)

pybind11_add_module(wicked ${SRC_LIST} ${module_SOURCES})

# CTest
#enable_testing()
#include(CTest)
#add_subdirectory(tests)

