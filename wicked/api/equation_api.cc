#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/equation.h"
#include "../wicked/algebra/expression.h" // for rhs_expression

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Equation class
void export_Equation(py::module &m) {
  py::class_<Equation, std::shared_ptr<Equation>>(m, "Equation")
      .def(py::init<const SymbolicTerm &, const SymbolicTerm &, scalar_t>())
      .def("lhs", &Equation::lhs)
      .def("rhs", &Equation::rhs)
      .def("rhs_expression", &Equation::rhs_expression)
      .def("rhs_factor", &Equation::rhs_factor)
      .def("__repr__", &Equation::str)
      .def("__str__", &Equation::str)
      .def("latex", &Equation::latex)
      .def("compile", &Equation::compile);
}