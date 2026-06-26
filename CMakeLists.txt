cmake_minimum_required(VERSION 3.15)

project(wicked)

set(CMAKE_CXX_STANDARD 20)

option(CODE_COVERAGE "Enable coverage reporting" OFF)

# Prefer an installed pybind11 (find_package CONFIG); fall back to the
# vendored submodule in external/pybind11 if none is found.
find_package(pybind11 CONFIG QUIET)
if(pybind11_FOUND)
  message(STATUS "Using installed pybind11 ${pybind11_VERSION}")
else()
  message(STATUS "Using vendored pybind11 from external/pybind11")
  add_subdirectory(external/pybind11)
endif()

add_subdirectory (wicked)
