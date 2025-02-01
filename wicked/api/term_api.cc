#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "../wicked/algebra/term.h"

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the Indexclass
void export_SymbolicTerm(nb::module_ &m) {
  nb::class_<SymbolicTerm>(m, "SymbolicTerm")

      .def(nb::init<>())
      .def("__repr__", &SymbolicTerm::str)
      .def("__str__", &SymbolicTerm::str)
      .def("latex", &SymbolicTerm::latex)
      .def("add", nb::overload_cast<const std::vector<SQOperator> &>(
                      &SymbolicTerm::add))
      .def("add", nb::overload_cast<const SQOperator &>(&SymbolicTerm::add))
      .def("add", nb::overload_cast<const Tensor &>(&SymbolicTerm::add))
      .def("set", nb::overload_cast<const std::vector<SQOperator> &>(
                      &SymbolicTerm::set))
      .def("set_normal_ordered", &SymbolicTerm::set_normal_ordered)
      .def("ops", &SymbolicTerm::ops)
      .def("tensors", &SymbolicTerm::tensors);

  nb::class_<Term>(m, "Term")
      .def(nb::init<>())
      .def(nb::init<const SymbolicTerm>())
      .def("__repr__", &Term::str)
      .def("__str__", &Term::str)
      .def("latex", &Term::latex)
      .def("add",
           nb::overload_cast<const std::vector<SQOperator> &>(&Term::add))
      .def("add", nb::overload_cast<const SQOperator &>(&Term::add))
      .def("add", nb::overload_cast<const Tensor &>(&Term::add))
      .def("set", nb::overload_cast<scalar_t>(&Term::set))
      .def("set_normal_ordered", &Term::set_normal_ordered);
}