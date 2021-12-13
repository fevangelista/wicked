cmake_minimum_required(VERSION 3.1)

project(wicked)

set(CMAKE_CXX_STANDARD 17)

option(CODE_COVERAGE "Enable coverage reporting" OFF)

add_subdirectory(external/pybind11)
add_subdirectory (wicked)
