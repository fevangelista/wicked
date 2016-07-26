#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../src/orbital_space.h"
#include "../src/woperator.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace py = pybind11;

PYBIND11_PLUGIN(pywicked) {
    py::module m("pywicked", "Wicked python interface");

    py::enum_<OrbitalSpaceType>(m, "OrbitalSpaceType")
            .value("Core", OrbitalSpaceType::Core)
            .value("Active", OrbitalSpaceType::Active)
            .value("Virtual", OrbitalSpaceType::Virtual)
            .value("Hole", OrbitalSpaceType::Hole)
            .value("Particle", OrbitalSpaceType::Particle)
            .value("All", OrbitalSpaceType::All)
            .value("RI", OrbitalSpaceType::RI)
            .value("None", OrbitalSpaceType::None)
            .export_values();

    py::enum_<SpinType>(m, "SpinType")
            .value("SpinOrbital", SpinType::SpinOrbital)
            .value("SpinFree", SpinType::SpinFree)
            .value("Alpha", SpinType::Alpha)
            .value("Beta", SpinType::Beta)
            .export_values();

    py::class_<OrbitalSpaceInfo,std::shared_ptr<OrbitalSpaceInfo>>(m, "OrbitalSpaceInfo")
            .def(py::init<>())
            .def("default_spaces",&OrbitalSpaceInfo::default_spaces);

    py::class_<WOperator,std::shared_ptr<WOperator>>(m, "WOperator")
            .def(py::init<const std::string&,const std::vector<int>&,const std::vector<int>&>())
            .def("label",&WOperator::label)
            .def("num_indices",&WOperator::num_indices)
            .def("str",&WOperator::str);

    m.def("get_osi", &get_osi, "Return the orbital space");

    std::cout << "Initializing the pywicked module." << std::endl;

    osi = std::make_shared<OrbitalSpaceInfo>();
    osi->default_spaces();

    return m.ptr();
}
