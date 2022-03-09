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

pybind11_add_module(_wicked ${SRC_LIST} ${module_SOURCES})
