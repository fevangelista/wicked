#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/sqoperator.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_SQOperator(py::module &m) {
  py::enum_<SQOperatorStatistics>(m, "stat")
      .value("fermion", SQOperatorStatistics::Fermion)
      .value("boson", SQOperatorStatistics::Boson);

  py::enum_<SQOperatorType>(m, "type")
      .value("cre", SQOperatorType::Creation)
      .value("ann", SQOperatorType::Annihilation);

  py::class_<SQOperator, std::shared_ptr<SQOperator>>(m, "SQOperator")
      .def(py::init<SQOperatorType, Index, SQOperatorStatistics>())
      .def("type", &SQOperator::type)
      .def("index", &SQOperator::index)
      .def("statistics", &SQOperator::statistics)
      .def("__repr__", &SQOperator::str)
      .def("__str__", &SQOperator::str)
      .def("latex", &SQOperator::latex);

  m.def("sqoperator", &make_sqoperator, "index"_a, "type"_a, "statistics"_a);
  m.def(
      "Fcre",
      [](const std::string &str) {
        return make_sqoperator(str, SQOperatorType::Creation,
                               SQOperatorStatistics::Fermion);
      },
      "index"_a);
  m.def(
      "Fann",
      [](const std::string &str) {
        return make_sqoperator(str, SQOperatorType::Annihilation,
                               SQOperatorStatistics::Fermion);
      },
      "index"_a);
  m.def(
      "Bcre",
      [](const std::string &str) {
        return make_sqoperator(str, SQOperatorType::Creation,
                               SQOperatorStatistics::Boson);
      },
      "index"_a);
  m.def(
      "Bann",
      [](const std::string &str) {
        return make_sqoperator(str, SQOperatorType::Annihilation,
                               SQOperatorStatistics::Boson);
      },
      "index"_a);
}