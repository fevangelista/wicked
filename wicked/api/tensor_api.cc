#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "../wicked/algebra/tensor.h"

namespace nb = nanobind;
using namespace nanobind::literals;

/// Export the Indexclass
void export_Tensor(nb::module_ &m) {
  nb::enum_<SymmetryType>(m, "sym")
      .value("symm", SymmetryType::Symmetric)
      .value("anti", SymmetryType::Antisymmetric)
      .value("none", SymmetryType::Nonsymmetric);

  nb::class_<Tensor>(m, "Tensor")
      .def(nb::init<const std::string &, const std::vector<Index> &,
                    const std::vector<Index> &, SymmetryType>())
      .def("__repr__", &Tensor::str)
      .def("__str__", &Tensor::str)
      .def("label", &Tensor::label)
      .def("lower", &Tensor::lower)
      .def("upper", &Tensor::upper)
      .def("symmetry", &Tensor::symmetry)
      .def("latex", &Tensor::latex)
      .def("compile", &Tensor::compile);

  m.def("tensor", &make_tensor, "label"_a, "lower"_a, "upper"_a, "symmetry"_a);
  m.def("tensor", &make_tensor_from_str, "s"_a, "symmetry"_a);
}