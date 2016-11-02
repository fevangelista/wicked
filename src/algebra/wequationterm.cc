#include <algorithm>
#include <iostream>

#include "helpers.h"
#include "wicked-def.h"
#include "walgebraicterm.h"
#include "wequationterm.h"
#include "wsqoperator.h"
#include "wtensor.h"

WEquationTerm::WEquationTerm(const WAlgebraicTerm &lhs,
                             const WAlgebraicTerm &rhs, scalar_t factor)
    : lhs_(lhs), rhs_(rhs), factor_(factor) {}

const WAlgebraicTerm &WEquationTerm::lhs() const { return lhs_; }

const WAlgebraicTerm &WEquationTerm::rhs() const { return rhs_; }

scalar_t WEquationTerm::rhs_factor() const { return factor_; }

std::string WEquationTerm::str() const {
  std::vector<std::string> str_vec;
  str_vec.push_back(lhs_.str());
  str_vec.push_back("+=");
  str_vec.push_back(factor_.str());
  str_vec.push_back(rhs_.str());
  return (to_string(str_vec, " "));
}

std::string WEquationTerm::latex() const { return str(); }

std::string WEquationTerm::ambit() const {
  std::vector<std::string> str_vec;
  str_vec.push_back(lhs_.ambit() + " += " + factor_.ambit());
  str_vec.push_back(rhs_.ambit());
  return (to_string(str_vec, " * ") + ";");
}

std::ostream &operator<<(std::ostream &os, const WEquationTerm &eterm) {
  os << eterm.str();
  return os;
}
