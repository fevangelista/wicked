#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/diagrams/contraction.h"
#include "../wicked/diagrams/diag_operator.h"
#include "../wicked/diagrams/diag_operator_expression.h"
#include "../wicked/diagrams/wick_theorem.h"

namespace py = pybind11;
using namespace pybind11::literals;

void export_DiagOperator(py::module &m) {
  py::class_<DiagOperator, std::shared_ptr<DiagOperator>>(m, "DiagOperator")
      .def(py::init<const std::string &, const std::vector<int> &,
                    const std::vector<int> &>())
      .def("__repr__", &DiagOperator::str)
      .def("__str__", &DiagOperator::str);
  m.def("diag_operator", &make_diag_operator, "Create a DiagOperator object");
}

void export_DiagOpExpression(py::module &m) {
  py::class_<DiagOpExpression, std::shared_ptr<DiagOpExpression>>(
      m, "DiagOpExpression")
      .def(py::init<>())
      .def(py::init<const std::vector<std::vector<DiagOperator>> &, scalar_t>(),
           py::arg("vec_vec_dop"), py::arg("factor") = rational(1))
      .def("add", &DiagOpExpression::add)
      .def("__repr__", &DiagOpExpression::str)
      .def("__str__", &DiagOpExpression::str)
      .def("__matmul__", [](const DiagOpExpression &lhs,
                            const DiagOpExpression &rhs) { return lhs * rhs; });
  m.def("op", &make_diag_operator_expression2,
        "Create a DiagOpExpression object");
  m.def("commutator", &commutator,
        "Create the commutator of two DiagOpExpression objects");

  m.def("bch_series", &bch_series,
        "Creates the Baker-Campbell-Hausdorff "
        "expansion of exp(-B) A exp(B) truncated at "
        "a given order n");
}

void export_WickTheorem(py::module &m) {
  py::enum_<PrintLevel>(m, "PrintLevel")
      .value("no", PrintLevel::No)
      .value("basic", PrintLevel::Basic)
      .value("summary", PrintLevel::Summary)
      .value("detailed", PrintLevel::Detailed)
      .value("all", PrintLevel::All);

  py::class_<WickTheorem, std::shared_ptr<WickTheorem>>(m, "WickTheorem")
      .def(py::init<>())
      .def("contract",
           py::overload_cast<scalar_t, const std::vector<DiagOperator> &, int,
                             int>(&WickTheorem::contract))
      .def("contract",
           py::overload_cast<scalar_t, const DiagOpExpression &, int, int>(
               &WickTheorem::contract))
      .def("set_print", &WickTheorem::set_print);
}