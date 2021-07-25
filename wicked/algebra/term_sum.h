#ifndef _wicked_term_sum_h_
#define _wicked_term_sum_h_

#include <map>
#include <vector>

#include "wicked-def.h"
#include "index.h"

class WAlgebraicTerm;
class WEquationTerm;

/// A class to represent a sum of SQ terms.
class TermSum {
public:
  // ==> Constructor <==
  TermSum();

  // ==> Class public interface <==

  /// Return a map term -> factor
  const std::map<WAlgebraicTerm, scalar_t> &terms() const { return terms_; }

  /// Add a term that can optionally be scaled
  void add(const WAlgebraicTerm &term, scalar_t factor = 1);

  /// Add a term that can optionally be scaled
  void add(const std::pair<WAlgebraicTerm, scalar_t> &term_factor,
           scalar_t scale = 1);

  /// Add a term that can optionally be scaled
  void add_sum(const TermSum &terms, scalar_t scale = 1);

  /// Canonicalize this sum
  TermSum &canonicalize();

  /// Reindex this sum
  TermSum &reindex(index_map_t &idx_map);

  bool operator==(const TermSum &terms);

  /// Add a sum
  TermSum &operator+=(const TermSum &terms);

  /// Substract a sum
  TermSum &operator-=(const TermSum &terms);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex(const std::string &sep = " \\\\ \n") const;

  /// Convert this sum to a vector of many-body equations
  std::vector<WEquationTerm> to_manybody_equation(const std::string &label);

private:
  // ==> Class private data <==

  std::map<WAlgebraicTerm, scalar_t> terms_;

  // ==> Class private functions <==
};

/// addition
TermSum operator+(TermSum lhs, const TermSum &rhs);
/// subtraction
TermSum operator-(TermSum lhs, const TermSum &rhs);

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const TermSum &sum);

/// The syntax used to input a tensor expression
enum class TensorSyntax { Wicked, TCE };

///// Create a sum from a string
TermSum string_to_sum(const std::string &s,
                      TensorSyntax syntax = TensorSyntax::Wicked);

#endif // _wicked_term_sum_h_
