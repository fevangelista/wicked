// #include <algorithm>
// #include <iostream>

// #include "helpers.h"
// #include "sqoperator.h"
// #include "term.h"
// #include "wequationterm.h"
// #include "wicked-def.h"
// #include "wtensor.h"

// WEquationSymbolicTerm::WEquationSymbolicTerm(const WAlgebraicSymbolicTerm
// &lhs,
//                              const WAlgebraicSymbolicTerm &rhs, scalar_t
//                              factor)
//     : lhs_(lhs), rhs_(rhs), factor_(factor) {}

// const WAlgebraicSymbolicTerm &WEquationSymbolicTerm::lhs() const { return
// lhs_; }

// const WAlgebraicSymbolicTerm &WEquationSymbolicTerm::rhs() const { return
// rhs_; }

// scalar_t WEquationSymbolicTerm::rhs_factor() const { return factor_; }

// std::string WEquationSymbolicTerm::str() const {
//   std::vector<std::string> str_vec;
//   str_vec.push_back(lhs_.str());
//   str_vec.push_back("+=");
//   str_vec.push_back(factor_.str());
//   str_vec.push_back(rhs_.str());
//   return (to_string(str_vec, " "));
// }

// std::string WEquationSymbolicTerm::latex() const { return str(); }

// std::string WEquationSymbolicTerm::ambit() const {
//   std::vector<std::string> str_vec;
//   str_vec.push_back(lhs_.ambit() + " += " + factor_.ambit());
//   str_vec.push_back(rhs_.ambit());
//   return (to_string(str_vec, " * ") + ";");
// }

// std::ostream &operator<<(std::ostream &os, const WEquationSymbolicTerm
// &eterm) {
//   os << eterm.str();
//   return os;
// }
