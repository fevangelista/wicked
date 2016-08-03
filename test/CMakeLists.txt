cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

set(TEST_INIT_SRC test_init.cc ../src/orbital_space.cc)
include_directories(../src)
add_executable(test_init ${TEST_INIT_SRC})
add_test(NAME init COMMAND test_init)
#target_link_libraries(test_init wicked)
