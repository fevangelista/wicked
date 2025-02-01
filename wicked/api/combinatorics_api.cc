#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include "helpers/combinatorics.h"

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the combinatorics
void export_combinatorics(nb::module_ &m) {
  m.def("integer_partitions", &integer_partitions);
}