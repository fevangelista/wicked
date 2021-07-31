#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../wicked/algebra/sqoperator.h"
#include "../wicked/algebra/tensor.h"
#include "../wicked/algebra/term.h"
// #include "../wicked/algebra/term_sum.h"
#include "../wicked/algebra/wequationterm.h"
#include "../wicked/diagrams/wdiag_operator.h"
#include "../wicked/diagrams/wdiag_operator_sum.h"
#include "../wicked/diagrams/wick_theorem.h"
#include "../wicked/orbital_space.h"
#include "../wicked/rational.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace py = pybind11;
using namespace pybind11::literals;
void export_Expression(py::module &m);
void export_Index(py::module &m);
void export_SQOperator(py::module &m);
void export_SQOpProd(py::module &m);
void export_Tensor(py::module &m);
void export_SymbolicTerm(py::module &m);

PYBIND11_MODULE(wicked, m) {
  m.doc() = "Wicked python interface";

  py::class_<rational, std::shared_ptr<rational>>(m, "rational")
      .def(py::init<>())
      .def(py::init<int>())
      .def(py::init<int, int>());

  export_Index(m);
  export_SQOperator(m);
  export_SQOpProd(m);
  export_Tensor(m);
  export_SymbolicTerm(m);
  export_Expression(m);

  m.def(
      "reset_space", []() { osi->reset(); }, "Reset the orbital space");

  m.def(
      "add_space",
      [](char label, const std::string &type,
         const std::vector<std::string> &indices) {
        RDMType structure = string_to_rdmtype(type);
        osi->add_space(label, structure, indices);
      },
      "label"_a, "type"_a, "indices"_a,
      "Add an orbital space. `type` can be any of "
      "(occupied,unoccupied,general)");

  py::enum_<RDMType>(m, "rdm")
      .value("occupied", RDMType::Occupied)
      .value("unoccupied", RDMType::Unoccupied)
      .value("general", RDMType::General);

  py::enum_<SymmetryType>(m, "sym")
      .value("symm", SymmetryType::Symmetric)
      .value("anti", SymmetryType::Antisymmetric)
      .value("none", SymmetryType::Nonsymmetric);

  //   py::enum_<WDiagPrint>(m, "WDiagPrint")
  //       .value("no", WDiagPrint::No)
  //       .value("basic", WDiagPrint::Basic)
  //       .value("summary", WDiagPrint::Summary)
  //       .value("detailed", WDiagPrint::Detailed)
  //       .value("all", WDiagPrint::All);

  py::class_<OrbitalSpaceInfo, std::shared_ptr<OrbitalSpaceInfo>>(
      m, "OrbitalSpaceInfo")
      .def(py::init<>())
      .def("default_spaces", &OrbitalSpaceInfo::default_spaces)
      .def("reset", &OrbitalSpaceInfo::reset)
      .def("add_space", &OrbitalSpaceInfo::add_space)
      .def("__str__", &OrbitalSpaceInfo::str);

  //   py::class_<WEquationSymbolicTerm,
  //   std::shared_ptr<WEquationSymbolicTerm>>(m, "WEquationSymbolicTerm")
  //       .def(py::init<const SymbolicTerm &, const SymbolicTerm &,
  //       scalar_t>()) .def("lhs", &WEquationSymbolicTerm::lhs) .def("rhs",
  //       &WEquationSymbolicTerm::rhs) .def("str", &WEquationSymbolicTerm::str)
  //       .def("latex", &WEquationSymbolicTerm::latex)
  //       .def("ambit", &WEquationSymbolicTerm::ambit);

  py::class_<WDiagOperator, std::shared_ptr<WDiagOperator>>(m, "WDiagOperator")
      .def(py::init<const std::string &, const std::vector<int> &,
                    const std::vector<int> &>())
      .def("str", &WDiagOperator::str);

  m.def("make_diag_operator", &make_diag_operator, "Make an operator");

  py::class_<OperatorSum, std::shared_ptr<OperatorSum>>(m, "OperatorSum")
      .def(py::init<>())
      .def(
          py::init<const std::vector<std::vector<WDiagOperator>> &, scalar_t>(),
          py::arg("vec_vec_dop"), py::arg("factor") = rational(1))
      .def("add", &OperatorSum::add)
      .def("str", &OperatorSum::str);

  m.def("commutator", &commutator,
        "Create the commutator of two OperatorSum objects");

  m.def("bch_series", &bch_series,
        "Creates the Baker-Campbell-Hausdorff "
        "expansion of exp(-B) A exp(B) truncated at "
        "a given order n");

  m.def("operator", &make_operator, "Create a OperatorSum object");

  //   py::class_<WickTheorem, std::shared_ptr<WickTheorem>>(m, "WickTheorem")
  //       .def(py::init<>())
  //       .def("contract",
  //            py::overload_cast<scalar_t, const std::vector<WDiagOperator> &,
  //            int,
  //                              int>(&WickTheorem::contract))
  //       .def("contract",
  //            py::overload_cast<scalar_t, const OperatorSum &, int, int>(
  //                &WickTheorem::contract))
  //       .def("set_print", &WickTheorem::set_print);

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

  m.def("get_osi", &get_osi, "Return the orbital space");

  //  std::cout << "Initializing the pywicked module." << std::endl;

  //  osi = std::make_shared<OrbitalSpaceInfo>();
  //  osi->default_spaces();

  //  m.attr("osi") = py::cast(osi);
}
