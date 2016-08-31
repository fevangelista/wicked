#ifndef _wicked_diag_theorem_h_
#define _wicked_diag_theorem_h_

#include <vector>
#include <string>

class WDiagOperator;
class WDiagVertex;

/// A class to contract a product of operators
class WDiagTheorem {

public:
  /// Constructor
  WDiagTheorem();

  /// Contract a product of operators
  void contract(double factor, const std::vector<WDiagOperator> &ops);

private:
  /// Generates all valid contractions
  std::vector<std::vector<WDiagVertex>>
  generate_basic_contractions(const std::vector<WDiagOperator> &ops);

  /// The largest allowed cumulant
  int maxcumulant_ = 3;
};

#endif // _wicked_diag_theorem_h_
