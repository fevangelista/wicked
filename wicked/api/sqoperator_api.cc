#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "../wicked/algebra/sqoperator.h"

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the Indexclass
void export_SQOperator(nb::module_ &m) {
  nb::enum_<FieldType>(m, "stat")
      .value("fermion", FieldType::Fermion)
      .value("boson", FieldType::Boson);

  nb::enum_<SQOperatorType>(m, "type")
      .value("cre", SQOperatorType::Creation)
      .value("ann", SQOperatorType::Annihilation);

  nb::class_<SQOperator>(m, "SQOperator")
      .def(nb::init<SQOperatorType, Index>())
      .def("type", &SQOperator::type)
      .def("index", &SQOperator::index)
      .def("field_type", &SQOperator::field_type)
      .def("__repr__", &SQOperator::str)
      .def("__str__", &SQOperator::str)
      .def("__lt__",
           [](const SQOperator &l, const SQOperator &r) { return l < r; })
      .def("__eq__",
           [](const SQOperator &l, const SQOperator &r) { return l == r; })
      .def("latex", &SQOperator::latex)
      .def("normal_ordered_less", &SQOperator::normal_ordered_less);

  m.def("sqoperator", &make_sqoperator, "index"_a, "type"_a);
  m.def(
      "cre",
      [](const std::string &str) {
        return make_sqoperator(str, SQOperatorType::Creation);
      },
      "index"_a);
  m.def(
      "ann",
      [](const std::string &str) {
        return make_sqoperator(str, SQOperatorType::Annihilation);
      },
      "index"_a);
}