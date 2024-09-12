#pragma once

#include "symbolic_term.h"

/// A class to represent a term in a SQ expression. A term includes:
/// 1) a product of tensors
/// 2) a product of operators normal ordered with respect to the vacuum
class Term : public SymbolicTerm {
public:
  Term();

  Term(scalar_t c, bool normal_ordered, const std::vector<SQOperator> &op,
       const std::vector<Tensor> &tensors);

  Term(const SymbolicTerm &term);

  Term(scalar_t c, const SymbolicTerm &term);

  /// Set the coefficient
  void set(scalar_t c);

  /// Return the coefficient
  scalar_t coefficient() const;

  SymbolicTerm symterm() const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

private:
  scalar_t coefficient_ = {1, 1};
};

// Helper functions

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const Term &term);

// /// @brief Create a term from a coefficient and a symbolic term
// Term operator*(scalar_t c, const SymbolicTerm &term);

///// Create an operator
Term make_term(const std::string &label, const std::vector<std::string> &cre,
               const std::vector<std::string> &ann,
               scalar_t coefficient = scalar_t(1));

std::vector<Term> wick_product(const Term &A, const Term &B);
