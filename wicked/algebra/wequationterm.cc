#include <algorithm>
#include <iostream>

#include "helpers.h"
#include "sqoperator.h"
#include "tensor.h"
#include "term.h"
#include "wequationterm.h"
#include "wicked-def.h"

WEquationTerm::WEquationTerm(const Term &lhs, const Term &rhs, scalar_t factor)
    : lhs_(lhs), rhs_(rhs), factor_(factor) {}

const Term &WEquationTerm::lhs() const { return lhs_; }

const Term &WEquationTerm::rhs() const { return rhs_; }

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
