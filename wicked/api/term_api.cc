#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/tensor.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_Term(py::module &m) {
  py::class_<Term, std::shared_ptr<Term>>(m, "Term")
      .def(py::init<>())
      .def("__repr__", &Tensor::str)
      .def("__str__", &Tensor::str)
      .def("latex", &Term::latex)
      .def("ambit", &Term::ambit)
      .def("add", py::overload_cast<const SQOperator &>(&Term::add))
      .def("add", py::overload_cast<const Tensor &>(&Term::add));
}