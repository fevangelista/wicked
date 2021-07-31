#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/sqopprod.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_SQOpProd(py::module &m) {
  py::class_<SQOpProd, std::shared_ptr<SQOpProd>>(m, "SQOpProd")
      .def(py::init<const std::vector<Index> &, const std::vector<Index> &>())
      .def("__repr__", &SQOpProd::str)
      .def("__str__", &SQOpProd::str)
      .def("__eq__", &SQOpProd::operator==)
      .def("__lt__", &SQOpProd::operator<)
      .def("latex", &SQOpProd::latex);

  m.def(
      "sqopprod",
      py::overload_cast<const std::vector<Index> &, const std::vector<Index> &>(
          &make_sqopprod));
  m.def("sqopprod",
        py::overload_cast<const std::vector<std::string> &,
                          const std::vector<std::string> &>(&make_sqopprod));
}