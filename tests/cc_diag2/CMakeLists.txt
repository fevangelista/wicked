include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/algebra)
include_directories(${PROJECT_SOURCE_DIR}/src/diagrams)
include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CC_DIAG2_SRC cc_diag2.cc)

add_executable(cc_diag2 ${CC_DIAG2_SRC})
add_test(NAME test_cc_diag2 COMMAND cc_diag2)

target_link_libraries(cc_diag2 wicked_lib)
