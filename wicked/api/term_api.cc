#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/term.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_SymbolicTerm(py::module &m) {
  py::class_<SymbolicTerm, std::shared_ptr<SymbolicTerm>>(m, "SymbolicTerm")

      .def(py::init<>())
      .def("__repr__", &SymbolicTerm::str)
      .def("__str__", &SymbolicTerm::str)
      .def("latex", &SymbolicTerm::latex)
      .def("add", py::overload_cast<const std::vector<SQOperator> &>(
                      &SymbolicTerm::add))
      .def("add", py::overload_cast<const SQOperator &>(&SymbolicTerm::add))
      .def("add", py::overload_cast<const Tensor &>(&SymbolicTerm::add))
      .def("set", py::overload_cast<const std::vector<SQOperator> &>(
                      &SymbolicTerm::set))
      .def("set_normal_ordered", &SymbolicTerm::set_normal_ordered)
      .def("tensors", &SymbolicTerm::tensors);
  ;

  py::class_<Term, std::shared_ptr<Term>>(m, "Term")
      .def(py::init<>())
      .def(py::init<const SymbolicTerm>())
      .def("__repr__", &Term::str)
      .def("__str__", &Term::str)
      .def("latex", &Term::latex)
      .def("add",
           py::overload_cast<const std::vector<SQOperator> &>(&Term::add))
      .def("add", py::overload_cast<const SQOperator &>(&Term::add))
      .def("add", py::overload_cast<const Tensor &>(&Term::add))
      .def("set", py::overload_cast<scalar_t>(&Term::set))
      .def("set_normal_ordered", &Term::set_normal_ordered);
}