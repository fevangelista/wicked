#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/diagrams/contraction.h"
#include "../wicked/diagrams/operator.h"
#include "../wicked/diagrams/operator_expression.h"
#include "../wicked/diagrams/wick_theorem.h"

namespace py = pybind11;
using namespace pybind11::literals;

void export_WickTheorem(py::module &m) {
  py::enum_<PrintLevel>(m, "PrintLevel")
      .value("none", PrintLevel::None)
      .value("basic", PrintLevel::Basic)
      .value("summary", PrintLevel::Summary)
      .value("detailed", PrintLevel::Detailed)
      .value("all", PrintLevel::All);

  py::class_<WickTheorem, std::shared_ptr<WickTheorem>>(m, "WickTheorem")
      .def(py::init<>())
      .def("contract",
           py::overload_cast<scalar_t, const OperatorProduct &, int, int>(
               &WickTheorem::contract))
      .def("contract",
           py::overload_cast<scalar_t, const OperatorExpression &, int, int>(
               &WickTheorem::contract))
      .def(
          "contract",
          [](WickTheorem &wt, const OperatorExpression &expr, const int minrank,
             const int maxrank) {
            return wt.contract(scalar_t(1), expr, minrank, maxrank);
          },
          "expr"_a, "minrank"_a, "maxrank"_a)
      .def("set_print", &WickTheorem::set_print)
      .def("set_max_cumulant", &WickTheorem::set_max_cumulant)
      .def("do_canonicalize_graph", &WickTheorem::do_canonicalize_graph)
      .def("timers", &WickTheorem::timers);
}