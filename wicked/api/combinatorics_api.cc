#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/combinatorics.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the combinatorics
void export_combinatorics(py::module &m) {
  m.def("integer_partitions", &integer_partitions);
}