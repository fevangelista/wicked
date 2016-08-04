#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../src/orbital_space.h"
#include "../src/wdiag_operator.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace py = pybind11;

PYBIND11_PLUGIN(pywicked) {
  py::module m("pywicked", "Wicked python interface");

  py::enum_<SpinType>(m, "SpinType")
      .value("SpinOrbital", SpinType::SpinOrbital)
      .value("SpinFree", SpinType::SpinFree)
      .value("Alpha", SpinType::Alpha)
      .value("Beta", SpinType::Beta)
      .export_values();

  py::class_<OrbitalSpaceInfo, std::shared_ptr<OrbitalSpaceInfo>>(
      m, "OrbitalSpaceInfo")
      .def(py::init<>())
      .def("default_spaces", &OrbitalSpaceInfo::default_spaces);

  py::class_<WDiagOperator, std::shared_ptr<WDiagOperator>>(m, "WOperator")
      .def(py::init<const std::string &, const std::vector<int> &,
                    const std::vector<int> &>())
      .def("label", &WDiagOperator::label)
      .def("num_indices", &WDiagOperator::num_indices)
      .def("str", &WDiagOperator::str);

  m.def("get_osi", &get_osi, "Return the orbital space");

  std::cout << "Initializing the pywicked module." << std::endl;

  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->default_spaces();

  return m.ptr();
}
