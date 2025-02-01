#include <nanobind/nanobind.h>

#include "helpers/combinatorics.h"

namespace nb = nanobind;
using namespace nb::literals;

void export_combinatorics(nb::module_ &m);
void export_OrbitalSpaceInfo(nb::module_ &m);
void export_Index(nb::module_ &m);
void export_SQOperator(nb::module_ &m);
void export_Tensor(nb::module_ &m);
void export_SymbolicTerm(nb::module_ &m);
void export_Expression(nb::module_ &m);
void export_Equation(nb::module_ &m);
void export_Operator(nb::module_ &m);
void export_OperatorExpression(nb::module_ &m);
void export_WickTheorem(nb::module_ &m);
void export_rational(nb::module_ &m);

NB_MODULE(_wicked, m) {
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
