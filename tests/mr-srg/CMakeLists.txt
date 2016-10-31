include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/algebra)
include_directories(${PROJECT_SOURCE_DIR}/src/diagrams)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(MR_SRG_SRC mr-srg.cc)

add_executable(mr-srg ${MR_SRG_SRC})
add_test(NAME test_mr-srg COMMAND mr-srg)

target_link_libraries(mr-srg wicked_lib)
