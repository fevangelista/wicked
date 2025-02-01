#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "../wicked/algebra/expression.h"

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the Indexclass
void export_Expression(nb::module_ &m) {
  nb::class_<Expression>(m, "Expression")
      .def(nb::init<>())
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
          "__iadd__",
          [](Expression &lhs, const Term &term) -> Expression & {
            lhs += {term.symterm(), term.coefficient()};
            return lhs;
          },
          nb::is_operator()) // Bind in-place addition with Term
      .def(
          "__iadd__",
          [](Expression &lhs,
             const std::pair<SymbolicTerm, scalar_t> &term) -> Expression & {
            lhs += term;
            return lhs;
          },
          nb::is_operator()) // Bind in-place addition with Term
      .def(
          "__iadd__",
          [](Expression &lhs, const SymbolicTerm &sterm) -> Expression & {
            lhs += {sterm, scalar_t(1)};
            return lhs;
          },
          nb::is_operator()) // Bind in-place addition with Term
      .def(
          "__iter__",
          [](Expression &e) {
            return nb::make_iterator(
                nb::type<std::map<SymbolicTerm, scalar_t>>(), "iterator",
                e.begin(), e.end());
          },
          nb::keep_alive<0, 1>())
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
