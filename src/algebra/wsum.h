#ifndef _wicked_wsum_h_
#define _wicked_wsum_h_

#include <map>
#include <vector>

#include "wicked-def.h"

class WAlgebraicTerm;
class WEquationTerm;

/// A class to represent a sum of SQ terms.
class WSum {
public:
  // ==> Constructor <==
  WSum();

  // ==> Class public interface <==

  /// Return a map term -> factor
  std::map<WAlgebraicTerm, scalar_t> &terms() { return terms_; }

  /// Return a vector of pairs (term,factor)
  std::vector<std::pair<WAlgebraicTerm, scalar_t>> vector() const;

  /// Add a term that can optionally be scaled
  void add(const WAlgebraicTerm &term, scalar_t factor = 1);

  /// Add a term that can optionally be scaled
  void add(const std::pair<WAlgebraicTerm, scalar_t> &term_factor,
           scalar_t scale = 1);

  /// Add a term that can optionally be scaled
  void add_sum(WSum &&terms, scalar_t scale = 1);

  bool operator==(WSum &terms);

  /// Add a sum
  WSum &operator+=(WSum &terms);
  WSum &operator+=(WSum &&terms);

  /// Substract a sum
  WSum &operator-=(WSum &terms);
  WSum &operator-=(WSum &&terms);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

//  /// Convert this sum to a vector of projective equations
//  std::vector<WEquationTerm>
//  to_projective_equation(const std::string &residual_label);

  /// Convert this sum to a vector of many-body equations
  std::vector<WEquationTerm> to_manybody_equation(const std::string& label);

private:
  // ==> Class private data <==

  std::map<WAlgebraicTerm, scalar_t> terms_;

  // ==> Class private functions <==
};

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const WSum &sum);

/// The syntax used to input a tensor expression
enum class TensorSyntax { Wicked, TCE };

///// Create a sum from a string
WSum string_to_sum(const std::string &s,
                   TensorSyntax syntax = TensorSyntax::Wicked);

#endif // _wicked_wsum_h_
