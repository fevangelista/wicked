#ifndef _wicked_wsum_h_
#define _wicked_wsum_h_

#include <vector>

#include "wicked-def.h"
#include "walgebraicterm.h"

/// A class to represent a sum of SQ terms.
class WSum {
public:
  // ==> Constructor <==
  WSum();

  // ==> Class public interface <==

  /// Return the map (term,factor)
  std::map<WAlgebraicTerm, scalar_t> &sum() { return sum_; }

  /// Add a term that can optionally be scaled
  void add(const WAlgebraicTerm &term, scalar_t factor = 1);

  /// Add a term that can optionally be scaled
  void add(const std::pair<WAlgebraicTerm, scalar_t> &term_factor,
           scalar_t scale = 1);

  /// Add a term that can optionally be scaled
  void add_sum(WSum&& sum, scalar_t scale = 1);

  /// Add a sum
  WSum &operator+=(WSum &sum);
  WSum &operator+=(WSum &&sum);

  /// Substract a sum
  WSum &operator-=(WSum &sum);
  WSum &operator-=(WSum &&sum);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

private:
  // ==> Class private data <==

  std::map<WAlgebraicTerm, scalar_t> sum_;

  // ==> Class private functions <==
};

/// Print to an output stream
std::ostream &operator<<(std::ostream &os, const WSum &sum);

#endif // _wicked_wsum_h_
