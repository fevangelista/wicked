#include <algorithm>
#include <iostream>

#include "equation.h"
#include "helpers.h"
#include "sqoperator.h"
#include "tensor.h"
#include "term.h"
#include "wicked-def.h"

Equation::Equation(const SymbolicTerm &lhs, const SymbolicTerm &rhs,
                   scalar_t factor)
    : lhs_(lhs), rhs_(rhs), factor_(factor) {}

const SymbolicTerm &Equation::lhs() const { return lhs_; }

const SymbolicTerm &Equation::rhs() const { return rhs_; }

scalar_t Equation::rhs_factor() const { return factor_; }

bool Equation::operator==(Equation const &other) const {
  return ((lhs() == other.lhs()) and (rhs() == other.rhs()) and
          (rhs_factor() == other.rhs_factor()));
}

std::string Equation::str() const {
  std::vector<std::string> str_vec;
  str_vec.push_back(lhs_.str());
  str_vec.push_back("+=");
  str_vec.push_back(factor_.str());
  str_vec.push_back(rhs_.str());
  return (join(str_vec, " "));
}

std::string Equation::latex() const { return str(); }

std::string Equation::ambit() const {
  std::vector<std::string> str_vec;
  str_vec.push_back(lhs_.ambit() + " += " + factor_.ambit());
  str_vec.push_back(rhs_.ambit());
  return (join(str_vec, " * ") + ";");
}

std::ostream &operator<<(std::ostream &os, const Equation &eterm) {
  os << eterm.str();
  return os;
}
