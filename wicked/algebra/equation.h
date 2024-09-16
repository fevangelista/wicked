#pragma once

#include "symbolic_term.h"
#include <vector>

class Expression;

/// A class to represent a sum of SQ terms.
class Equation {
public:
  // ==> Constructor <==
  Equation(const SymbolicTerm &lhs, const SymbolicTerm &rhs, scalar_t factor);

  // ==> Class public interface <==

  /// Return the symbolic term on the left-hand side of the equation
  const SymbolicTerm &lhs() const;

  /// Return the symbolic term on the right-hand side of the equation
  const SymbolicTerm &rhs() const;

  /// Return the factor for the left-hand side equation
  scalar_t rhs_factor() const;

  /// Return the right-hand side of the equation
  Expression rhs_expression() const;

  /// Comparison operator
  bool operator==(Equation const &other) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return a compilable representation
  std::string compile(const std::string &format) const;

private:
  // ==> Class private data <==

  /// The right-hand term
  SymbolicTerm lhs_;
  /// The left-hand term
  SymbolicTerm rhs_;
  /// The factor of the left-hand side
  scalar_t factor_;

  // ==> Class private functions <==
};

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const Equation &eterm);
