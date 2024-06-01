#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "helpers/orbital_space.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_OrbitalSpaceInfo(py::module &m) {
  py::class_<OrbitalSpaceInfo, std::shared_ptr<OrbitalSpaceInfo>>(
      m, "OrbitalSpaceInfo")
      .def(py::init<>())
      .def("reset_space", &OrbitalSpaceInfo::reset)
      .def("add_space", &OrbitalSpaceInfo::add_space)
      .def("num_spaces", &OrbitalSpaceInfo::num_spaces)
      .def("label", &OrbitalSpaceInfo::label)
      .def("indices", &OrbitalSpaceInfo::indices)
      .def("to_dict", &OrbitalSpaceInfo::to_dict)
      .def("__str__", &OrbitalSpaceInfo::str);

  m.def("osi", []() { return orbital_subspaces; });

  m.def(
      "reset_space", []() { orbital_subspaces->reset(); },
      "Reset the orbital space");

  m.def(
      "add_space",
      [](char label, const std::string &field_type_str,
         const std::string &space_type_str,
         const std::vector<std::string> &indices,
         const std::vector<char> &elementary_spaces) {
        FieldType field_type = string_to_field_type(field_type_str);
        SpaceType space_type = string_to_space_type(space_type_str);
        orbital_subspaces->add_space(label, field_type, space_type, indices,
                                     elementary_spaces);
      },
      "label"_a, "field_type"_a, "space_type"_a, "indices"_a,
      "elementary_spaces"_a = std::vector<char>(),
      "Add an orbital space. `field_type` can be any of "
      "[fermion,boson]. `space_type` can be any of "
      "[occupied,unoccupied,general]");

  m.def("num_spaces", []() { return orbital_subspaces->num_spaces(); });
}