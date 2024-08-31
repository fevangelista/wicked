#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/sqoperator.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_SQOperator(py::module &m) {
  py::enum_<FieldType>(m, "stat")
      .value("fermion", FieldType::Fermion)
      .value("boson", FieldType::Boson);

  py::enum_<SQOperatorType>(m, "type")
      .value("cre", SQOperatorType::Creation)
      .value("ann", SQOperatorType::Annihilation);

  py::class_<SQOperator, std::shared_ptr<SQOperator>>(m, "SQOperator")
      .def(py::init<SQOperatorType, Index>())
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