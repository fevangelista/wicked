project(pywicked)
cmake_minimum_required(VERSION 2.9)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

# CTest
#enable_testing()
#include(CTest)
#add_subdirectory(test)

aux_source_directory(SRC SRC_LIST)
add_executable(wicked ${SRC_LIST})

add_subdirectory(pybind11)
set(module_SOURCES src_module/python_wrapper.cc)
pybind11_add_module(pywicked ${SRC_LIST} ${module_SOURCES})
