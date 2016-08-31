#ifndef _wicked_diag_theorem_h_
#define _wicked_diag_theorem_h_

#include <vector>
#include <string>

class WDiagOperator;

/// A class to contract a product of operators
class WDiagTheorem {

public:
  /// Constructor
  WDiagTheorem();

  /// Contract a product of operators
  void contract(double factor, const std::vector<WDiagOperator> &ops);

private:
};

#endif // _wicked_diag_theorem_h_
