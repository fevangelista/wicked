include_directories(.)
include_directories(algebra)
include_directories(diagrams)
include_directories(fmt)

aux_source_directory(. SRC_LIST)
aux_source_directory(algebra SRC_LIST)
aux_source_directory(diagrams SRC_LIST)
aux_source_directory(helpers SRC_LIST)
aux_source_directory(api SRC_LIST)
aux_source_directory(fmt SRC_LIST)

if(CODE_COVERAGE)
  message("-- Code coverage enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif(CODE_COVERAGE)

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

pybind11_add_module(_wicked ${SRC_LIST} ${module_SOURCES})
