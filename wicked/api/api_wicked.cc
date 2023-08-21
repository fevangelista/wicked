#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "helpers/combinatorics.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace py = pybind11;
using namespace pybind11::literals;

void export_combinatorics(py::module &m);
void export_OrbitalSpaceInfo(py::module &m);
void export_Index(py::module &m);
void export_SQOperator(py::module &m);
void export_Tensor(py::module &m);
void export_SymbolicTerm(py::module &m);
void export_Expression(py::module &m);
void export_Equation(py::module &m);
void export_Operator(py::module &m);
void export_OperatorExpression(py::module &m);
void export_WickTheorem(py::module &m);
void export_rational(py::module &m);

PYBIND11_MODULE(_wicked, m) {
  m.doc() = "Wicked python interface";
  export_rational(m);
  export_combinatorics(m);
  export_OrbitalSpaceInfo(m);
  export_Index(m);
  export_SQOperator(m);
  export_Tensor(m);
  export_SymbolicTerm(m);
  export_Expression(m);
  export_Equation(m);
  export_Operator(m);
  export_OperatorExpression(m);
  export_WickTheorem(m);
}
