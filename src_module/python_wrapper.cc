#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../src/rational.h"
#include "../src/orbital_space.h"
#include "../src/algebra/windex.h"
#include "../src/algebra/wtensor.h"
#include "../src/algebra/wsqoperator.h"
#include "../src/algebra/walgebraicterm.h"
#include "../src/algebra/wequationterm.h"
#include "../src/algebra/wsum.h"
#include "../src/diagrams/wdiag_operator.h"
#include "../src/diagrams/wdiag_operator_sum.h"
#include "../src/diagrams/wdiag_theorem.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace py = pybind11;

PYBIND11_PLUGIN(pywicked) {
  py::module m("pywicked", "Wicked python interface");

  py::class_<rational, std::shared_ptr<rational>>(m, "rational")
      .def(py::init<>())
      .def(py::init<int>())
      .def(py::init<int, int>());

  py::enum_<RDMType>(m, "rdmtype")
      .value("occupied", RDMType::Occupied)
      .value("unoccupied", RDMType::Unoccupied)
      .value("general", RDMType::General);

  py::enum_<WDiagPrint>(m, "WDiagPrint")
      .value("no", WDiagPrint::No)
      .value("basic", WDiagPrint::Basic)
      .value("summary", WDiagPrint::Summary)
      .value("detailed", WDiagPrint::Detailed)
      .value("all", WDiagPrint::All);

  py::class_<OrbitalSpaceInfo, std::shared_ptr<OrbitalSpaceInfo>>(
      m, "OrbitalSpaceInfo")
      .def(py::init<>())
      .def("default_spaces", &OrbitalSpaceInfo::default_spaces)
      .def("reset", &OrbitalSpaceInfo::reset)
      .def("add_space", &OrbitalSpaceInfo::add_space);

  py::class_<WIndex, std::shared_ptr<WIndex>>(m, "WIndex")
      .def(py::init<int, int>());

  py::class_<WTensor, std::shared_ptr<WTensor>>(m, "WTensor");

  py::class_<WSQOperator, std::shared_ptr<WSQOperator>>(m, "WSQOperator")
      .def(py::init<SQOperatorType, WIndex>());

  py::class_<WAlgebraicTerm, std::shared_ptr<WAlgebraicTerm>>(m,
                                                              "WAlgebraicTerm")
      .def(py::init<>())
      .def("str", &WAlgebraicTerm::str)
      .def("latex", &WAlgebraicTerm::latex)
      .def("ambit", &WAlgebraicTerm::ambit);

  py::class_<WEquationTerm, std::shared_ptr<WEquationTerm>>(m, "WEquationTerm")
      .def(py::init<const WAlgebraicTerm &, const WAlgebraicTerm &, scalar_t>())
      .def("lhs", &WEquationTerm::lhs)
      .def("rhs", &WEquationTerm::rhs)
      .def("str", &WEquationTerm::str)
      .def("latex", &WEquationTerm::latex)
      .def("ambit", &WEquationTerm::ambit);

  py::class_<WSum, std::shared_ptr<WSum>>(m, "WSum")
      .def(py::init<>())
      .def("canonicalize", &WSum::canonicalize)
      .def("to_manybody_equation", &WSum::to_manybody_equation)
      .def("str", &WSum::str);

  py::class_<WDiagOperator, std::shared_ptr<WDiagOperator>>(m, "WDiagOperator")
      .def(py::init<const std::string &, const std::vector<int> &,
                    const std::vector<int> &>())
      .def("str", &WDiagOperator::str);

  m.def("make_diag_operator", &make_diag_operator, "Make an operator");

  py::class_<WDiagOperatorSum, std::shared_ptr<WDiagOperatorSum>>(
      m, "WDiagOperatorSum")
      .def(py::init<>())
      .def(
          py::init<const std::vector<std::vector<WDiagOperator>> &, scalar_t>(),
          py::arg("vec_vec_dop"), py::arg("factor") = rational(1))
      .def("add", &WDiagOperatorSum::add)
      .def("str", &WDiagOperatorSum::str);

  m.def("commutator", &commutator,
        "Create the commutator of two WDiagOperatorSum objects");

  m.def("bch_series", &bch_series, "Creates the Baker-Campbell-Hausdorff "
                                   "expansion of exp(-B) A exp(B) truncated at "
                                   "a given order n");

  m.def("make_operator", &make_operator, "Create a WDiagOperatorSum object");

  py::class_<WDiagTheorem, std::shared_ptr<WDiagTheorem>>(m, "WDiagTheorem")
      .def(py::init<>())
      .def("contract", &WDiagTheorem::contract)
      .def("contract_sum", &WDiagTheorem::contract_sum)
      .def("set_print", &WDiagTheorem::set_print);

  //  py::class_make_diag_operator(const std::string &label,
  //                                   const std::vector<std::string>
  //                                   &cre_labels,
  //                                   const std::vector<std::string>
  //                                   &ann_labels);

  //  py::enum_<SpinType>(m, "SpinType")
  //      .value("SpinOrbital", SpinType::SpinOrbital)
  //      .value("SpinFree", SpinType::SpinFree)
  //      .value("Alpha", SpinType::Alpha)
  //      .value("Beta", SpinType::Beta)
  //      .export_values();

  //  py::class_<WDiagOperator, std::shared_ptr<WDiagOperator>>(m, "WOperator")
  //      .def(py::init<const std::string &, const std::vector<int> &,
  //                    const std::vector<int> &>())
  //      .def("label", &WDiagOperator::label)
  //      .def("num_indices", &WDiagOperator::num_indices)
  //      .def("str", &WDiagOperator::str);

  //  m.def("get_osi", &get_osi, "Return the orbital space");

  //  std::cout << "Initializing the pywicked module." << std::endl;

  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->default_spaces();

  m.attr("osi") = py::cast(osi);

  return m.ptr();
}
