#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "../wicked/algebra/index.h"

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the Indexclass
void export_Index(nb::module_ &m) {
  nb::class_<Index>(m, "Index")
      .def(nb::init<int, int>())
      .def("space", &Index::space)
      .def("pos", &Index::pos)
      .def("__repr__", &Index::str)
      .def("__str__", &Index::str)
      .def("__lt__", [](const Index &l, const Index &r) { return l < r; })
      .def("__eq__", [](const Index &l, const Index &r) { return l == r; })
      .def("latex", &Index::latex)
      .def("compile", &Index::compile);

  m.def("index", &make_index_from_str, "index"_a);
  m.def("indices", &make_indices_from_str, "indices"_a);
  m.def("canonicalize_indices", [](std::vector<Index> indices, bool reverse) {
    scalar_t sign = canonicalize_indices(indices, reverse);
    return std::make_pair(sign, indices);
  });
}