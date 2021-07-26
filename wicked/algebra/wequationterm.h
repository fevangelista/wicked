#ifndef _wicked_equation_term_h_
#define _wicked_equation_term_h_

#include <vector>

class Term;

/// A class to represent a sum of SQ terms.
class WEquationTerm {
public:
  // ==> Constructor <==
  WEquationTerm(const Term &lhs, const Term &rhs, scalar_t factor);

  // ==> Class public interface <==

  /// Return the left-hand side of the equation
  const Term &lhs() const;

  /// Return the right-hand side of the equation
  const Term &rhs() const;

  /// Return the factor for the left-hand side equation
  scalar_t rhs_factor() const;

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

  /// Return an ambit (C++ code) representation
  std::string ambit() const;

private:
  // ==> Class private data <==

  /// The right-hand term
  Term lhs_;
  /// The left-hand term
  Term rhs_;
  /// The factor of the left-hand side
  scalar_t factor_;

  // ==> Class private functions <==
};

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const WEquationTerm &eterm);

#endif // _wicked_equation_term_h_
