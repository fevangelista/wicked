#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/tensor.h"

namespace py = pybind11;
using namespace pybind11::literals;

/// Export the Indexclass
void export_Tensor(py::module &m) {
  py::class_<Tensor, std::shared_ptr<Tensor>>(m, "Tensor")
      .def(py::init<const std::string &, const std::vector<Index> &,
                    const std::vector<Index> &, SymmetryType>())
      .def("__repr__", &Tensor::str)
      .def("__str__", &Tensor::str)
      .def("latex", &Tensor::latex)
      .def("ambit", &Tensor::ambit);

  m.def("tensor", [](const std::string &label,
                     const std::vector<std::pair<std::string, int>> &lower,
                     const std::vector<std::pair<std::string, int>> &upper,
                     SymmetryType symmetry) {
    return make_tensor(label, lower, upper, symmetry);
  });
}