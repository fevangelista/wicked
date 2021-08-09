cmake_minimum_required(VERSION 3.1)

project(wicked)

set(CMAKE_CXX_STANDARD 17)
find_package(pybind11 REQUIRED)

add_subdirectory (wicked)

