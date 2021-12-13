include_directories(.)
include_directories(algebra)
include_directories(diagrams)
include_directories(fmt)

aux_source_directory(. SRC_LIST)
aux_source_directory(algebra SRC_LIST)
aux_source_directory(diagrams SRC_LIST)
aux_source_directory(api SRC_LIST)
aux_source_directory(fmt SRC_LIST)

pybind11_add_module(wicked ${SRC_LIST} ${module_SOURCES})
