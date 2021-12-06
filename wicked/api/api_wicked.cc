#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/combinatorics.h"
#include "../wicked/rational.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace py = pybind11;
using namespace pybind11::literals;

void export_combinatorics(py::module &m);
void export_OrbitalSpaceInfo(py::module &m);
void export_Index(py::module &m);
void export_SQOperator(py::module &m);
void export_SQOpProd(py::module &m);
void export_Tensor(py::module &m);
void export_SymbolicTerm(py::module &m);
void export_Expression(py::module &m);
void export_Equation(py::module &m);

void export_DiagOperator(py::module &m);
void export_DiagOpExpression(py::module &m);
void export_WickTheorem(py::module &m);

PYBIND11_MODULE(wicked, m) {
  m.doc() = "Wicked python interface";

  py::class_<rational, std::shared_ptr<rational>>(m, "rational")
      .def(py::init<>())
      .def(py::init<int>())
      .def(py::init<int, int>())
      .def("__eq__",
           [](const rational &lhs, const rational &rhs) { return lhs == rhs; })
      .def("__repr__", &rational::repr);

  m.def("make_rational", &make_rational_from_str);

  m.def("int_part", &integer_partitions);

  export_combinatorics(m);
  export_OrbitalSpaceInfo(m);
  export_Index(m);
  export_SQOperator(m);
  export_SQOpProd(m);
  export_Tensor(m);
  export_SymbolicTerm(m);
  export_Expression(m);
  export_Equation(m);
  export_DiagOperator(m);
  export_DiagOpExpression(m);
  export_WickTheorem(m);
}
