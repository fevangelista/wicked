#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/diagrams/contraction.h"
#include "../wicked/diagrams/operator.h"
#include "../wicked/diagrams/operator_expression.h"
#include "../wicked/diagrams/operator_product.h"
#include "../wicked/diagrams/wick_theorem.h"

namespace py = pybind11;
using namespace pybind11::literals;

void export_Operator(py::module &m) {
  py::class_<Operator, std::shared_ptr<Operator>>(m, "Operator")
      .def(py::init<const std::string &, const std::vector<int> &,
                    const std::vector<int> &>())
      .def("__repr__", &Operator::str)
      .def("__str__", &Operator::str);
  m.def("diag_operator", &make_diag_operator, "Create a Operator object");
}

void export_OperatorExpression(py::module &m) {
  py::class_<OperatorExpression, std::shared_ptr<OperatorExpression>>(
      m, "OperatorExpression")
      .def(py::init<>())
      .def(py::init<const OperatorExpression &>())
      .def(py::init<const std::vector<OperatorProduct> &, scalar_t>(),
           py::arg("vec_vec_dop"), py::arg("factor") = rational(1))
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
      .def("__repr__", &OperatorExpression::str)
      .def("__str__", &OperatorExpression::str)
      .def("__matmul__",
           [](const OperatorExpression &lhs, const OperatorExpression &rhs) {
             return lhs * rhs;
           })
      .def("canonicalize", &OperatorExpression::canonicalize);
  m.def("op", &make_diag_operator_expression2,
        "Create a OperatorExpression object");
  m.def("commutator", &commutator,
        "Create the commutator of two OperatorExpression objects");

  m.def("bch_series", &bch_series,
        "Creates the Baker-Campbell-Hausdorff "
        "expansion of exp(-B) A exp(B) truncated at "
        "a given order n");
}
