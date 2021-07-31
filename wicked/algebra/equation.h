#ifndef _wicked_equation_h_
#define _wicked_equation_h_

#include "symbolic_term.h"
#include <vector>

/// A class to represent a sum of SQ terms.
class Equation {
public:
  // ==> Constructor <==
  Equation(const SymbolicTerm &lhs, const SymbolicTerm &rhs, scalar_t factor);

  // ==> Class public interface <==

  /// Return the left-hand side of the equation
  const SymbolicTerm &lhs() const;

  /// Return the right-hand side of the equation
  const SymbolicTerm &rhs() const;

  /// Return the factor for the left-hand side equation
  scalar_t rhs_factor() const;

  /// Comparison operator
  bool operator==(Equation const &other) const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit (C++ code) representation
  std::string ambit() const;

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

#endif // _wicked_equation_h_
