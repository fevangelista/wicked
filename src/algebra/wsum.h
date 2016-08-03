#ifndef _wicked_wsum_h_
#define _wicked_wsum_h_

#include <vector>

#include "wicked-def.h"
#include "wterm.h"

/// A class to represent a sum of SQ terms.
class WSum {
public:
  // ==> Constructor <==
  WSum();

  // ==> Class public interface <==

  /// Return the map (term,factor)
  std::vector<WTerm> &sum() { return sum_; }

  /// Add a term that can optionally be scaled
  void add(const WTerm &term, scalar_t scale = 1);

  /// Add a term that can optionally be scaled
  void add(const WSum &sum, scalar_t scale = 1);

  /// Add a sum
  WSum &operator+=(WSum &sum);

  /// Substract a sum
  WSum &operator-=(WSum &sum);

  /// Return a string representation
  std::string str() const;

  /// Return a LaTeX representation
  std::string latex() const;

private:
  // ==> Class private data <==

  std::vector<WTerm> sum_;

  // ==> Class private functions <==
};

#endif // _wicked_wsum_h_
