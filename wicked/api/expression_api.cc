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
           "term"_a, "coefficient"_a = scalar_t(1, 1))
      .def("add",
           py::overload_cast<const Expression &, scalar_t>(&Expression::add),
           "expr"_a, "scale"_a = scalar_t(1))
      .def("__repr__", &Expression::str)
      .def("__str__", &Expression::str)
      .def("__len__", &Expression::size)
      .def("__eq__", &Expression::operator==)
      .def("__neg__", [](const Expression &rhs) { return -rhs; })
      .def("__add__",
           [](Expression lhs, const Expression &rhs) {
             lhs += rhs;
             return lhs;
           })
      .def("__sub__",
           [](Expression rhs, const Expression &lhs) {
             rhs -= lhs;
             return rhs;
           })
      .def("__matmul__",
           [](Expression rhs, const Expression &lhs) {
             rhs *= lhs;
             return rhs;
           })
      .def("__mul__",
           [](Expression rhs, scalar_t r) {
             rhs *= r;
             return rhs;
           })
      .def(
          "__iter__",
          [](Expression &e) { return py::make_iterator(e.begin(), e.end()); },
          py::keep_alive<0, 1>())
      .def("dot", &Expression::dot, "rhs"_a)
      .def("norm", &Expression::norm)
      .def("latex", &Expression::latex, "sep"_a = " \\\\ \n")
      .def("to_manybody_equation", &Expression::to_manybody_equation)
      .def("to_manybody_equations", &Expression::to_manybody_equation)
      .def("canonicalize", &Expression::canonicalize)
      .def("adjoint", &Expression::adjoint,
           "Return the adjoint of this expression")
      .def("vacuum_normal_ordered", &Expression::vacuum_normal_ordered,
           "only_same_index_contractions"_a = false,
           "Return a vacuum normal ordered version of this expression")
      //  .def("normal_ordered", &Expression::normal_ordered,
      //       "only_same_index_contractions"_a = false,
      //       "Return a vacuum normal ordered version of this expression")
      .def("is_vacuum_normal_ordered", &Expression::is_vacuum_normal_ordered,
           "Return true if this expression is vacuum normal ordered");

  m.def("operator_expr", &make_operator_expr, "label"_a, "components"_a,
        "normal_ordered"_a, "symmetry"_a = SymmetryType::Antisymmetric,
        "coefficient"_a = scalar_t(1));

  m.def("expression", &make_expression, "s"_a,
        "symmetry"_a = SymmetryType::Antisymmetric);
}
