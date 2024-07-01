#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/tensor.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_Tensor(py::module &m) {
  py::enum_<SymmetryType>(m, "sym")
      .value("symm", SymmetryType::Symmetric)
      .value("anti", SymmetryType::Antisymmetric)
      .value("none", SymmetryType::Nonsymmetric);

  py::class_<Tensor, std::shared_ptr<Tensor>>(m, "Tensor")
      .def(py::init<const std::string &, const std::vector<Index> &,
                    const std::vector<Index> &, SymmetryType>())
      .def("__repr__", &Tensor::str)
      .def("__str__", &Tensor::str)
      .def("label", &Tensor::label)
      .def("lower", &Tensor::lower)
      .def("upper", &Tensor::upper)
      .def("symmetry", &Tensor::symmetry)
      .def("latex", &Tensor::latex)
      .def("compile", &Tensor::compile)
      .def("symmetry_factor", &Tensor::symmetry_factor)
      .def("reindex", &Tensor::reindex);

  m.def("tensor", &make_tensor, "label"_a, "lower"_a, "upper"_a, "symmetry"_a);
  m.def("tensor", &make_tensor_from_str, "s"_a, "symmetry"_a);
}