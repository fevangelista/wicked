#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_PLUGIN(pywicked) {
    py::module m("pywicked", "Wicked python interface");

//    py::enum_<>(pywicked, "OrbitalSpaceType")
//        .value("Core", OrbitalSpaceType::Core)
//        .value("Active", OrbitalSpaceType::Active)
//        .export_values();

    return m.ptr();
}
