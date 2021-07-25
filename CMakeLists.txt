cmake_minimum_required(VERSION 3.1)

project(wicked)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall")

find_package(pybind11 REQUIRED)

include_directories(wicked)
include_directories(wicked/algebra)
include_directories(wicked/diagrams)

aux_source_directory(wicked SRC_LIST)
aux_source_directory(wicked/algebra SRC_LIST)
aux_source_directory(wicked/diagrams SRC_LIST)
aux_source_directory(wicked/api SRC_LIST)

pybind11_add_module(wicked ${SRC_LIST} ${module_SOURCES})
