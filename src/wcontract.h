#ifndef _wicked_contract_h_
#define _wicked_contract_h_

#include <vector>
#include <string>

class WDiagOperator;

/// A class to contract a product of operators
class WContract {

public:
  /// Constructor
  WContract();

  /// Contract a product of operators
  void contract(double factor, const std::vector<WDiagOperator> &ops);

private:
};

#endif // _wicked_contract_h_
