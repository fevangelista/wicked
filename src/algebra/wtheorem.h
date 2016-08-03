#ifndef _wicked_wtheorem_h_
#define _wicked_wtheorem_h_

#include <vector>

#include "wterm.h"

/// A class to apply Wick's theorem to a product of terms
class WTheorem {
public:
  // ==> Constructor <==

  WTheorem();

  // ==> Class public interface <==

  /// Apply Wick's theorem to a product of terms
  void theorem(const std::vector<WTerm> &terms);

private:
};

#endif // _wicked_wtheorem_h_
