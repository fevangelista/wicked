#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "diagrams/operator.h"
#include "diagrams/operator_expression.h"
#include "helpers/rational.h"

namespace nb = nanobind;
using namespace nanobind::literals;

void export_rational(nb::module_ &m) {

  nb::class_<rational>(m, "rational")
      .def(nb::init<>())
      .def(nb::init<int>())
      .def(nb::init<int, int>())
      .def("latex", &rational::latex)
      .def("compile", &rational::compile)
      .def("numerator",
           [](const rational &r) {
             return static_cast<long long int>(r.numerator());
           })
      .def("denominator",
           [](const rational &r) {
             return static_cast<long long int>(r.denominator());
           })
      .def("__float__", &rational::to_double)
      .def("__eq__",
           [](const rational &lhs, const rational &rhs) { return lhs == rhs; })
      .def("__add__",
           [](const rational &lhs, const rational &rhs) { return lhs + rhs; })
      .def("__sub__",
           [](const rational &lhs, const rational &rhs) { return lhs - rhs; })
      .def("__mul__",
           [](const rational &lhs, const rational &rhs) { return lhs * rhs; })
      .def(
          "__mul__",
          [](const rational &lhs, OperatorExpression rhs) {
            rhs *= lhs;
            return rhs;
          },
          nb::is_operator())
      .def("__truediv__",
           [](const rational &lhs, const rational &rhs) { return lhs / rhs; })
      .def("__repr__", &rational::repr)
      .def("str", &rational::str);

  m.def("make_rational", &make_rational_from_str);
  m.def("use_boost_1024_int", &use_boost_1024_int,
        "Return true if 1024-bit integers are used");
}