include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/algebra)
include_directories(${PROJECT_SOURCE_DIR}/src/diagrams)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(SR_SRG_SRC sr-srg.cc)

add_executable(sr-srg ${SR_SRG_SRC})
add_test(NAME test_srg COMMAND sr-srg)

target_link_libraries(sr-srg wicked_lib)
