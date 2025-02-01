cmake_minimum_required(VERSION 3.15...3.26)

project(wicked LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files.")
endif()

# Try to import all Python components potentially needed by nanobind
find_package(Python 3.8
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

# Import nanobind through CMake's find_package mechanism
find_package(nanobind CONFIG REQUIRED)

include_directories(wicked)
include_directories(wicked/algebra)
include_directories(wicked/diagrams)

aux_source_directory(wicked SRC_LIST)
aux_source_directory(wicked/algebra SRC_LIST)
aux_source_directory(wicked/diagrams SRC_LIST)
aux_source_directory(wicked/helpers SRC_LIST)
aux_source_directory(wicked/api SRC_LIST)

# We are now ready to compile the actual extension module
nanobind_add_module(
  # Name of the extension
  _wicked

  # Target the stable ABI for Python 3.12+, which reduces
  # the number of binary wheels that must be built. This
  # does nothing on older Python versions
  STABLE_ABI

  # Build libnanobind statically and merge it into the
  # extension (which itself remains a shared library)
  #
  # If your project builds multiple extensions, you can
  # replace this flag by NB_SHARED to conserve space by
  # reusing a shared libnanobind across libraries
  NB_STATIC

  # Source code goes here
  ${SRC_LIST} ${module_SOURCES}
)

# Install directive for scikit-build-core
install(TARGETS _wicked LIBRARY DESTINATION wicked)

# Look for the Boost libraries
find_package(Boost)

# Check if Boost was found
if(Boost_FOUND)
    message(STATUS "Boost found")

    # Define the USE_BOOST_1024_INT flag
    add_definitions(-DUSE_BOOST_1024_INT)

    # Add Boost's include directories to the build
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(STATUS "Boost not found")
endif()

option(CODE_COVERAGE "Enable coverage reporting" OFF)

if(CODE_COVERAGE)
  message("-- Code coverage enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif(CODE_COVERAGE)