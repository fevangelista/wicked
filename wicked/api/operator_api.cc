#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "../wicked/diagrams/contraction.h"
#include "../wicked/diagrams/operator.h"
#include "../wicked/diagrams/operator_expression.h"
#include "../wicked/diagrams/operator_product.h"
#include "../wicked/diagrams/wick_theorem.h"

namespace nb = nanobind;
using namespace nanobind::literals;

void export_Operator(nb::module_ &m) {
  nb::class_<Operator>(m, "Operator")
      .def(nb::init<const std::string &, const std::vector<int> &,
                    const std::vector<int> &>())
      .def("__repr__", &Operator::str)
      .def("__str__", &Operator::str);
  m.def("diag_operator", &make_diag_operator, "Create a Operator object");
}

void export_OperatorExpression(nb::module_ &m) {
  nb::class_<OperatorExpression>(m, "OperatorExpression")
      .def(nb::init<>())
      .def(nb::init<const OperatorExpression &>())
      .def(nb::init<const std::vector<OperatorProduct> &, scalar_t>(),
           nb::arg("vec_vec_dop"), nb::arg("factor") = rational(1))
      .def("size", &OperatorExpression::size)
      .def("add", &OperatorExpression::add)
      .def("adjoint", &OperatorExpression::adjoint)
      .def("add2", &OperatorExpression::add2)
      .def("__add__",
           [](OperatorExpression rhs, const OperatorExpression &lhs) {
             rhs += lhs;
             return rhs;
           })
      .def("__sub__",
           [](OperatorExpression rhs, const OperatorExpression &lhs) {
             rhs -= lhs;
             return rhs;
           })
      .def("__eq__",
           [](const OperatorExpression &rhs, const OperatorExpression &lhs) {
             return rhs.is_equal(lhs);
           })
      .def("__repr__", &OperatorExpression::str)
      .def("__str__", &OperatorExpression::str)
      .def("__matmul__",
           [](const OperatorExpression &lhs, const OperatorExpression &rhs) {
             return lhs * rhs;
           })
      .def("canonicalize", &OperatorExpression::canonicalize);
  // m.def("op", &make_diag_operator_expression, "label"_a, "components"_a,
  //       "unique"_a = false,
  //       nb::call_guard<nb::scoped_ostream_redirect,
  //                      nb::scoped_estream_redirect>(),
  //       "Create a OperatorExpression object");

  m.def("op", &make_diag_operator_expression, nb::arg("label"),
        nb::arg("components"), nb::arg("unique") = false,
        // nb::call_guard<nb::ostream_redirect>(),
        "Create an OperatorExpression object");

  m.def(
      "commutator",
      [](nb::args args) {
        int k = 0;
        OperatorExpression result;
        for (const auto &arg : args) {
          OperatorExpression &object = nb::cast<OperatorExpression &>(arg);
          if (k == 0) {
            result = object;
          } else {
            result = commutator(result, object);
          }
          k += 1;
        }
        return result;
      },
      "Create the commutator of a list of OperatorExpression objects");

  m.def("bch_series", &bch_series,
        "Creates the Baker-Campbell-Hausdorff "
        "expansion of exp(-B) A exp(B) truncated at "
        "a given order n");
}
