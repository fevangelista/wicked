#include <algorithm>

#include "term.h"

using namespace std;

Term::Term() {}

Term::Term(scalar_t c, bool normal_ordered,
           const std::vector<SQOperator> &operators,
           const std::vector<Tensor> &tensors)
    : coefficient_(c), SymbolicTerm(normal_ordered, operators, tensors) {}

Term::Term(const SymbolicTerm &term) : SymbolicTerm(term) {}

Term::Term(scalar_t c, const SymbolicTerm &term)
    : coefficient_(c), SymbolicTerm(term) {
  ;
}

/// Return the coefficient
scalar_t Term::coefficient() const { return coefficient_; }

void Term::set(scalar_t c) { coefficient_ = c; }

SymbolicTerm Term::symterm() const {
  return SymbolicTerm(normal_ordered(), ops(), tensors());
}

std::string Term::str() const {
  std::string term_str = SymbolicTerm::str();
  std::string coeff_str = coefficient_.str();
  if ((term_str == "") and (coeff_str == "")) {
    return "1";
  } else if ((term_str == "") and (coeff_str != "")) {
    return coeff_str;
  } else if ((term_str != "") and (coeff_str == "")) {
    return term_str;
  }
  return coeff_str + " " + term_str;
}

std::string Term::latex() const {
  return coefficient_.latex(true) + " " + SymbolicTerm::latex();
}

std::ostream &operator<<(std::ostream &os, const Term &term) {
  os << term.str();
  return os;
}

std::vector<Term> wick_product(const Term &A, const Term &B) {
  std::vector<Term> result;
  scalar_t ab = A.coefficient() * B.coefficient();
  return result;
}

// /// @brief Create a term from a coefficient and a symbolic term
// Term operator*(scalar_t c, const SymbolicTerm &term) { return Term(c, term);
// }