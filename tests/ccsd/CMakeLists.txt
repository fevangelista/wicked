include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/algebra)
include_directories(${PROJECT_SOURCE_DIR}/src/diagrams)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CCSD_SRC ccsd.cc)

add_executable(ccsd ${CCSD_SRC})
add_test(NAME test_ccsd COMMAND ccsd)

target_link_libraries(ccsd wicked_lib)
