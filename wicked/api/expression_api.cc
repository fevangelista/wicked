#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/expression.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_Expression(py::module &m) {
  py::class_<Expression, std::shared_ptr<Expression>>(m, "Expression")
      .def(py::init<>())
      .def("add", py::overload_cast<const Term &>(&Expression::add))
      .def("add",
           py::overload_cast<const SymbolicTerm &, scalar_t>(&Expression::add),
           "term"_a, "coefficient"_a = scalar_t(1))
      .def("add",
           py::overload_cast<const Expression &, scalar_t>(&Expression::add),
           "expr"_a, "scale"_a = scalar_t(1))
      .def("__repr__", &Expression::str)
      .def("__str__", &Expression::str)
      .def("__eq__", &Expression::operator==)
      .def("__add__", [](const Expression &lhs,
                         const Expression &rhs) { return lhs + rhs; })
      .def("latex", &Expression::latex, "sep"_a = " \\\\ \n")
      .def("to_manybody_equation", &Expression::to_manybody_equation)
      .def("canonicalize", &Expression::canonicalize);

  m.def("operator_expr", &make_operator_expr, "label"_a, "components"_a,
        "normal_ordered"_a, "coefficient"_a = scalar_t(1));

  m.def("string_to_expr", &string_to_sum);
}