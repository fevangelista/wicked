include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/algebra)
include_directories(${PROJECT_SOURCE_DIR}/src/diagrams)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CC_DIAG1_SRC cc_diag1.cc)

add_executable(cc_diag1 ${CC_DIAG1_SRC})
add_test(NAME test_cc_diag1 COMMAND cc_diag1)

target_link_libraries(cc_diag1 wicked_lib)
