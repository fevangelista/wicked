#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/index.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_Index(py::module &m) {
  py::class_<Index, std::shared_ptr<Index>>(m, "Index")
      .def(py::init<int, int>())
      .def("space", &Index::space)
      .def("pos", &Index::pos)
      .def("__repr__", &Index::str)
      .def("__str__", &Index::str)
      .def("__lt__", [](const Index &l, const Index &r) { return l < r; })
      .def("__eq__", [](const Index &l, const Index &r) { return l == r; })
      .def("__hash__", &Index::hash)
      .def("latex", &Index::latex)
      .def("compile", &Index::compile);

  m.def("index", &make_index_from_str, "index"_a);
  m.def("indices", &make_indices_from_str, "indices"_a);
  m.def("canonicalize_indices", [](std::vector<Index> indices, bool reverse) {
    scalar_t sign = canonicalize_indices(indices, reverse);
    return std::make_pair(sign, indices);
  });
}