#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "algebra/equation.h"
#include "algebra/expression.h" // for rhs_expression

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the Equation class
void export_Equation(nb::module_ &m) {
  nb::class_<Equation>(m, "Equation")
      .def(nb::init<const SymbolicTerm &, const SymbolicTerm &, scalar_t>())
      .def("lhs", &Equation::lhs)
      .def("rhs", &Equation::rhs)
      .def("rhs_expression", &Equation::rhs_expression)
      .def("rhs_factor", &Equation::rhs_factor)
      .def("__repr__", &Equation::str)
      .def("__str__", &Equation::str)
      .def("latex", &Equation::latex)
      .def("compile", &Equation::compile);
}