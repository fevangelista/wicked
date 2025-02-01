#include <nanobind/nanobind.h>

#include "../wicked/diagrams/contraction.h"
#include "../wicked/diagrams/operator.h"
#include "../wicked/diagrams/operator_expression.h"
#include "../wicked/diagrams/wick_theorem.h"

namespace nb = nanobind;
using namespace nanobind::literals;

void export_WickTheorem(nb::module_ &m) {
  nb::enum_<PrintLevel>(m, "PrintLevel")
      .value("none", PrintLevel::None)
      .value("basic", PrintLevel::Basic)
      .value("summary", PrintLevel::Summary)
      .value("detailed", PrintLevel::Detailed)
      .value("all", PrintLevel::All);

  nb::class_<WickTheorem>(m, "WickTheorem")
      .def(nb::init<>(), "Constructor")
      .def("contract",
           nb::overload_cast<scalar_t, const OperatorProduct &, int, int>(
               &WickTheorem::contract),
           "factor"_a, "ops"_a, "minrank"_a, "maxrank"_a,
           "Contract a product of operators")
      .def("contract",
           nb::overload_cast<scalar_t, const OperatorExpression &, int, int>(
               &WickTheorem::contract),
           "factor"_a, "ops"_a, "minrank"_a, "maxrank"_a,
           "Contract a product of operators")
      .def(
          "contract",
          [](WickTheorem &wt, const OperatorExpression &expr, const int minrank,
             const int maxrank) {
            return wt.contract(scalar_t(1), expr, minrank, maxrank);
          },
          "expr"_a, "minrank"_a, "maxrank"_a,
          "Contract a product of sums of operators")
      .def("set_print", &WickTheorem::set_print, "print"_a,
           "Set the amount of printing (PrintLevel.[none, basic, "
           "summary,detailed, all])")
      .def("set_max_cumulant", &WickTheorem::set_max_cumulant, "val"_a,
           "Set the maximum cumulant level")
      .def("set_single_threaded", &WickTheorem::set_single_threaded, "val"_a,
           "Set the single-threaded mode (default: false)")
      .def("do_canonicalize_graph", &WickTheorem::do_canonicalize_graph,
           "val"_a, "Turn on/off graph canonicalization")
      .def("timers", &WickTheorem::timers,
           "Return a dictionary with the timers used in the contractions");
}